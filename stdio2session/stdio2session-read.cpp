/*
    stdio to session - read
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
    // セットアップ
    {
        s2s::InitializeIpc();
    }    
    // あるだけ読み出す
    for(;;)
    {
        // 返信があるかチェック
        const string ipcFileName = s2s::GetReadableIpcFile("down");
        if( ipcFileName == "" )
        {
            break;;
        }
        // 返信を読み出して cout に流す
        {
            const string text = s2s::ReadIpcFile("down", ipcFileName);
            cout << text << flush;
        }
    }
    // 正常終了
    return 0;
}
