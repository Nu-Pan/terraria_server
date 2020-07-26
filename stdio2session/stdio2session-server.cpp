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
    // 子プロセス監視スレッドを立ち上げる
    atomic_bool doesExit(false);
    thread tgtWaitingThread([&](){ wait(nullptr); doesExit = true; });

    // メインループ
    for(;;)
    {
        // 子プロセスの終了をチェック
        if( doesExit )
        {
            break;
        }
        // クライアントからのリクエストを取得
        const string ipcFileName = GetReadableIpcFile("up");
        if( ipcFileName.empty() )
        {
            sleep(1);
            continue;
        }
        // リクエストを処理
        {
            const string request = s2s::ReadIpcFile("up", ipcFileName);
            if( request.empty() )
            {
                // ラップ対象プロセスから stdout を読み取る
                string readBuffer;
                {
                    readBuffer.resize( 64 * 1024 );
                    const int result = read(*tgtPipeR, &readBuffer[0], readBuffer.size());
                    if( (result < 0) && (errno != EAGAIN) )
                    {
                        ostringstream oss;
                        oss << "Failed to read(result=" << result << ", errno=" << errno << ")." << endl;
                        throw runtime_error(oss.str());
                    }
                    readBuffer.resize( max(result, 0) );
                }
                // 読み取った内容をクライアントに返送する
                {
                    WriteIpcFile("down", ipcFileName, readBuffer);
                }
            }
            else
            {
                // リクエストの内容をそのままラップ対象プロセスの stdin に流す
                const int result = write(*tgtPipeW, &request[0], request.size());
                if( result < 0 )
                {
                    ostringstream oss;
                    oss << "Failed to read(result=" << result << ", errno=" << errno << ")." << endl;
                    throw runtime_error(oss.str());
                }
            }
        }
    }
    // スレッドを合流
    {
        tgtWaitingThread.join();
    }
    // 正常終了
    return 0;
}
