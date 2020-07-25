/*
    動作テスト用ダミープログラム
    cin をオウム返しするだけ。
    exit が来た場合はプログラムを終了。
*/

#include <iostream>

using namespace std;

// エントリーポイント
int main(int argc, char* argv[])
{
    for(;;)
    {
        string buffer;
        cin >> buffer;
        if( buffer == "exit" )
        {
            break;
        }
        cout << "echo: '" << buffer << "'" << endl;
    }
    return 0;
}