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
		wchar_t byte_other = 0x3f;    // ����λ����������ȡλֵ

		unsigned char utf_one = 0;
		unsigned char utf_other = 0x80;    // ����λ���ñ�UTF-8����

		size_t test_length = 0;
		size_t test_chars = 0;

		wchar_t tmp_wchar = L'\0';    // ���ڿ��ַ�λ����ȡ��λ��(����6λ)
		unsigned char tmp_char = '\0';

		const wchar_t* src_wstr = in.c_str();

		do    // ��ѭ���ɼ�⵽�ַ�����β��L'\0'��ת��֮
		{
			for (;;)    // ����ֽ����г���
			{
				if (src_wstr[test_chars] <= 0x7f)
				{
					count_bytes = 1;    // ASCII�ַ�: 0xxxxxxx( ~ 01111111)
					byte_one = 0x7f;    // ����λ������, ��ȡ��Чλֵ, ��ͬ
					utf_one = 0x0;
					break;
				}

				if ((src_wstr[test_chars] > 0x7f) && (src_wstr[test_chars] <= 0x7ff))
				{
					count_bytes = 2;    // 110xxxxx 10xxxxxx[1](���11��1λ, ��дΪ11*1)
					byte_one = 0x1f;    // 00011111, ������(1λ�������ݼ�)
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

				return dst;    // ���ϽԲ�������Ϊ�Ƿ�����
			}

			// ���¼�����ȡ���ֽ��е���Ӧλ, ������ΪUTF-8����ĸ����ֽ�
			tmp_wchar = src_wstr[test_chars];
			if (dst.length() < (test_length + count_bytes))
			{
				dst.resize(test_length + count_bytes);
			}

			for (int i = count_bytes; i > 1; i--)
			{    // һ�����ַ��Ķ��ֽڽ���ֵ
				tmp_char = (unsigned char)(tmp_wchar & byte_other);
				dst[test_length + i - 1] = (tmp_char | utf_other);
				tmp_wchar >>= 6;
			}

			tmp_char = (unsigned char)(tmp_wchar & byte_one);
			dst[test_length] = (tmp_char | utf_one);
			// λֵ��ȡ����__End!
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
		unsigned char byte_other = 0x3f;    // ����λ����������ȡλֵ
		long test_length = 0;
		size_t test_chars = 0;
		wchar_t tmp_wchar = L'\0';

		const unsigned char* src_str = reinterpret_cast<const unsigned char*>(in.c_str());

		if (src_str == NULL || (strlen((const char*)src_str) == 0))
		{
			return dst;
		}

		do    // ��ѭ���ɼ�⵽�ַ����Ľ�����'\0'��ת��֮
		{
			for (;;)    // ����ֽ����г���
			{
				if (src_str[test_length] <= 0x7f){
					count_bytes = 1;    // ASCII�ַ�: 0xxxxxxx( ~ 01111111)
					byte_one = 0x7f;    // ����λ������, ��ȡ��Чλֵ, ��ͬ
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
				return dst;    // ���ϽԲ�������Ϊ�Ƿ�����
			}
			// ���¼�����ȡUTF-8�����ַ������ֽڵ���Чλֵ
			tmp_wchar = src_str[test_length] & byte_one;
			for (int i = 1; i < count_bytes; i++)
			{
				tmp_wchar <<= 6;    // ����6λ��������ֽڵ���Чλֵ"λ��"��ֵ
				tmp_wchar = tmp_wchar | (src_str[test_length + i] & byte_other);
			}
			// λֵ��ȡ__End!
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