/*
    stdio to session - interpreter
*/

#include "module/precompiled_header.h"

#include "module/ipc_util.h"

using namespace std;

// エントリーポイント
int main(int argc, char* argv[])
{
    // セットアップ
    {
        s2s::InitializeIpc();
    }

    for(;;)
    {
        // リクエストを cin から取り出す
        string requestText;
        {
            cout << ">" << flush;
            cin >> requestText;
            if(!cin)
            {
                break;
            }
        }
        // リクエストを書き込み
        const string ipcFIleName = s2s::WriteIpcFile("up", "", requestText);
        if(ipcFIleName == "")
        {
            ostringstream oss;
            oss << "Failed to WriteIpcFile(argc=" << argc << ")." << endl;
            throw runtime_error(oss.str());
        }
        cout << ipcFIleName << ":" << requestText;

        // 空文字リクエストの場合は返信を得る
        if( requestText.empty() )
        {
            sleep( 1 );
            const string text = s2s::ReadIpcFile("down", ipcFIleName);
            cout << text << flush;
        }
    }
    return 0;
}
