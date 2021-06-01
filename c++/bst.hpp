#include <iostream>
#include <memory>
#include <utility>
#include <string>
#include <vector>
#include <iterator>

// NODE =================================================================================

template <typename T>
struct Node{

    T data;
    Node* parent;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    //++++++++++ CTOR AND DESTRUCTOR ++++++++++++

    // custom ctor to create a void object root when we istantiate an object of bst
    Node() noexcept
    : data{}, parent{nullptr}, left{nullptr}, right{nullptr} {
        #ifdef __DEBUG
        std::cout << "CALL: Node - calling custom ctor (root)" << std::endl;
        #endif
    }

    // custom ctor with l-reference
    Node(const T& data, Node* parent)
    : data{data}, parent{parent}, left{nullptr}, right{nullptr} {
        #ifdef __DEBUG
        std::cout << "CALL: Node - calling custom ctor (with l-reference)" << std::endl;
        #endif
    }
    
    // custom ctor with r-reference
    Node(T&& data, Node* parent)
    : data{data}, parent{parent}, left{nullptr}, right{nullptr} {
        #ifdef __DEBUG
        std::cout << "CALL: Node - calling custom ctor (with r-reference)" << std::endl;
        #endif
    }

    // copy semantics
    Node(const Node& node) = delete; // don't want two nodes with same key
    Node& operator=(const Node& node) = delete;

    //move semantics
    Node(Node&& node) = delete;
    Node& operator=(Node&& node) = delete;

    ~Node() noexcept = default;

};


//----------- OPERATORS -------------------------

template <typename T>
std::ostream& operator<<(std::ostream& os, Node<T>& node){
    //os << "[" << node.data.first << "," << node.data.second << "]";
    os << node.data.first;
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Node<T>& node){
    //os << "[" << node.data.first << "," << node.data.second << "]";
    os << node.data.first;
    return os;
}

template <typename N>
bool operator==(const Node<N>& lhs, const Node<N>& rhs) { return lhs.data.first == rhs.data.first; }

template <typename N>
bool operator!=(const Node<N>& lhs, const Node<N>& rhs) { return !(lhs==rhs); }


// BST ==================================================================================

template <typename KT, typename VT, typename OP = std::less<KT>>
class bst{

    using T = std::pair<const KT, VT>;

    OP op {};
    std::size_t _size {0};
    std::unique_ptr<Node<T>> root;

  public:

    template <typename N>
    struct Iterator;

    using iterator = Iterator<Node<T>>;
    using const_iterator = Iterator<const Node<T>>;

    //++++++++++ CTOR AND DESTRUCTOR ++++++++++++

    bst() noexcept : op{}, root{nullptr} {// custom ctor
        #ifdef __DEBUG
        std::cout << "CALL: bst - calling custom ctor (with no args)" << std::endl;
        #endif
    } 
    explicit bst(OP x) : op{x}, root{nullptr} { // custom ctor
        #ifdef __DEBUG
        std::cout << "CALL: bst - calling custom ctor (args: op)" << std::endl;
        #endif
    }

    ~bst() noexcept = default; // deafault destructor

    //++++++++++ COPY SEMANTICS +++++++++++++++++
    
    // auxiliary function to recursively deep-copy
    void _copy(const bst& b, std::unique_ptr<Node<T>>& ptr){
    insert(ptr->data);
    if (ptr->left)
        _copy(b, ptr->left);
    if (ptr->right)
        _copy(b, ptr->right);
    return;
}
    
    // copy ctor
    bst(const bst& t)
    : op{t.op},
      _size{t._size},
      root{std::unique_ptr<Node<T>>(new Node<T>(t.root->data, nullptr))}  
    {
        #ifdef __DEBUG
        std::cout << "CALL: bst - calling copy ctor" << std::endl;
        #endif
        if (t.root->left)          
            _copy(t, t.root->left);
        if (t.root->right)          
            _copy(t, t.root->right);
    }

    // copy assignment
    bst& operator=(const bst& b){
        #ifdef __DEBUG
        std::cout << "CALL: bst - calling copy assignment" << std::endl;
        #endif
        clear();
        auto tmp = b;
        *this = std::move(tmp);
        return *this;
    }

    //++++++++++ MOVE SEMANTICS +++++++++++++++++

    bst(bst&& t) noexcept = default; // move ctor (default is fine - no raw ptr)
    bst& operator=(bst&& t) noexcept = default; // move assignment (default is fine - no raw ptr)
    

