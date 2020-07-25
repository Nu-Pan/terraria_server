/*
    stdio to session - Server
*/

#include "module/precompiled_header.h"

#include "module/ipc_util.h"
#include "module/unix_util.h"

using namespace std;
using namespace s2s;

//----------------------------------------------------------------------------
// Link-local Functions
//----------------------------------------------------------------------------
namespace
{
    // パイプ付きでプロセスを立ち上げる
    void CreatePipedProcess(pid_t& outPid, UniqueFD& outPipeR, UniqueFD& outPipeW, const char* cmd, char* const argv[])
    {
        // パイプを生成
        UniqueFD pipeDR, pipeDW;
        UniqueFD pipeUR, pipeUW;
        {
            /* @note:
                ４種類あるディスクリプタの図解

                        Down Stream
                        DW --> DR
                Parent              Child
                        UR <-- UW
                        Up Stream

                Parent は UR と DW を触る。
                Child は DR と UW を触る。
            */
            auto createPipe = [](UniqueFD& outPipeR, UniqueFD& outPipeW, bool isNonBlock )
            {
                int tempPipe[2];
                const int result = pipe2(tempPipe, isNonBlock?O_NONBLOCK:0);
                if( result < 0 )
                {
                    ostringstream oss;
                    oss << "Failed to pipe(result=" << result << ", errno" << errno << ")." << endl;
                    throw runtime_error(oss.str());
                }
                outPipeR = tempPipe[0];
                outPipeW = tempPipe[1];
            };
            createPipe(pipeDR, pipeDW, false);
            createPipe(pipeUR, pipeUW, true);
        }
        // fork
        const pid_t pid = fork();
        if( pid < 0 )
        {
            ostringstream oss;
            oss << "failed to fork(argc=" << pid << ", errno=" << errno << ")." << endl;
            throw runtime_error(oss.str());
        }
        // 自身が子プロセスの場合、自身をラップ対象プロセスで置き換える。
        if( pid == 0 )
        {
            // パイプ設定
            {
                pipeUR.Reset();
                pipeDW.Reset();
                dup2(static_cast<int>(pipeDR), 0);
                dup2(static_cast<int>(pipeUW), 1);
                pipeDR.Reset();
                pipeUW.Reset();
            }
            // プロセス置き換え
            const int result = execv(cmd, argv);
            if( result < 0 )
            {
                ostringstream oss;
                oss << "failed to execv(result=" << result << ", errno=" << errno << ")." << endl;
                throw runtime_error(oss.str());
            }
            /* @note:
                execv でプロセスが置き換わるので、コードパスはここまで来ない。
            */
        }
        // 自分が親プロセスの場合はこのまま続行
        pipeDR.Reset();
        pipeUW.Reset();
        outPid = pid;
        outPipeR = move(pipeUR);
        outPipeW = move(pipeDW);
        return;
    }
}

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
                    const int result = read(static_cast<int>(tgtPipeR), &readBuffer[0], readBuffer.size());
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
                const int result = write(static_cast<int>(tgtPipeW), &request[0], request.size());
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
