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

    {
        // リクエストをチェック
        const string ipcFileName = s2s::GetReadableIpcFile("up");
        if( ipcFileName.empty() )
        {
            return 1;
        }
        // リクエストの中身をダンプ
        {
            cout << ipcFileName << ":" << endl;
            cout << s2s::ReadIpcFile("up", ipcFileName) << endl;
        }
    }

    return 0;
}
