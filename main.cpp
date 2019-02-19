#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
using std::ostream;

template <typename T>
class List;

template <typename T>
void swap(List<T> &fst, List<T> &snd) noexcept;

template <typename T>
class List {
public:
#pragma pack(push,4)
    struct Node {
        T data;
        Node *pNext;
        Node(T v, Node *pn) : data{v}, pNext{pn} {}
    };
#pragma pack(pop)
    List() : head_{nullptr}, size_{0} {}
    List(std::initializer_list<T> iLst);
    List(const List<T>& other);
    List(List<T>&& other);
    ~List();
    List<T>& operator=(const List<T>& other);
    List<T>& operator=(List<T>&& other);
    void pushFront(T elem) { head_ = new Node(elem, head_); this->size_++; }
    void pushBack(T elem);
    void popFront();
    void popBack();
    void eraseAll();
    void reverse();
    bool contains(T elem) const;
    std::string toString() const;
    size_t size() const { return size_; }
    // ...
private:
    Node *head_ = nullptr;
    size_t size_ = 0;
    friend void swap<T>(List<T> &fst, List<T> &snd) noexcept;
};

template <typename T>
void swap(List<T> &fst, List<T> &snd) noexcept
{
    std::swap(fst.head_, snd.head_);
    std::swap(fst.size_, snd.size_);
}

template <typename T>
ostream& operator<<(ostream& os, const List<T>& lst)
{
    os << lst.toString();
    return os;
}

template <typename T>
List<T>::List(std::initializer_list<T> iLst)
{
    for (auto e : iLst) { pushFront(e); }
    this->reverse();
}

template <typename T>
List<T>::List(const List<T>& other)
{
    for (Node *pNode = other.head_; pNode != nullptr; pNode = pNode->pNext) {
        pushFront(pNode->data);
    }
    reverse();
}

template <typename T>
List<T>::List(List<T>&& other)
{
    swap(*this, other);
}

template <typename T>
List<T>::~List()
{
    if (size_ > 1) { eraseAll(); }
}

template <typename T>
List<T>& List<T>::operator=(const List<T>& other)
{
    List<T> copy {other};
    swap<T>(*this, copy);
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List<T>&& other)
{
    swap<T>(*this, other);
    return *this;
}

template <typename T>
void List<T>::pushBack(T elem)
{
    Node *pNode = head_;
    if (pNode != nullptr) {
        while (pNode->pNext != nullptr) { pNode = pNode->pNext; }
        pNode->pNext = new Node(elem, nullptr);
    } else {
        head_ = new Node(elem, nullptr);
    }
    size_++;
}

template <typename T>
void List<T>::popFront()
{
    if (head_ != nullptr) {
        Node *pNode = head_;
        head_ = head_->pNext;
        delete pNode;

        size_--;
    }
}

template <typename T>
void List<T>::popBack()
{
    if (head_ != nullptr) {
        if (size_ == 1) {
            delete(head_);
            head_ = nullptr;
        } else { // size_ > 1
            Node *pNode = head_, *pPrev = nullptr;
            while (pNode->pNext != nullptr) {
                pPrev = pNode;
                pNode = pNode->pNext;
            }

            delete(pNode);
            pPrev->pNext = nullptr; // size_ > 1 => pPrev != nullptr
        }
        size_--;
    }
}

template <typename T>
void List<T>::eraseAll()
{
    while (size_ > 0) popFront();
}

template <typename T>
void List<T>::reverse()
{
    Node *pPrevNode = nullptr, *pNode = this->head_, *pNextNode;
    while (pNode != nullptr) {
        // swap/reconnect
        pNextNode = pNode->pNext;
        pNode->pNext = pPrevNode;

        // move to the next node
        pPrevNode = pNode;
        pNode = pNextNode;
    }
    this->head_ = pPrevNode;
}

template <typename T>
bool List<T>::contains(T elem) const
{
    for (Node *pNode = this->head_; pNode != nullptr; pNode = pNode->pNext) {
        if (pNode->data == elem) { return true; }
    }
    return false;
}

template <typename T>
std::string List<T>::toString() const
{
    ostringstream sstr;
    sstr << "[";
    for (Node *pNode = this->head_; pNode != nullptr; pNode = pNode->pNext) {
        sstr << pNode->data;
        if (pNode->pNext != nullptr) { sstr << ','; }
    }
    sstr << "]";
    return sstr.str();
}

#endif // LIST_H



#ifndef BST_H
#define BST_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using std::cout;
using std::endl;
using std::ostringstream;
using std::ostream;
using std::vector;
using std::string;

