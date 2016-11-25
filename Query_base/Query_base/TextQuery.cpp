#include "TextQuery.h"
#include "QueryResult.h"
#include "Make_plural.h"
#include <cstddef>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <utility>
using std::size_t;
using std::shared_ptr;
using std::istringstream;
using std::string;
using std::getline;
using std::vector;
using std::map;
using std::set;
using std::cerr;
using std::cout;
using std::cin;
using std::ostream;
using std::endl;
using std::ifstream;
using std::ispunct;
using std::tolower;
using std::strlen;
using std::pair;
TextQuery::TextQuery(ifstream &is) : file(new std::vector<std::string>) {
	string text;
	while (getline(is, text)) {
		file->push_back(text);
		int n = file->size() - 1;
		istringstream line(text);
		string word;
		while (line >> word) {
			word = cleanup_str(word);
			auto &lines = wm[word]; 
			if (!lines) { 
				lines.reset(new set<line_no>);
			}
			lines->insert(n);
		}
	}
}
QueryResult TextQuery::query(const string &sought) const {
	
	static shared_ptr<set<line_no>> nodata(new set<line_no>);
	
	auto loc = wm.find(cleanup_str(sought));
	if (loc == wm.end())
		return QueryResult(sought, nodata, file); 
	else
		return QueryResult(sought, loc->second, file);
}
std::string TextQuery::cleanup_str(const std::string& s) {
	string ret;
	for (auto it = s.begin(); it != s.end(); ++it) {
		if (!ispunct(*it)) {
			ret += tolower(*it);
		}
	}
	return ret;
}
std::ostream &print(std::ostream &os, const QueryResult &qr) {
	os << qr.sought << " occurs " << qr.lines->size() << " "
		<< make_plural(qr.lines->size(), "time", "s") << endl;
	for (auto &num : *qr.lines) {

		cout << "\t(line " << num + 1 << ") " << qr.file->at(num) << endl;
	}
	return os;
}
std::ostream &print(std::ostream &os, const QueryResult &qr, size_t low, size_t high) {
	std::vector<size_t> sz_v;
	if (low > high) {
		throw std::out_of_range("Low out of range!\n");
	}
	for (auto iter = qr.lines->begin(); iter != qr.lines->end(); ++iter) {
		if ((*iter + 1) >= low && (*iter + 1 ) <= high) {
			sz_v.push_back(*iter);
		}
	}
	os << qr.sought << " occurs " << sz_v.size() << " "
			<< make_plural(sz_v.size(), "time", "s") << endl;
	for (auto &s : sz_v) {

		cout << "\t(line " << s + 1 << ") " << qr.file->at(s) << endl;
	}
	return os;
}