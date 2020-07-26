/*
    stdio to session - write
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
    // EOF まで繰り返す
    for(;;)
    {
        // テキストを cin から取り出す
        string cinText;
        {
            cin >> cinText;
            if(!cin)
            {
                break;
            }
        }
        // リクエスト文字列の末尾に改行を付加
        string requestText = [&]()
        {
            ostringstream oss;
            oss << cinText << "\n";
            return oss.str();
        }();
        // リクエストを書き込み
        const string ipcFIleName = s2s::WriteIpcFile("up", "", requestText);
        if(ipcFIleName == "")
        {
            ostringstream oss;
            oss << "Failed to WriteIpcFile(argc=" << argc << ")." << endl;
            throw runtime_error(oss.str());
        }
    }
    // 正常終了
    return 0;
}
