#ifndef HW2_RBT_H
#define HW2_RBT_H

#include<iostream>
#include <utility>
#include <functional>
#include <stdexcept>

namespace pic10c {

    template<typename T, typename compare_type = std::less<T>>

    class rbt {
    private:

        class node;

        node *root;

        compare_type pred;

        size_t sz{};

        /**
         * Function to recursively delete the tree
         * @param Node to delete
         */
        void deleteTree(node *Node);

        /**
         * Function to help with copying
         * @param Node to copy
         */
        void traverseInsert(node *Node);


    public:

        /**
         * print function (as you can see in the sample output) to display the red black
         * tree structure with `(r)' or `(b)' indicating a no de is red or black.
         */
        constexpr void print() {
            if (this->root == nullptr) {
                return;
            } else {
                traversePrint (this->root, 0);
            }
        }

        /**
         *
         * @param n
         * @param space
         */
        constexpr void traversePrint(node *n, size_t space) {
            if (root == nullptr) {
                return;
            }
            space += sz;
            if ((n->right) != nullptr) {
                traversePrint (n->right, space);
            }

            std::cout << "\n";

            for (size_t i = sz; i < space; i++) {
                std::cout << " ";
            }
            if (n->parent == nullptr) {
                std::cout << '-';
            } else if (n->parent->left == n) {
                std::cout << '\\';
            } else if (n->parent->right == n) {
                std::cout << '/';
            }

            std::cout << n->value << " (" << n->color << ")" << "\n";

            if ((n->left) != nullptr) {
                traversePrint (n->left, space);
            }
        }

        /**
         * Single Default constructor
         * @param _compare
         */
        explicit rbt(const compare_type &_compare = compare_type ()) : pred (_compare), root (nullptr), sz (0) {}

        class iterator;

        class const_iterator;

        /**
         * De constructor
         */
        ~rbt();

        /**
         * copy constructor
         * @param rhs
         */
        rbt(const rbt &rhs) : root (nullptr), sz (rhs.sz), pred (rhs.pred) {
            traverseInsert (rhs.root);
        }

        /**
         * move constructor
         * @param rhs
         */
        rbt(rbt &&rhs) noexcept;

        /**
         * copy assignment
         * @param rhs
         * @return
         */
        rbt &operator=(const rbt &rhs);

        /**
         * move assigment
         * @param rhs
         * @return
         */
        rbt &operator=(rbt &&rhs) noexcept;

        /**
         * iterator points to the beginning of the tree
         * @return
         */
        iterator begin();

        /**
        * const iterator points to the beginning of the tree
        * @return
        */
        const_iterator begin() const;

        /**
         * iterator points to the end of hte tree
         * @return
         */
        iterator end();

        /**
         * const iterator points to the end of hte tree
         * @return
         */
        const_iterator end() const;

        /**
         * member function swap
         * @param rhs
         */
        void swap(rbt &rhs);

        /**
         * free function swap
         * @param left
         * @param right
         */

        friend void swap(rbt<T, compare_type> &left, rbt<T, compare_type> &right);

        /**
         * Insert member function with O(log n) time cost, suitably implemented to
         * take in a T and attempt to add the value to the tree, dealing with rvalues and
         * lvalues appropriately;
         * @param value
         */
        void insert(const T &value);

        /**
         * emplace function that accepts a variadic list of arguments to construct a
         * T and attempt to place it within the tree with O(log n) time cost;
         * @tparam Type
         * @param _value
         */
        template<typename ...Type>
        void emplace(Type &&..._value);

        /**
         *
         * @param n
         */
        void fixDeletion(node *n);

        /**
         * erase member function with O(log n) time cost accepting an iterator and
         * removing the no de managed by the iterator from the tree
         * @param itr
         */
        void erase(iterator itr);

        /**
         * size member function returning the numb er of elements in the bst
         * @return
         */
        size_t size();

        /**
         * Find member function running in O(log n) time cost returning the iterator
         * to the no de with a given value if found and otherwise returning the past-the-end
         * iterator.
         * @param value
         * @return
         */
        iterator find(const T &value);

        /**
         * Find member function running in O(log n) time cost returning the const_iterator
         * to the no de with a given value if found and otherwise returning the past-the-end
         * iterator.
         * @param values
         * @return
         */
        const_iterator find(const T &values) const;

    };

    template<typename T, typename compare_type>
    class rbt<T, compare_type>::node {
        friend rbt;
        friend iterator;
        friend const_iterator;
    private:
        T value;
        char color;
        node *left = nullptr;
        node *right = nullptr;
        node *parent = nullptr;
        node *sibling = nullptr;
        node *uncle = nullptr;
        node *grandpa = nullptr;
        rbt *container;


        /**
         * Function to get the left most node of the tree
         * @return the leftmost node
         */
        node *leftMost();

        /**
         * Function to get the right most of the tree
         * @return the rightmost node
         */
        node *rightMost();

        /**
         * function to replace deleted node with a new node
         * @return
         */
        node *replace();

