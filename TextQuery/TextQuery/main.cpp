#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "TextQuery.h"
#include <memory>
using std::cout; using std::endl;
using std::string;
using std::set;
using std::ifstream; 
void runQueries(ifstream &infile) {
	TextQuery tq(infile); 
	while (true) {
		cout << "enter word to look for, or q to quit: ";
		string s;
		if (!(std::cin >> s) || s == "q") break;
		print(cout, tq.query(s)) << endl;
	}
}
int main(int argc, char *argv[]) {
	ifstream ifs(argv[1]);
	runQueries(ifs);

	system("pause");
}
