#ifndef MYMAP_HPP
#define MYMAP_HPP

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

template <typename Key, typename T, typename Allocator = std::allocator<std::pair<Key, T>>, typename Compare = std::less<Key>>
class myMap {
private:
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<Key, T>;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using key_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename Allocator::pointer;
	using const_pointer = typename Allocator::const_pointer;
	using iterator = typename std::vector<value_type>::iterator;
	using const_iterator = typename std::vector<value_type>::const_iterator;
	using reverse_iterator = typename std::vector<value_type>::reverse_iterator;
	using const_reverse_iterator = typename std::vector<value_type>::const_reverse_iterator;

	std::vector<value_type> vec;
	Allocator allocator;
	Compare compare;

public:
	// ======================================================================================== //
	// Member classes																			//
	// 	- 	value_compare 																		//
	// ======================================================================================== //
	//TO-DO class value_compare{};


	// ======================================================================================== //
	// Member functions 																		//
	// 	- 	Constructors 																		//
	// 	- 	Destructor 																			//
	// 	- 	Operator= 																			//
	// 	- 	get_allocator 																		//
	// ======================================================================================== //

	// ===============
	// Constructors
	// ===============
	myMap() {};
	myMap(std::initializer_list<value_type> pairs, const Compare& comp = Compare(), const Allocator& alloc = Allocator()) {
		allocator = alloc;
		compare = comp;
		vec.reserve(pairs.size());
		for (const value_type& pair : pairs) {
			vec.push_back(pair);
		}
		std::sort(vec.begin(), vec.end(),
			[](const value_type& pair1, const value_type& pair2) { return pair1.first < pair2.first;});
	};
	myMap(const myMap& other) { *this = other; }

	//TO-DO 
	// myMap() : myMap(Compare()) {}

	// explicit myMap(const Compare& comp, const Allocator& alloc = Allocator());

	// explicit myMap( const Allocator& alloc );

	// template< class InputIt >
	// myMap(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator());

	// myMap(const myMap& other, const Allocator& alloc);

	// myMap(myMap&& other);

	// myMap(myMap&& other, const Allocator& alloc);

	// ===============
	// Operator=
	// ===============
	myMap& operator=(const myMap& other) {
		if (this == &other) return *this;
		vec.clear();
		vec.reserve(other.size());
		for (const value_type pair : other.vec) {
			vec.push_back(pair);
		}
		return *this;
	}

	// ===============
	// Destructor
	// ===============
	~myMap() { vec.clear(); }

	// ===============
	// get_allocator
	// ===============
	allocator_type get_allocator() { return allocator; }


	// ======================================================================================== //
	// Element access																			//
	// 	- 	operator[] 																			//
	// 	- 	at			 																		//
	// ======================================================================================== //

	// ===============
	// Operator[]
	// ===============
	T& operator[](const Key& key) {
		iterator it = lower_bound(key);
		if (it != end() && it->first == key) {
			return it->second;
		}
		it = insert({ key, T() }).first;
		return it->second;
	}
	// TO-DO: T& operator[]( Key&& key );

	// ===============
	// at
	// ===============
	T& at(const Key& key) {
		iterator it = lower_bound(key);
		if (it != end() && it->first == key) {
			return it->second;
		}
		throw std::out_of_range("Key doesn't exist");
	}
	const T& at(const Key& key) const {
		const_iterator it = lower_bound(key);
		if (it != end() && it->first == key) {
			return it->second;
		}
		throw std::out_of_range("Key doesn't exist");
	}


	// ======================================================================================== //
	// Iterators																				//
	// 	- 	begin / cbegin 																		//
	// 	- 	end / cend	 																		//
	// 	- 	rbegin / crbegin 																	//
	// 	- 	rend / crend 																		//
	// ======================================================================================== //

	// ===============
	// begin / cbegin
	// ===============
	iterator begin() { return iterator(vec.begin()); }
	const_iterator begin() const { return const_iterator(vec.begin()); }
	iterator cbegin() { return iterator(vec.cbegin()); }
	const_iterator cbegin() const { return const_iterator(vec.cbegin()); }

