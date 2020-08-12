/*
    unix util
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
// Functions
//----------------------------------------------------------------------------
namespace s2s
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
                dup2(*pipeDR, 0);
                dup2(*pipeUW, 1);
                pipeDR.Reset();
                pipeUW.Reset();
            }
            // プロセス置き換え
            const int result = execvp(cmd, argv);
            if( result < 0 )
            {
                ostringstream oss;
                oss << "failed to execvp(result=" << result << ", errno=" << errno << ")." << endl;
                throw runtime_error(oss.str());
            }
            /* @note:
                execvp でプロセスが置き換わるので、コードパスはここまで来ない。
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