        /**
         * insert a node into a tree
         * @param n
         * @param pred_type
         * @return
         */
        bool insertNode(node *n, compare_type pred_type);

        /**
         * left rotate around the center
         * @param center
         */
        void leftRotate(node *center);

        /**
         * right rotate around the center
         * @param center
         */
        void rightRotate(node *center);

        /**
         *
         * @param n
         */
        void fixRed(node *n);

        /**
         * Default constructor for the node
         * @param _value
         */
        node(T _value, rbt *_container) : value (_value), right (nullptr), left (nullptr), parent (nullptr),
                                          uncle (nullptr), sibling (nullptr), grandpa (nullptr), color ('r'),
                                          container (_container) {}

    };

    template<typename T, typename compare_type>
    class rbt<T, compare_type>::iterator {
        friend rbt;
    private:
        node *curr;//current position
        const rbt *container;//pointer to the tree
        iterator(node *n, const rbt *_container) : curr (n), container (_container) {}

    public:
        /**
         * prefix++
         * @return
         */
        iterator &operator++();

        /**
         * prefix --
         * @return
         */
        iterator &operator--();

        /**
         * postfix++
         * @return
         */
        const iterator operator++(int);

        /**
         * postfix--
         * @return
         */
        const iterator operator--(int);

        /**
         * free function == operator
         * @param rhs
         * @return
         */
        friend bool operator==(const iterator &first, const iterator &second) {
            return (first == second);
        }

        /**
         * member function ==
         * @param rhs
         * @return
         */
        bool operator==(const iterator &rhs) {
            return ((curr == rhs.curr) && (container == rhs.container));
        }


        /**
         * free function != operator
         * @param rhs
         * @return
         */
        friend bool operator!=(const iterator &first, const iterator &second) {
            return first != second;
        }

        /**
         * member function !=
         * @param rhs
         * @return
         */
        bool operator!=(const iterator &rhs) {
            return (!((curr == rhs.curr) && (container == rhs.container)));
        }

        /**
         * De referencing operator
         * @return
         */
        const T &operator*() const;

        /**
         * Arrow operator
         * @return
         */
        const T *operator->() const;
    };

    template<typename T, typename compare_type>
    class rbt<T, compare_type>::const_iterator {
        friend rbt;
    private:
        node *curr;//current position
        const rbt *container;//pointer to the tree
        const_iterator(node *n, const rbt *_container) : curr (n), container (_container) {}

    public:
        /**
         * prefix++
         * @return
         */
        const_iterator &operator++();

        /**
         * prefix --
         * @return
         */
        const_iterator &operator--();

        /**
         * postfix++
         * @return
         */
        const const_iterator operator++(int);

        /**
         * postfix--
         * @return
         */
        const const_iterator operator--(int);

        /**
         * free function == operator
         * @param rhs
         * @return
         */
        friend bool operator==(const const_iterator &first, const const_iterator &second) {
            return (first == second);
        }

        /**
         * member function == operator
         * @param rhs
         * @return
         */
        bool operator==(const const_iterator &rhs) {
            return ((curr == rhs.curr) && (container == rhs.container));
        }

        /**
         * free function != operator
         * @param rhs
         * @return
         */
        friend bool operator!=(const const_iterator &first, const const_iterator &second) {
            return first != second;
        }

        /**
         * member function != operator
         * @param rhs
         * @return
         */
        bool operator!=(const const_iterator &rhs) {
            return (!((curr == rhs.curr) && (container == rhs.container)));
        }

        /**
         * De referencing operator
         * @return
         */
        const T &operator*() const;

        /**
         * Arrow operator
         * @return
         */
        const T *operator->() const;
    };

    //BTS---------------------------------------------------------------------------------------------------------------

    //Delete each node
    template<typename T, typename compare_type>
    void rbt<T, compare_type>::deleteTree(rbt::node *Node) {
        if (Node->right != nullptr) {//if right child exist
            deleteTree (Node->right);//recurse in right
        } else if (Node->left != nullptr) {//if left child exist
            deleteTree (Node->left);//recurse in left
        }
        delete Node;//delete node
    }

    //Deconstruct
    template<typename T, typename compare_type>
    rbt<T, compare_type>::~rbt() {
        if (root != nullptr) {
            deleteTree (root);//call deleteTree
        }
    }

    //move constructor
    template<typename T, typename compare_type>
    rbt<T, compare_type>::rbt(rbt &&rhs) noexcept:rbt () {
        using std::swap;
        swap (rhs.root, root);//swap root
        swap (rhs.pred, pred);//swap pred
        swap (rhs.sz, sz);//swap sz
    }

    //copy assignment
    template<typename T, typename compare_type>
    pic10c::rbt<T, compare_type> &rbt<T, compare_type>::operator=(const rbt &rhs) {
        if (this == &rhs) {
            return *this;//if the rhs is itself return this
        }
        using std::swap;//else
        rbt &copy (rhs);//make a copy
        swap (copy.root, root);//swap root
        swap (copy.pred, pred);//swap pred
        swap (copy.sz, sz);//swap sz
        return *this;//return this
    }

