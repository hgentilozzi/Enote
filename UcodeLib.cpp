#include <io.h> // For _setmode
#include <fcntl.h> // For _O_U16TEXT
#include <map>
#include <iostream>


std::map<char, wchar_t> char2SuperMap = {
	{'0',L'\u2070'},{'1',L'\u2071'},{'2',L'\u00B2'},{'3',L'\u00B3'},{'4',L'\u2074'},
	{'5',L'\u2075'},{'6',L'\u2076'},{'7',L'\u2077'},{'8',L'\u2078'},{'9',L'\u2079'},
	{'i',L'\u2071'},{'j',L'\u02b2'},{'k',L'\u1d4f'},{'l',L'\u02e1'},{'m',L'\u1d50'},{'n',L'\u207f'}
};
// 0x2096, 
std::map<char, wchar_t> char2SubMap = {
	{'0',L'\u2080'},{'1',L'\u2081'},{'2',L'\u2082'},{'3',L'\u2083'},{'4',L'\u2084'},
	{'5',L'\u2085'},{'6',L'\u2086'},{'7',L'\u2087'},{'8',L'\u2088'},{'9',L'\u2089'},
	{'i',L'\u1d62'},{'j',L'\u2c7c'},{'k',L'\u2096'},{'l',L'\u2097'},{'m',L'\u2098'},{'n',L'\u2099'}
};

/// <summary>
/// Allow uniocde in wcout strings
/// </summary>
void unicode_init() {
	auto a = _setmode(_fileno(stdout), _O_U16TEXT);
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
wchar_t char_to_superscript(char c) {
	wchar_t ret = c;
	if (char2SuperMap.contains(c))
		ret = char2SuperMap[c];

	return ret;
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
wchar_t char_to_subscript(char c) {
	wchar_t ret = c;
	if (char2SubMap.contains(c))
		ret = char2SubMap[c];

	return ret;
}