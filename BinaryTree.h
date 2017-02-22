/*
Binary tree data structure.
By Simon Karasik, BSU, FAMCS, 2017.
*/

template <typename T>
class BinaryTree
{
public:
    BinaryTree();
    ~BinaryTree();

private:
    struct Node
    {
        Node *parent, *left, *right;
        T data;
    } m_root;
};