    //move assignment
    template<typename T, typename compare_type>
    pic10c::rbt<T, compare_type> &rbt<T, compare_type>::operator=(rbt &&rhs) noexcept {
        using std::swap;
        swap (root, rhs.root);//swap root
        swap (pred, rhs.pred);//swap pred
        swap (sz, rhs.sz);//swap sz
        return *this;//return this
    }

    //iterator begin
    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::iterator rbt<T, compare_type>::begin() {
        if (root == nullptr) {//if the root is nullprt
            return iterator (nullptr, this);//return null
        }
        node *n = root;//else make start from the root
        while (n->left != nullptr) {//while the left is not null
            n = n->left;//go to left
        }
        return iterator (n, this);//return the begin
    }

    //const_iterator begin
    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::const_iterator rbt<T, compare_type>::begin() const {
        if (root == nullptr) {//if the root is nullprt
            return const_iterator (nullptr, this);//return null
        }
        node *n = root;//else make start from the root
        while (n->left != nullptr) {//while the left is not null
            n = n->left;//go to left
        }
        return const_iterator (n, this);//return the begin
    }

    //iterator end
    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::iterator rbt<T, compare_type>::end() {
        return iterator (nullptr, this);//return null
    }

    //const_iterator end
    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::const_iterator rbt<T, compare_type>::end() const {
        return const_iterator (nullptr, this);//return null
    }

    //member function swap
    template<typename T, typename compare_type>
    void rbt<T, compare_type>::swap(rbt &rhs) {
        using std::swap;
        swap (sz, rhs.sz);//swap sz
        swap (pred, rhs.pred);//swap pred
        swap (root, rhs.root);//swap root
    }

    //free function swap
    template<typename T, typename compare_type>
    void swap(rbt<T, compare_type> &left, rbt<T, compare_type> &right) {
        left.swap (right);//swap right and left
    }


    //insert a node with value
    template<typename T, typename compare_type>
    void rbt<T, compare_type>::insert(const T &value) {
        if (!root) {//if there is no root
            root = new node (value, this);//create a new node
            root->color = 'b'; //color of te root is black
            //sz = 1;//increment sz;
        } else {//if there is root
            node *newNode = new node (value, this);//create a new node
            root->insertNode (newNode, pred);
            ++sz;
        }
    }


    template<typename T, typename compare_type>
    void rbt<T, compare_type>::traverseInsert(rbt::node *Node) {
        insert (Node->value);
        if (Node->left != nullptr) {
            traverseInsert (Node->left);//recursive
        }
        if (Node->right != nullptr) {
            traverseInsert (Node->right);//recursive
        }
    }

    template<typename T, typename compare_type>
    template<typename... Type>
    void rbt<T, compare_type>::emplace(Type &&... _value) {
        if (!root) {//if there is no root
            root = new node (T (std::forward<Type> (_value)...), this);//make the root the new Node
            ++sz;//increment sz;
        } else {//if there is root
            node *newNode = new node (T (std::forward<Type> (_value)...), this);//create a new node
            if (root->insertNode (newNode, pred)) {
                ++sz;//increment size;
            }//check where to put that node
        }
    }

