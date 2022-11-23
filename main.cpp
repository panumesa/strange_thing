#include <iostream>
#include <stack>
//#include <algorithm>
//#include <forward_list>
using ll =  long long ;
template <typename T>
struct Dynarr;
template <typename T>
struct Block {
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
        Block<T> tmp (other);
//        memcpy(arr, other.arr, other.sz *sizeof(T));
        std::swap(arr, tmp.arr);
        std::swap(sz,tmp.sz);
        std::swap(cap,tmp.cap);
        return *this;
    }

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
        next->prev = this;
        next->add(val);
    }
    ~Block(){
        delete[] arr;
    }
};
template <typename T>
std::ostream& operator << (std::ostream& out, Dynarr<T>& a);

template <typename T>
struct Dynarr {
//    struct iterator;
    Block<T>* head = nullptr;
    int size_block = 0;
    int size = 0;
    Dynarr():size_block(50),head(new Block<T>(50)){}
    Dynarr(int n): size_block(n), head(new Block<T>(n)){}
    Dynarr(const Dynarr& other):
        head(new Block<T>(other.size_block)), size_block(other.size_block){
        Block<T> * tmp = other.head;
        *head = *tmp;
        if(tmp->next !=nullptr) {
            while (true) {
                head->next = new Block<T>(*(tmp->next));
                head->next->prev = head;
                tmp = tmp->next;
                if (tmp->next == nullptr)
                    break;
            }
        }
    }
    void swap(Dynarr& other){
        std::swap(other.head, head);
        std::swap(size_block,other.size_block);
        std::swap(size,other.size);
    }
    Dynarr& operator =(Dynarr& other){
        this->swap(other);
        return *this;
    }
    void push_back(const T& val){
        head->add(val);
        size++;
    }
    T& operator [](const size_t& n){ // access by index;
        Block<T>* tmp = head;
        ll nn = static_cast<ll>(n);
        for (;;) {
            if(tmp == nullptr)
                throw std::runtime_error("check index(or Matb)");
            if (nn < tmp->sz)
                return tmp->arr[nn];
            nn-=tmp->sz;
            tmp = tmp->next;

        }
    }
    void insert(const ll idx , const T& val) {// inserts right after index
        Block<T> *tmp = head;
        ll index = idx;
        while(index - tmp->sz >= 0) {
            index -= tmp->sz;
            if(tmp->next == nullptr)
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
            Block<T>* creature = new Block<T>(head->cap);
            creature->sz = 1;
            creature->arr[0] = val;
            creature->next = tmp->next;
            creature->prev = tmp;
            tmp->next = creature;
            creature->next->prev = creature;
            return;
        }
        if(index < tmp->sz){
            Block<T>* creature = new Block<T>(head->cap);
            creature->sz = 1;
            creature->arr[0] = val;
            creature->next = tmp->next;
            creature->prev = tmp;
            tmp->next = creature;
            if(creature->next !=nullptr)
                creature->next->prev = creature;
            std::swap(creature->arr[0], tmp->arr[tmp->sz - 1]);
            for (int i = tmp->sz - 1; i > index + 1; --i) {// -2 ?? ?
                std::swap(tmp->arr[i],tmp->arr[i-1]);
            }
        }
        size++;
    }
    void remove(const ll idx){
        ll index = idx;
        Block<T>* tmp = head;
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
        Block<T>* block;
        int pos;
        iterator() = delete;
        iterator(const Dynarr& a): block(&a.head) , pos(0){}
        iterator(Block<T>* b): block(b) , pos(0){}
        T& operator*(){
            return block->arr[pos];
        }
        iterator& operator+=(int n){
            while(n-block->sz >= 0) {
                block = block->next;
                n -= block->sz;
            }
            pos += n;
            return *this;
        }
        T* operator->(){
            return block->arr;
        }
        inline bool operator ==(const iterator& other) const{
            return (pos == other.pos) && (block == other.block);
        }
        inline bool operator !=(const iterator& other) const{
            return !(*this == other);
        }
        iterator& operator++() {
            if (++pos == block->sz) {
                block = block->next;
                pos = 0;
            }
            return *this;
        }

    };
    iterator begin() const{
        return iterator(head);
    }
    iterator end() const{
        return iterator(head) += size;
    }

    ~Dynarr(){
        std::stack<Block<T>*> s;
        while(head != nullptr){
            s.push(head);
            head = head->next;
        }
        while(!s.empty()){
            delete s.top();
            s.pop();
        }
    }
    friend std::ostream& operator << <T>(std::ostream& out, Dynarr<T>& a);
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
     std::ostream& operator << (std::ostream& out, Dynarr<T>& a){
            Block<T>* runthrough = a.head;
            while(runthrough != nullptr){
                for (int i = 0; i < runthrough->sz; ++i) {
                    out << runthrough->arr[i] << ' ';
                }
                runthrough = runthrough->next;
            }
            return out;
    }

int main() {
    int n , sz ;
    ll pos ;
    std::cin >> n >> sz;
    Dynarr<int> a(n);
    Dynarr<int> b(n);
    for (int i = 0; i < sz; ++i) {
        int z;
        std::cin >> z; a.push_back(z);
        }
//    std::cin >> pos;
//    a.insert(pos,-1);
     std::cout << a << '\n';
//    sort(a);
     std::cout << *(a.begin()+=4) <<'\n';
//    a.remove(0);
//    std::cout << a;
    return 0;
}