// 1st options
template <typename T> class BST;
template <typename T> std::ostream& operator<<(std::ostream&, const BST<T>&);
//

template <typename T>
class BST
{
public:
    BST& insert(T elem);
    bool isElem(T e) const;
    vector<T> toVector() const;
    void removeAll();

    BST() = default;
    BST(const BST&) = delete;
    BST(BST&&) = delete;
    ~BST();
    BST& operator=(const BST&) = delete;
    BST& operator=(BST&&) = delete;

    BST& operator+=(T elem);

private:
    struct Node
    {
        Node(T d, Node *lhs, Node *rhs) : data{d}, pLeft{lhs}, pRight{rhs} {}
        ~Node() { cout << "Deletion of: " << this->data << endl; }
        //
        T data;
        Node *pLeft;
        Node *pRight;
    };

    // helper functions
    Node* insertAsSubtreeOf(Node* node, T elem);
    void toStringStream(Node* node, ostringstream &sstr) const;
    void toVectorFrom(Node* node, vector<T> &v) const;
    bool isElemOf(Node* node, T e) const;
    void removeAllFrom(Node* node);

    Node *pRoot_ = nullptr;
    static constexpr char sep_ = ',';

    // 1st option
    friend ostream& operator<< <T>(ostream&, const BST<T>&);

    // 2nd option (note: operator<< for double is a friend of operator<< for int)
    //template <typename U>
    //friend ostream& operator<< (ostream&, const BST<U>&);
};

template <typename T>
ostream& operator<<(ostream& os, const BST<T>& bst)
{
    ostringstream sstr;
    bst.toStringStream(bst.pRoot_, sstr);

    string s = sstr.str();
    if (s.back() == bst.sep_) { s.pop_back(); }

    os << "[" << s << "]";
    return os;
}

template <typename T>
BST<T>& BST<T>::insert(T elem)
{
    if (pRoot_ == nullptr) {
        pRoot_ = new BST<T>::Node(elem, nullptr, nullptr);
    } else {
        insertAsSubtreeOf(pRoot_, elem);
    }

    return *this;
}

template <typename T>
bool BST<T>::isElem(T e) const
{
    return isElemOf(pRoot_, e);
}

template <typename T>
vector<T> BST<T>::toVector() const
{
    if (this->pRoot_ == nullptr) {
        return vector<T>(0);
    } else {
        vector<T> vr;
        toVectorFrom(pRoot_, vr);
        return vr;
    }
}

template <typename T>
void BST<T>::removeAll()
{
    removeAllFrom(pRoot_);
    pRoot_ = nullptr;
}

template <typename T>
BST<T>::~BST()
{
    removeAll();
}

template <typename T>
BST<T>& BST<T>::operator+=(T elem)
{
    return insert(elem);
}

template <typename T>
typename BST<T>::Node* BST<T>::insertAsSubtreeOf(BST<T>::Node* node, T elem)
{
    if (node == nullptr) {
        return new BST<T>::Node(elem, nullptr, nullptr);
    } else {
        if (elem <= node->data) {
            node->pLeft = insertAsSubtreeOf(node->pLeft, elem);
            return node;
        }
        else {
            node->pRight = insertAsSubtreeOf(node->pRight, elem);
            return node;
        }
    }
}

template <typename T>
void BST<T>::toStringStream(BST<T>::Node* node, ostringstream &sstr) const
{
    if (node != nullptr) {
        // in-order
        toStringStream(node->pLeft, sstr);
        sstr << node->data;
        sstr << sep_;
        toStringStream(node->pRight, sstr);
    }
}

template <typename T>
void BST<T>::toVectorFrom(BST<T>::Node* node, vector<T> &v) const
{
    if (node != nullptr) {
        // in-order
        toVectorFrom(node->pLeft, v);
        v.push_back(node->data);
        toVectorFrom(node->pRight, v);
    }
}

template <typename T>
bool BST<T>::isElemOf(Node* node, T e) const
{
    if (node == nullptr) { return false; }
    else {
        if (e == node->data) {
            return true;
        } else if (e <= node->data) {
            return isElemOf(node->pLeft, e);
        } else {
            return isElemOf(node->pRight, e);
        }
    }
}

template <typename T>
void BST<T>::removeAllFrom(Node* node)
{
    if (node != nullptr) {
        removeAllFrom(node->pLeft);
        removeAllFrom(node->pRight);
        delete node;
    }
}

#endif // BST_H
