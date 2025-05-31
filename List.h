template <class T>
class List {
public:
    int _size = 0;
    // element of List
    class Node {
    public:
        Node() {};
        Node(T value) : val(value) {}
        Node(List<T>::Node&& other) {
            next = other.next;
            prev = other.prev;
        }
        
        T val;
        // pointer to next element and prevous element
        List<T>::Node* next = nullptr;
        List<T>::Node* prev = nullptr;

        // dont delete next or prev to avoid double deletes
        ~Node() {
            next = nullptr;
            prev = nullptr;
        }

        // CHECK THIS
        List<T>::Node operator=(const List<T>::Node& other) {
            next = other.next;
            prev = other.prev;

            return *this;
        }
    };
    
    // pointer to first and last element of list
    List<T>::Node* head = nullptr;
    List<T>::Node* tail = nullptr;

private:
    void delete_node(List<T>::Node* node_to_delete) {
        if (node_to_delete == this->tail) {
            throw std::runtime_error("Attemping to delete tail node\n");
            return;
        }

        if (node_to_delete == this->head) {
            List<T>::Node* new_head = head->next;

            tail->next = new_head;
            new_head->prev = tail;

            head->next = nullptr;
            head->prev = nullptr;

            delete head;

            head = new_head;
            this->_size--;
        }

        // link over the node that wil be removed
        node_to_delete->prev->next = node_to_delete->next;
        node_to_delete->next->prev = node_to_delete->prev;

        node_to_delete->next = nullptr;
        node_to_delete->prev = nullptr;

        delete node_to_delete;

        this->_size--;
    }

public:
    class Iterator : public Node {
    public:
        List<T>::Node* ptr = nullptr;

        // create an iterator starting at a given node
        Iterator(List<T>::Node* head) {
            ptr = head;
        }

        // constructor creates an Iterator from the 'list' head node passed through as argument
        Iterator(const List<T>& list) noexcept {
            ptr = list.head;
        }

        Iterator(const Iterator& other) noexcept {
            ptr = other.ptr;
        }
        
        Iterator(Iterator&& other) noexcept {
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        
        ~Iterator() {
            // dont wnat to delete ptr because it still belongs to another list
            ptr = nullptr;
        }

        Iterator& operator++(){
            ptr = ptr->next;
            
            return *this;
        }

        Iterator& operator=(const Iterator& other) {
            if (this != &other)
                this->ptr = other.ptr;

            return *this;
        }

        Iterator&& operator=(Iterator&& other) {
            if (this != &other)
                this->ptr = other.ptr;
            

            return *this;
        }

        Iterator& operator--(){
            ptr = ptr->prev;

            return *this;
        }

        T& operator*(){
            return ptr->val;
        }

        bool operator==(const Iterator& other) const {
            if (this->ptr == other.ptr) return true;
            return false;
        }

        bool operator!=(const Iterator& other) const {
            if (this->ptr == other.ptr) return false;
            return true;
        }
    
        Iterator& operator+=(unsigned int n){
            for (int i = 0; i < n; i++) {
                ++(*this);
            }

            return *this;
        }
    };

    // create and return iterator linked to instance of 'List'
    Iterator create_iterator() {
        return Iterator(*this);
    }

    Iterator begin() {
        return Iterator(*this);
    }

    // returns an iterator pointing to the last pointer
    Iterator end() {
        List<T>::Iterator it = create_iterator();
        --it;
        return it;
    }



    List() {
        tail = new List<T>::Node();
        head = tail;
    }

    ~List() {
        this->clear();
    }

    // copy constructor
    List(const List& other) {
        _size = other._size;
        head = other.head;
        tail = other.tail;
    }
    
    List(List&& other) noexcept {
        head = other.head;
        tail = other.tail;
        _size = other.size();
        
        other.head = nullptr;
        other.tail = nullptr;
        otehr._size = 0;
    }

    // copy assignment
    List& operator=(const List& other) {
        head = other.head;
        tail = other.tail;
        _size = other.size();

        return *this;
    }

    // move assignment
    List& operator=(List&& other) noexcept {
        head = other.head;
        tail = other.tail;
        _size = other.size();

        return *this;
    }



    // returns true if List is empty
    bool empty() const noexcept {
        if (size == 0) return true;
        return false;
    };

    int size() const noexcept {return _size;};



    // return refrence to first element
    typename List<T>::Node& front() {
        if (size == 0) {
            throw std::out_of_range("List is empty\n");
        }

        return *head;
    };

    // return refrence to last element
    typename List<T>::Node& back() {
        if (size == 0) {
            throw std::out_of_range("List is empty\n");
        }

        return *(tail->prev);
    };



