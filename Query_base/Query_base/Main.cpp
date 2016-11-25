#include "Query_base.h"
#include "QueryResult.h"
#include "TextQuery.h"
#include <Windows.h>
#include <fstream>
#include <vector>
#include <memory>
class QueryHistory {
public:
	Query& operator[](size_t n) {
		return *(query_v[n]);
	}
	void add_query(const Query&);
private:
	std::vector<std::shared_ptr<Query>> query_v;
};
void QueryHistory::add_query(const Query& q) {
	std::shared_ptr<Query> p = std::make_shared<Query>(q);
	query_v.push_back(p);
}
int main(int argc, char *argv[]) {
	std::ifstream ifs(argv[1]);
	TextQuery text(ifs);

	Query t = ~Query("alice");
	print(std::cout, t.eval(text), 1, 7);
	system("pause");
}