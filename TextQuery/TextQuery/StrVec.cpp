#include "Strvec.h"
#include <memory>
void StrVec::push_back(const std::string& s) {
	chk_n_alloc();
	alloc.construct(first_free++, s);
}
inline
std::pair<std::string*, std::string*>
StrVec::alloc_n_copy(const std::string *b, const std::string *e)
{
	auto data = alloc.allocate(e - b);
	return{ data, uninitialized_copy(b, e, data) };
}
#include <algorithm>
void StrVec::frees() {
	std::for_each(elements, first_free, [this](std::string &rhs) {alloc.destroy(&rhs); });
}

StrVec::StrVec(const StrVec &v) {
	auto newdata = alloc_n_copy(v.begin(), v.end());
	elements = newdata.first; 
	first_free = cap = newdata.second; 
}
StrVec::~StrVec() {
	frees();
}
StrVec& StrVec::operator=(const StrVec& v) {
	auto data = alloc_n_copy(v.begin(), v.end());
	frees(); 
	elements = data.first; 
	first_free = cap = data.second;
	return *this; 
}

#include <iostream>
inline
void StrVec::reallocate() {

	auto newcapacity = size() ? 2 * size() : 1;
	auto newdata = alloc.allocate(newcapacity);

	auto dest = newdata; 
	auto elem = elements; 
	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	frees(); 
	elements = newdata; 
	first_free = dest; 
	cap = elements + newcapacity;
}
void StrVec::alloc_n_move(size_t s) {
	auto newdata = alloc.allocate(s);
	auto dest = newdata;
	auto elem = elements;
	for (size_t i = 0; i != size(); ++i)
		alloc.construct(dest++, std::move(*elem++));
	frees();
	elements = newdata;
	first_free = dest;
	cap = elements + s;
}
void StrVec::reserve(size_t s) {
	if (s <= capacity()) return;
	alloc_n_move(s);
}
void StrVec::resize(size_t s) {
	resize(s, std::string());
}

void StrVec::resize(size_t s, const std::string& str) {
	if (s > size()) {
		if (s > capacity()) {
			reserve(s * 2);
		}
		for (size_t i = size(); i != s; ++i)
			alloc.construct(first_free++, str);
	}
	else if (s < size()) {
		while (first_free != elements + s) {
			alloc.destroy(--first_free);
		}
	}
}