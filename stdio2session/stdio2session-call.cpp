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
    s2s::InitializeIpc();

    cout << s2s::WriteIpcFile("up", "hogehoge") << endl;
    return 0;
}
