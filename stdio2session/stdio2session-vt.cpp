/*
    stdio to session - virtual terminal
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
    // 終了フラグ
    atomic_bool doesTerminate(false);
    
    // cin 処理スレッド
    thread inputThread([&]()
    {
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
        // 終了フラグを立てる
        {
            sleep(1);
            doesTerminate = true;
        }
        // 正常終了
        return;
    });
    // cout 処理スレッド
    thread outputThread([&]()
    {
        for(;;)
        {
            // スリープ
            {
                usleep(1000);
            }
            // 終了フラグをチェック
            if(doesTerminate)
            {
                break;
            }
            // 返信があるかチェック
            const string ipcFileName = s2s::GetReadableIpcFile("down");
            if( ipcFileName == "" )
            {
                continue;
            }
            // 返信を読み出して cout に流す
            {
                const string text = s2s::ReadIpcFile("down", ipcFileName);
                cout << "> " << text << flush;
            }
        }
        // 正常終了
        return;
    });
    // スレッド終了を待機
    {
        inputThread.join();
        outputThread.join();
    }
    return 0;
}
