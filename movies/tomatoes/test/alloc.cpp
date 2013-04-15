#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
//#include <list>

template<typename T>
class Allocator {
public : 
    //    typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

public : 
    //    convert an allocator<T> to allocator<U>
    template<typename U>
    struct rebind {
        typedef Allocator<U> other;
    };

public : 
    inline explicit Allocator() {}
    inline ~Allocator() {}
    inline explicit Allocator(Allocator const&) {}
    template<typename U>
    inline explicit Allocator(Allocator<U> const&) {}

    //    address
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //    memory allocation
    inline pointer allocate(size_type cnt, 
       typename std::allocator<void>::const_pointer = 0) { 
      return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T))); 
    }
    inline void deallocate(pointer p, size_type) { 
        ::operator delete(p); 
    }

    //    size
    inline size_type max_size() const { 
        return std::numeric_limits<size_type>::max() / sizeof(T);
 }

    //    construction/destruction
    inline void construct(pointer p, const T& t) { new(p) T(t); }
    inline void destroy(pointer p) { p->~T(); }

    inline bool operator==(Allocator const&) { return true; }
    inline bool operator!=(Allocator const& a) { return !operator==(a); }
};    //    end of class Allocator


template<typename T>
class ObjectTraits {
    public :
    //    convert an ObjectTraits<T> to ObjectTraits<U>
    template<typename U>
    struct rebind {
        typedef ObjectTraits<U> other;
    };
    
    public :
    inline explicit ObjectTraits() {}
    inline ~ObjectTraits() {}
    template <typename U>
    inline explicit ObjectTraits(ObjectTraits<U> const&) {}
    
    //    address
    inline T* address(T& r) { return &r; }
    inline T const* address(T const& r) { return &r; }
    
    inline void construct(T* p, const T& t) { new(p) T(t); }
    inline void destroy(T* p) { p->~T(); }
};    //    end of class ObjectTraits


template<typename T>
class StandardAllocPolicy {
    public :
    //    typedefs
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    
    public :
    //    convert an StandardAllocPolicy<T> to StandardAllocPolicy<U>
    template<typename U>
    struct rebind {
        typedef StandardAllocPolicy<U> other;
    };
    
    public :
    inline explicit StandardAllocPolicy() {}
    inline ~StandardAllocPolicy() {}
    inline explicit StandardAllocPolicy(StandardAllocPolicy const&) {}
    template <typename U>
    inline explicit StandardAllocPolicy(StandardAllocPolicy<U> const&) {}
    
    //    memory allocation
    inline pointer allocate(size_type cnt,
                            typename std::allocator<void>::const_pointer = 0) {
        return reinterpret_cast<pointer>(::operator
                                         new(cnt * sizeof (T)));
    }
    inline void deallocate(pointer p, size_type)
    { ::operator delete(p); }
    
    //    size
    inline size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }
};    //    end of class StandardAllocPolicy

// determines if memory from another
// allocator can be deallocated from this one
template<typename T, typename T2>
inline bool operator==(StandardAllocPolicy<T> const&,
                       StandardAllocPolicy<T2> const&) {
    return true;
}
template<typename T, typename OtherAllocator>
inline bool operator==(StandardAllocPolicy<T> const&,
                       OtherAllocator const&) {
    return false;
}


//template<typename T, typename Policy =
//StandardAllocPolicy<T>, typename Traits = ObjectTraits<T> >
//class Allocator : public Policy, public Traits {
//    private :
//    typedef Policy AllocationPolicy;
//    typedef Traits TTraits;
//    
//    public :
//    typedef typename AllocationPolicy::size_type size_type;
//    typedef typename AllocationPolicy::difference_type difference_type;
//    typedef typename AllocationPolicy::pointer pointer;
//    typedef typename AllocationPolicy::const_pointer const_pointer;
//    typedef typename AllocationPolicy::reference reference;
//    typedef typename AllocationPolicy::const_reference const_reference;
//    typedef typename AllocationPolicy::value_type value_type;
//    
//    public :
//    template<typename U>
//    struct rebind {
//        typedef Allocator<U, typename AllocationPolicy::rebind<U>::other,
//        typename TTraits::rebind<U>::other > other;
//    };
//    
//    public :
//    inline explicit Allocator() {}
//    inline ~Allocator() {}
//    inline Allocator(Allocator const& rhs):Traits(rhs), Policy(rhs) {}
//    template <typename U>
//    inline Allocator(Allocator<U> const&) {}
//    template <typename U, typename P, typename T2>
//    inline Allocator(Allocator<U, P,
//                     T2> const& rhs):Traits(rhs), Policy(rhs) {}
//};    //    end of class Allocator
//
//// determines if memory from another
//// allocator can be deallocated from this one
//template<typename T, typename P, typename Tr>
//inline bool operator==(Allocator<T, P,
//                       Tr> const& lhs, Allocator<T,
//                       P, Tr> const& rhs) {
//    return operator==(static_cast<P&>(lhs),
//                      static_cast<P&>(rhs));
//}
//template<typename T, typename P, typename Tr,
//typename T2, typename P2, typename Tr2>
//inline bool operator==(Allocator<T, P,
//                       Tr> const& lhs, Allocator<T2, P2, Tr2> const& rhs) {
//    return operator==(static_cast<P&>(lhs),
//                      static_cast<P2&>(rhs));
//}
//template<typename T, typename P, typename Tr, typename OtherAllocator>
//inline bool operator==(Allocator<T, P,
//                       Tr> const& lhs, OtherAllocator const& rhs) {
//    return operator==(static_cast<P&>(lhs), rhs);
//}
//template<typename T, typename P, typename Tr>
//inline bool operator!=(Allocator<T, P, Tr> const& lhs,
//                       Allocator<T, P, Tr> const& rhs) {
//    return !operator==(lhs, rhs);
//}
//template<typename T, typename P, typename Tr,
//typename T2, typename P2, typename Tr2>
//inline bool operator!=(Allocator<T, P, Tr> const& lhs,
//                       Allocator<T2, P2, Tr2> const& rhs) {
//    return !operator==(lhs, rhs);
//}
//template<typename T, typename P, typename Tr,
//typename OtherAllocator>
//inline bool operator!=(Allocator<T, P,
//                       Tr> const& lhs, OtherAllocator const& rhs) {
//    return !operator==(lhs, rhs); 
//}


int main()
{
//    std::list< int, Allocator<int> > a = std::list< int, Allocator<int> > ();
//    a.push_front(12);
    
    std::vector<int, Allocator<int> > v;
//    std::vector<int, Allocator<int, TrackAllocPolicy<int> > > v;
    
    return 0;
}