	// ===============
	// end / cend
	// ===============
	iterator end() { return iterator(vec.end()); }
	const_iterator end() const { return const_iterator(vec.end()); }
	iterator cend() { return iterator(vec.cend()); }
	const_iterator cend() const { return const_iterator(vec.cend()); }

	// ===============
	// rbegin / crbegin
	// ===============
	iterator rbegin() { return iterator(vec.rbegin()); }
	const_iterator rbegin() const { return const_iterator(vec.rbegin()); }
	iterator crbegin() { return iterator(vec.crbegin()); }
	const_iterator crbegin() const { return const_iterator(vec.crbegin()); }

	// ===============
	// rend / crend
	// ===============
	iterator rend() { return iterator(vec.rend()); }
	const_iterator rend() const { return const_iterator(vec.rend()); }
	iterator crend() { return iterator(vec.crend()); }
	const_iterator crend() const { return const_iterator(vec.crend()); }


	// ======================================================================================== //
	// Capacity																					//
	// 	- 	empty		 																		//
	// 	- 	size		 																		//
	// 	- 	max_size		 																	//
	// ======================================================================================== //

	// ===============
	// empty
	// ===============
	bool empty() const { return vec.empty(); }

	// ===============
	// size
	// ===============
	size_type size() const { return vec.size(); }

	// ===============
	// max_size
	// ===============
	size_type max_size() const { return vec.max_size(); }


	// ======================================================================================== //
	// Modifiers																				//
	// 	- 	clear		 																		//
	// 	- 	insert		 																		//
	// 	- 	emplace																		 		//
	// 	- 	emplace_hint																		//
	// 	- 	erase																				//
	// 	- 	swap																				//
	// ======================================================================================== //

	// ===============
	// clear
	// ===============
	void clear() { vec.clear(); }

	// ===============
	// insert
	// ===============
	std::pair<iterator, bool> insert(const value_type& value) {
		iterator it = lower_bound(value.first);
		if (it != end() && it->first == value.first) {
			return std::make_pair(it, false);
		}
		it = emplace(value.first, value.second).first;
		return std::make_pair(it, true);
	}
	//TO-DO
	// template< class P >
	// std::pair<iterator, bool> insert(P&& value);

	// iterator insert( iterator pos, const value_type& value );

	// iterator insert( const_iterator pos, const value_type& value );

	// template< class P >
	// iterator insert( const_iterator pos, P&& value );

	// template< class InputIt >
	// void insert(InputIt first, InputIt last);

	// void insert( std::initializer_list<value_type> ilist );

	// ===============
	// emplace
	// ===============
	template < class ...Args>
	std::pair<iterator, bool> emplace(Args &&... args) {
		value_type keyVal(std::forward<Args>(args)...);
		iterator it = find(keyVal.first);
		if (it != end()) {
			return std::make_pair(it, false);
		}
		iterator vecIt = std::lower_bound(vec.begin(), vec.end(), keyVal.first,
			[](const value_type& pair, const Key& key) { return pair.first < key;});
		vec.insert(vecIt, keyVal);
		return std::make_pair(find(keyVal.first), true);
	}

	// ===============
	// emplace_hint
	// ===============
	template< class... Args >
	iterator emplace_hint(const_iterator hint, Args&&... args) {
		value_type keyVal(std::forward<Args>(args)...);
		iterator it = find(keyVal.first);
		if (it != end()) {
			return it;
		}
		vec.insert(hint, keyVal);
		return find(keyVal.first);
	}

	// ===============
	// erase
	// ===============
	iterator erase(iterator pos) { return vec.erase(pos); }
	iterator erase(const_iterator pos) { return vec.erase(pos); }
	iterator erase(iterator first, iterator last) { return vec.erase(first, last); }
	iterator erase(const_iterator first, const_iterator last) { return vec.erase(first, last); }
	size_type erase(const Key& key) {
		iterator found = find(key);
		if (found != end()) {
			erase(found);
			return 1;
		}
		return 0;
	}

	// ===============
	// swap
	// ===============
	void swap(myMap& other) {
		std::vector<value_type> temp = vec;
		vec = other.vec;
		other.vec = temp;
	}


