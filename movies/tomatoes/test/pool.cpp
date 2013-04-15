#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Pool
{
	struct Link { Link* next; };
	
	struct Chunk
	{
		enum { size = 8*1024 - 16 };
		char mem[size];
		Chunk* next;
	};
	
	Chunk* chunks;
	const unsigned int esize;
    Link* head;
    Pool(Pool&);
    void operator=(Pool&);
    void grow();
    
public:
    Pool(unsigned int n);
    ~Pool();
    
    void* alloc();
    void free(void* b);
};

inline void* Pool::alloc()
{
    if (head==0) grow();
    Link* p = head;
    head = p->next;
    return p;
}

inline void Pool::free(void* b)
{
    Link* p = static_cast<Link*>(b);
    p->next = head;
    head = p;
}

Pool::Pool(unsigned int sz)
    :esize(sz<sizeof(Link)?sizeof(Link):sz)
{
    head = 0;
    chunks = 0;
}

Pool::~Pool()
{
    Chunk* n = chunks;
    while (n)
    {
        Chunk* p = n;
        n = n->next;
        delete p;
    }
}

void Pool::grow()
{
    Chunk* n = new Chunk;
    n->next = chunks;
    chunks = n;
    
    const int nelem = Chunk::size/esize;
    char* start = n->mem;
    char* last = &start[ (nelem-1) * esize ];
    
    for (char* p = start; p<last; p+=esize)
        reinterpret_cast<Link*> (p)->next = reinterpret_cast<Link*> (p+esize);
    
    reinterpret_cast<Link*> (last)->next = 0;
    head = reinterpret_cast<Link*> (start);
}

//template<class T>class allocator {
//public:
//    typedef T value_type;
//    typedef size_t size_type;
//    typedef ptrdiff_t difference_type;
//    
//    typedef T*pointer;
//    typedef const T*const_pointer;
//    
//    typedef T&reference;
//    typedef const T&const_reference;
//    
//    pointer address(reference r)const{return&r;}
//    const_pointer address(const_reference r)const{return&r;}
//    
//    allocator() throw();
//    template<class U>allocator(const allocator<U>&)throw();
//    ~allocator() throw();
//    
//    pointer allocate(size_type n, allocator<void>::const_pointer hint = 0);// space for n Ts
//    void deallocate(pointer p,size_type n); //deallocate n Ts, don’t destroy
//    
//    void construct(pointer p, const T& val) { new(p) T(val); } // initialize *p by val
//    void destroy(pointer p){p->~T();} //destroy *p but don’t deallocate
//    size_type max_size()const throw();
//    template<class U>
//    struct rebind { typedef allocator<U> other; }; // in effect: typedef allocator<U> other
//};
//template<class T>bool operator==(const allocator<T>&,const allocator<T>&)throw();
//template<class T>bool operator!=(const allocator<T>&,const allocator<T>&)throw();

template <class T> class Pool_alloc {
private:
    static Pool mem;
public:
    typedef T value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    
    typedef T*pointer;
    typedef const T*const_pointer;
    
    typedef T&reference;
    typedef const T&const_reference;
    
    T* allocate(size_type n, void* = 0);
    void deallocate(pointer p, size_type n);
    
    Pool_alloc();
};

template <class T> Pool Pool_alloc<T>::mem(sizeof(T));
template <class T> Pool_alloc<T>::Pool_alloc() { }

template <class T>
T* Pool_alloc<T>::allocate(size_type n, void*) {
    if (n==1) return static_cast<T*>(mem.alloc());
    // ...
}

template <class T>
void Pool_alloc<T>::deallocate(pointer p, size_type n) {
    if (n==1) {
        mem.free(p);
        return;
    }
    // ...
}

int main( int argc, const char* argv[] )
{
    cout << "Pool testing" << endl;
    
    vector<int,Pool_alloc> v;
    
    Pool p(1);
    while (true) {
        void *e = p.alloc();
        p.free(e);
    }
    
    return 0;
}

