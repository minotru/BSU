//By Simon Karasik, BSU, FAMCS, 2017.
//Template set data structure on binary search tree.
#ifndef KSET_H
#define KSET_H

#include <algorithm>

#if (__cplusplus <= 199711L) || (__cplusplus <= 201103L)     
//#define KSETCPP11
#endif

#ifdef KSETCPP11
#include <initializer_list>
#else
//#include <cstddef>
#define nullptr 0
#endif

//T must have operator< to be used.
template <typename T>
class KSet
{
    struct Node;
public:
    class iterator;

    KSet() :
        m_root(nullptr), m_size(0),
        m_begin(nullptr), m_end(nullptr) {}
    KSet(const KSet&);
    ~KSet();

    #ifdef KSETCPP11 //c++11 initializer list support.
    KSet(std::initializer_list<T>);
    KSet& operator=(std::initializer_list<T>);
    #endif

    KSet& operator=(const KSet&);

    KSet& insert(const T&);
    KSet& remove(const T&);
    bool contains(const T&) const;
    bool empty() const;
    size_t size() const;
    void clear();
    void swap(KSet&);

    KSet operator+(const KSet&) const; //unite sets
    KSet& operator+=(const KSet&); //unite sets
    KSet operator*(const KSet&) const; //intersect sets
    KSet& operator*=(const KSet&); //intersect sets
    KSet operator/(const KSet&) const; //difference of sets
    KSet& operator/=(const KSet&); //difference of sets
    KSet operator-(const KSet&) const; //simmetric difference:A/B+B/A
    KSet& operator-=(const KSet&); //simmetric difference:A/B+B/A

    KSet operator[](const KSet& /*other*/) const; //maximum subset, that is subset of other
    KSet operator[](const T& /*elemet*/) const; //intersection of set and 1-element set of element
    //KSet operator[](const KSet& /**/); //replace
    //KSet operator[](const T&);

    bool operator==(const KSet&) const;
    bool operator<(const KSet&) const;
    bool operator<=(const KSet&) const;
    bool operator>(const KSet&) const;
    bool operator>=(const KSet&) const;

    iterator begin() const;
    iterator end() const;
private:
    Node *m_root, *m_begin, *m_end;
    size_t m_size;

    Node* find(const T&, Node*&) const;
    Node* find(const T&) const;
    void clear(Node*);
    void zero();
    //if parentNode is nullptr, makes childNode root node.
    void link(Node *parentNode, Node *node);
    //if parentNode is  nullptr, sets root to nullptr.
    void unlink(Node *parentNode, Node *node);
    //void buildList(Node *);
    void linkToList(Node *);
    void unlinkFromList(Node *);
};

template <typename T>
struct KSet<T>::Node
{
    Node *left, *right, *prev, *next;
    T data;

    Node() :
        left(nullptr), right(nullptr),
        prev(nullptr), next(nullptr) {}
    Node(const T& d) :
        data(d), left(nullptr), right(nullptr),
        prev(nullptr), next(nullptr) {}
    Node(const Node* other); //copies node with it's tree
};

template <typename T>
class KSet<T>::iterator
{
    friend class KSet<T>;
public:
    iterator() : m_ptr(nullptr) {}
    iterator(const iterator& other) : m_ptr(other.m_ptr) {}
    iterator& operator=(const iterator& other)
    {
        if (m_ptr != other.m_ptr)
            m_ptr = other.m_ptr;
        return *this;
    }
    iterator& operator++() {m_ptr = m_ptr->next; return *this;}
    iterator operator++(int)
    {
        iterator it(m_ptr);
        m_ptr = m_ptr->next;
        return it;
    }
    const T& operator*() const {return m_ptr->data;}
    T operator*() {return m_ptr->data;}
    bool operator==(const iterator& other) const {return m_ptr == other.m_ptr;}
    bool operator!=(const iterator& other) const {return m_ptr != other.m_ptr;}
private:
    iterator(const Node *node) : m_ptr(node) {}
    const Node *m_ptr;
};

