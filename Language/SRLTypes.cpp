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

#include "SRLTypes.h"
#include "Methods/Method.h"

VariableObject::VariableObject(const Method &object) : _type(vMethod) { _m = new Method(object); }

// Clear
void VariableObject::clear()
{
	switch (_type)
	{
	case vNone:
		break;
	case vBool:
		if (_b != nullptr)
		{
			delete _b;
			_b = nullptr;
		} // endif

		break;
	case vChar:
		if (_c != nullptr)
		{
			delete[] _c;
			_c = nullptr;
		} // endif

		break;
	case vInt:
		if (_int != nullptr)
		{
			delete _int;
			_int = nullptr;
		} // endif

		break;
	case vDouble:
		if (_d != nullptr)
		{
			delete _d;
			_d = nullptr;
		} // endif

		break;
	case vString:
		if (_s != nullptr)
		{
			delete _s;
			_s = nullptr;
		} // endif

		break;
	case vArray:
		if (_vv != nullptr)
		{
			delete _vv;
			_vv = nullptr;
		} // endif

		break;
	case vMethod:
		if (_m != nullptr)
		{
			delete _m;
			_m = nullptr;
		} // endif

		break;
	case vLiterally:
		if (_l != nullptr)
		{
			delete _l;
			_l = nullptr;
		} // endif

		break;
	case vFunction:
		if (_f != nullptr)
		{
			_f = nullptr;
		} // endif

		break;
	} // end switch

	_type = vNone;
} // end function clear

// Copy
VariableObject::VariableObject(const VariableObject &object) 
{
	clear();

	switch (object.type())
	{
	case vNone:
		break;
	case vBool:
		_type = vBool;
		_b = new bool(*object._b);
		break;
	case vChar:
		_type = vChar;
		_c = new char(*object._c);
		break;
	case vInt:
		_type = vInt;
		_int = new int64_t(*object._int);
		break;
	case vDouble:
		_type = vDouble;
		_d = new double(*object._d);
		break;
	case vString:
		_type = vString;
		_s = new string(*object._s);
		break;
	case vArray:
		_type = vArray;
		_vv = new vector<VariableObject>(*object._vv);
		break;
	case vMethod:
		_type = vMethod;
		_m = new Method(*object._m);
		break;
	case vLiterally:
		_type = vLiterally;
		_l = new Literally(*object._l);
		break;
	case vFunction:
		_type = vFunction;
		_f = object._f;
		break;
	} // end switch
} // end copy constructr

VariableObject& VariableObject::operator=(const VariableObject &object)
{
	clear();

	switch (object.type())
	{
	case vNone:
		break;
	case vBool:
		_type = vBool;
		_b = new bool(*object._b);
		break;
	case vChar:
		_type = vChar;
		_c = new char(*object._c);
		break;
	case vInt:
		_type = vInt;
		_int = new int64_t(*object._int);
		break;
	case vDouble:
		_type = vDouble;
		_d = new double(*object._d);
		break;
	case vString:
		_type = vString;
		_s = new string(*object._s);
		break;
	case vArray:
		_type = vArray;
		_vv = new vector<VariableObject>(*object._vv);
		break;
	case vMethod:
		_type = vMethod;
		_m = new Method(*object._m);
		break;
	case vLiterally:
		_type = vLiterally;
		_l = new Literally(*object._l);
		break;
	case vFunction:
		_type = vFunction;
		_f = object._f;
		break;
	}

	return *this;
} // end operator=

// Query
void * VariableObject::getObject()
{
	switch (_type)
	{
	case vNone: return nullptr;
	case vBool: return &(*_b);
	case vChar: return &(*_c);
	case vInt: return &(*_int);
	case vDouble: return &(*_d);
	case vString: return &(*_s);
	case vArray: return &(*_vv);
	case vMethod: return &(*_m);
	case vLiterally: return &(*_l);
	case vFunction: return &_f;
	} // end switch

	throw invalid_argument("Invalid type");
} // end function getObject

Method VariableObject::getMethod() const
{
	_ASSERT(_type == vMethod);
	return *_m;
} // end function getMethod


