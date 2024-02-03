#ifndef MYMAP_HPP
#define MYMAP_HPP

#include <stdexcept>
#include <vector>
#include <algorithm>
#include <memory>

template <typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<Key, T>>>
class myMap {

public:
	// ======================================================================================== //
	// Member types																				//
	// ======================================================================================== //
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


	// ======================================================================================== //
	// Member classes																			//
	// 	- 	value_compare 																		//
	// ======================================================================================== //
	class value_compare {
	protected:
		key_compare comp;
	public:
		bool operator()(const value_type& lhs, const value_type& rhs) const {
			return comp(lhs.first, rhs.first);
		}
		value_compare(key_compare c) : comp(c) {}
	};


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
	myMap() : myMap(Compare()) {}
	myMap(std::initializer_list<value_type> pairs, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
		:
		compare(comp),
		allocator(alloc),
		vec(pairs) {
		std::sort(vec.begin(), vec.end(), value_comp());
	};
	myMap(const myMap& other) { *this = other; }

	explicit myMap(const Compare& comp, const Allocator& alloc = Allocator()) : compare(comp), allocator(alloc) {}

	explicit myMap(const Allocator& alloc) : allocator(alloc) {}

	template< class InputIt >
	myMap(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
		:
		compare(comp),
		allocator(alloc),
		vec(first, last) {
		std::sort(vec.begin(), vec.end(), value_comp());
	}

	myMap(const myMap& other, const Allocator& alloc) : allocator(alloc) { *this = other; }

	myMap(myMap&& other)
		: compare(std::move(other.compare)),
		allocator(std::move(other.allocator)),
		vec(std::move(other.vec)) {}

	myMap(myMap&& other, const Allocator& alloc)
		: compare(std::move(other.compare)),
		allocator(alloc),
		vec(std::move(other.vec)) {}

	// ===============
	// Operator=
	// ===============
	myMap& operator=(const myMap& other) {
		if (this == &other) return *this;
		vec.clear();
		vec.reserve(other.size());
		for (const value_type& pair : other.vec) {
			vec.emplace_back(pair);
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
	T& operator[](Key&& key) {
		iterator it = lower_bound(key);
		if (it != end() && it->first == key) {
			return it->second;
		}
		it = insert({ std::move(key), T() }).first;
		return it->second;
	}

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
	const_iterator cbegin() const { return const_iterator(vec.cbegin()); }

	// ===============
	// end / cend
	// ===============
	iterator end() { return iterator(vec.end()); }
	const_iterator end() const { return const_iterator(vec.end()); }
	const_iterator cend() const { return const_iterator(vec.cend()); }

	// ===============
	// rbegin / crbegin
	// ===============
	reverse_iterator rbegin() { return iterator(vec.rbegin()); }
	const_reverse_iterator rbegin() const { return const_iterator(vec.rbegin()); }
	const_reverse_iterator crbegin() const { return const_reverse_iterator(vec.crbegin()); }

	// ===============
	// rend / crend
	// ===============
	reverse_iterator rend() { return iterator(vec.rend()); }
	const_reverse_iterator rend() const { return const_iterator(vec.rend()); }
	const_reverse_iterator crend() const { return const_reverse_iterator(vec.crend()); }


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
		it = vec.insert(it, value);
		return std::make_pair(it, true);
	}
	template< class P >
	std::pair<iterator, bool> insert(P&& value) {
		iterator it = lower_bound(value.first);
		if (it != end() && it->first == value.first) {
			return std::make_pair(it, false);
		}
		it = vec.insert(it, value);
		return std::make_pair(it, true);
	}

	iterator insert(const_iterator pos, const value_type& value) {
		iterator it = vec.begin() + std::distance(vec.cbegin(), pos);
		if (pos != cend() && pos->first == value.first) {
			return it;
		}
		it = vec.insert(it, value);
		return it;
	}

	template< class P >
	iterator insert(const_iterator pos, P&& value) {
		iterator it = vec.begin() + std::distance(vec.cbegin(), pos);
		if (pos != cend() && pos->first == value.first) {
			return it;
		}
		it = vec.insert(it, value);
		return it;
	}

	template< class InputIt >
	void insert(InputIt first, InputIt last) {
		while (first != last) {
			insert(std::make_pair(first->first, first->second));
			first++;
		}
	}

	void insert(std::initializer_list<value_type> ilist) {
		for (const value_type& pair : ilist) {
			insert(pair);
		}
	}

	// ===============
	// emplace
	// ===============
	template < class ...Args>
	std::pair<iterator, bool> emplace(Args &&... args) {
		value_type keyVal(std::forward<Args>(args)...);
		iterator vecIt = find(keyVal.first);
		if (vecIt != vec.end() && vecIt->first == keyVal.first) {
			return std::make_pair(vecIt, false);
		}
		vecIt = insert(keyVal).first;
		return std::make_pair(vecIt, true);
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

	// ===============
	// key_comp
	// ===============
	key_compare key_comp() const {
		return key_compare();
	}

	// ===============
	// value_comp
	// ===============
	myMap::value_compare value_comp() const {
		return value_compare(compare);
	}


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

private:
	key_compare compare;
	Allocator allocator;
	std::vector<value_type, Allocator> vec;
};

#endif