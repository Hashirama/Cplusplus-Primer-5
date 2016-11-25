#ifndef MAKE_PLURAL_H
#define MAKE_PLURAL_H
#include <string>

inline std::string make_plural(const size_t cnt, const std::string &word, const std::string &end) {
	return (cnt > 1) ? word + end : word;
}

#endif