#pragma once

#include <iostream>
#include <iterator>
#include <set>
#include <unordered_map>
#include <vector>
#include <map>
#include <memory>
#include <stack>

namespace ariel {
    template<class T>
    struct Node {
        Node<T> *parent = nullptr;
        std::unique_ptr<Node<T>> left;
        std::unique_ptr<Node<T>> right;
        T data;

        explicit Node<T>(T data) : data(data) {
            left = nullptr;
            right = nullptr;
        }

        Node<T>(T data, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right) : data(data), left(left), right(right)
        {

        }

        friend std::ostream &operator<<(std::ostream &stream, const Node<T> &node) {
            return stream << node.t;
        }
    };

    //https://www.cs.odu.edu/~zeil/cs361/latest/Public/treetraversal/index.html
    // based on BST code
    template<class T>
    class inorder_iterator {
    private:
        Node<T> *current;
        Node<T> *root;

    public:
        explicit inorder_iterator(Node<T> *ptr = nullptr) : current(ptr) {
            if (current == nullptr) {
                return;
            }
            while (current->left.get() != nullptr) {
                current = current->left.get();
            }
        }

//        inorder_iterator(Node<T> *root, Node<T> *current)
//        {
//            this->root = root;
//            this->current = current;
//        }

        T &operator*() const {
            return current->data;
        }

        T *operator->() const {
            return &(current->data);
        }

        inorder_iterator &operator++() {
//            inorder_iterator<T> copy = inorder_iterator<T>{root, current};
            Node<T> *p = nullptr;
            if (current->right.get() != nullptr) {
                current = current->right.get();
                while (current->left.get() != nullptr) {
                    current = current->left.get();
                }
            } else {
                p = current->parent;
                while (p != nullptr && current == p->right.get()) {
                    current = p;
                    p = p->parent;
                }
                current = p;
            }
            return *this;
        }

        inorder_iterator operator++(int) {
            inorder_iterator tmp(*this);
            ++(*this);
            return tmp;
            Node<T> *p = nullptr;
            if (current->right.get() != nullptr) {
                current = current->right.get();
                while (current->left.get() != nullptr) {
                    current = current->left.get();
                }
            } else {
                p = current->parent;
                while (p != nullptr && current == p->right.get()) {
                    current = p;
                    p = p->parent;
                }
                current = p;
            }
            return *this;
        }

        bool operator==(const inorder_iterator &other) const {
            return current == other.current;
        }

        bool operator!=(const inorder_iterator &other) const {
            return current != other.current;
        }

        auto getCurrent() {
            return current;
        }
    };

    template<class T>
    class postorder_iterator {
    private:
        Node<T> *current;
        Node<T> *root;
        bool done;
    public:
        explicit postorder_iterator(Node<T> *ptr = nullptr) : current(ptr) {
            if (current == nullptr) {
                return;
            }
            done = false;
            root = current;
            while (current->left.get() != nullptr || current->right.get() != nullptr) {
                if (current->left.get() == nullptr) {
                    current = current->right.get();
                    continue;
                }
                current = current->left.get();
            }
        }

//        postorder_iterator(Node<T> *root, Node<T> * current, bool done)
//        {
//            this->root = root;
//            this->current = current;
//            this->done = done;
//        }

        T &operator*() const {
            return current->data;
        }

        T *operator->() const {
            return &(current->data);
        }

        postorder_iterator &operator++() {

//            auto copy = postorder_iterator<T>{root, current, done};
            if (current == root) {
                current = nullptr;
                return *this;
            }
            if (current->parent == root) {
                if (current->parent->right.get() == current || current->parent->right.get() == nullptr) {
                    current = root;
                    return *this;
                }

                // We explored the right wing part of the tree, begin the same process by traversing right, and then going down to most left
                current = current->parent->right.get();
                while (current->left.get() != nullptr || current->right.get() != nullptr) {
                    if (current->left.get() == nullptr) {
                        current = current->right.get();
                        continue;
                    }
                    current = current->left.get();
                }
                return *this;
            }

            if (current->parent->left.get() == current && current->parent->right.get() != nullptr) {
                // we are a left child, and the parent has a right child, meaning there's an unexplored subtree
                current = current->parent->right.get();
                while (current->left.get() != nullptr || current->right.get() != nullptr) {
                    if (current->left.get() == nullptr) {
                        current = current->right.get();
                        continue;
                    }
                    current = current->left.get();
                }
            }
                //we are a current child WITHOUT right, go up to the parent
            else if (current->parent->left.get() == current || current->parent->right.get() == current) {
                current = current->parent;
            }
//            } else if (current->parent->right.get() == current) {
//                current = current->parent;
//            } else {
//                return *this;
//            }
            return *this;
        }