    //++++++++++ BEGIN ++++++++++++++++++++++++++

    // _begin()
    const Node<T>* _begin() const noexcept;
    Node<T>* _begin() noexcept;    

    // begin() and cbegin()
    iterator begin() noexcept { return iterator{_begin()}; }
    const_iterator begin() const noexcept { return const_iterator{_begin()}; }
    const_iterator cbegin() const noexcept { return const_iterator{_begin()}; }

    //++++++++++ END ++++++++++++++++++++++++++++

    // _end()
    const Node<T>* _end() const noexcept { return nullptr; }
    Node<T>* _end() noexcept { return nullptr; }

    // end() and cend()
    iterator end() noexcept { return iterator{_end()}; }
    const_iterator end() const noexcept { return const_iterator{_end()}; }
    const_iterator cend() const noexcept { return const_iterator{_end()}; }   


    //++++++++++ INSERT +++++++++++++++++++++++++

    template <typename O>
    std::pair<iterator, bool> _insert(O&& x);

    std::pair<iterator, bool> insert(const T& x){
        #ifdef __DEBUG
        std::cout << "CALL: bst - insert (l)" << std::endl;
        #endif
        return _insert(x);
    }
    std::pair<iterator, bool> insert(T&& x){
        #ifdef __DEBUG
        std::cout << "CALL: bst - insert (r)" << std::endl;
        #endif
        return _insert(std::move(x));
    }

    //++++++++++ EMPLACE ++++++++++++++++++++++++

    template< class... Types >
    std::pair<iterator,bool> emplace(Types&&... args){
        #ifdef __DEBUG
        std::cout << "CALL: bst - emplace" << std::endl;
        #endif
        return insert(T{std::forward<Types>(args)...});
    }

    //++++++++++ CLEAR ++++++++++++++++++++++++++

    void clear() noexcept{
        #ifdef __DEBUG
        std::cout << "CALL: bst - clear" << std::endl;
        #endif
        root.reset();
        _size = 0;
    }

    //+++++++++ FIND ++++++++++++++++++++++++++++

    iterator find(const KT& x) noexcept{
        #ifdef __DEBUG
        std::cout << "CALL: bst - calling find" << std::endl;
        #endif
        auto tmp = root.get();
        while (tmp){
            if (op(x,tmp->data.first)) // x is in the left branch
                tmp = tmp->left.get();
            else if (op(tmp->data.first,x))
                tmp = tmp->right.get();
            else{
                return iterator(tmp);
            }
        }
        return iterator(nullptr);
    }

    const_iterator find(const KT& x) const noexcept{
        #ifdef __DEBUG
        std::cout << "CALL: bst - calling find" << std::endl;
        #endif
        auto tmp = root.get();
        while (tmp){
            if (op(x,tmp->data.first)) // x is in the left branch
                tmp = tmp->left.get();
            else if (op(tmp->data.first,x))
                tmp = tmp->right.get();
            else
                return iterator(tmp);
        }
        return iterator(nullptr);
    }
    
    //++++++++ BALANCE ++++++++++++++++++++++++++

    void balance();
    void _balance(std::vector<T>& v, long long int begin, long long int end);

    //++++++++ SUBSCRIPTING +++++++++++++++++++++

    VT& operator[](const KT& x) noexcept;
    VT& operator[](KT&& x) noexcept;


    //++++++++ PUT-TO +++++++++++++++++++++++++++

    friend
    std::ostream& operator<<(std::ostream& os, const bst& b){
        for (auto& x : b)
            os << x << " ";
        return os;
    }

    //++++++++ _SWAP_NODES ++++++++++++++++++++++

    void _swap_nodes(Node<T>& x, Node<T>& y, iterator& iter) noexcept;

    //++++++++ ERASE ++++++++++++++++++++++++++++

    void erase(const KT& x) noexcept;


    //+++++++ PRINT +++++++++++++++++++++++++++++
    void Print() noexcept;

    void _add_to_string(int level, const Node<T>* ptr, std::vector<std::string>& s) noexcept;
       

};


// ITERATOR =============================================================================

template <typename KT, typename VT, typename OP>
template<typename N>
struct bst<KT, VT, OP>::Iterator{

    N* current_ptr;

  public:
    using value_type = N;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type&;
    using pointer = value_type*;

