// ///////////////////////////////////////////////////////////////// //
// *SRL-C++ 11 Library                                 
// *Copyright(c) 2019  Mbadiwe Nnaemeka Ronald                 
// *Github Repository <https://github.com/ron4fun>             

// *Distributed under the MIT software license, see the accompanying file LICENSE 
// *or visit http ://www.opensource.org/licenses/mit-license.php.           

// *Acknowledgements:                                  
// ** //
// *Thanks to Simple Regex (https://simple-regex.com/) for his creative        
// *development of this library in Javascript                         

// ////////////////////////////////////////////////////// ///////////////

#ifndef UTILS_H
#define UTILS_H


#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNING


#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <sstream>
#include <cstdarg>
#include <functional>

using namespace std;

struct mapperStruct {
	string add;
	int type;
	int allowed;

	mapperStruct() {}

	mapperStruct(const string &_add, const int _type, const int _allowed)
		: add(_add), type(_type), allowed(_allowed)
	{}
};

typedef map<string, string> Dictionary;


class Utils
{
public:
	// Utility
	static bool isDigit(const string &str)
	{
		return !str.empty() && find_if(str.begin(), str.end(), [](char c) {return !isdigit(c); }) == str.end();
	} // end function isDigit

	static string substr(const string &str, const int start)
	{
		return substr(str, start, str.length());
	} // end function substr

	static string substr(const string &str, const int start, const size_t length)
	{
		int _start = start;
		size_t _length = length;

		if (_start < 0) _start += (int)str.length();
		if (_length < 0) _length = str.length() + _length - _start;
		if (_length < 0) return "";

		return str.substr(_start, _length);
	} // end function substr

	static void tokenize(const string &s, const char &delim, vector<string> &out)
	{
		size_t start;
		size_t end = 0;

		while ((start = s.find_first_not_of(delim, end)) != string::npos)
		{
			end = s.find(delim, start);
			out.push_back(s.substr(start, end - start));
		} // end while
	} // end function explode

	static bool startsWith(const string &str, const string &key, bool casesensitive = false)
	{
		string _str = str;
		string _key = key;

		if (casesensitive) {
			transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
			transform(_key.begin(), _key.end(), _key.begin(), ::tolower);
		} // end if

		if (_str.find(_key) == 0) return true;
		return false;
	} // end function startsWith

	static string arrayShift(vector<string> &arr)
	{
		if (arr.empty()) return "";

		string firstItem = arr[0];

		arr = vector<string>(begin(arr) + 1, end(arr));

		return firstItem;
	} // end function arrayShift

	static string strToLower(const string &s)
	{
		string temp = s;
		for_each(temp.begin(), temp.end(), [](char &c) {
			c = tolower(c);
		});

		return temp;
	} // end function strToLower

	static string strToUpper(const string &s)
	{
		string temp = s;
		for_each(temp.begin(), temp.end(), [](char &c) {
			c = toupper(c);
		});

		return temp;
	} // end function strToUpper

	static string trim(const string &s)
	{
		if (s.empty() || s == " ") return "";

		size_t l_index = s.find_first_not_of(" ");
		size_t r_index = s.find_last_not_of(" ");

		if (l_index == string::npos) return s;

		return s.substr(l_index, r_index - l_index + 1);
	} // end function trim

	static string join(const vector<string> &arr, const char *delim) 
	{
		if (arr.empty()) return "";

		ostringstream s;
		copy(arr.begin(), arr.end() - 1, ostream_iterator<string>(s, delim));
		s << arr.back();
		return s.str();
	} // end function join

	static string removeAll(const string &str, const char find) {
		string temp;
		for (auto s : str) {
			if (s != find) {
				temp += s;
			} // end if
		} // end for

		return temp;
	} // end function removeAll

	static string replace(const string &str, const string &find, const string &replace_with) {
		string temp = str;

		// Get the first occurrence
		size_t pos = temp.find(find);

		// Repeat till end is reached
		while (pos != string::npos)
		{
			// Replace this occurence of substr
			temp.replace(pos, find.size(), replace_with);

			// Get the next occurrence from the current position
			pos = temp.find(find, pos + replace_with.size());
		} // end while
		
		return temp;
	} // end function replace

	static string iReplace(const string &str, const string &find, const string &replace_with)
	{
		string s = str;

		auto pos = search(s.begin(), s.end(), find.begin(), find.end(), [](const char c1, const char c2) {return ::tolower(c1) == ::tolower(c2);});
		if (pos == s.end()) return "";

		auto pos2 = pos;
		advance(pos2, find.size());
		s.replace(pos, pos2, replace_with);

		return s;
	} // end function iReplace

	static vector<string> split(const string &str, const string delimiter = " ")
	{
		string temp = str;
		vector<string> result;
		size_t pos = 0;
		string token;

		while ((pos = temp.find(delimiter)) != string::npos)
		{
			token = temp.substr(0, pos);
			result.push_back(token);
			temp.erase(0, pos + delimiter.length());
		} // end while

		temp.empty() ? NULL : result.push_back(temp); // the remaining part

		return result;
	} // end function split

