#include <iostream>
#include <stack>
//#include <algorithm>
//#include <forward_list>
using ll =  long long ;
template <typename T>
struct Dynarr;

template <typename U>
std::ostream& operator << (std::ostream& out, Dynarr<U>& a);

template <typename T>
struct Dynarr {
private:
    struct Block;
//    struct iterator;
    Block* head = nullptr;
    Block* tail = nullptr;
    int size_block = 0;
    int size = 0;
public:
    Dynarr():size_block(50),head(new Block(50)),tail(new Block){
        head->next = tail ;
        tail->prev = head;
    }
    Dynarr(int n): size_block(n), head(new Block(n)),tail(new Block){
        head->next = tail;
        tail->prev = head;
    }
    Dynarr(const Dynarr& other):
            head(new Block(other.size_block)), size_block(other.size_block){
        Block * tmp = other.head;
        *head = *tmp;
        if(tmp->next !=tail) {
            while (true) {
                head->next = new Block(*(tmp->next));
                head->next->prev = head;
                tmp = tmp->next;
                if (tmp->next == tail)
                    break;
            }
            tmp->next = tail;
            tail->prev = tmp->next;
        }
    }
    void swap(Dynarr& other){
        std::swap(other.head, head);
        std::swap(size_block,other.size_block);
        std::swap(size,other.size);
    }
    Dynarr& operator =(Dynarr& other){
        Dynarr tmp = other;
        this->swap(tmp);
        return *this;
    }
    /*
    void add(const T& val) {
        if (sz < cap) {
            arr[sz++] = val;
            return;
        }
        if (next != nullptr) {
            next->add(val);
            return;
        }
        next = new Block(cap);
        next->next = tail;
        tail->prev = next;
        next->prev = this;
        next->add(val);
    }*/
    void push_back(const T& val) {
        Block *tmp = head;
        if (tmp->sz < tmp->cap) {
            tmp->arr[tmp->sz++] = val;size++;
            return;
        }
        while (tmp->next != tail) {
            tmp = tmp->next;
        }
        if(tmp->next == tail)
        {
            if (tmp->sz < tmp->cap) {
                tmp->arr[tmp->sz++] = val;size++;
                return;
            }
            tmp->next = new Block(tmp->cap);
            tmp->next->prev = tmp;
            tmp = tmp->next;
            tmp->arr[tmp->sz++] = val;
            tail->prev = tmp;
            tmp->next = tail;

        }
        size++;
    }
    T& operator [](const size_t& n){ // access by index;
        Block* tmp = head;
        ll nn = static_cast<ll>(n);
        for (;;) {
            if(tmp == tail)
                throw std::runtime_error("check index(or Matb)");
            if (nn < tmp->sz)
                return tmp->arr[nn];
            nn-=tmp->sz;
            tmp = tmp->next;

        }
    }
    void insert(const ll idx , const T& val) {// inserts right after index
        Block* tmp = head;
        ll index = idx;
        while(index - tmp->sz >= 0) {
            index -= tmp->sz;
            if(tmp->next == tail)
                throw std::runtime_error("bad insertion (out of range)\n");
            tmp = tmp->next;
            if(index - tmp->sz < 0)
                break;
        }
        if(index == tmp->sz - 1 && tmp->sz < tmp->cap){
            tmp->arr[tmp->sz++] = val;
            return;
        }
        if(index == tmp->sz - 1)
        {
            this->push_back(val);
//            Block* creature = new Block(head->cap);
//            creature->sz = 1;
//            creature->arr[0] = val;
//            creature->next = tmp->next;
//            creature->prev = tmp;
//            tmp->next = creature;
//            creature->next->prev = creature;
            return;
        }
        if(index < tmp->sz){
            Block* creature = new Block(head->cap);
            creature->sz = 1;
            creature->arr[0] = val;
            creature->next = tmp->next;
            creature->prev = tmp;
            tmp->next = creature;
//            if(creature->next !=nullptr)
            creature->next->prev = creature;
            std::swap(creature->arr[0], tmp->arr[tmp->sz - 1]);
            for (int i = tmp->sz - 1; i > index + 1; --i) {// -2 ?? ?
                std::swap(tmp->arr[i], tmp->arr[i - 1]);
            }
        }
        size++;
    }
    void remove(const ll idx){
        ll index = idx;
        Block* tmp = head;
        while(index - tmp->sz >= 0) {
            index -= tmp->sz;
            if(tmp->next == nullptr)
                throw std::runtime_error("check index in remove method\n");
            tmp = tmp->next;
            if(index - tmp->sz < 0)
                break;
        }
        --(tmp->sz);
        if(tmp->sz == 0){
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
            delete tmp;
            size--;
            return;
        }
        for (int i = index; i < tmp->sz - index ; ++i) {
            std::swap(tmp->arr[i],tmp->arr[1 + i]);
        }
        size--;
    }