    //++++++++++ CTOR AND DESTRUCTOR ++++++++++++

    explicit Iterator(value_type* current_ptr)
    : current_ptr{current_ptr} {
    #ifdef __DEBUG
    std::cout << "CALL: Iterator - calling custom ctor" << std::endl;
    #endif
    } // custom ctor

    ~Iterator() noexcept = default;

    //++++++++++ OPERATORS ++++++++++++++++++++++

    Iterator& operator++() noexcept; // pre-increment operator

    reference operator*() const { return *current_ptr; } // dereference operator
    pointer operator->() const noexcept { return &**this; } // arrow operator


    friend
    bool operator==(const bst<KT,VT,OP>::Iterator<N>& lhs, const bst<KT,VT,OP>::Iterator<N>& rhs) noexcept {
        return lhs.current_ptr == rhs.current_ptr; }

    friend
    bool operator!=(const bst<KT,VT,OP>::Iterator<N>& lhs, const bst<KT,VT,OP>::Iterator<N>& rhs) noexcept {
        return !(lhs==rhs); }
    
};

//=======================================================================================


template <typename KT, typename VT, typename OP>
using iterator = typename bst<KT,VT,OP>::iterator;

template <typename KT, typename VT, typename OP>
using const_iterator = typename bst<KT,VT,OP>::const_iterator;


// ITERATOR FUNCTION DEFINITIONS

//----------- OPERATOR ++ -----------------------

template <typename KT, typename VT, typename OP>
template <typename N>
bst<KT, VT, OP>::Iterator<N>& bst<KT,VT,OP>::Iterator<N>::operator++() noexcept{ // pre-incrememnt enough for forward iterator
    if (!(current_ptr)){ // if it is a nullptr
        return *this;
    }else if (current_ptr->right){ // the successor is the furthermost left node in the right subtree
        current_ptr = current_ptr->right.get();
        while (current_ptr->left)
            current_ptr = current_ptr->left.get();
    }else{
        value_type* tmp = current_ptr->parent;
        while (tmp && current_ptr == tmp->right.get()){
            current_ptr = tmp;
            tmp = tmp->parent;
        }
        current_ptr = tmp;
    }
    return *this;            
}


// BST FUNCTION DEFINITION

//----------- BEGIN -----------------------------

// _begin()
template <typename KT, typename VT, typename OP>
const Node<std::pair<const KT, VT>>* bst<KT,VT,OP>::_begin() const noexcept { 
    if (!(root)){
        std::cout << "The tree is empty";
        return nullptr;
    }
    auto tmp = root.get();     
    while (tmp->left){
        tmp = tmp->left.get();
    }
    return tmp;
}    

template <typename KT, typename VT, typename OP>
Node<std::pair<const KT, VT>>* bst<KT,VT,OP>::_begin() noexcept {
    if (!(root)){
        std::cout << "The tree is empty";
        return nullptr;
    }
    auto tmp = root.get();        
    while (tmp->left)
        tmp = tmp->left.get();
    return tmp;
}    

//----------- INSERT ----------------------------

template <typename KT, typename VT, typename OP>
template <typename O>
std::pair<iterator<KT,VT,OP>, bool> bst<KT,VT,OP>::_insert(O&& x) {
    auto tmp = root.get();
    while (tmp){
        if (op(x.first,tmp->data.first)){
            if (tmp->left)
                tmp = tmp->left.get();
            else{
                tmp->left.reset(new Node<T>{std::forward<O>(x), tmp});
                _size++;
                #ifdef __DEBUG
                std::cout << "Inserting node with key " << x.first << " and parent " << *tmp << std::endl;
                #endif
                return std::pair<iterator, bool>{iterator{tmp->left.get()}, true};
            }
        }
        else if (!(op(x.first,tmp->data.first))){
            if (tmp->right)
                tmp = tmp->right.get();
            else{
                tmp->right.reset(new Node<T>{std::forward<O>(x), tmp});
                _size++;
                #ifdef __DEBUG
                std::cout << "Inserting node with key " << x.first << " and parent " << *tmp << std::endl;
                #endif
                return std::pair<iterator, bool>{iterator{tmp->right.get()}, true};
            }
        }
        else //( !op(new_key,current_node.key) && ( !op(current_node.key,new_key)))
            return std::pair<iterator, bool>{iterator{tmp}, false}; // key already exists!
    }
    root.reset(new Node<T>{std::forward<O>(x), nullptr});
    _size++;
    #ifdef __DEBUG
    std::cout << "Inserted root node with key " << x.first << std::endl;
    #endif
    return std::pair<iterator, bool>{iterator{tmp}, true};
}
    

