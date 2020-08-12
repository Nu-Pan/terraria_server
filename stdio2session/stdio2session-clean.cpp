/*
    stdio to session - clean
*/

//----------------------------------------------------------------------------
// File Dependencies
//----------------------------------------------------------------------------

#include "module/precompiled_header.h"

#include "module/ipc_util.h"

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
    // プロセス間通信ファイルを全部消すだけ
    s2s::ResetIpcFile();
    return 0;
}
