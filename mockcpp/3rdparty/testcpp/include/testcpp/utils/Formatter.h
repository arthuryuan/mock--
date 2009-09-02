
#ifndef __TESTCPP_FORMATTER_H
#define __TESTCPP_FORMATTER_H

#include <testcpp/testcpp.h>

#include <sstream>
#include <string>

#include <testcpp/utils/TypeString.h>

TESTCPP_NS_START

///////////////////////////////////////////////////////
std::string toBufferString(void* buf, size_t size);

///////////////////////////////////////////////////////
template <typename T>
std::string toString(T val)
{
   return TESTCPP_NS::toBufferString((void*)&val, sizeof(val));
}

///////////////////////////////////////////////////////
std::string toPointerString(void*);

///////////////////////////////////////////////////////
template <typename T>
std::string toString(T* p)
{
   return TESTCPP_NS::toPointerString((void*)p);
}

///////////////////////////////////////////////////////
template <typename T>
std::string toString(const T* s)
{
	return TESTCPP_NS::toString(const_cast<T*>(s));
}

///////////////////////////////////////////////////////
std::string toString(std::string s);
std::string toString(char* s);
std::string toString(const char* s);
std::string toString(float f);
std::string toString(double d);
std::string toString(bool b);
std::string toString(char c);
std::string toString(unsigned char c);
std::string toString(short s);
std::string toString(unsigned short s);
std::string toString(int i);
std::string toString(unsigned int i);
std::string toString(long l);
std::string toString(unsigned long l);

#if (TESTCPP_SUPPORT_LONG_LONG == 1)
std::string toString(long long ll);
std::string toString(unsigned long long ll);
#endif

/////////////////////////////////////////////////////////////////
template <typename T>
std::string toTypeAndValueString(const T& val)
{
	std::stringstream ss;
	ss << "(" << TESTCPP_NS::TypeString<T>::value() << ")" 
      << TESTCPP_NS::toString(val);
	return ss.str();
}

TESTCPP_NS_END

#endif