//----------- BALANCE ---------------------------

template <typename KT, typename VT, typename OP>
void bst<KT,VT,OP>::balance(){
    #ifdef __DEBUG
    std::cout << "CALL: bst - balance" << std::endl;
    #endif
    std::vector<T> v;
    for (auto iter = begin(); iter != end(); ++iter)
        v.push_back(iter->data);

    long long int begin = 0;
    long long int end = _size - 1;
    long long int mid = (_size - begin) / 2;

    clear();
    #ifdef __DEBUG
    std::cout << "balance() is calling insert (key " << v[mid].first << " in position " << mid << " of sorted array: "<< std::endl;
    #endif
    insert(v[mid]);

    _balance(v, begin, mid-1);
    _balance(v, mid+1, end);
    return;
    
}

template <typename KT, typename VT, typename OP>
void bst<KT,VT,OP>::_balance(std::vector<T>& v, long long int begin, long long int end){
    if (end < begin)
        return;
    long long int m = (end + begin) / 2;
    #ifdef __DEBUG
    std::cout << "balance() is calling insert (key " << v[m].first << " in position " << m << " of sorted array) "<< std::endl;
    #endif
    insert(v[m]);
    _balance(v, begin, m-1);
    _balance(v, m+1, end);
    return;
}


//----------- SUBSCRITPING OPERATOR -------------

template <typename KT, typename VT, typename OP>
VT& bst<KT,VT,OP>::operator[](const KT& x) noexcept{
    iterator iter = find(x);
    if (iter.current_ptr)
        return iter.current_ptr->data.second;
    else {
        insert({x, VT{}});
        return find(x).current_ptr->data.second;
    }            
}

template <typename KT, typename VT, typename OP>
VT& bst<KT,VT,OP>::operator[](KT&& x) noexcept{
    iterator iter = find(x);
    if (iter.current_ptr)
        return iter.current_ptr->data.second;
    else {
        insert({x, VT{}});
        return find(x).current_ptr->data.second;
    }   
}

//----------- _SWAP_NODES -----------------------

template <typename KT, typename VT, typename OP>
void bst<KT,VT,OP>::_swap_nodes(Node<std::pair<const KT, VT>>& x, Node<std::pair<const KT, VT>>& y, iterator& node_iter) noexcept{
    #ifdef __DEBUG
    std::cout << "CALL: bst - _swap_nodes" << std::endl;
    #endif

    /* returns an iterator to the node which is the replacement of y (has the key of x) */

    Node<T>* aux1;
    Node<T>* aux2;
        
    // parent
    if (!x.parent){ // x is the root!
        root.release();
        root.reset(new Node<T> {y.data, nullptr});
        aux1 = root.get();
    }else{
        if (*x.parent->left == x){ // x is the left child
            x.parent->left.release();
            x.parent->left.reset(new Node<T> {y.data, nullptr});
            aux1 = x.parent->left.get();
        }else{
            x.parent->right.release();
            x.parent->right.reset(new Node<T> {y.data, nullptr});
            aux1 = x.parent->right.get();
        }
        aux1->parent = x.parent;
    }

    if (x.left){ // left child
        aux1->left.reset(x.left.release()); // aux.left points to the left child of x
        aux1->left->parent = aux1;   
    }

    if (x.right){ // right child
        aux1->right.reset(x.right.release()); // aux.right points to the right child of x
        aux1->right->parent = aux1;    
    }

    // parent
    if (!y.parent){ //  this is to allow a general use of the swap
        root.release();
        root.reset(new Node<T> {x.data, nullptr});
        aux2 = root.get();
    }else{
        if (*y.parent->left == y){ // y is the left child
            y.parent->left.release();
            y.parent->left.reset(new Node<T> {x.data, nullptr});
            aux2 = y.parent->left.get();
        }else{
            y.parent->right.release();
            y.parent->right.reset(new Node<T> {x.data, nullptr});
            aux2 = y.parent->right.get();
        }
        aux2->parent = y.parent;
    }
    
    if (y.left){ // left child
        aux2->left.reset(y.left.release()); // aux.left points to the left child of x
        aux2->left->parent = aux2;   
    }
    
    if (y.right){ // right child
        aux2->right.reset(y.right.release()); // aux.right points to the right child of x
        aux2->right->parent = aux2;    
    }        

    delete &x;
    delete &y;

    node_iter = iterator(aux2);

    return;

}