    template<typename T, typename compare_type>
    void rbt<T, compare_type>::erase(rbt::iterator itr) {
        node *toDelete = itr.curr;
        while (toDelete != nullptr) {
            node *newNode = toDelete->replace ();//replace the node with new node
            if (toDelete->parent != nullptr && toDelete->color == 'r' && newNode == nullptr) {//#2 case
                if (toDelete->parent->right == toDelete) {//if the node is on the right
                    toDelete->parent->right = nullptr;//make the right to null
                    toDelete = nullptr;//make the node to null
                    delete toDelete;//delete node
                    --sz;//decrement sz
                    break;//break
                } else if (toDelete->parent->left == toDelete) {//if node is on the left
                    toDelete->parent->left = nullptr;//make the left to null
                    toDelete = nullptr;//make the node to null
                    delete toDelete;//delete the node
                    --sz;//decrement sz
                    break;
                }
            } else if (toDelete->parent == nullptr && toDelete->color == 'r' && newNode == nullptr) {
                //if node is root, and it is red, and it has no children
                toDelete = nullptr;//make node to null
                delete toDelete;//delete node
                --sz;//decrement sz
                break;
            } else if (newNode != nullptr) {//if newNode is not null
                if (toDelete->color == 'b' && toDelete->parent != nullptr) {//#1 case
                    if (toDelete->parent->right == toDelete) {//toDelete is a right child
                        if (((toDelete->right == newNode) || toDelete->left == newNode) && newNode->color == 'r') {
                            //newNode is red right child of toDelete
                            newNode->color = 'b';//make the new node black
                            newNode->parent = toDelete->parent;//replace toDelete to newNode
                            toDelete->parent->right = newNode;//make newNode be the right node of toDelete's parent
                            delete toDelete;//delete toDelete
                            toDelete = nullptr;//make it null
                            --sz;//decrement sz
                            break;
                        }
                    } else if (toDelete->parent->left == toDelete) {//to delete is a left child
                        if (((toDelete->right == newNode) || toDelete->left == newNode) &&
                            ((newNode->color == 'r') ||
                             (newNode->color == 'b'))) {//newNode is red right child of toDelete
                            newNode->color = 'b';//make the new node black
                            newNode->parent = toDelete->parent;//replace toDelete to newNode
                            toDelete->parent->left = newNode;//make newNode be the left node of toDelete's parent
                            toDelete = nullptr;//delete toDelete
                            delete toDelete;//make it null
                            --sz;//decrement sz
                            break;
                        }
                    }
                } else if (toDelete->color == 'r' && toDelete->parent != nullptr) {
                    //if toDelete is red and its parent is not nullprt
                    if (toDelete->right != nullptr) {
                        node *smallest = toDelete->right->leftMost ();
                        toDelete->value = smallest->value;//replace the toDelete's value to smallest value
                        toDelete->color = smallest->color;//replace the color
                        node *replacedSmall = smallest->replace ();//replace the smallest to replaceSmall
                        if (replacedSmall != nullptr) {//if replaceSmall is not null
                            toDelete->right->left = replacedSmall;//reconnect replacementSmall
                            replacedSmall->parent = toDelete->right;
                        } else {
                            if (toDelete->right != smallest) {
                                toDelete->right->left = nullptr;//connect to null
                            } else if (toDelete->right == smallest) {
                                toDelete->right = nullptr;
                            }
                        }
                        smallest = nullptr;//make the smallest null
                        delete smallest;//delete smallest
                        --sz;//decrement sz
                        break;
                    } else {
                        if (toDelete->parent->left == toDelete) {
                            toDelete->parent->left = toDelete->left;
                            toDelete->left->parent = toDelete->parent;
                            toDelete = nullptr;
                            delete toDelete;
                            break;
                        } else {
                            toDelete->parent->right = toDelete->right;
                            toDelete->left->parent = toDelete->parent;
                            toDelete = nullptr;
                            delete toDelete;
                            break;
                        }
                    }
                }
            } else if (toDelete->color == 'b' && toDelete->parent != nullptr) {
                if (toDelete->parent->color == 'r') {
                    if (toDelete->parent->right == toDelete) {//if toDelete is a right child
                        toDelete->parent->right = nullptr;
                    } else {
                        toDelete->parent->left = nullptr;
                    }
                    if ((toDelete->parent->right == toDelete && toDelete->parent->left == nullptr) ||
                        (toDelete->parent->left == toDelete && toDelete->parent->right == nullptr)) {
                        toDelete->parent->color = 'b';
                    }
                    --sz;
                    toDelete = nullptr;
                    delete toDelete;
                    break;
                } else {
                    toDelete->color = 'd';
                    fixDeletion (toDelete);
                    --sz;
                    if (toDelete->parent->right == toDelete) {//if toDelete is a right child
                        toDelete->parent->right = nullptr;
                    } else {
                        toDelete->parent->left = nullptr;
                    }
                    toDelete = nullptr;
                    delete toDelete;
                    break;
                }
            }
            if ((toDelete->color == 'b' && toDelete->parent == nullptr)
                && ((toDelete->left != nullptr) || (toDelete->right != nullptr))) {
                //if to delete is root
                node *smallest = toDelete->right->leftMost ();//get the replaced one
                toDelete->value = smallest->value;//exchange value
                node *NewNode = smallest->replace ();//get the replaced one
                if (NewNode != nullptr) {//if there is one
                    smallest->parent->right = NewNode;//link
                    NewNode->parent = smallest->parent;//link
                    NewNode->color = 'b';//turn it to black
                } else {//if there does not have one to replace
                    if (smallest->parent->right == smallest) {//link
                        smallest->parent->right = nullptr;//link
                    }
                    smallest->parent->left = nullptr;//link
                }
                if (smallest->parent == nullptr) {//link
                    root = NewNode;//make the NewNode the root
                    NewNode->parent = nullptr;//link
                    NewNode->color = 'b';//link
                }
                --sz;//decrement the size
                smallest = nullptr;//free
                delete smallest;//delete
                break;
            } else if (toDelete->parent == nullptr && toDelete->color == 'b'
                       && toDelete->left == nullptr && toDelete->right == nullptr) {
                //if there is only one black node
                toDelete = nullptr;//free
                root = nullptr;//link
                delete toDelete;//delete
            }
        }
    }

