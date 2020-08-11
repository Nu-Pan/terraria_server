/*
    stdio to session - server
*/

//----------------------------------------------------------------------------
// File Dependencies
//----------------------------------------------------------------------------

#include "module/precompiled_header.h"

#include "module/ipc_util.h"
#include "module/unix_util.h"

//----------------------------------------------------------------------------
// Namespace Dependencies
//----------------------------------------------------------------------------

using namespace std;
using namespace s2s;

//----------------------------------------------------------------------------
// Entry point
//----------------------------------------------------------------------------

// エントリーポイント
int main(int argc, char* argv[])
{
    // セットアップ
    {
        s2s::InitializeIpc();
    }
    // 引数チェック
    if( argc < 2 )
    {
        ostringstream oss;
        oss << "Too few parameters(argc=" << argc << ")." << endl;
        throw runtime_error(oss.str());
    }
    // ラップ対象プロセスを立ち上げ
    pid_t tgtPid;
    UniqueFD tgtPipeR, tgtPipeW;
    {
        CreatePipedProcess(tgtPid, tgtPipeR, tgtPipeW, argv[1], argc<3?nullptr:&argv[2]);
    }
    // 終了フラグ
    atomic_bool doesExit(false);

    // 子プロセス監視スレッド
    thread tgtWaitingThread([&]()
    {
         wait(nullptr); doesExit = true;
    });
    // リクエスト処理スレッド
    thread requestServerThread([&]()
    {
        // メインループ
        for(;;)
        {
            // ビジーループ防止用スリープ
            {
                usleep(1);
            }
            // 子プロセスの終了をチェック
            if( doesExit )
            {
                break;
            }
            // クライアントからのリクエストをチェック
            const string ipcFileName = GetReadableIpcFile("up");
            if( ipcFileName.empty() )
            {
                // 暇なときは少し長めにスリープ
                usleep(1000);
                continue;
            }
            // リクエストの内容をそのままラップ対象プロセスの stdin に流す
            {
                const string request = s2s::ReadIpcFile("up", ipcFileName);
                const int result = write(*tgtPipeW, &request[0], request.size());
                if( result < 0 )
                {
                    ostringstream oss;
                    oss << "Failed to read(result=" << result << ", errno=" << errno << ")." << endl;
                    throw runtime_error(oss.str());
                }
            }
        }
    });
    // stdout リダイレクトスレッド
    thread stdoutRedirectionThread([&]()
    {
        // メインループ
        for(;;)
        {
            // ビジーループ防止用スリープ
            {
                usleep(1);
            }
            // 子プロセスの終了をチェック
            if( doesExit )
            {
                break;
            }
            // ラップ対象プロセスから stdout を読み取る
            string readBuffer;
            {
                // 読み出しを試行
                readBuffer.resize( 64 * 1024 );
                const int result = read(*tgtPipeR, &readBuffer[0], readBuffer.size());
                if( (result < 0) && (errno != EAGAIN) )
                {
                    ostringstream oss;
                    oss << "Failed to read(result=" << result << ", errno=" << errno << ")." << endl;
                    throw runtime_error(oss.str());
                }
                // データなしの場合は何もしない
                if( result < 0 )
                {
                    // 暇なときは少し長めにスリープ
                    usleep(1000);
                    continue;
                }
                // 実際に読めたサイズでバッファをリサイズ
                {
                    readBuffer.resize( max(result, 0) );
                }
            }
            // 読み取った内容をクライアントに返送する
            {
                WriteIpcFile("down", "", readBuffer);
            }
        }
    });
    // スレッドを合流
    {
        tgtWaitingThread.join();
        requestServerThread.join();
        stdoutRedirectionThread.join();
    }
    // 正常終了
    return 0;
}
