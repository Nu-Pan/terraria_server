/*
    stdio to session - inter process communication util
*/

#pragma once

namespace s2s
{
    /** すべてのプロセス間通信ファイルを削除する
    */
    void ResetIpcFile();

    /** プロセス間通信に必要なセットアップを行う
     */
    void InitializeIpc();

    /** プロセス間通信ファイルにテキストを書き込む
    @param[in]  tag     チャンネル識別タグ
    @param[in]  name    書き込むファイルの名前
                        空文字列を渡した場合は自動決定
    @param[in]  text    ファイルに書き込むテキスト
    @return             書き込んだファイルの名前
    */
    std::string WriteIpcFile( const std::string& tag, const std::string& name, const std::string& text );

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

    /** 指定されたプロセス間通信ファイルが読み取り可能な状態か調べます
    ノンブロッキングです。
    @param[in]  tag         チャンネル識別タグ
    @param[in]  ipcFileName ファイル名
    @retval     false       読み取り不能
    @retval     true        読み取り可能
    */
    bool IsReadableIpcFile(const std::string& tag, const std::string& ipcFileName);
}