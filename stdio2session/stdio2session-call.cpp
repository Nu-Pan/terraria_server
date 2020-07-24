/*
    stdio to session - Call
*/

#include "module/precompiled_header.h"

#include "module/ipc_util.h"

using namespace std;

// エントリーポイント
int main(int argc, char* argv[])
{
    try
    {
        // セットアップ
        s2s::InitializeIpc();
        for(int i=0; i<100; ++i)
        {
            s2s::WriteIpcFile("up", "hogehoge");
        }
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        throw;
    }
    return 0;
}