    // empty the list
    void clear() {
        // pop_back will change '_size' so it cant be used in the for loop condition
        int size = _size;

        for (int i = 0; i < size; i++) {
            this->pop_back();
        }
    }

    // add element to the start of the list
    void push_front(const T& value){
        List<T>::Node* new_node = new List<T>::Node(value);
        
        if (_size == 0) {
            head = new_node;

            tail->prev = new_node;
            tail->next = new_node;
            
            new_node->next = tail;
            new_node->prev = tail;
        }

        else {
            // double link the new node backwards
            tail->next = new_node;
            new_node->prev = tail;
            
            // double link the new node forwards
            new_node->next = head;
            head->prev = new_node;

            // this new node becomes the head of the list
            head = new_node;
        }

        _size++;
    }

    // add element to back of list just before the tail node
    void push_back(const T& value){
        List<T>::Node* new_node = new List<T>::Node(value);

        if (_size == 0){
            head = new_node;

            tail->next = new_node;
            tail->prev = new_node;

            new_node->next = tail;
            new_node->prev = tail;
        }
        
        else {
            // double link the new node backwards
            (tail->prev)->next = new_node;
            new_node->prev = tail->prev;

            // double link the new node forwards
            new_node->next = tail;
            tail->prev = new_node;
        }

        _size++;
    }

    // remove the first element
    void pop_front() {
        if (_size > 1) {
            List<T>::Node* node_to_delete = head;
            List<T>::Node* new_head = head->next;

            // forward link the tail to the new head node
            tail->next = new_head;
            // backwards link the new head to the tail
            new_head->prev = tail;

            delete node_to_delete;
            head = new_head;

            _size--;
        }

        else if (_size == 1) {
            delete head;

            head = tail;
            head->next = tail;
            head->prev = tail;

            _size--;
        }
    }
    
    // remove the last element
    void pop_back() {
        if (_size > 1) {
            List<T>::Node* node_to_delete = tail->prev;

            // pop_back is called by .clear() which is called by the destructor of 'List'
            // this means when a function exits, the compiler may have already deleted the nodes of 'List'
            // as they would've been defined before 'this', and therefore all nodes except tail will already be freed
            if (!tail->prev) return;

            List<T>::Node* new_last_node = node_to_delete->prev;// ACCESS VIOLATION
            
            // link the tail backwards
            tail->prev = tail->prev->prev;
            // link the new final node forwards to tail node
            new_last_node->next = tail;

            delete node_to_delete;
            
            _size--;
        }

        else if (_size == 1) {
            delete tail->prev;

            tail->prev = tail;
            tail->next = tail;

            head = tail;

            _size--;
        }
    }

    // resizes list with any new nodes having value 0
    // if nodes must be removed, they will be removed from the back
    void resize(unsigned int n_size) {
        if (n_size == _size) return;
        
        else if (n_size > _size) {
            int num_to_add = n_size - _size;
            
            for (int i = 0; i < num_to_add; i++)
                this->push_back(0);
        }

        else if (n_size < _size) {
            int num_to_remove = _size - n_size;

            for (int i = 0; i < num_to_remove; i++)
                this->pop_back();
        }
    }

    // resizes list with any new elements as the given value
    void resize(int n_size, const T& val) {
        if (n_size == _size) return;
        
        else if (n_size > _size) {
            int num_to_add = n_size - _size;
            
            for (int i = 0; i < num_to_add; i++)
                this->push_back(val);
        }

        else if (n_size < _size) {
            int num_to_remove = _size - n_size;

            for (int i = 0; i < num_to_remove; i++)
                this->pop_back();
        }
    }

