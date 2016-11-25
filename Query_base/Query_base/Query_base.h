#ifndef QUERY_BASE_H
#define QUERY_BASE_H
#include "TextQuery.h"
#include <string>
#include <memory>

class Query_base {
	friend class Query;
protected:
	using line_no = TextQuery::line_no;
	virtual ~Query_base() = default;
private:
	
	virtual QueryResult eval(const TextQuery&) const = 0;
	
	virtual std::string rep() const = 0;

};

class Query {
	
	friend Query operator~(const Query &);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);
public:
	Query(const std::string&);  
	QueryResult eval(const TextQuery &t) const {	
		return q->eval(t);
	}
	std::string rep() const { std::cout << "Query::rep() \n"; return q->rep(); }
private:
	Query(std::shared_ptr<Query_base> query) : q(query) { std::cout << "Query::Query(std::shared_ptr<Query_base> query)\n"; }
	std::shared_ptr<Query_base> q;
};

class WordQuery : public Query_base {
	friend class Query;
	WordQuery(const std::string &s) : query_word(s) { std::cout << "WordQuery::WordQuery(" + s + ")\n"; }
	QueryResult eval(const TextQuery &t) const override { return t.query(query_word); }
	std::string rep() const override { std::cout << "WordQuery::rep()\n"; return query_word; }
	std::string query_word;
};
inline Query::Query(const std::string &s) : q(new WordQuery(s)) { std::cout << "Query::Query(const std::string& s) where s=" << s << "\n"; }

class NotQuery : public Query_base {
	friend Query operator~(const Query &);
	NotQuery(const Query &q) : query(q) { std::cout << "NotQuery::NotQuery()\n"; }
	std::string rep() const { std::cout << "NotQuery::rep()\n"; return "~(" + query.rep() + ")"; }
	QueryResult eval(const TextQuery&) const override;
	Query query;
};

class BinaryQuery : public Query_base {
protected:
	BinaryQuery(const Query &l, const Query &r, std::string s) :
		lhs(l), rhs(r), opSymbol(s) {
		std::cout << "BinaryQuery::BinaryQuery()  where s=" + s + "\n";
	}
	
	std::string rep() const {
		std::cout << "BinaryQuery::rep()\n";
		return "(" + lhs.rep() + " " + opSymbol
			+ " " + rhs.rep() + ")";
	}
	Query lhs, rhs;
	std::string opSymbol;
};

class AndQuery : public BinaryQuery {
	friend Query operator&(const Query&, const Query&);
	AndQuery(const Query& left, const Query& right) :
		BinaryQuery(left, right, "&") {
		std::cout << "AndQuery::AndQuery()\n";
	}
	QueryResult eval(const TextQuery&) const override;
};

class OrQuery : public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
	OrQuery(const Query &left, const Query &right) :
		BinaryQuery(left, right, "|") {
		std::cout << "OrQuery::OrQuery\n";
	}
	QueryResult eval(const TextQuery&) const override;
};

inline Query operator~(const Query &operand) {
	return std::shared_ptr<Query_base>(new NotQuery(operand));
}

inline Query operator&(const Query& lhs, const Query& rhs) {
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

inline Query operator|(const Query &lhs, const Query &rhs) {
	return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}
std::ostream &operator<<(std::ostream &os, const Query &query);
#endif