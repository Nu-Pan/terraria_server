/*
    stdio to session - Call
*/

#include "module/precompiled_header.h"

#include "module/ipc_util.h"

using namespace std;

// エントリーポイント
int main(int argc, char* argv[])
{
    // セットアップ
    {
        s2s::InitializeIpc();
    }
    // 引数チェック
    if( argc != 2 )
    {
        ostringstream oss;
        oss << "Too many parameters(argc=" << argc << ")." << endl;
        throw runtime_error(oss.str());
    }
    // リクエスト文字列を構築
    const string requetText = [&]()
    { 
        ostringstream oss;
        oss << argv[1] << "\n";
        return oss.str();
    }();
    // リクエストを書き込み
    {
        const string ipcFileName = s2s::WriteIpcFile("up", "", requetText);
        if(ipcFileName == "")
        {
            ostringstream oss;
            oss << "Failed to WriteIpcFile(argc=" << argc << ")." << endl;
            throw runtime_error(oss.str());
        }
    }
    // 気持ち待つ
    {
        usleep( 3 * 1000 );
    }
    // stdout を読み出す
    {
        // 空リクエストを送信
        const string emptyRequestFileName =[&]()
        {
            const string ipcFileName = s2s::WriteIpcFile("up", "", "");
            if(ipcFileName == "")
            {
                ostringstream oss;
                oss << "Failed to WriteIpcFile(argc=" << argc << ")." << endl;
                throw runtime_error(oss.str());
            }
            return ipcFileName;
        }();
        // ファイルが返ってくるのを待つ
        for(int i=0; true; ++i)
        {
            // ファイルの存在をチェック
            if( s2s::IsReadableIpcFile("down", emptyRequestFileName) )
            {
                break;
            }
            // タイムアウト
            if( 2000 < i )
            {
                ostringstream oss;
                oss << "Response timed out(emptyRequestFileName=" << emptyRequestFileName << ")." << endl;
                throw runtime_error(oss.str());
            }
            // スリープ
            {
                usleep(1000);
            }
        }
        // 返ってきたファイルを読み出す
        {
            const string text = s2s::ReadIpcFile("down", emptyRequestFileName);
            cout << text << endl;
        }
    }
    return 0;
}
