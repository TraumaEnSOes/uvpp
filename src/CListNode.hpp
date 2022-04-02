#ifndef CLISTNODE_HPP
#define CLISTNODE_HPP

template< typename T > class CListNode {
public:
    CListNode( ) :
        m_prev( static_cast< T * >( this ) ),
        m_next( static_cast< T * >( this ) )
    {
    }

    void reset( ) noexcept {
        m_prev = this;
        m_next = this;
    }

    bool isUnique( ) const noexcept { return m_prev == this; }

    void unlink( bool makeUnique = false ) noexcept {
        if( !isUnique( ) ) {
            m_prev->m_next = m_next;
            m_next->m_prev = m_prev;

            if( makeUnique ) {
                m_prev = m_next = static_cast< T * >( this );
            }
        }
    }

    void insertBefore( T *newNode ) noexcept {
        m_prev->m_next = newNode;
        newNode->m_prev = m_prev;
        newNode->m_next = static_cast< T * >( this );
        m_prev = newNode;
    }

    T *prev( ) const noexcept { return m_prev; }
    T *next( ) const noexcept { return m_next; }

private:
    T *m_prev;
    T *m_next;
};

#endif