	// ======================================================================================== //
	// Lookup																					//
	// 	- 	count		 																		//
	// 	- 	find		 																		//
	// 	- 	equal_range																	 		//
	// 	- 	lower_bound																	 		//
	// 	- 	upper_bound																	 		//
	// ======================================================================================== //

	// ===============
	// count
	// ===============
	size_type count(const Key& key) const {
		if (find(key) != end())
			return 1;
		return 0;
	}

	// ===============
	// find
	// ===============
	iterator find(const Key& key) {
		iterator it = lower_bound(key);
		if (it != end() && it->first == key) {
			return it;
		}

		return end();
	}
	const_iterator find(const Key& key) const {
		const_iterator it = lower_bound(key);
		if (it != end() && it->first == key) {
			return it;
		}

		return end();
	}

	// ===============
	// equal_range
	// ===============
	std::pair<iterator, iterator> equal_range(const Key& key) {
		iterator lower = lower_bound(key);
		iterator upper = upper_bound(key);
		return std::pair<iterator, iterator>(lower, upper);
	}
	std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
		const_iterator lower = lower_bound(key);
		const_iterator upper = upper_bound(key);
		return std::pair<const_iterator, const_iterator>(lower, upper);
	}

	// ===============
	// lower_bound
	// ===============
	iterator lower_bound(const Key& key) {
		iterator it = begin();
		while (it != end()) {
			if (it->first >= key) {
				return it;
			}
			it++;
		}
		return it;
	}
	const_iterator lower_bound(const Key& key) const {
		const_iterator it = begin();
		while (it != end()) {
			if (it->first >= key) {
				return it;
			}
			it++;
		}
		return it;
	}

	// ===============
	// upper_bound
	// ===============
	iterator upper_bound(const Key& key) {
		iterator it = begin();
		while (it != end()) {
			if (it->first > key) {
				return it;
			}
			it++;
		}
		return it;
	}
	const_iterator upper_bound(const Key& key) const {
		const_iterator it = begin();
		while (it != end()) {
			if (it->first > key) {
				return it;
			}
			it++;
		}
		return it;
	}


	// ======================================================================================== //
	// Observers																				//
	// 	- 	key_comp	 																		//
	// 	- 	value_comp	 																		//
	// ======================================================================================== //
	// TO-DO
	// key_compare key_comp() const;
	// myMap::value_compare value_comp() const;


	// ======================================================================================== //
	// Non-member functions																		//
	// 	- 	Operator==	 																		//
	// 	- 	Operator!=	 																		//
	// 	- 	Operator<	 																		//
	// 	- 	Operator<=	 																		//
	// 	- 	Operator>	 																		//
	// 	- 	Operator>=	 																		//
	// ======================================================================================== //

	// ===============
	// Operator==
	// ===============
	friend bool operator==(const myMap<Key, T, Allocator, Compare>& lhs, const myMap<Key, T, Allocator, Compare>& rhs) {
		return lhs.vec == rhs.vec;
	}

	// ===============
	// Operator!=
	// ===============
	friend bool operator!=(const myMap<Key, T, Allocator, Compare>& lhs, const myMap<Key, T, Allocator, Compare>& rhs) {
		return lhs.vec != rhs.vec;
	}

	// ===============
	// Operator<
	// ===============
	friend bool operator<(const myMap<Key, T, Allocator, Compare>& lhs, const myMap<Key, T, Allocator, Compare>& rhs) {
		return lhs.vec < rhs.vec;
	}

	// ===============
	// Operator<=
	// ===============
	friend bool operator<=(const myMap<Key, T, Allocator, Compare>& lhs, const myMap<Key, T, Allocator, Compare>& rhs) {
		return lhs.vec <= rhs.vec;
	}

	// ===============
	// Operator>
	// ===============
	friend bool operator>(const myMap<Key, T, Allocator, Compare>& lhs, const myMap<Key, T, Allocator, Compare>& rhs) {
		return lhs.vec > rhs.vec;
	}

	// ===============
	// Operator>=
	// ===============
	friend bool operator>=(const myMap<Key, T, Allocator, Compare>& lhs, const myMap<Key, T, Allocator, Compare>& rhs) {
		return lhs.vec >= rhs.vec;
	}
};

#endif