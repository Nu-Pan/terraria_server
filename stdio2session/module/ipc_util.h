
#pragma once

namespace s2s
{
    /** プロセス間通信に必要なセットアップを行う
     */
    void InitializeIpc();

    /** プロセス間通信ファイルにテキストを書き込む
    @param[in]  tag     チャンネル識別タグ
    @param[in]  text    ファイルに書き込むテキスト
    @return             書き込んだファイルの名前
    */
    std::string WriteIpcFile( const std::string& tag, const std::string& text );

    /**　プロセス間通信ファイルからテキストを読み出す
    読み出したファイルは削除される。
    @param[in]  tag         チャンネル識別タグ
    @param[in]  ipcFileName 読み込み対象ファイルの名前
    @return                 読み込んだテキスト
    */
    std::string ReadIpcFile( const std::string& tag, const std::string& ipcFileName );

    /** 読み取り可能なプロセス間通信ファイルを１つ取得する
    tag が一致するファイルのみ取得する
    @param[in]  tag チャンネル識別タグ
    @return         ファイル名
                    ファイルがない場合は空白文字
    */
    std::string GetReadableIpcFile( const std::string& tag );
}