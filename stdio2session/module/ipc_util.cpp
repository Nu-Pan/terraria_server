/*
    inter process communication uils
*/

//----------------------------------------------------------------------------
// File Dependencies
//----------------------------------------------------------------------------

#include "precompiled_header.h"

#include "ipc_util.h"

#include "unix_util.h"

//----------------------------------------------------------------------------
// Namespace Dependencies
//----------------------------------------------------------------------------

using namespace std;
using namespace s2s;

//----------------------------------------------------------------------------
// Link-local Constants
//----------------------------------------------------------------------------
namespace
{
    // プロセス間通信ファイルパス
    static const char* const IPC_DIR_PATH = "/tmp/s2s";

    // プロセス間通信ファイル権限
    static mode_t IPC_FILE_PERMISSION = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;
}

//----------------------------------------------------------------------------
// Link-local Variables
//----------------------------------------------------------------------------
namespace
{
    static random_device s_seedGenerator;
    static mt19937 s_randomGenerator(s_seedGenerator());
}

//----------------------------------------------------------------------------
// Link-local Functions
//----------------------------------------------------------------------------
namespace
{
    /* タグからプロセス間通信タグディレクトリのパスを生成する
    */
    static string _MakeIpcTagDirPath( const string& tag )
    {
        ostringstream oss;
        oss << IPC_DIR_PATH << "/" << tag;
        return oss.str();
    }

    /* タグと名前からプロセス間通信ファイルのパスを生成する
    */
    static string _MakeIpcFilePath( const string& tag, const string& ipcFileName )
    {
        ostringstream oss;
        oss << IPC_DIR_PATH << "/" << tag << "/" << ipcFileName;
        return oss.str();
    }

    /* 指定ディレクトリの存在を保証する
    */
    static void _SafeMakeDir( const string& dirPath )
    {
        // ディレクトリ存在チェック
        const bool doesExists = [&]()
        {
            // stat で問い合わせ
            struct stat statData = {};
            {
                const int result = stat(dirPath.c_str(), &statData);
                if( result != 0 )
                {
                    // ディレクトリは存在せず
                    return false;
                }
            }
            // 通常ファイルの場合は致命的失敗として扱う
            if( S_ISREG(statData.st_mode) )
            {
                ostringstream oss;
                oss << "There are '" << dirPath << "' as regular file.";
                oss << "'" << dirPath << "' *MUST BE* directory.";
                throw runtime_error(oss.str());
            }
            // ディレクトリは存在する
            return true;
        }();
        // ディレクトリを生成
        if( !doesExists )
        {
            const int result = mkdir(dirPath.c_str(), IPC_FILE_PERMISSION);
            if( result )
            {
                ostringstream oss;
                oss << "Failed to create directory(path=" << dirPath << ").";
                throw runtime_error(oss.str());
            }
        }
        // 正常終了
        return;
    }
}

//----------------------------------------------------------------------------
// Defenitions
//----------------------------------------------------------------------------
namespace s2s
{
    /** プロセス間通信に必要なセットアップを行う
     */
    void InitializeIpc()
    {
        _SafeMakeDir(IPC_DIR_PATH);
        return;
    }

    /** プロセス間通信ファイルにテキストを書き込む
    */
    std::string WriteIpcFile( const std::string& tag, const std::string& name, const std::string& text )
    {
        // ファイル名を生成
        const string ipcFileName = [&]()
        {
            // 外部指定がある場合はそれを使う
            if( !name.empty() )
            {
                return name;
            }
            // システム時刻を問い合わせ
            const struct timeval rawTime = []()
            {
                struct timeval temp = {};
                const int result = gettimeofday(&temp, NULL);
                if(result != 0)
                {
                    ostringstream oss;
                    oss << "Failed to 'gettimeofday'.";
                    throw runtime_error(oss.str());
                }
                return temp;
            }();
            // 時刻をそのままファイル名化
            ostringstream oss;
            oss << rawTime.tv_sec << "_" << rawTime.tv_usec;
            return oss.str();
        }();
        // ディレクトリの存在を保証
        {
            _SafeMakeDir( _MakeIpcTagDirPath(tag) );
        }
        // ファイルパスを生成
        const string ipcFilePath = [&]()
        {
            return _MakeIpcFilePath(tag, ipcFileName);
        }();
        // ファイルを開く
        const UniqueFD fd = open(ipcFilePath.c_str(), O_WRONLY|O_CREAT, IPC_FILE_PERMISSION);
        if( !*fd )
        {
            ostringstream oss;
            oss << "Failed to open file(file=" << ipcFilePath << ", fd=" << *fd << ", errno " << errno << ").";
            throw runtime_error(oss.str());
        }
        // ファイルをロックする
        {
            // 記述
            const struct flock fl
            {
                F_WRLCK,
                SEEK_SET,
                /*l_start=*/0,
                /*l_len=*/0,
                getpid()                    
            };
            // ロック
            const int result = fcntl(*fd, F_SETLKW, &fl);
            if( result != 0 )
            {
                ostringstream oss;
                oss << "Failed to fcntl(file=" << ipcFilePath << ", result=" << result << ").";
                throw runtime_error(oss.str());
            }
        }
        // ファイルにテキストを書き込み
        {
            const int result = write(*fd, text.data(), text.size());
            if( result < 0 )
            {
                ostringstream oss;
                oss << "Failed to write text(file=" << ipcFilePath << ").";
                throw runtime_error(oss.str());
            }
        }
        // 正常終了
        return ipcFileName;
    }