        postorder_iterator operator++(int) {
            postorder_iterator tmp(*this);
            ++(*this);
            return tmp;
            if (current == root) {
                current = nullptr;
                return *this;
            }
            if (current->parent == root) {
                if (current->parent->right.get() == current || current->parent->right.get() == nullptr) {
                    current = root;
                    return *this;
                }

                // We explored the right wing part of the tree, begin the same process by traversing right, and then going down to most left
                current = current->parent->right.get();
                while (current->left.get() != nullptr || current->right.get() != nullptr) {
                    if (current->left.get() == nullptr) {
                        current = current->right.get();
                        continue;
                    }
                    current = current->left.get();
                }
                return *this;
            }

            if (current->parent->left.get() == current && current->parent->right.get() != nullptr) {
                // we are a left child, and the parent has a right child, meaning there's an unexplored subtree
                current = current->parent->right.get();
                while (current->left.get() != nullptr || current->right.get() != nullptr) {
                    if (current->left.get() == nullptr) {
                        current = current->right.get();
                        continue;
                    }
                    current = current->left.get();
                }
            }
                //we are a current child WITHOUT right, go up to the parent
            else if (current->parent->left.get() == current || current->parent->right.get() == current) {
                current = current->parent;
            }
//            } else if (current->parent->right.get() == current) {
//                current = current->parent;
//            } else {
//                return *this;
//            }
            return *this;
        }

        bool operator==(const postorder_iterator &other) const {
            return current == other.current;
        }

        bool operator!=(const postorder_iterator &other) const {
            return current != other.current;
        }

        auto getCurrent() {
            return current;
        }
    };

    template<class T>
    class preorder_iterator {
    private:
        Node<T> *current;
        Node<T> *root;
        Node<T> *prev;
        Node<T> * end;
        bool done;
    public:

        explicit preorder_iterator(Node<T> *ptr = nullptr) : current(ptr) {
            if (current == nullptr) {
                return;
            }
            done = false;
            root = current;
            prev = current;

            end = current;

            while(end->right.get() != nullptr || end->left.get() != nullptr)
            {
                if(end->right.get() != nullptr)
                {
                    end = end->right.get();
                    continue;
                }
                if(end->left.get() != nullptr)
                {
                    end = end->left.get();
                }
            }
        }

//        preorder_iterator(Node<T>* root, Node<T>* current, bool done)
//        {
//            root = root;
//            current = current;
//            done = done;
//        }

        T &operator*() const {
            return current->data;
        }

        T *operator->() const {
            return &(current->data);
        }

        auto getCurrent() {
            return current;
        }


        //38, 5, 1, 9, 8, 15, 13, 45, 47, 46
        preorder_iterator &operator++() {
            if(current == end)
            {
                current = nullptr;
                return *this;
            }
            if (current->left != nullptr) {
                // visit left
                current = current->left.get();
            } else if (current->right.get() != nullptr) {
                current = current->right.get();
            } else {
                // both children are null, look for right subtree that isn't mine
                Node<T> *p = current;
                while (p->parent != nullptr) {
                    if (current == current->parent->left.get()) {
                        // look for any right subtree
                        if (p->parent->right.get() != nullptr) {
                            current = p->parent->right.get();
                            return *this;
                        }
                    } else {
                        if (p->parent->right.get() != p) // not my subtree
                        {
                            current = p->parent->right.get();
                            return *this;
                        }
                    }
                    p = p->parent;
                }
            }
            return *this;
        }

        //make copy, do ++*this, return copy
        //this is postcrement - it++



