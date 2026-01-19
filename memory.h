#include <new>

template <class T>
class shared_ptr {
private:
    T* ptr = nullptr;

    struct ControlBlock {
        public:
        T* cptr;
        size_t strong_count;
        size_t weak_count;
        
        ControlBlock() : strong_count(1), weak_count(0), cptr(nullptr) {};

        // check strong and weak count and update
        void update() {
            if (strong_count == 0) {
                if (cptr) {
                    // cptr was allocated with placement new so must be deleted properly
                    delete cptr;
                    cptr = nullptr;
                }
            }

            if (strong_count == 0 && weak_count == 0) {
                this->~ControlBlock();
                operator delete(this);
            }
        }

        // calling this means both strong_count and weak_count are zero
        ~ControlBlock() {
            if (cptr) {
                printf("deleting %d\n", *cptr);
                delete cptr;
                cptr = nullptr;

            }
        };
    };

    ControlBlock* ctrl = nullptr;

public:
    shared_ptr() {
        ctrl = new ControlBlock();
        ptr = new T;
    }

    shared_ptr(std::nullptr_t);

    explicit shared_ptr(T* n_ptr) {
        ctrl = new ControlBlock();

        ptr = n_ptr;
    }

    shared_ptr(const shared_ptr& other) {
        ptr = nullptr;
        if (ctrl) {
            ctrl->strong_count--;
            ctrl->update();
        }

        ptr = other.ptr;
        other.ctrl->strong_count++;
        ctrl = other.ctrl;
    }

    shared_ptr(shared_ptr&& other) {
        ptr = other.ptr;
        ctrl = other.cltr;

        other.ctrl = nullptr;
        other.ptr = nullptr;
    }

    ~shared_ptr() {
        if (ctrl) {
            ctrl->strong_count--;
            
            ctrl->update();
        }
        
        ptr = nullptr;
        ctrl = nullptr;
    }



    shared_ptr& operator=(const shared_ptr& other) {
        this->ptr = other.ptr;
        other.ctrl->strong_count++;
        this->ctrl = other.ctrl;
    
        return *this;
    }

    shared_ptr& operator=(shared_ptr&& other) {
        this->ptr = other.ptr;
        this->ctrl = other.ctrl;

        other.ctrl = nullptr;
        other.ptr = nullptr;

        // we do not need to change the strong_count here as because we unbound the
        // control block from 'other' the strong_count will not be modified in 'other's destructor
        return *this;
    }

    shared_ptr& operator=(std::nullptr_t);



    T* get() const noexcept {return ptr;}

    T& operator*() const noexcept {
        return *ptr;
    }

    T* operator->() const noexcept {
        return ptr;
    }

    size_t use_count() const noexcept {
        return ctrl->strong_count;
    }

    bool unique() const noexcept {
        if (ctlr->strong_count == 1 && ctrl->weak_count == 0) return true;
        else return false;
    }

    explicit operator bool() const noexcept {
        if (ptr) return true;
        else return false;
    }

    T& operator[](size_t i) const;



    void reset() {
        ctrl->strong_count--;
        ctrl->update();

        ptr = nullptr;
        ctrl = nullptr;
    }

    void reset(T* new_ptr) {
        ctrl->strong_count--;
        ctrl->update();
        ptr = nullptr;

        *this = shared_ptr(new_ptr);
    }

    void swap(shared_ptr<T>& other) noexcept {
        ControlBlock* this_ctrl = this->ctrl;
        T* this_ptr = ptr;

        if (ptr && other.ptr) {
            this->ptr = other.ptr;
            this->ctrl = other.ctrl;

            other.ptr = this_ptr;
            other.ctrl = this_ctrl;
        }

        else if (ptr == nullptr && other.ptr == nullptr) {

        }

        else if (ptr == nullptr) {

        }

        else if (other.ptr == nullptr) {

        }
    }

    static void swap(const shared_ptr<T>& sp1, const shared_ptr<T>& sp2);



    //static make_shared(args...);
};


// NOT SURE IF DONE
template <class T>
class unique_ptr {
private:
    T* ptr;

public:
    unique_ptr() noexcept {
        ptr = nullptr;
    }

    // take ownership of raw pointer p
    explicit unique_ptr(T* p) noexcept {
        ptr = p;
    }

    // move constructor
    unique_ptr(unique_ptr&& u) noexcept {
        ptr = u.ptr;
        u.ptr = nullptr;
    }

    // dont allow copying
    unique_ptr(const unique_ptr&) = delete;

    ~unique_ptr() {
        if (ptr)
            delete ptr;
    }



    // transfer ownership
    unique_ptr& operator=(unique_ptr&& u) noexcept {
        if (ptr)
            delete ptr;
        
        ptr = u.ptr;
        u.ptr = nullptr;
    }

    // reset the pointer
    unique_ptr& operator=(std::nullptr_t) noexcept {
        ptr = nullptr;
    }

    // delete copy assignment
    unique_ptr& operator=(const unique_ptr&) = delete;



    // returns raw pointer
    T* get() const noexcept {
        return ptr;
    }

    // derefrence
    T& operator*() const noexcept {
        return *ptr;
    }

    T* operator->() const noexcept {
        return ptr;
    }

    // checks if not nullptr
    // marked explicit so that it wont be converted to integers silently
    explicit operator bool() const noexcept {
        if (ptr) return true;
        else return false;
    }



    // release ownership and return raw pointer
    T* release() noexcept {
        temporary_ptr = ptr;
        ptr = nullptr;
        return temporary_ptr;
    }

    // deletes current object and take ownership of a new one
    void reset(T* new_ptr = nullptr) noexcept {
        if (ptr)
            delete ptr;

        ptr = new_ptr;
    }

    // swaps managed object with another unique_ptr
    void swap(unique_ptr& u) noexcept {
        temporary_ptr = ptr;
        ptr = u.ptr;
        u.ptr = temporary_ptr;
    }
};