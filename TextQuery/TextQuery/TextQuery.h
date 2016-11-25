#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <set>
#include "QueryResult.h"
//#include "StrVec.h"
#include <iostream>

class QueryResult;
class TextQuery {
public:
	typedef std::tuple<std::string, std::shared_ptr<std::set<size_t>>, std::shared_ptr<std::vector<std::string>>> tup;
	friend std::ostream& print(std::ostream &, tup&);
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&); 
	tup query(const std::string&) const;
	TextQuery(const TextQuery&) = delete;
	TextQuery& operator=(const TextQuery&) = delete;
private:
	std::shared_ptr<std::vector<std::string>> file; 
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
	static std::string cleanup_str(const std::string& s);

};
class DebugDelete {
public:
	DebugDelete(std::ostream &o = std::cerr) : os(o) {}
	template <typename T> void operator()(T *p) const {
		//os << "Deleting..." << std::endl; 
		delete p;
	}
private:
	std::ostream &os;
};
#endif