template <typename T>
KSet<T>::KSet(const KSet<T>& other)
{
    zero();
    clear();
    *this = other;
}

template <typename T>
KSet<T>::~KSet()
{
    clear();
}

#ifdef KSETCPP11
template <typename T>
KSet<T>::KSet(std::initializer_list<T> il)
{
    zero();
    *this = il;
}

template <typename T>
KSet<T>& KSet<T>::operator=(std::initializer_list<T> il)
{
    if (!empty())
        clear();
    for (auto it = il.begin(); it != il.end(); it++)
        insert(*it);
    return *this;
}
#endif //C++11

template <typename T>
KSet<T>& KSet<T>::operator=(const KSet<T>& other)
{
    if (this != &other)
    {
        if (!empty())
            clear();
        if (!other.empty())
        {
            m_root = new Node(other.m_root);
            //buildList(m_root);
            for (Node *it = other.m_begin; it; it = it->next)
                linkToList(find(it->data));
            m_size = other.m_size;
        }
    }
    return *this;
}

template <typename T>
KSet<T>& KSet<T>::insert(const T& data)
{
    Node *node, *parent;
    node = find(data, parent);
    if (!node)
    {
        node = new Node(data);
        link(parent, node);
        linkToList(node);
        m_size++;
    }
    return *this;
}

template <typename T>
KSet<T>& KSet<T>::remove(const T& data)
{
    Node *it, *parent;
    it = find(data, parent);
    if (it)
    {
        if (it->left == nullptr && it->right == nullptr)
        {
            unlink(parent, it);
            unlinkFromList(it);
            delete it;
        }
        else if (it->right == nullptr)
        {
            link(parent, it->left);
            unlinkFromList(it);
            delete it;
        }
        else
        {
            Node *leastBigger = it->right;
            while (leastBigger->left)
            {
                parent = leastBigger;
                leastBigger = leastBigger->left;
            }
            it->data = leastBigger->data;
            unlink(parent, leastBigger);
            unlinkFromList(leastBigger);
            delete leastBigger;
        }
        m_size--;
    }
    return *this;
}

template <typename T>
bool KSet<T>::contains(const T& data) const
{
    return find(data) != nullptr;
}

template <typename T>
bool KSet<T>::empty() const
{
    return m_root == nullptr;
}

template <typename T>
size_t KSet<T>::size() const
{
    return m_size;
}

template <typename T>
void KSet<T>::clear()
{
    clear(m_root);
    zero();
}

template <typename T>
void KSet<T>::swap(KSet<T> & other)
{
    std::swap(m_begin, other.m_begin);
    std::swap(m_end, other.m_end);
    std::swap(m_size, other.m_size);
}

template <typename T>
KSet<T> KSet<T>::operator+(const KSet<T>& other) const //unite sets
{
    KSet res = *this;
    for (Node *it = other.m_begin; it != nullptr; it = it->next)
        res.insert(it->data);
    return res;
}
template <typename T>
KSet<T>& KSet<T>::operator+=(const KSet<T>& other) //unite sets
{
    return (*this = *this + other);
}

template <typename T>
KSet<T> KSet<T>::operator*(const KSet<T>& other) const //intersect sets
{
    KSet<T> res;
    for (Node *it = m_begin; it; it = it->next)
        if (other.contains(it->data))
            res.insert(it->data);
    return res;
}


template <typename T>
KSet<T>& KSet<T>::operator*=(const KSet<T>& other) //intersect sets
{
    return (*this = *this * other);
}

template <typename T>
KSet<T> KSet<T>::operator/(const KSet<T>& other) const //difference of sets
{
    KSet<T> res;
    for (Node *it = m_begin; it; it = it->next)
        if (!other.contains(it->data))
            res.insert(it->data);
    return res;
}

template <typename T>
KSet<T>& KSet<T>::operator/=(const KSet<T>& other) //difference of sets
{
    return (*this = *this / other);
}

template <typename T>
KSet<T> KSet<T>::operator-(const KSet<T>& other) const //simmetric difference:A/B+B/A
{
    return (*this/other + other/(*this));
}

