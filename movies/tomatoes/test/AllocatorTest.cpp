#include "allocator.hpp"
#include "typetraits.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <stack>

int main() {
	std::stack<int, Dot::Allocator<int> > s;
	std::deque<int, Dot::Allocator<int> > d;
	std::map<int, int, Dot::Allocator<int> > m;

	std::vector<int, Dot::Allocator<int, Dot::TrackAllocPolicy<int> > > v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	v.push_back(4);
	v.push_back(5);
	v.push_back(6);
	v.push_back(7);
	v.push_back(8);
	v.push_back(9);
	v.push_back(10);
	std::cout << static_cast<Dot::Traits::StreamTypeTraits<std::size_t>::StreamT>(v.get_allocator().CurrentAllocations()) << std::endl;
	std::cout << static_cast<Dot::Traits::StreamTypeTraits<std::size_t>::StreamT>(v.get_allocator().TotalAllocations()) << std::endl;
	std::cout << static_cast<Dot::Traits::StreamTypeTraits<std::size_t>::StreamT>(v.get_allocator().PeakAllocations()) << std::endl;

	std::vector<int, Dot::Allocator<int, Dot::FixedArrayAllocPolicy<int, 10> > > v2;
	v2.reserve(10);
	v2.push_back(1);
	v2.push_back(2);
	v2.push_back(3);
	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);
	v2.push_back(7);
	v2.push_back(8);
	v2.push_back(9);
	v2.push_back(10);

	std::list<int, Dot::Allocator<int, Dot::SmallObjectAllocPolicy<int, 2> > > vTest;

	std::cout << "Pre" << std::endl;
	std::list<int, Dot::Allocator<int, Dot::SmallObjectAllocPolicy<int, 2> > >::iterator p = vTest.begin();
	std::list<int, Dot::Allocator<int, Dot::SmallObjectAllocPolicy<int, 2> > >::iterator pEnd = vTest.end();
	while ( p != pEnd ) {
		std::cout << (*p) << ", ";
		++p;
	}
	std::cout << std::endl;

	vTest.push_back(5);
	vTest.push_back(10);
	vTest.push_back(20);

	std::cout << "During" << std::endl;
	p = vTest.begin();
	pEnd = vTest.end();
	while ( p != pEnd ) {
		std::cout << (*p) << ", ";
		++p;
	}
	std::cout << std::endl;

	vTest.pop_front();
	vTest.pop_front();
	vTest.pop_front();

	std::cout << "After" << std::endl;
	p = vTest.begin();
	pEnd = vTest.end();
	while ( p != pEnd ) {
		std::cout << (*p) << ", ";
		++p;
	}
	std::cout << std::endl;

	return 0;
}