    template<typename T, typename compare_type>
    void rbt<T, compare_type>::fixDeletion(rbt::node *n) {
        if (n->parent == nullptr) {
            //if n is root
            n->color = 'b';
            return;
        } else {
            if (n->parent->left == n) {
                //if n is a left child
                if (n->parent->right != nullptr) {
                    n->sibling = n->parent->right;
                    n->parent->right->sibling = n;
                } else {
                    n->sibling = nullptr;
                }
            } else {//if n is a right child
                if (n->parent->left != nullptr) {
                    n->sibling = n->parent->left;
                    n->parent->left->sibling = n;
                } else {
                    n->sibling = nullptr;
                }
            }
            if (n->sibling != nullptr && n->sibling->color == 'r') {
                //if n has a red sibling
                if (n->parent->left == n) {
                    //n is a left child of its parent
                    node *tempparent = n->parent;
                    node *tempsib = n->sibling;
                    n->leftRotate (n->parent);
                    tempparent->color = 'r';
                    tempsib->color = 'b';
                    fixDeletion (n);
                    return;
                } else {
                    //n is a right child of its parent
                    node *tempparent = n->parent;
                    node *tempsib = n->sibling;
                    n->rightRotate (n->parent);
                    tempparent->color = 'r';
                    tempsib->color = 'b';
                    fixDeletion (n);
                    return;
                }
            } else if (n->sibling != nullptr && n->sibling->color == 'b') {
                // if n has a black sibling
                if (n->parent->left == n) {
                    //if n is the left child
                    if (n->sibling->left != nullptr && n->sibling->left->color == 'r') {
                        //if s has one red child on its left
                        if (n->sibling->right == nullptr || n->sibling->right->color == 'b') {
                            //if sibling's other child is not red
                            node *tempSib = n->sibling;
                            node *tempX = n->sibling->left;
                            n->rightRotate (n->sibling);
                            tempSib->color = 'r';
                            tempX->color = 'b';
                            fixDeletion (n);
                            return;
                        }
                    }
                } else {
                    //if n is the right child
                    if (n->sibling->right != nullptr && n->sibling->right->color == 'r') {
                        //if s has one red child on its right
                        if (n->sibling->left == nullptr || n->sibling->left->color == 'b') {
                            node *tempSib = n->sibling;
                            node *tempX = n->sibling->right;
                            n->leftRotate (n->sibling);
                            tempSib->color = 'r';
                            tempX->color = 'b';
                            fixDeletion (n);
                            return;
                        }
                    }
                }

                //IV
                if (n->parent->color == 'r') {
                    if (n->parent->left == n) {
                        //if n is the left child
                        if (n->sibling->right != nullptr && n->sibling->right->color == 'r') {
                            //if s has a red right child
                            node *tempSib = n->sibling;
                            node *tempParent = n->parent;
                            node *tempX = n->sibling->right;
                            n->leftRotate (n->parent);
                            tempSib->color = 'r';
                            tempParent->color = 'b';
                            tempX->color = 'b';
                            n->color = 'b';
                        }
                    } else {
                        //if n is the right child
                        if (n->sibling->left != nullptr && n->sibling->left->color == 'r') {
                            //if s has a red right child
                            node *tempSib = n->sibling;
                            node *tempParent = n->parent;
                            node *tempX = n->sibling->left;
                            n->rightRotate (n->parent);
                            tempSib->color = 'r';
                            tempParent->color = 'b';
                            tempX->color = 'b';
                            n->color = 'b';
                            return;
                        }
                    }
                } else if (n->parent->color == 'b') {
                    if (n->parent->left == n) {
                        //if n is the left child
                        if (n->sibling->right != nullptr && n->sibling->right->color == 'r') {
                            //if s has a red right child
                            node *tempX = n->sibling->right;
                            n->leftRotate (n->parent);
                            tempX->color = 'b';
                            n->color = 'b';
                            return;
                        }
                    } else {
                        //if n is the right child
                        if (n->sibling->left != nullptr && n->sibling->left->color == 'r') {
                            //if s has a red right child
                            node *tempX = n->sibling->left;
                            n->rightRotate (n->parent);
                            tempX->color = 'b';
                            n->color = 'b';
                            return;
                        }
                    }
                }

                //V
                if ((n->sibling->left != nullptr && n->sibling->right != nullptr
                     && n->sibling->left->color == 'b' && n->sibling->right->color == 'b')
                    || (n->sibling->left == nullptr && n->sibling->right == nullptr)) {
                    //if Both children of S are black
                    if (n->parent->color == 'r') {
                        //if parent node of n is red
                        n->parent->color = 'b';
                        n->color = 'b';
                        n->sibling->color = 'r';
                        return;
                    } else {
                        //if parent node of n is black
                        n->sibling->color = 'r';
                        n->parent->color = 'd';
                        n->color = 'b';
                        fixDeletion (n->parent);
                        return;
                    }
                }
            }
        }
    }