    struct iterator{
        Block* block;
        int pos;
        iterator() = delete;
        iterator(const Dynarr& a): block(&a.head) , pos(0){}
        iterator(Block* b): block(b) , pos(0){}
        T& operator*(){
            return block->arr[pos];
        }
        iterator& operator+=(int n){
            while(pos + n > 0 && pos + n >= block->sz) {
                n -= block->sz - pos;
                pos = 0;
                block = block->next;
            }
            while(pos + n < 0) {
                n += -pos + block->sz;
                pos = block->sz - 1;
                block = block->prev;
            }
            pos += n;
            return *this;
        }
        T* operator->(){
            return block->arr + pos;
        }
        inline bool operator ==(const iterator& other) const{
            return (pos == other.pos) && (block == other.block);
        }
        inline bool operator !=(const iterator& other) const{
            return *this != other;
        }
        iterator& operator++() {
            return *(this) += 1;
        }
        iterator operator ++(int){
            iterator copy = *this;
            (*this)+=1;
            return copy;
        }
        iterator& operator-=(int n){
            return *this+=(-n);
        }

    };
    iterator begin() const{
        return iterator(head);
    }
    iterator end() const{
        return iterator(tail);
    }

    ~Dynarr(){
        Block * copy_of_last_node = head;
        while(head != nullptr){
            head = head->next;
            delete copy_of_last_node;
            copy_of_last_node = head;
        }
    }
    friend std::ostream& operator << <T>(std::ostream& out, Dynarr<T>& a);
};
template <typename T>
struct Dynarr<T>::Block {
    Block *next = nullptr;
    Block *prev = nullptr;
    int cap = 0;
    int sz = 0;
    T *arr = nullptr;
    Block() = default;
    Block(int n) : arr(new T [n]), cap(n), next(nullptr), prev(nullptr), sz(0){}
    Block(const Block& other): next(nullptr), prev(nullptr), cap(other.cap), sz(other.sz), arr(new T[cap]){
        memcpy(arr, other.arr, cap*sizeof(T));
    }
//    bool operator ==(const Block& other) const{
//        return
//    }
    Block& operator =(Block& other){
        Block tmp (other);
//        memcpy(arr, other.arr, other.sz *sizeof(T));
        std::swap(arr, tmp.arr);
        std::swap(sz,tmp.sz);
        std::swap(cap,tmp.cap);
        return *this;
    }
    ~Block(){
        delete[] arr;
    }
};
template<typename T>
void sort(Dynarr<T>& a){
    for (int i = 0; i < a.size ; ++i) {
        for (int j = i; j < a.size; ++j) {
            if(a[i] > a[j]){
                std::swap(a[i] , a[j]);
            }
        }
    }
}
template <typename T>
std::ostream& operator <<(std::ostream& out, Dynarr<T>& a){
//        Dynarr<T>::Block* runthrough = a.head;
    typename Dynarr<T>::Block * shit = a.head;
    while(shit != a.tail){
        for (int i = 0; i < shit->sz; ++i) {
            out << shit->arr[i] << ' ';
        }
        shit = shit->next;
    }
    return out;
}

int main() {
    using iterator = Dynarr<int>::iterator;
    int n , sz ;
    std::cin >> n >> sz;
    Dynarr<int> a(n);
//    Dynarr<int>::Block as;
    for (int i = 0; i < sz; ++i) {
        int z;
        std::cin >> z; a.push_back(z);
    }
    iterator it = a.begin();
    for (int i = 0; i < sz; ++i) {
         std::cout << *(it++) << ' ';
    }
    std::cout << (it == a.end()) << std::endl;
    std::cout << a << ' ';
    std::cout << '\n';

    return 0;
}