    // adds all elements from 'other' to 'this' list just before pos
    void splice(List<T>::Iterator& pos, List<T>& other) {
        if (pos == this->begin()) {
            List<T>::Node* start_link = this->tail;
            start_link->next = nullptr;

            List<T>::Node* end_link = this->head;
            end_link->prev = nullptr;

            // link start of list forwards and backwards
            start_link->next = other.head;
            other.head->prev = start_link;

            // link end of list forwards and backwards
            // ignore the sentinal node of other
            end_link->prev = other.tail->prev;
            other.tail->prev->next = end_link;

            // the head of 'other' is now the head because the splice happened at the first node
            this->head = other.head;

            // unlink the sentinel node
            other.tail->prev = other.tail;
            other.tail->next = other.head;

            this->_size += other._size;
            other._size = 0;
        }

        else if (pos == this->end()) {
            List<T>::Node* start_link = this->tail->prev;
            start_link->next = nullptr;

            List<T>::Node* end_link = this->tail;
            end_link->prev = nullptr;

            start_link->next = other.head;
            other.head->prev = start_link;

            end_link->prev = other.tail->prev;
            other.tail->prev->next = end_link;
            
            // unlink the sentinel node
            other.tail->prev = other.tail;
            other.tail->next = other.head;

            this->_size += other._size;
            other._size = 0;
        }

        else {
            // break the forward link where the list is being inseted
            // start_link is the node just before the splice
            List<T>::Node* start_link = pos.ptr->prev;
            start_link->next = nullptr;
            
            // break the backwards link
            // end_link is the node just after the splice
            List<T>::Node* end_link = pos.ptr;
            end_link->prev = nullptr;
            
            // link the start node forward/backwards to the list being inserted
            start_link->next = other.head;
            other.head->prev = start_link;
            
            // link the final node forwards/backwards to the list being inserted
            // ignore the final sentinel node 'tail' and instead link to the final node with an actual value
            end_link->prev = other.tail->prev;
            other.tail->prev->next = end_link;
            
            // unlink the sentinel node
            other.tail->prev = other.tail;
            other.tail->next = other.head;
            
            this->_size += other._size;
            other._size = 0;
        }
    }

    // remove all elements with value 'val'
    void remove(T val) {
        if (this->_size > 1) {
            auto it = this->create_iterator();
            // skip the first two nodes so that we can check the node previous to the iterator
            // to prevent deleting a node the iterator points to
            // we skip the head node becuase the linking is slightly different when deleting the head node
            ++(++it);
            
            for (it; it != this->end(); ++it) {
                if (it.ptr->prev->val == val) {
                    List<T>::Node* node_to_remove = it.ptr->prev;
                    
                    // link nodes next to 'node_to_remove' forwards and backwards
                    node_to_remove->prev->next = node_to_remove->next;
                    node_to_remove->next->prev = node_to_remove->prev;
                    
                    // remove 'node_to_remove' from list
                    node_to_remove->next = nullptr;
                    node_to_remove->prev = nullptr;
                    
                    delete node_to_remove;
                    
                    this->_size--;
                }
            }
            
            // check head node
            if (this->head->val == val) {
                List<T>::Node* head = this->head;
                
                // link tail to new head
                this->tail->next = head->next;
                head->next->prev = this->tail;
                
                // update the new head of the list
                this->head = head->next;
                
                // unlink old head node and delete
                head->next = nullptr;
                head->prev = nullptr;
                
                delete head;
                
                this->_size--;
            }

            // the node just before the tail wont be checked in the for loop above
            else if (this->tail->prev->val == val) {
                List<T>::Node* node_to_remove = this->tail->prev;

                // link nodes surrounding 'node_to_remove'
                node_to_remove->prev->next = this->tail;
                this->tail->prev = node_to_remove->prev;

                node_to_remove->next = nullptr;
                node_to_remove->prev = nullptr;

                delete node_to_remove;

                this->_size--;
            }
        }

        else {
            if (this->head->val == val) {
                this->head->next = nullptr;
                this->head->prev = nullptr;

                this->_size = 0;

                this->tail->next = this->tail;
                this->tail->prev = this->tail;

                delete this->head;

                this->head = nullptr;
            }
        }
    }
    
    // remove any copies of elements
    // list MUST be sorted
    void unique() {
        auto it = this->create_iterator();
        
        // list of nodes that need deleting
        std::vector<List<T>::Node*> nodes_to_delete;
        
        // stop one before checking the end node
        for (it; it.ptr != this->tail->prev; ++it) {
            // check weather the next node is the same as the current node
            if (*it == (it.ptr->next->val)) {
                // add to the vector of nodes that must be deleted
                nodes_to_delete.push_back(it.ptr->next);
            }
        }

        for (int i = 0; i < nodes_to_delete.size(); i++) {
            this->delete_node(nodes_to_delete[i]);
        }
    }
};

template <class T>
bool operator==(const List<T>& lhs, const List<T>& rhs) {
    int lhs_size = lhs.size();
    int rhs_size = rhs.size();

    if (rhs_size != lhs_size) return false;

    auto lhs_it = lhs.create_iterator();
    auto rhs_it = rhs.create_iterator();

    for (int i = 0; i < lhs_size; i++) {
        if (*lhs_it != *rhs_it)
            return false;
    }

    return true;
}

template <class T>
bool operator!=(const List<T>& lhs, const List<T>& rhs) {
    if (lhs == rhs) return false;
    return true;
}
