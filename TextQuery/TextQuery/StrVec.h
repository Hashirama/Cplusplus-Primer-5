#ifndef STR_VEC_H
#define STR_VEC_H
#include <string>
#include <utility>
#include <memory>
#include <initializer_list>
class StrVec {
public:
	StrVec() :
		elements(nullptr), first_free(nullptr), cap(nullptr) {
	}
	StrVec(std::initializer_list<std::string> il) {
		auto newdata = alloc_n_copy(il.begin(), il.end());
		elements = newdata.first;
		first_free = cap = newdata.second;
	}
	StrVec(const StrVec&); 
	StrVec &operator=(const StrVec&); 
	~StrVec(); 
	void push_back(const std::string&); 
	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }
	std::string *begin() const { return elements; }
	std::string *end() const { return first_free; }
	void alloc_n_move(size_t new_cap);
	void reserve(size_t s);
	void resize(size_t s);
	void resize(size_t s, const std::string&);
	std::string& at(size_t loc) { return *(elements + loc); }
	StrVec::StrVec(StrVec &&s) noexcept;
	StrVec &StrVec::operator=(StrVec &&rhs) noexcept;
	// ...
private:
	std::allocator<std::string> alloc;
	void chk_n_alloc() {
		if (size() == capacity()) reallocate();
	}
	
	std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
	void frees();
	void reallocate();
	std::string *elements; 
	std::string *first_free; 
	std::string *cap; 
};
inline
StrVec::StrVec(StrVec &&s) noexcept  
	: elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;
}
inline
StrVec &StrVec::operator=(StrVec &&rhs) noexcept
{

	if (this != &rhs) {
		frees();                 
		elements = rhs.elements;  
		first_free = rhs.first_free;
		cap = rhs.cap;
		
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}
#endif