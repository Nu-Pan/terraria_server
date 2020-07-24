/*
    stdio to session - Peek
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
    }
    catch(const std::exception& e)
    {
        cerr << e.what() << endl;
        throw;
    }
    return 0;
}
