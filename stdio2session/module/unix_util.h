/*
    unix util
*/
#pragma once

//----------------------------------------------------------------------------
// UniqueFD
//----------------------------------------------------------------------------
namespace s2s
{
    /**
    @brief  ファイルディスクリプタ用スマートポインタ
    std::unique_ptr みたいな挙動をする
    */
    class UniqueFD
    {
    public:
        // デフォルトコンストラクタ
        UniqueFD()
        :m_fd(-1)
        {
            // nop
        }

        // コンストラクタ
        UniqueFD(int fd)
        :m_fd(fd)
        {
            // nop
        };

        // コピーコンストラクタ
        UniqueFD( const UniqueFD& ) = delete;

        // ムーブコンストラクタ
        UniqueFD( UniqueFD&& rho )
        : m_fd(rho.m_fd)
        {
            rho.m_fd = -1;
        }

        // デストラクタ
        ~UniqueFD()
        {
            Reset(-1);
        }

        // コピー演算子
        UniqueFD& operator =( const UniqueFD& ) = delete;

        // ムーブ代入演算子
        UniqueFD& operator =( UniqueFD&& rho )
        {
            std::swap(this->m_fd, rho.m_fd);
            return *this;
        }

        // リセット
        void Reset( int fd = -1)
        {
            if( m_fd != -1 )
            {
                close(m_fd);
            }
            m_fd = fd;
        }

        // デリファレンス
        int operator *() const
        {
            return m_fd;
        }

    private:
        int m_fd;   //!< ファイルディスクリプタ
    };
}

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------
namespace s2s
{
    /**
    @brief  パイプ付きでプロセスを立ち上げる
    @param[out] outPid      立ち上げたプロセスの ID
    @param[out] outPipeR    立ち上げたプロセスとつながっているパイプ（読み込み）
    @param[out] outPipeW    立ち上げたプロセスとつながっているパイプ（書き込み）
    @param[in]  cmd         実行コマンド
    @param[in]  argv        実行コマンド引数
    */
    void CreatePipedProcess(pid_t& outPid, UniqueFD& outPipeR, UniqueFD& outPipeW, const char* cmd, char* const argv[]);
}