
#pragma once

//----------------------------------------------------------------------------
// UniqueFD
//----------------------------------------------------------------------------
namespace s2s
{
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

        // int へのキャスト
        explicit operator int() const
        {
            return m_fd;            
        }
    private:
        int m_fd;
    };
}
