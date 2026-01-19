#include <iostream>
#include <stack>

template <typename T>
class BST {
private:
    // Node structure for tree
    struct Node {
        T data;
        // lesser that 'data'
        Node* left;
        // greater than 'data'
        Node* right;
        // parent node
        Node* head;

        Node(const T& value = 0) : data(value), left(nullptr), right(nullptr), head(nullptr) {}

        T operator*() {
            return data;
        }

        bool operator>(const T& value) const {
            if (data > value) return true;
            else return false;
        }

        bool operator<(const T& value) const {
            if (data < value) return true;
            else return false;
        }
        
        bool operator==(const T& value) const {
            if (data == value) return true;
            else return false;
        }

        ~Node() {
            left = nullptr;
            right = nullptr;
            head = nullptr;
        };
    };

    Node* root;


    // called by the public insert method
    // works as a recursive function
    // 'insert()' works its way (recursively) through the tree, starting at the root node
    // 'node' can only ever be nullptr if the root node is given
    // 'node' is nullptr if the root hasn't been assigned a value yet
    Node* insert(Node* node, const T& value) {

        // if the node was nullptr, then the root node was uninitialized
        if (!node) {
            
            node = new Node;
            // give the root node its value
            node->data = value;
            
            return node;
        }
        
        // dont store duplicate copies
        else if (value == **node) return nullptr;


        else if (value > **node) {
            
            // if the right node is not empty continue through the tree
            if (node->right) {
                insert(node->right, value);
            }

            // if the right node was nullptr
            // we must create a new node and link it to the tree
            else {
                Node* new_node = new Node(value); 

                // link new_node to tree
                node->right = new_node;

                // link the parent node
                new_node->head = node;

                return new_node;
            }
        }


        else if (value < **node) {
            
            // if the left node is not empty continue through the tree
            if (node->left) {
                insert(node->left, value);
            }

            // if the left node was nullptr
            // we must create a new node and link it to the tree
            else {
                Node* new_node = new Node(value);

                // link new_node to tree
                node->left = new_node;

                // link the parent node
                new_node->head = node;

                return new_node;
            }
        }
    }

    // merge two subtrees t1, t2
    // return the new root node of the merged subtrees
    // this method is private because it is designed to only work with 'tree.remove()'
    // t1 must be a smaller node than t2
    Node* merge(Node* t1, Node* t2) {

        // merge is a private function, it is only called in this class
        // so we can be sure that t1 and t2 cant both be nullptr
        if (!t1) return t2;
        else if (!t2) return t1;

        // if t1 > t2 then switch them
        if (**t1 > **t2) {
            Node* temp = t1;
            t1 = t2;
            t2 = temp;
        }

        // move elements from t1 to t2 same as above
        else if (**t1 < **t2) {

            while (true) {

                // gets left most node element in t1, (not necessarily the node lowest down tree)
                Node* left_most_node = findMin(t1);
                
                // if the left_most_node is nullptr, the merge is done
                if (!left_most_node) return t2;
                
                // add a node of value 'left_most_node' to t1
                insert(t2, **left_most_node);
                
                // we need to move the nodes in the right of the subtree to the left so that
                // they get picked up by 'findMin(t1)' at the start of the while loop
                // if we dont move them to the left of t1, they will never be returned by 'findMin()'
                // because it only chooses nodes to the right of t1. The algorightm for moving them to the left
                // is different depending on whether 'left_most_node' is t1
                if (left_most_node->right && left_most_node != t1) {
                    left_most_node->head->left = left_most_node->right;
                    left_most_node->right->head = left_most_node->head;
                }
                
                else if (left_most_node->right && left_most_node == t1) {
                    t1 = left_most_node->right;
                    t1->head = left_most_node->head;
                }

                // now we must remove 'left_most_node' from the tree so that it doesn't continue
                // to loop over the same node forever. If 'left_most_node' doesn't equal 't1' then we just
                // disconnect it
                else if(left_most_node != t1) {
                    // now remove left_most_node from tree
                    left_most_node->head->left = nullptr;
                    left_most_node = nullptr;
                }

                // we know from previous else if statements that 'left_most_node'
                // doesn't have any children so because 'left_most_node' = 't1' because
                // this is the last node in 't1' to be moved to 't2' so we just delete it
                // and set 't1' to nullptr, which is the end condition for the loop
                else if (left_most_node == t1) {
                    delete t1;
                    t1 = nullptr;
                }
            }
        }
    }



    // finds smallest element in tree by always taking left node
    [[nodiscard]] Node* findMin(Node* node) {
        
        if (!node) return nullptr;

        auto it = Iterator(node);

        while (true) {
            if (it.ptr->left) {
                --it;
            }
            
            else 
                return it.ptr;
        }
    }