    template<typename T, typename compare_type>
    size_t rbt<T, compare_type>::size() {
        return sz;
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::iterator rbt<T, compare_type>::find(const T &value) {
        for (auto i = begin (); i != end (); ++i) {
            if (*i == value) {
                return i;
            }
        }
        return end ();
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::const_iterator rbt<T, compare_type>::find(const T &value) const {
        for (auto i = begin (); i != end (); ++i) {
            if (*i == value) {
                return i;
            }
        }
        return end ();
    }

    //Node definitions----------------------------------------------------------------------------------------------

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::node *rbt<T, compare_type>::node::leftMost() {
        if (left == nullptr) {//if left child is null
            return this;//return this
        }
        return left->leftMost ();//go to the left
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::node *rbt<T, compare_type>::node::rightMost() {
        if (right == nullptr) {//if right child is null
            return this;//return this
        }
        return right->rightMost ();//go to the right
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::node *rbt<T, compare_type>::node::replace() {
        node *newNode;//create a new node
        if (this->left == nullptr && this->right == nullptr) {
            //if the node does not have any child
            newNode = nullptr;//newNode is null
        } else if (this->left != nullptr && this->right != nullptr) {
            //if it has two child
            newNode = this->right->leftMost ();
            //newNode is its right's leftMost
        } else if (this->left != nullptr) {//if only have one left child
            newNode = this->left;//make  newNode its left child
        } else if (this->right != nullptr) {//if only have one right node
            newNode = this->right;//make newNode its right child
        }
        return newNode;//return the newNode
    }

    template<typename T, typename compare_type>
    void rbt<T, compare_type>::node::leftRotate(rbt<T, compare_type>::node *center) {
        node *temp = center->right;//make the right child the temp
        center->right = temp->left;//if there is a left node for temp, make it the right node of center
        if (temp->left != nullptr) {
            temp->left->parent = center;
            center->right = temp->left;
            if (center->left != nullptr) {
                center->left->sibling = temp->left;
                temp->left->sibling = center->left;
            }
        } else {
            center->right = nullptr;
        }
        if (center->parent == nullptr) {
            //if the center is the root
            temp->parent = nullptr;//make the temp the root
            container->root = temp;
        } else {
            //right right case
            if (center->parent->right == center) {//if the center is a right child
                center->parent->right = temp;//make the temp the right child
                temp->parent = center->parent;
                if (center->parent->left != nullptr) {
                    temp->sibling = center->parent->left;
                    center->parent->left->sibling = temp;
                    if (temp->right != nullptr) {
                        center->sibling = temp->right;
                        temp->right->sibling = center;
                    } else {
                        center->sibling = nullptr;
                    }
                } else {
                    temp->sibling = nullptr;
                }
                temp->grandpa = center->parent->parent;
                temp->uncle = center->parent->sibling;
            } else if (center->parent->left == center) {//if the center is a left child
                center->parent->left = temp;//make the temp the left child
                temp->parent = center->parent;
                if (center->parent->right != nullptr) {
                    temp->sibling = center->parent->right;
                    center->parent->right->sibling = temp;
                    if (temp->right != nullptr) {
                        center->sibling = temp->right;
                        temp->right->sibling = center;
                    } else {
                        center->sibling = nullptr;
                    }
                } else {
                    temp->sibling = nullptr;
                }
                temp->grandpa = center->parent->parent;
                temp->uncle = center->parent->sibling;
            }
        }
        temp->left = center;
        center->parent = temp;//link back
    }

    template<typename T, typename compare_type>
    void rbt<T, compare_type>::node::rightRotate(rbt<T, compare_type>::node *center) {
        node *temp = center->left;//make the left child of center be the temp
        center->left = temp->right;//if there is a right child for the temp, make it the left child of center
        if (temp->right != nullptr) {
            center->left = temp->right;
            temp->right->parent = center;
            if (center->right != nullptr) {
                center->right->sibling = temp->right;
                temp->right->sibling = center->right;
            } else {
                temp->right->sibling = nullptr;
            }
        } else {
            center->left = nullptr;
        }
        if (center->parent == nullptr) {
            //if the center is the root
            temp->parent = nullptr;//make the temp be the root
            container->root = temp;
        } else {//if the center is not root
            temp->parent = center->parent;//make the temp's grandparent be its parent
            //left left case
            if (center->parent->left == center) {//if the center is on the left
                center->parent->left = temp;//link it to temp
                temp->parent = center->parent;
                if (center->parent->right != nullptr) {
                    temp->sibling = center->parent->right;
                    center->parent->right->sibling = temp;
                    center->sibling = nullptr;
                } else {
                    temp->sibling = nullptr;
                }
                temp->uncle = center->parent->sibling;
                temp->grandpa = center->parent->parent;
            } else if (center->parent->right == center) {//if the center is on the right
                center->parent->right = temp;//link it to temp
                temp->parent = center->parent;
                if (center->parent->left != nullptr) {
                    temp->sibling = center->parent->left;
                    center->parent->left->sibling = temp;
                    center->sibling = nullptr;
                } else {
                    temp->sibling = nullptr;
                }
                temp->uncle = center->parent->sibling;
                temp->grandpa = center->parent->parent;
            }

        }

        temp->right = center;//link back
        center->parent = temp;
    }

    template<typename T, typename compare_type>
    bool rbt<T, compare_type>::node::insertNode(rbt<T, compare_type>::node *n, compare_type pred_type) {
        bool insert = true;//insert is true
        if (pred_type (value, n->value)) {
            //if this value is greater
            if (right) {//if there is a right node
                right->insertNode (n, pred_type);//recurse down
            } else {//if right is null
                right = n;//make n be the right node of this
                n->parent = this;//this is the parent
                n->grandpa = n->parent->parent;//grandpa is parent's parent
                n->sibling = n->parent->left;//sibling is parent's left child
                if (n->sibling != nullptr) {//if sibling is not null
                    n->sibling->sibling = n;//link n to also be the sibling of its sibling
                }
                if (n->grandpa != nullptr) {
                    n->uncle = n->parent->sibling;
                }// uncle is n's parent's sibling
                fixRed (n);//call fix red
            }
        } else if (pred_type (n->value, value)) {
            //if this value is smaller
            if (left) {//if there is a left node
                left->insertNode (n, pred_type);//recurse down
            } else {//if there is no left node
                left = n;//make the left node n
                n->parent = this;//parent is this
                n->grandpa = n->parent->parent;//grandpa is parent's parent
                n->sibling = n->parent->right;//sibling is parent's left child
                if (n->sibling != nullptr) {//if sibling is not null
                    n->sibling->sibling = n;//make n also be the sibling of its sibling
                }
                if (n->grandpa != nullptr) {
                    n->uncle = n->parent->sibling;
                }// uncle is n's parent's sibling
                fixRed (n);//fixRed
            }
        } else {//if equal
            insert = false;//not insert
            delete n;//freed memory
            n = NULL;
        }
        return insert;//return true
    }

    template<typename T, typename compare_type>
    void rbt<T, compare_type>::node::fixRed(rbt<T, compare_type>::node *n) {
        if (n->parent == nullptr) {
            n->color = 'b';//if n is the root, make the n color to 'b'
        } else {//if n is not root

            n->grandpa = n->parent->parent;//update grandpa

            if (n->parent->right == n) {//if n is a right node
                if (n->parent->left != nullptr) {
                    n->sibling = n->parent->left;
                    n->parent->left->sibling = n;
                } else {
                    n->sibling = nullptr;
                }//its sibling is its parent's left child
            } else {
                if (n->parent->right != nullptr) {
                    n->sibling = n->parent->right;
                    n->parent->right->sibling = n;
                } else {
                    n->sibling = nullptr;
                }//its sibling is its parent's right child
            }
            //n->uncle = n->parent->sibling;
            if (n->grandpa != nullptr) {//if grandpa is not null
                if (n->grandpa->left == n->parent) {//if n's parent is a left node
                    n->uncle = n->grandpa->right;//link
                } else {
                    n->uncle = n->grandpa->left;//link
                }
            }
            if (n->parent->color == 'b') {//if parent's color is black
                return;//simply return
            } else if ((n->parent->color == 'r') && ((n->uncle != nullptr) && (n->uncle->color == 'r'))) {//#3 case
                //if parent is red and uncle is not null and uncle is red
                n->parent->color = 'b';//color parent to black
                n->uncle->color = 'b';//color uncle to black
                n->grandpa->color = 'r';//color grandpa to red
                fixRed (n->grandpa);//call fix red
            } else if (n->parent->right == n) {//#4 if n is right child
                if (((n->uncle == nullptr) || (n->uncle->color == 'b')) && (n->parent->color == 'r')) {
                    //parent is red and (uncle is black or null)
                    if (n->grandpa->right == n->parent) {//if n's parent is a right child
                        //right right case
                        node *tempParent = n->parent;
                        node *tempGrand = n->grandpa;
                        leftRotate (n->grandpa);//left rotate around grandpa
                        tempParent->color = 'b';//color parent to black
                        tempGrand->color = 'r';//color grandpa to red
                        return;
                    } else if (n->grandpa->left == n->parent) {//if n's parent is a left child
                        node *tempParent = n->parent;//temporarily store parent
                        leftRotate (n->parent);//make the parent be the left child of n;
                        fixRed (tempParent);//fix
                    }
                }
            } else {//node is a left child
                if ((n->parent->color == 'r') && (((n->uncle == nullptr)) || (n->uncle->color == 'b'))) {
                    //if parent is red and uncle is null or uncle is black
                    if (n->grandpa->left == n->parent) {
                        //left left case
                        node *tempParent = n->parent;
                        node *tempGrand = n->grandpa;
                        rightRotate (n->grandpa);//left rotate
                        tempParent->color = 'b';//turn parent to black
                        tempGrand->color = 'r';//turn grandpa to red
                        return;
                    } else if (n->grandpa->right == n->parent) {
                        //right left case
                        node *tempParent = n->parent;//temporarily store parent
                        rightRotate (n->parent);//rotate
                        fixRed (tempParent);
                    }
                }
            }
        }
    }

    //Iterator definitions---------------------------------------------------------------------------------------------

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::iterator &rbt<T, compare_type>::iterator::operator++() {
        if (this->curr == nullptr) {//reach the end, can not go further
            throw std::logic_error ("Out of scoop");
        } else if (this->curr == container->root->rightMost ()) {//reach the end
            curr = nullptr;
        } else if (curr->right != nullptr) {//if the node have a right node
            curr = curr->right->leftMost ();//go to the leftMost of the right tree
        } else {
            node *next = curr;
            while (next->parent != nullptr && next == next->parent->right) {//if the curr is now at the right
                next = next->parent;//go back to its parent
            }
            if (nullptr == next->parent) {
                curr = nullptr;
            } else {//if the node is now on the left
                curr = next->parent;
            }
        }
        return *this;
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::const_iterator &rbt<T, compare_type>::const_iterator::operator++() {
        if (this->curr == nullptr) {//reach the end, can not go further
            throw std::logic_error ("Out of scoop");
        } else if (this->curr == container->root->rightMost ()) {//reach the end
            curr = nullptr;
        } else if (curr->right != nullptr) {//if the node have a right node
            curr = curr->right->leftMost ();//go to the leftMost of the right tree
        } else {
            node *next = curr;
            while (next->parent != nullptr && next == next->parent->right) {//if the curr is now at the right
                next = next->parent;//go back to its parent
            }
            if (nullptr == next->parent) {
                curr = nullptr;
            } else {//if the node is now on the left
                curr = next->parent;
            }
        }
        return *this;
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::iterator &rbt<T, compare_type>::iterator::operator--() {
        if (this->curr == container->begin ()) {//reach the end can not go further
            throw std::logic_error ("Out of scoop");
        } else if (this->curr == nullptr) {//at the end of the tree
            curr = container->root->rightMost ();//go to the end of the tree
        } else if (this->curr->left != nullptr) {//if there is left children
            curr = curr->left->rightMost ();//go to the biggest one on the left tree
        } else {
            if (curr == curr->parent->right) {//if the node is on the right
                curr = curr->parent;//go to its parent
            } else {//not a right child
                while (curr->parent != nullptr && curr == curr->parent->left) {
                    curr = curr->parent;
                }
                if (curr->parent == nullptr) {
                    curr = nullptr;
                } else {//if it is a right child
                    curr = curr->parent;
                }
            }
        }
        return *this;
    }

    template<typename T, typename compare_type>
    typename rbt<T, compare_type>::const_iterator &rbt<T, compare_type>::const_iterator::operator--() {
        if (this->curr == container->begin ()) {//reach the end can not go further
            throw std::logic_error ("Out of scoop");
        } else if (this->curr == nullptr) {//at the end of the tree
            curr = container->root->rightMost ();//go to the end of the tree
        } else if (this->curr->left != nullptr) {//if there is left children
            curr = curr->left->rightMost ();//go to the biggest one on the left tree
        } else {
            if (curr == curr->parent->right) {//if the node is on the right
                curr = curr->parent;//go to its parent
            } else {//not a right child
                while (curr->parent != nullptr && curr == curr->parent->left) {
                    curr = curr->parent;
                }
                if (curr->parent == nullptr) {
                    curr = nullptr;
                } else {//if it is a right child
                    curr = curr->parent;
                }
            }
        }
        return *this;
    }

    template<typename T, typename compare_type>
    const typename rbt<T, compare_type>::iterator rbt<T, compare_type>::iterator::operator++(int) {
        iterator &copy (*this);
        ++(*this);
        return copy;
    }

    template<typename T, typename compare_type>
    const typename rbt<T, compare_type>::const_iterator rbt<T, compare_type>::const_iterator::operator++(int) {
        const_iterator &copy (*this);
        ++(*this);
        return copy;
    }

    template<typename T, typename compare_type>
    const typename rbt<T, compare_type>::iterator rbt<T, compare_type>::iterator::operator--(int) {
        iterator &copy (*this);
        --(*this);
        return copy;
    }

    template<typename T, typename compare_type>
    const typename rbt<T, compare_type>::const_iterator rbt<T, compare_type>::const_iterator::operator--(int) {
        const_iterator &copy (*this);
        --(*this);
        return copy;
    }

    template<typename T, typename compare_type>
    const T &rbt<T, compare_type>::iterator::operator*() const {
        return curr->value;//value of curr in iterator
    }

    template<typename T, typename compare_type>
    const T &rbt<T, compare_type>::const_iterator::operator*() const {
        return curr->value;//value or curr in const iterator
    }

    template<typename T, typename compare_type>
    const T *rbt<T, compare_type>::iterator::operator->() const {
        return &(curr->value);//value of curr in iterator
    }

    template<typename T, typename compare_type>
    const T *rbt<T, compare_type>::const_iterator::operator->() const {
        return &(curr->value);//value of curr in const_iterator
    }


}
#endif //HW2_RBT_H