	static string slice(const string &obj, const int _start, const int _end = INT32_MAX)
	{
		string temp = obj;
		int start = _start;
		int end = _end;

		if (start < 0)
		{
			start = (int)temp.size() + _start;
			if (start < 0) return "";
		} // end if
		else if (start >= (int)temp.size())
		{
			return "";
		} // end else if

		if (end < 0)
		{
			end = (int)temp.size() + _end;
			if (end <= 0) return "";
		} // end if
		else if (end >= (int)temp.size() - start)
		{
			end = (int)temp.size();
		} // end else if

		if (temp.begin() + end - 1 < temp.begin() + start) return "";

		temp = string(temp.begin() + start, temp.begin() + end);

		return temp;
	} // end function slice

	static vector<string> slice(const vector<string> &obj, const int _start, const int _end = INT32_MAX)
	{
		vector<string> temp = obj;
		int start = _start;
		int end = _end;

		if (start < 0)
		{
			start = (int)temp.size() + _start;
			if (start < 0) return vector<string>();
		} // end if
		else if (start > (int)temp.size())
		{
			return vector<string>();
		} // end else if


		if (end < 0)
		{
			end = (int)temp.size() + _end;
			if (end < 0) return vector<string>();
		} // end if

		else if (end >= (int)temp.size() - start)
		{
			end = (int)temp.size();
		} // end else if

		if (temp.begin() + end - 1 < temp.begin() + start) return vector<string>();

		temp = vector<string>(temp.begin() + start, temp.begin() + end);

		return temp;
	} // end function slice

	static vector<string> splice(vector<string> &arr, const int _start, const int _deleteCount = INT32_MIN, const vector<string> &v_str = {})
	{
		int start = _start;
		int deleteCount = _deleteCount;

		if (start < 0)
		{
			start = (int)arr.size() + _start;
			if (start < 0) start = 0;
		} // end if
		else if (start > (int)arr.size())
		{
			arr = {};
			return {};
		} // end else if

		vector<string>::iterator start_it = arr.begin() + start;
		vector<string>::iterator end_it = arr.end();

		if (deleteCount <= 0)
		{
			if (v_str.size() == 1) arr.insert(start_it, v_str[0]);
			else if (v_str.size() > 1) arr.insert(start_it, v_str.begin(), v_str.end());
			return{};
		} // end if
		else if (deleteCount >= (int)arr.size() - start)
		{
			deleteCount = (int)arr.size() - start;
		} // end else if

		vector<string>::iterator deleteCountStart = start_it;
		vector<string>::iterator deleteCountEnd = deleteCountStart + deleteCount;

		vector<string> result = vector<string>(deleteCountStart, deleteCountEnd);

		arr.erase(deleteCountStart, deleteCountEnd);

		if (v_str.size() == 1) arr.insert(arr.begin() + start, v_str[0]);
		else if (v_str.size() > 1) arr.insert(arr.begin() + start, v_str.begin(), v_str.end());

		return result;
	} // end function splice

	template<typename T>
	static string format(const char *str, const size_t num_args, ...) {
		va_list args;
		va_start(args, num_args);

		static char buff[100];
		memset(buff, NULL, sizeof(buff));

		T val = T();
		T val2 = T();

		switch (num_args)
		{
		case 1:
			val = va_arg(args, T);
			sprintf(buff, str, val);
			break;
		case 2:
			val = va_arg(args, T);
			val2 = va_arg(args, T);
			sprintf(buff, str, val, val2);
			break;
		default:
			throw runtime_error("Invalid argument");
		}
					
		va_end(args);
		
		return trim(buff);
	} // end function format

	static string remove(const string &str, const char find)
	{
		string temp;

		size_t index = str.find_first_of(find);
	
		temp = str.substr(0, index);
		temp += str.substr(index + 1);

		return temp;
	} // end function remove

	template <typename T>
	static void remove(vector<T> &var, size_t pos)
	{
		vector<T>::iterator it = var.begin();
		advance(it, pos);
		var.erase(it);
	} // end function remove

	template <typename T>
	static vector<T> remove(vector<T> &var, const T value)
	{
		vector<T> temp;
		for (register size_t i = 0; i < var.size(); i++) {
			if (var[i] == value) continue;
			temp.push_back(var[i]);
		} // end for
		
		return temp;
	} // end function remove

	template <typename T>
	static bool contains(const vector<T> &var, const T value) {
		for (auto v : var) {
			if (v == value) return true;
		} // end for

		return false;
	} // end function contains

	static bool contains(const string &var, const char value) {
		if (var.find(value) != string::npos) return true;
		return false;
	} // end function contains

	static bool contains(const string &var, const string &value) {
		if (var.find(value) != string::npos) return true;
		return false;
	} // end function contains

	static bool contains(const map<string, mapperStruct> &var, const string &value, mapperStruct &result) {
		for (auto &v : var) {
			if (v.first == value) {
				result = v.second;
				return true;
			} // end if
		} // end for

		return false;
	} // end function contains

	template <typename T, typename S>
	static S get(const T, const string &value) {
		for (auto &v : var) {
			if (v.first == value) {
				return v.second;
			} // end if
		} // end for

		return S();
	} // end function contains

}; // end class Utils

#endif // !UTILS_H