bool vUtils::isEqual(const MixedArray &arr1, const MixedArray &arr2)
{
	if (arr1.size() != arr2.size()) return false;

	for (register size_t i = 0; i < arr1.size(); i++)
	{
		VariableObject v1 = arr1[i];
		VariableObject v2 = arr2[i];

		if (v1.type() != v2.type()) return false;

		switch (v1.type())
		{
		case vNone:
			if (!v2.isNull()) return false;
			break;
		case vBool:
			if (v1.getBool() != v2.getBool()) return false;
			break;
		case vChar:
			if (v1.getChar() != v2.getChar()) return false;
			break;
		case vInt:
			if (v1.getInt() != v2.getInt()) return false;
			break;
		case vDouble:
			if (v1.getDouble() != v2.getDouble()) return false;
			break;
		case vString:
			if (v1.getString() != v2.getString()) return false;
			break;
		case vArray:
			if (!isEqual(v1.getArray(), v2.getArray())) return false;
			break;
		case vMethod:
			if (v1.getMethod() != v2.getMethod()) return false;
			break;
		case vLiterally:
			if (v1.getLiterally().toString() != v2.getLiterally().toString()) return false;
			break;
		case vFunction: return false;
		} // end switch

	} // end for

	return true;
} // end function isEqual

MixedArray vUtils::slice(const MixedArray &obj, const int _start, const int _end)
{
	MixedArray temp = obj;
	int start = _start;
	int end = _end;

	if (start < 0)
	{
		start = (int)temp.size() + _start;
		if (start < 0) return{};
	} // end if
	else if (start > (int)temp.size())
	{
		return {};
	} // end else if


	if (end < 0)
	{
		end = (int)temp.size() + _end;
		if (end < 0) return {};
	} // end if
	else if (end >= (int)temp.size() - start)
	{
		end = (int)temp.size();
	} // end else if

	if (temp.begin() + end - 1 < temp.begin() + start) return {};

	temp = MixedArray(temp.begin() + start, temp.begin() + end);

	return temp;
} // end function slice

MixedArray vUtils::splice(MixedArray &arr, const int _start, const int _deleteCount, const vector<string> &v_str)
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

	MixedArray::iterator start_it = arr.begin() + start;
	MixedArray::iterator end_it = arr.end();

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

	MixedArray::iterator deleteCountStart = start_it;
	MixedArray::iterator deleteCountEnd = deleteCountStart + deleteCount;

	MixedArray result = MixedArray(deleteCountStart, deleteCountEnd);

	arr.erase(deleteCountStart, deleteCountEnd);

	if (v_str.size() == 1) arr.insert(arr.begin() + start, v_str[0]);
	else if (v_str.size() > 1) arr.insert(arr.begin() + start, v_str.begin(), v_str.end());

	return result;
} // end function splice

bool vUtils::contains(const MixedArray &var, const VariableObject &value)
{
	for (VariableObject v : var) {
		if (v.type() == value.type()) {
			switch (value.type())
			{
			case vNone: return true;
			case vBool: return v.getBool() == value.getBool();
			case vChar: return v.getChar() == value.getChar();
			case vInt: return v.getInt() == value.getInt();
			case vDouble: return v.getDouble() == value.getDouble();
			case vString: return v.getString() == value.getString();
			case vArray:
				MixedArray arr1 = v.getArray();
				MixedArray arr2 = value.getArray();
				bool equal = true;

				for (register size_t i = 0; i < arr1.size(); i++)
				{
					switch (arr1[i].type())
					{
					case vNone:
						if (arr2[i].type() != vNone) {
							equal = false;
						} // end if
						break;
					case vBool:
						if (arr2[i].type() != vBool || arr1[i].getBool() != arr2[i].getBool()) {
							equal = false;
						} // end if
						break;
					case vChar:
						if (arr2[i].type() != vChar || arr1[i].getChar() != arr2[i].getChar()) {
							equal = false;
						} // end if
						break;
						return v.getChar() == value.getChar();
					case vInt:
						if (arr2[i].type() != vInt || arr1[i].getInt() != arr2[i].getInt()) {
							equal = false;
						} // end if
						break;
					case vDouble:
						if (arr2[i].type() != vDouble || arr1[i].getDouble() != arr2[i].getDouble()) {
							equal = false;
						} // end if
						break;
					case vString:
						if (arr2[i].type() != vString || arr1[i].getString() != arr2[i].getString()) {
							equal = false;
						} // end if
						break;
					default:
						equal = false;
					} // end switch

					if (!equal) break;
				} // end for

				if (equal) return true;
			} // end switch
		} // end if
	} // end for

	return false;
} // end function contains

string vUtils::tformat(const char *format) // base function
{
	return format;
} // end function tformat

template<typename T, typename... Targs>
string vUtils::tformat(const char *format, T value, Targs... Fargs) // recursive variadic function
{
	if (*format == '\0') return "";
	else if (*format == '%') {
		string output = VariableObject(value).toString();
		return output + tformat(format + 1, Fargs...); // recursive call
	} // end if
	else {
		string output;
		for (; *format != '\0'; format++)
		{
			if (*format != '%') output += *format;
			else if (*format == '%') break;
		} // end for

		return output + tformat(format, value, Fargs...); // recursive call
	} // end else
} // end function tformat