//----------- ERASE -----------------------------

template <typename KT, typename VT, typename OP>
void bst<KT,VT,OP>::erase(const KT& x) noexcept{
    #ifdef __DEBUG
    std::cout << "CALL: bst - erase" << std::endl;
    #endif

    iterator node_iter = find(x);

    if (!node_iter.current_ptr) // the given key is not in the tree
        return;
    

    if (node_iter.current_ptr->left && node_iter.current_ptr->right) {
        #ifdef __DEBUG        
        std::cout << *node_iter << ": has 2 non-NIL children" << std::endl;
        #endif
        iterator tmp {node_iter.current_ptr};
        do {
            ++node_iter;
        } while (node_iter.current_ptr->left  && node_iter.current_ptr->right);

        Node<T>& successor = *(node_iter.current_ptr);
        #ifdef __DEBUG    
        std::cout << "Lets swap the node " << *tmp << " with its successor " << successor << std::endl;
        #endif
        Node<T>& tmp_node = *tmp.current_ptr;
        _swap_nodes(tmp_node, successor, node_iter); 
    }

    Node<T>& node = *node_iter.current_ptr;
    
    if (!node.left && !node.right){    
        #ifdef __DEBUG
        std::cout << node << ": leaf - deleted node " << node << std::endl;
        #endif
        if (node.parent->left && *node.parent->left == node) // is a left child
            node.parent->left.reset();
        else // is a right child
            node.parent->right.reset();
        _size--;
        return;
    }// the node has at most one child
    else if (!node.right){ // only left child
        #ifdef __DEBUG
        std::cout << node << ": only the left child is not-NIL" << std::endl;
        #endif
        if (node.parent->left && *node.parent->left == node){ // node is the left child of its parent
            node.left->parent = node.parent; // the parent of the left child of node becomes the former parent of node
            node.parent->left.reset(node.left.release()); // the left child of the parent of node becomes the left child of node
        } // only right child
        else { // node is the right child of its parent
            node.left->parent = node.parent;
            node.parent->right.reset(node.left.release());
        }
        _size--;
        return;
    } else if (!node.left){ // only right child
        #ifdef __DEBUG    
        std::cout << node << ": only the right child is not-NIL" << std::endl;
        #endif 
        if (node.parent->left && *node.parent->left == node){ // node is the left child of its parent
            node.right->parent = node.parent; // the parent of the left child of node becomes the former parent of node
            node.parent->left.reset(node.right.release()); // the left child of the parent of node becomes the left child of node
        } // only right child
        else { // node is the right child of its parent
            node.right->parent = node.parent;
            node.parent->right.reset(node.right.release());
        }
        _size--;
        return;
    }
}


//----------- PRINT -----------------------------

template <typename KT, typename VT, typename OP>
void bst<KT,VT,OP>::Print() noexcept{
    auto max_height = _size;
    if (_size == 0){
        std::cout << "The tree is empty" << std::endl;
        return;
    }
    std::vector<std::string> strings {max_height,""};
    int level {0};
    strings[level] += std::to_string(root->data.first);
    level++;

    _add_to_string(level, root.get(), strings);

    std::cout << std::endl;
    for (auto x : strings)
        if (x!="")
            std::cout << x << '\n';
    std::cout << std::endl;
}

template <typename KT, typename VT, typename OP>
void bst<KT,VT,OP>::_add_to_string(int level, const Node<std::pair<const KT, VT>>* ptr, std::vector<std::string>& s) noexcept{
    if (ptr){
            Node<T>* l;
            Node<T>* r;
        if (ptr->left){
            l = ptr->left.get();
            s[level] += std::to_string(l->data.first);
            s[level] += "(";                
            s[level] += std::to_string(ptr->data.first);
            s[level] += "l)  ";
            auto level_l = level+1;
            _add_to_string(level_l, l, s);
        }
        if (ptr->right){
            r = ptr->right.get();
            s[level] += std::to_string(r->data.first);
            s[level] += "(";                
            s[level] += std::to_string(ptr->data.first);
            s[level] += "r)  ";       
            auto level_r = level+1;
            _add_to_string(level_r, r, s);
        }
    }
}
