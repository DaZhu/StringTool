#ifndef __STRINGHELPER__H__
#define __STRINGHELPER__H__
#include <string>


namespace StringHelper
{
	std::string UnicodeToUTF8(const std::wstring& in);
	std::wstring UTF8ToUnicode(const std::string& in);
	std::string UnicodeToAnsi(const std::wstring& in);
	std::string UTF8ToAnsi(const std::string& in);
};
#endif //__STRINGHELPER__H__