    // finds largest element in tree by always taking right node
    [[nodiscard]] Node* findMax(Node* node) {
        
        if (!node) return nullptr;

        auto it = Iterator(node);

        while (true) {
            if (it.ptr->right) {
                ++it;
            }
            
            else 
                return it.ptr;
        }
    }



public:
    BST() {
        root = nullptr;
    }

    BST(Node* node) : root(node) {};

    ~BST() {};

    struct Iterator {
        Node* ptr;

        Iterator() {
            ptr = nullptr;
        }

        Iterator(Node* starting_node) {
            ptr = starting_node;
        }

        Iterator(Iterator&& it) {
            ptr = it.ptr;
            it.ptr = nullptr;
        }

        Iterator(const Iterator& it) {
            ptr = it.ptr;
        }

        ~Iterator() {
            ptr = nullptr;
        }

        Iterator& operator=(const Iterator& it) {
            return Iterator(it.ptr);
        }

        Iterator& operator=(Iterator&& it) {
            return Iterator(it.ptr);
        }

        // move to bigger node
        void operator++() {
            ptr = ptr->right;
        }
        
        // move to smaller node
        void operator--() {
            ptr = ptr->left;
        }

        // moves the iterator through the tree 'count' number of times always to the bigger node
        void operator+=(const size_t& count) {
            for (int i = 0; i < count; i++)
                ++(*this);
        }
        
        // moves the iterator through the tree 'count' number of times always to the smaller node
        void operator-=(const size_t& count) {
            for (int i = 0; i < count; i++)
                --(*this);
        }

        T& operator*() const {
            return ptr->data;
        }

        // resets the iterator to point to the root node
        // does not check if tree is empty, so could point to nullptr
        void reset(BST<T> tree) {
            ptr = tree.root;
        }
    };

    Iterator create_iterator() const {
        if (!root) throw std::runtime_error("ROOT NODE WAS NULLPTR\n");

        Iterator it(root);

        return it;
    }


    void insert(const T& value) {
        // call the private insert function (which is recursive), and start it at the root node
        Node* new_subtree = insert(root, value);

        // if root is nullptr then insert will have created the root
        // otherwise we dont need the new_subtree becuase 'insert' linked it to the tree
        if (!root)
            root = new_subtree;
    }

    void remove(const T& value) {
        if (!root) return;

        if (*root == value) {
            Node* old_root = root;
            
            // we are deleting the root node so we need a new root node
            // if the left node exists make that the new root node of tree
            if (root->left) {

                root = root->left;
                // the root node doesn't have a head
                root->head = nullptr;

                // now that the left node is now the root of the tree, we have to merge the 3 subtrees -
                // the left and right subtrees of the new root node and the right subtree of the original root node
                // if there is no right subtree of the new root node, then right subtree becomes the right subtree of the original root node
                if (!root->right) {

                    root->right = old_root->right;
                    root->right->head = root;

                    delete old_root;
                    return;
                }

                // if the left subtree of the new root doesn't exists then move the right subtree of the new root to the left node
                // and make the right subtree of the old root, the right subtree of the new root
                else if (!root->left) {
                    
                    // connect the right subtree's head connection, to the new root
                    old_root->right->head = root;

                    root->left = root->right;
                    root->right = old_root->right;

                    delete old_root;
                    return;
                }

                // merge the two subtrees of the new root node and the right subtree of the old root node 
                else {
                    Node* right_sub_tree = merge(root->right, old_root->right);

                    root->right = right_sub_tree;
                    right_sub_tree->head = root;

                    delete old_root;
                    return;
                }
            }
        }
    }

    // search tree for value
    // iterator moves through tree comparing at each step
    bool search(const T& value) const {
        // if the tree is empty return false
        if (!root) return false;

        auto it = this->create_iterator();
        
        while (true) {
            if (*(it) == value) return true;

            else if (*(it) > value) {

                // if the value to look for is smaller than the current node move to the smaller node
                // but if no node exists to the left, then the value is not in the tree
                if (!it.ptr->left) return false;

                // if a smaller node exists, progress to it
                --it;
            }

            else if (*(it) < value) {
                
                // if the value to look for is larger than the current node move to the larger node
                // but if no node exists to the right, then the value is not in the tree
                if (!it.ptr->right) return false;

                // if a larger node exists, progress to it
                ++it;
            }
        }
    }



    void inorder() const {
        
        std::stack<Node*> stack;
        Node* current = root;

        // keep going until the stack is empty or current is nullptr
        while (current || !stack.empty()) {
            
            // this loop moves left down the tree, eventually current will by nullptr
            // when there are no nodes left, and then the loop will no longer run
            while (current) {

                // add nodes that are very left
                stack.push(current);
                current = current->left;
            }

            // current is always nulltpr after the while loop
            // now it is the left most node
            current = stack.top();
            stack.pop();

            std::cout << **current << " ";

            // move right. If there are no nodes right then current is nullptr
            // and the nest while loop while not run
            // then current will be the top of the stack again and we repeat the process
            current = current->right;
        }

        std::cout << "\n";
    }
};
