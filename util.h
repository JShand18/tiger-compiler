#if ! defined UTIL_H
#define UTIL_H

#include <string>
typedef std::string String;  // for compatibility with some old code
typedef std::string string;  // like 'using namespace std', but doesn't pull in the whole namespace, just 'string'


// figuring out template specialization, based in part on http://en.cppreference.com/w/cpp/language/template_specialization

template<class T> string str(T *p)   { return p->__str__(); }
template<class T> string str(T &x)   { return x.__str__(); }
static            string str(int x)  { return std::to_string(x); }
static            string str(long x)  { return std::to_string(x); }
static            string str(unsigned x)  { return std::to_string(x); }

string repr_for_std_string(const string &s);  // non-trivial; in util.cc (changes non-printing characters into \x##)
template<class T> string repr(T *p)   { return p->__repr__(); }
template<class T> string repr(T &x)   { return x.__repr__(); }
// template<>        string repr(string &s) { return repr_for_std_string(s); }
// template<>        string repr(string s) { return repr_for_std_string(s); }
static            string repr(string s)    { return repr_for_std_string(s); }
static            string repr(int x)       { return std::to_string(x); }
static            string repr(long x)      { return std::to_string(x); }
static            string repr(unsigned x)  { return std::to_string(x); }

// Restored for 2016, since it had been in the starter files:
string appendchar(string, char);

#endif
