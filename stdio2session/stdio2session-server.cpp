/*
    stdio to session - Server
*/

#include "module/precompiled_header.h"

#include "module/ipc_util.h"

using namespace std;

// エントリーポイント
int main(int argc, char* argv[])
{
    // セットアップ
    s2s::InitializeIpc();

    for(;;)
    {
        // リクエストをチェック
        const string ipcFileName = s2s::GetReadableIpcFile("up");
        if( ipcFileName.empty() )
        {
            sleep(1);
            continue;
        }
        // リクエストの中身をダンプ
        {
            const string text = s2s::ReadIpcFile("up", ipcFileName);
            if( text != "hogehoge" )
            {
                cout << "!";
            }
        }
    }
    return 0;
}
