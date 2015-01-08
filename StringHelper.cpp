#include "stdafx.h"
#include "StringHelper.h"

namespace StringHelper
{

	std::string UnicodeToUTF8(const std::wstring& in)
	{
		std::string dst;
		if (in.empty())
		{
			return dst;
		}

		int count_bytes = 0;
		wchar_t byte_one = 0;
		wchar_t byte_other = 0x3f;    // 用于位与运算以提取位值

		unsigned char utf_one = 0;
		unsigned char utf_other = 0x80;    // 用于位或置标UTF-8编码

		size_t test_length = 0;
		size_t test_chars = 0;

		wchar_t tmp_wchar = L'\0';    // 用于宽字符位置析取和位移(右移6位)
		unsigned char tmp_char = '\0';

		const wchar_t* src_wstr = in.c_str();

		do    // 此循环可检测到字符串结尾的L'\0'并转换之
		{
			for (;;)    // 检测字节序列长度
			{
				if (src_wstr[test_chars] <= 0x7f)
				{
					count_bytes = 1;    // ASCII字符: 0xxxxxxx( ~ 01111111)
					byte_one = 0x7f;    // 用于位与运算, 提取有效位值, 下同
					utf_one = 0x0;
					break;
				}

				if ((src_wstr[test_chars] > 0x7f) && (src_wstr[test_chars] <= 0x7ff))
				{
					count_bytes = 2;    // 110xxxxx 10xxxxxx[1](最多11个1位, 简写为11*1)
					byte_one = 0x1f;    // 00011111, 下类推(1位的数量递减)
					utf_one = 0xc0;     // 11000000
					break;
				}

				if ((src_wstr[test_chars] > 0x7ff) && (src_wstr[test_chars] <= 0xffff))
				{
					count_bytes = 3;    // 1110xxxx 10xxxxxx[2](MaxBits: 16*1)
					byte_one = 0xf;     // 00001111
					utf_one = 0xe0;     // 11100000
					break;
				}

				if ((src_wstr[test_chars] > 0xffff) && (src_wstr[test_chars] <= 0x1fffff))
				{
					count_bytes = 4;    // 11110xxx 10xxxxxx[3](MaxBits: 21*1)
					byte_one = 0x7;     // 00000111
					utf_one = 0xf0;     // 11110000
					break;
				}

				if ((src_wstr[test_chars] > 0x1fffff) && (src_wstr[test_chars] <= 0x3ffffff))
				{
					count_bytes = 5;    // 111110xx 10xxxxxx[4](MaxBits: 26*1)
					byte_one = 0x3;     // 00000011
					utf_one = 0xf8;     // 11111000
					break;
				}

				if ((src_wstr[test_chars] > 0x3ffffff) && (src_wstr[test_chars] <= 0x7fffffff))
				{
					count_bytes = 6;    // 1111110x 10xxxxxx[5](MaxBits: 31*1)
					byte_one = 0x1;     // 00000001
					utf_one = 0xfc;     // 11111100
					break;
				}

				return dst;    // 以上皆不满足则为非法序列
			}

			// 以下几行析取宽字节中的相应位, 并分组为UTF-8编码的各个字节
			tmp_wchar = src_wstr[test_chars];
			if (dst.length() < (test_length + count_bytes))
			{
				dst.resize(test_length + count_bytes);
			}

			for (int i = count_bytes; i > 1; i--)
			{    // 一个宽字符的多字节降序赋值
				tmp_char = (unsigned char)(tmp_wchar & byte_other);
				dst[test_length + i - 1] = (tmp_char | utf_other);
				tmp_wchar >>= 6;
			}

			tmp_char = (unsigned char)(tmp_wchar & byte_one);
			dst[test_length] = (tmp_char | utf_one);
			// 位值析取分组__End!
			test_length += count_bytes;
			test_chars++;

		} while (src_wstr[test_chars] != L'\0');

		return dst;
	}

	std::wstring UTF8ToUnicode(const std::string& in)
	{
		std::wstring dst;
		if (in.empty())
		{
			return dst;
		}

		int count_bytes = 0;
		unsigned char byte_one = 0;
		unsigned char byte_other = 0x3f;    // 用于位与运算以提取位值
		long test_length = 0;
		size_t test_chars = 0;
		wchar_t tmp_wchar = L'\0';

		const unsigned char* src_str = reinterpret_cast<const unsigned char*>(in.c_str());

		if (src_str == NULL || (strlen((const char*)src_str) == 0))
		{
			return dst;
		}

		do    // 此循环可检测到字符串的结束符'\0'并转换之
		{
			for (;;)    // 检测字节序列长度
			{
				if (src_str[test_length] <= 0x7f){
					count_bytes = 1;    // ASCII字符: 0xxxxxxx( ~ 01111111)
					byte_one = 0x7f;    // 用于位与运算, 提取有效位值, 下同
					break;
				}
				if ((src_str[test_length] >= 0xc0) && (src_str[test_length] <= 0xdf)){
					count_bytes = 2;    // 110xxxxx(110 00000 ~ 110 111111) 
					byte_one = 0x1f;
					break;
				}
				if ((src_str[test_length] >= 0xe0) && (src_str[test_length] <= 0xef)){
					count_bytes = 3;    // 1110xxxx(1110 0000 ~ 1110 1111)
					byte_one = 0xf;
					break;
				}
				if ((src_str[test_length] >= 0xf0) && (src_str[test_length] <= 0xf7)){
					count_bytes = 4;    // 11110xxx(11110 000 ~ 11110 111)
					byte_one = 0x7;
					break;
				}
				if ((src_str[test_length] >= 0xf8) && (src_str[test_length] <= 0xfb)){
					count_bytes = 5;    // 111110xx(111110 00 ~ 111110 11)
					byte_one = 0x3;
					break;
				}
				if ((src_str[test_length] >= 0xfc) && (src_str[test_length] <= 0xfd)){
					count_bytes = 6;    // 1111110x(1111110 0 ~ 1111110 1)
					byte_one = 0x1;
					break;
				}
				return dst;    // 以上皆不满足则为非法序列
			}
			// 以下几行析取UTF-8编码字符各个字节的有效位值
			tmp_wchar = src_str[test_length] & byte_one;
			for (int i = 1; i < count_bytes; i++)
			{
				tmp_wchar <<= 6;    // 左移6位后与后续字节的有效位值"位或"赋值
				tmp_wchar = tmp_wchar | (src_str[test_length + i] & byte_other);
			}
			// 位值析取__End!
			if (dst.length() < test_chars + 1)
			{
				dst.resize(test_chars + 1);
			}
			dst[test_chars] = tmp_wchar;

			test_length += count_bytes;
			test_chars++;

		} while (src_str[test_length] != '\0');

		return dst;
	}

	std::string UnicodeToAnsi(const std::wstring& in)
	{
		std::string dst;
		if (in.empty())
		{
			return dst;
		}

		size_t nLen = in.size();


		dst.resize((nLen + 1) * 2);
		int nConvLength = WideCharToMultiByte(
			CP_ACP, 0, in.c_str(), static_cast<int>(nLen),
			const_cast<char*>(dst.c_str()),
			static_cast<int>(dst.length()), NULL, NULL);
		dst.resize(nConvLength);
		return dst;
	}

	std::string UTF8ToAnsi(const std::string& in)
	{
		return UnicodeToAnsi(UTF8ToUnicode(in));
	}

}