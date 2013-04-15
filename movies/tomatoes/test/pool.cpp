#include <iostream>
#include <memory>

namespace my_namespace {
    
    template <class T, class Allocator = std::allocator<T> >
    class set
    {
    public:
        // typedefs and allocator member as  above
        typedef Allocator allocator_type;
        typedef typename Allocator::size_type size_type;
        typedef typename Allocator::difference_type
        difference_type;
        typedef typename Allocator::reference reference;
        typedef typename Allocator::const_reference
        const_reference;
        
        // Our iterator will be a simple pointer
        typedef typename Allocator::pointer iterator;
        typedef Allocator::const_pointer iterator;
        
    protected:
        Allocator the_allocator;  // copy of the allocator
        
    private:
        size_type buffer_size;
        iterator buffer_start;
        iterator current_end;
        iterator end_of_buffer;
        
    public:
        // A constructor that initializes the set using a range
        // from some other container or array
        template <class Iterator>
        set(Iterator start, Iterator finish,
            Allocator alloc = Allocator());
        
        iterator begin() { return buffer_start; }
        iterator end() { return current_end; } 
    };                    // Construct instance
    
    template <class T, class Allocator>
    template <class Iterator>
    set<T,Allocator>::set(Iterator start, Iterator finish,
                          Allocator alloc)
    : buffer_size(finish-start + DEFAULT_CUSHION),
    buffer_start(0),
    current_end(0), end_of_buffer(0)
    {
        // Copy the argument to our internal object
        the_allocator = alloc;                               // 1
        
        // Create an initial buffer
        buffer_start = the_allocator.allocate(buffer_size);  // 2
        end_of_buffer = buffer_start + buffer_size;
        
        // Construct new values from iterator range on the buffer
        for (current_end = buffer_start;
             start != finish;
             current_end++, start++)
            the_allocator.construct(current_end,*start);      // 3
        
        // Now let's remove duplicates using a standard algorithm
        std::unique(begin(),end());
    }
} // End of my_namespace

int main( int argc, const char* argv[] )
{
    std::cout << "Pool testing" << std::endl;
    
    return 0;
}

