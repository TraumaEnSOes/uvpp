#ifndef CLISTNODE_HPP
#define CLISTNODE_HPP

class CListNode {
public:
    CListNode( ) :
        m_prev( this ),
        m_next( this )
    {
    }

    bool isUnique( ) const noexcept { return m_prev == this; }

    void unlink( bool makeUnique = false ) noexcept {
        if( !isUnique( ) ) {
            m_prev->m_next = m_next;
            m_next->m_prev = m_prev;

            if( makeUnique ) {
                m_prev = m_next = this;
            }
        }
    }

    void insertBefore( CListNode *newNode ) noexcept {
        m_prev->m_next = newNode;
        newNode->m_prev = m_prev;
        newNode->m_next = this;
        m_prev = newNode;
    }

    CListNode *prev( ) const noexcept { return m_prev; }
    CListNode *next( ) const noexcept { return m_next; }

private:
    CListNode *m_prev;
    CListNode *m_next;
};

#endif
