#include <ostream>

class String {
    char* data_ = nullptr;
    size_t size_;
    size_t capacity_;

public:
    // returns the number of characters in the 'c' not including the terminator '\0'
    static int len(const char* c) {
        // this will count up until we reach the terminator character
        int size = 0;

        while (true) {
            // check weather the 'c' is '\0'
            if (*c == 0)
                break;

            else {
                // add one to the size
                size++;
                // move to the next char in 'c'
                c++;
            }
        }

        return size;
    }

    // copy from src to dest
    static void strcpy(char* dest, const char* src) {
        // add one for null terminator
        int size_of_src = String::len(src) + 1;

        for (int i = 0; i < size_of_src; i++) {
            *dest = *src;
            dest++;
            src++;
        }
    }

    String() {
        size_ = 0;
        capacity_ = 0;
    }

    String(char c) {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        data_ = new char[1];    

        size_ = 1;
        capacity_ = 1;
    }

    String(const char* c) {
        // first free up old data_
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        // this gets the size but does not include the '\0' character        
        int char_length = String::len(c);

        // add 1 to size to include the \0 character
        data_ = new char[char_length + 1];

        String::strcpy(data_, c);

        size_ = char_length;
        // capacity includes null terminator
        capacity_ = char_length + 1;
    }

    String(const String& s) {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        // add one to size to include null terminator
        data_ = new char[s.size() + 1];
        strcpy(data_, s.c_str());

        size_ = s.size();
        capacity_ = s.capacity();
    }

    String(String&& s) {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        data_ = s.data_;
        s.data_ = nullptr;

        size_ = s.size();
        capacity_ = s.capacity();
    }
    
    String& operator=(const char* c) {
        // first free up old data_
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        // this gets the size but does not include the '\0' character
        int char_length = String::len(c);
        // add 1 to size to include the \0 character
        data_ = new char[char_length + 1];

        String::strcpy(data_, c);

        size_ = char_length;
        // capacity includes null terminator
        capacity_ = char_length + 1;

        return *this;
    }

    String& operator=(const String& s) {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        // add one to size to include null terminator
        data_ = new char[s.size() + 1];
        strcpy(data_, s.c_str());

        size_ = s.size();
        capacity_ = s.capacity();

        return *this;
    }

    String& operator=(String&& s) {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        // transfer ownership because 's' is being delete
        data_ = s.data_;
        s.data_ = nullptr;

        size_ = s.size();
        capacity_ = s.capacity();

        return *this;
    }

    ~String() {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }
        
        size_ = 0;
        capacity_ = 0;
    }

    char& operator[](size_t index) const {
        // check that the ptr is not nullptr
        if (!data_ || size_ == 0) throw std::out_of_range("OUT OF RANGE: String has no data\n");

        return data_[index];
    }

    bool operator==(const String& other) const {
        if (size_ != other.size()) return false;
        
        for (int i = 0; i < size_; i++) {
            if ((*this)[i] != other[i]) return false;
        }

        return true;
    }

    bool operator!=(const String& other) const {
        if (*this == other) return false;

        else return true;
    }

    bool operator<(const String& other) const {
        int this_sum = 0, other_sum = 0;

        for (int i = 0; i < this->size_; i++) {
            // add the ASCII value of data_[i]
            this_sum += (int)data_[i];
        }

        for (int i = 0; i < other.size(); i++) {
            other_sum += other.data_[i];
        }

        if (other_sum > this_sum) return true;

        else return false;
    }

    bool operator>(const String& other) {
        if (*this < other) return false;
        else return true;
    }



    size_t size() const noexcept {
        return size_;
    }

    size_t capacity() const noexcept {
        return capacity_;
    }

    char* c_str() const noexcept {
        if (data_)
            return data_;
        

        else return "";
    }

    char at(size_t index) {
        if (index >= size_ || index < 0) throw std::out_of_range("ERROR: Index out of range\n");

        else return (*this)[index];
    }

    // empty the string
    void empty() {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }

        size_ = 0;
        capacity_ = 0;
    }

    void push_back(char c) {
        // check there is enough space for new character
        if (capacity_ >= size_ + 1) {
            data_[size_] = c;
            size_++;
            data_[size_] = '\0';
        }

        else {
            resize(capacity_ + 1);

            // this will write over the null terminator
            data_[size_] = c;
            size_++;
            // add the null terminator back
            data_[size_] = '\0';
        }
    }

    void pop_back() {
        if (size_ > 0) {
            data_[size_ - 1] = '\0';
            size_--;
        }
    }



    void append(const char* c) {
        size_t char_length = String::len(c);

        // do we need to increase the size of the string?
        if (size_ + char_length > capacity_) {
            // double the size on random reserves to reduce number of reserves
            reserve(size_ * 2);
        }
        
        // add data from 'c' to this
        for (int i = 0; i < char_length; i++) {
            data_[size_ + i] = c[i];
        }

        size_ += char_length;
    }

    void insert(size_t pos, const char c) {
        if (pos > capacity_) throw std::out_of_range("Inserting out of string's capacity\n");

        if (capacity_ < size_ + 1) {
            resize(capacity_ + 1);
        }

        if (pos != 0) {
            for (int i = size_; i >= pos; i--) {
                if (i == pos) {
                    data_[i] = c;
                }
                
                else {
                    data_[i] = data_[i - 1];
                }

                size_++;
                data_[size_] = '\0';
            }
        }

        // if the insertion is happening at i = 0
        else {
            for (int i = size_; i > 0; i--) {
                data_[i] = data_[i - 1];
            }

            data_[0] = c;

            size_++;
            data_[size_] = '\0';
        }

        size_++;
        data_[size_] = '\0';
    }

    // erase at 'pos', 'len' characters
    void erase(size_t pos, size_t len) {
        if (pos + len > size_) throw std::out_of_range("Pos and/or len out of range\n");

        for (int i = pos; i < size_ - len; i++) {
            data_[i] = data_[i + len];
        }

        for (int i = size_ - len; i < size_; i++) {
            data_[i] = '\0';
        }

        size_-= len;
    }

    void resize(size_t new_size) {
        if (new_size > size_) {
            reserve(new_size);
            size_ = new_size;
        }

        else if (new_size < size_) {
            char* new_data = new char[new_size];

            for (int i = 0; i < new_size; i++) {
                new_data[i] = data_[i];
            }

            delete data_;
            data_ = new_data;

            capacity_ = new_size;
            size_ = new_size;
        }
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) return;

        char* new_data = new char[new_capacity];

        int old_capacity = capacity_;
        
        strcpy(new_data, data_);

        delete data_;
        data_ = new_data;

        new_data = nullptr;

        capacity_ = new_capacity;

        // set all new characters to null terminator
        for (int i = old_capacity; i < new_capacity; i++) {
            (*this)[i] = '\0';
        }
    }

    // swap data
    void swap(String& s) {
        char* this_data = data_;
        size_t this_size = size_;
        size_t this_capacity = capacity_;

        this->data_ = s.c_str();
        this->size_ = s.size();
        this->capacity_ = s.capacity();
        
        s.data_ = this_data;
        s.size_ = this_size;
        s.capacity_ = this_capacity;
    }
};

// operator for std::cout
std::ostream& operator<<(std::ostream& os, const String& s) {
    if (s.size() > 0)
        os << s.c_str();
    
    return os;
}