template <typename T>
KSet<T>& KSet<T>::operator-=(const KSet<T>& other) //simmetric difference:A/B+B/A
{
    return (*this = *this - other);
}

template<typename T>
KSet<T> KSet<T>::operator[](const KSet& other) const
{
    return *this * other;
}

template<typename T>
KSet<T> KSet<T>::operator[](const T& element) const
{
    KSet set;
    if (contains(element))
        set.insert(element);
    return set;
}

template <typename T>
bool KSet<T>::operator==(const KSet<T>& other) const
{
    return (*this * other).size() == other.size() && size() == other.size();
}

template <typename T>
bool KSet<T>::operator<(const KSet<T>& other) const
{
    return (*this * other).size() == size() && other.size() > size();
}

template <typename T>
bool KSet<T>::operator<=(const KSet<T>& other) const
{
    return (*this * other).size() == size() && other.size() >= size();
}

template <typename T>
bool KSet<T>::operator>(const KSet<T>& other) const
{
    return !(*this <= other);
}

template <typename T>
bool KSet<T>::operator>=(const KSet<T>& other) const
{
    return !(*this < other);
}

template <typename T>
typename KSet<T>::iterator KSet<T>::begin() const
{
    return iterator(m_begin);
}

template <typename T>
typename KSet<T>::iterator KSet<T>::end() const
{
    return iterator(nullptr);
}

template <typename T>
typename KSet<T>::Node* KSet<T>::find(const T& data) const
{
    Node *parent;
    return find(data, parent);
}

template <typename T>
typename KSet<T>::Node* KSet<T>::find(const T& data,
    typename KSet<T>::Node*& parent) const
{
    Node *it = m_root;
    parent = nullptr;
    while (it != nullptr && it->data != data)
    {
        parent = it;
        if (data < it->data)
            it = it->left;
        else
            it = it->right;
    }
    return it;
}

template <typename T>
void KSet<T>::clear(typename KSet<T>::Node *node)
{
    if (node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
void KSet<T>::zero()
{
    m_root = nullptr;
    m_begin = m_end = nullptr;
    m_size = 0;
}

template <typename T>
void KSet<T>::link(typename KSet<T>::Node *parentNode, Node *node)
{
    if (parentNode == nullptr)
        m_root = node;
    else if (node->data < parentNode->data)
        parentNode->left = node;
    else
        parentNode->right = node;
}

template <typename T>
void KSet<T>::unlink(typename KSet<T>::Node *parentNode, typename KSet<T>::Node *node)
{
    if (parentNode == nullptr)
        m_root = nullptr;
    else if (node == parentNode->left)
        parentNode->left = nullptr;
    else
        parentNode->right = nullptr;
}

template <typename T>
void KSet<T>::linkToList(typename KSet<T>::Node *node)
{
    node->next = nullptr;
    if (m_end == nullptr)
    {
        m_begin = m_end = node;
        node->prev = nullptr;
        node->next = nullptr;
    }
    else
    {
        m_end->next = node;
        node->prev =  m_end;
        node->next = nullptr;
        m_end = node;
    }
}

template <typename T>
void KSet<T>::unlinkFromList(typename KSet<T>::Node *node)
{
    if (node == m_begin && node == m_end)
        m_begin = m_end = nullptr;
    else if (node == m_end)
    {
        m_end->prev->next = nullptr;
        m_end = m_end->prev;
    }
    else if (node == m_begin)
    {
        m_begin->next->prev = nullptr;
        m_begin = m_begin->next;
    }
    else
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}

//template <typename T>
//void KSet<T>::buildList(typename KSet<T>::Node *node)
//{
//    /*linkToList(node);
//    if (node->left)
//        linkToList(node->left);
//    if (node->right)
//        linkToList(node->right);*/
//}

template <typename T>
KSet<T>::Node::Node(const typename KSet<T>::Node *other)
{
    data = other->data;
    if (other->left != nullptr)
        left = new Node(other->left);
    else
        left = nullptr;
    if (other->right != nullptr)
        right = new Node(other->right);
    else
        right = nullptr;
}

#endif //KSET_H