    /**　プロセス間通信ファイルからテキストを読み出す
    */
    std::string ReadIpcFile( const std::string& tag, const std::string& ipcFileName )
    {
        // ファイルパスを生成
        const string ipcFilePath = [&]()
        {
            return _MakeIpcFilePath(tag, ipcFileName);
        }();
        // 指定のファイルの中身をメモリ上に読み出す
        const string text = [&]()
        {
            // ファイルを開く
            UniqueFD fd = open(ipcFilePath.c_str(), O_RDONLY);
            if( *fd < 0 )
            {
                ostringstream oss;
                oss << "Failed to open file(file=" << ipcFilePath << ", fd=" << *fd << ", errno " << errno << ").";
                throw runtime_error(oss.str());
            }
            // ファイルをロックする
            {
                // 記述
                const struct flock fl
                {
                    F_RDLCK,
                    SEEK_SET,
                    /*l_start=*/0,
                    /*l_len=*/0,
                    getpid()                 
                };
                // ロック
                int tempFd = *fd;
                const int result = fcntl(tempFd, F_SETLKW, &fl);
                if( result != 0 )
                {
                    ostringstream oss;
                    oss << "Failed to fcntl(file=" << ipcFilePath << ", result=" << result << ", errno=" << errno << ").";
                    throw runtime_error(oss.str());
                }
            }
            // stat でファイルサイズを問い合わせる
            const size_t fileSizeInBytes = [&]()
            {
                struct stat statData = {};
                const int result = stat(ipcFilePath.c_str(), &statData);
                if( result != 0 )
                {
                    ostringstream oss;
                    oss << "Failed to stat file(file=" << ipcFilePath << ").";
                    throw runtime_error(oss.str());
                }
                return statData.st_size;
            }();
            // メモリに読み出す
            string textBuffer;
            {
                textBuffer.resize(fileSizeInBytes);
                const int result = read(*fd, &textBuffer[0], fileSizeInBytes);
                if( result < 0 )
                {
                    ostringstream oss;
                    oss << "Failed to read text(file=" << ipcFilePath << ").";
                    throw runtime_error(oss.str());
                }
            }
            return textBuffer;
        }();
        // 読み終わったファイルを削除する
        {
            /* @note:
                もしも、そもそもファイルが存在してなければ、ここまで処理が到達することはない。
                なので、ファイルの存在チェックとかは不要。
            */
            const int result = unlink(ipcFilePath.c_str());
            if( result != 0 )
            {
                ostringstream oss;
                oss << "Failed to unlink(file=" << ipcFilePath << ").";
                throw runtime_error(oss.str());                
            }
        }
        // 正常終了
        return text;
    }

    /** 読み取り可能なプロセス間通信ファイルを１つ取得する
    */
    std::string GetReadableIpcFile( const std::string& tag )
    {
        // タグディレクトリパスを生成
        const string ipcTagDirPath = [&]()
        {
            return _MakeIpcTagDirPath(tag);
        }();
        // ディレクトリの存在を保証
        {
            _SafeMakeDir( ipcTagDirPath );
        }
        // タグディレクトリを開く
        auto pDir = [&]()
        {
            DIR* const pTemp = opendir(ipcTagDirPath.c_str());
            if( !pTemp )
            {
                ostringstream oss;
                oss << "Failed to open(file=" << ipcTagDirPath << ").";
                throw runtime_error(oss.str());
            }
            return unique_ptr<DIR,decltype(&closedir)>(pTemp,&closedir);
        }();
        // ディレクトリ内の最も過去のファイルを得る
        const string ipcFileName = [&]()
        {
            struct dirent* pEntry = nullptr;
            string oldestName;
            string iterName;
            for(;;)
            {
                // 読み出し
                pEntry = readdir(pDir.get());
                if( !pEntry )
                {
                    break;
                }
                // 更新
                iterName = pEntry->d_name;
                if( (iterName == ".") || (iterName == "..") )
                {
                    continue;
                }
                else if( oldestName.empty() || (iterName < oldestName) )
                {
                    oldestName = iterName;
                }
            }
            return oldestName;
        }();
        // 正常終了
        return ipcFileName;
    }

    /** 指定されたプロセス間通信ファイルが読み取り可能な状態か調べます
    */
    bool IsReadableIpcFile(const std::string& tag, const std::string& ipcFileName)
    {
        // ファイルパスを生成
        const string ipcFilePath = _MakeIpcFilePath(tag, ipcFileName);

        // stat で問い合わせ
        struct stat statData = {};
        {
            const int result = stat(ipcFilePath.c_str(), &statData);
            if( result != 0 )
            {
                return false;
            }
        }
        // 対象が通常ファイルじゃない場合は致命的エラー扱い
        if( !S_ISREG(statData.st_mode) )
        {
            ostringstream oss;
            oss << "There are '" << ipcFileName << "' as non-regular file.";
            oss << "'" << ipcFileName << "' *MUST BE* regular file.";
            throw runtime_error(oss.str());
        }
        // 読める
        return true;
    }
}
