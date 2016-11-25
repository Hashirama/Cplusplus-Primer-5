#ifndef QUERYRESULT_H
#define QUERYRESULT_H
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <memory>
class QueryResult {
	friend std::ostream& print(std::ostream &, const QueryResult&);
	friend std::ostream& print(std::ostream&, const QueryResult&, size_t, size_t);
public:
	QueryResult() = default;
	typedef std::vector<std::string>::size_type line_no;
	QueryResult(std::string s, std::shared_ptr<std::set<line_no>> p, std::shared_ptr<std::vector<std::string>> f) :
		sought(s), lines(p), file(f) {}
	auto begin() const { return lines->begin(); };
	auto end() const { return lines->end(); };
	std::shared_ptr<std::vector<std::string>> get_file() const { return file; };

	QueryResult(const QueryResult&) = delete;
	QueryResult& operator=(const QueryResult&) = delete;
	QueryResult(QueryResult &&) noexcept = default;
private:
	std::string sought;
	std::shared_ptr<std::set<line_no>> lines;
	std::shared_ptr<std::vector<std::string>> file;
};
std::ostream& print(std::ostream&, const QueryResult&);
std::ostream& print(std::ostream&, const QueryResult&, size_t, size_t);
#endif