        preorder_iterator operator++(int) {
            preorder_iterator tmp(*this);
            ++(*this);
            return tmp;
            if(current == end)
            {
                current = nullptr;
                return *this;
            }
            if (current->left != nullptr) {
                // visit left
                current = current->left.get();
            } else if (current->right.get() != nullptr) {
                current = current->right.get();
            } else {
                // both children are null, look for right subtree that isn't mine
                Node<T> *p = current;
                while (p->parent != nullptr) {
                    if (current == current->parent->left.get()) {
                        // look for any right subtree
                        if (p->parent->right.get() != nullptr) {
                            current = p->parent->right.get();
                            return *this;
                        }
                    } else {
                        if (p->parent->right.get() != p) // not my subtree
                        {
                            current = p->parent->right.get();
                            return *this;
                        }
                    }
                    p = p->parent;
                }
            }
            return *this;
        }

        bool operator==(const preorder_iterator &other) const {
            return current == other.current;
        }

        bool operator!=(const preorder_iterator &other) const {
            return current != other.current;
        }
    };

    template<class T>
    class BinaryTree {
        std::unique_ptr<Node<T>> root;
        bool first_run = true;
    public:
        BinaryTree() = default;

        BinaryTree<T>(BinaryTree<T> &&) noexcept = default; // move ctor

        //copy constructor
        BinaryTree<T>(const BinaryTree<T> &other) {
            copy_tree(root, other.root);
        }

        void copy_tree(std::unique_ptr<Node<T>> &target, const std::unique_ptr<Node<T>> &node) {
            if (node != nullptr) {
                target = std::unique_ptr<Node<T>>(new Node<T>{node->data}); // copy data
                copy_tree(target->left, node->left);
                copy_tree(target->right, node->right);
                if (target->left != nullptr) {
                    target->left->parent = target.get();
                }
                if (target->right != nullptr) {
                    target->right->parent = target.get();
                }
            }
        }

        BinaryTree<T> &add_root(T value) {
            if (first_run) {
                first_run = false;
                root = std::unique_ptr<Node<T>>(new Node<T>{value});
            } else {
                root->data = value;
            }
            //check if root defined
            return *this;
        }

        auto search(T data) {
            auto it = begin_inorder();
            {
                while (it != end_inorder()) {
                    if (*it == data) {
                        return it.getCurrent();
                    }
                    it++;
                }
            }
            return it.getCurrent();
        }

        BinaryTree<T> &add_left(T parent, T left) {
            auto node = search(parent); // returns normal pointer
            if (node == nullptr) {
                throw std::invalid_argument("No such parent");
            }
            if (node->left == nullptr) {
                //no left child, need to create one and set it as the left child of node
                node->left = std::unique_ptr<Node<T>>(new Node<T>{left});
                //make left know its parent
                node->left->parent = node;
            } else {
                //just change the left data
                node->left->data = left;
            }
            return *this;
        }

        BinaryTree<T> &operator=(BinaryTree<T> &&other)
        noexcept {
            root = std::move(root);
            first_run = other.first_run;
        }

        BinaryTree<T> &operator=(const BinaryTree<T> &other) {
            // copy ctor
            copy_tree(root, other.root);
            return *this;
        }

        friend std::ostream &operator<<(std::ostream &stream,
                                        const BinaryTree<T> &tree) {
            return stream;
        }

        BinaryTree<T> &add_right(T parent, T right) {
            auto node = search(parent);
            if (node == nullptr) {
                throw std::invalid_argument("No such parent");
            }
            if (node->right == nullptr) {
                //no right child, need to create one and set it as the right child of node
                node->right = std::unique_ptr<Node<T>>(new Node<T>{right});
                //make right know its parent
                node->right->parent = node;
            } else {
                //just change the right data
                node->right->data = right;
            }
            return *this;
        }

        auto begin_preorder() {
            return preorder_iterator<T>{root.get()};
        }

        auto end_preorder() {
            return preorder_iterator<T>{nullptr};
        }

        auto begin_inorder() {
            return inorder_iterator<T>{root.get()};
        }

        auto end_inorder() {
            return inorder_iterator<T>{nullptr};
        }

        auto begin_postorder() {
            return postorder_iterator<T>{root.get()};
        }

        auto end_postorder() {
            return postorder_iterator<T>{nullptr};
        }

        auto begin() {
            return inorder_iterator<T>{root.get()};
        }

        auto end() {
            return inorder_iterator<T>{nullptr};
        }

        ~BinaryTree()
        = default;
    };

}
// namespace ariel