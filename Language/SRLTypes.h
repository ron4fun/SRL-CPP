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

#ifndef SRLTYPES_H
#define SRLTYPES_H

#include <memory>
#include <utility>
#include <functional>

#include <string>
#include <vector>

#include "Helpers\Literally.h"

#include "../Builder.h"


using namespace std;


typedef SRLBuilder::Builder Builder;

typedef function<void(Builder&)> func_ptr_t;

class Method;

enum VarTypes
{
	vNone, vBool, vChar, vInt, vDouble, vString, vArray, vMethod, vLiterally, vFunction
};


class VariableObject
{
private:
	int64_t * _int = nullptr;
	double * _d = nullptr;
	bool * _b = nullptr;
	char * _c = nullptr;
	string * _s = nullptr;
	vector<VariableObject> *_vv = nullptr;

	Literally * _l = nullptr;
	Method * _m = nullptr;

	//function<void (Builder&)> _f;
	func_ptr_t _f;

public:
	VariableObject() : _type(vNone) {}
	VariableObject(nullptr_t) : VariableObject() {}

	VariableObject(const bool object) : _type(vBool) {_b = new bool(object);}
	VariableObject(const int32_t object) : _type(vInt) { _int = new int64_t(object); }
	VariableObject(const int64_t object) : _type(vInt) { _int = new int64_t(object); }
	VariableObject(const double object) : _type(vDouble) { _d = new double(object); }
	VariableObject(const char &object) : _type(vChar) { _c = new char[2]; _c[0] = object; _c[1] = '\0'; }
	VariableObject(const char * object) : _type(vString) { _s = new string(object); }
	VariableObject(const string &object) : _type(vString) { _s = new string(object); }
	VariableObject(const string &&object) : _type(vString) { _s = new string(move(object)); }
	VariableObject(const vector<VariableObject> &object) : _type(vArray) { _vv = new vector<VariableObject>(object); }
	
	VariableObject(const Literally &object) : _type(vLiterally) { _l = new Literally(object); }

	VariableObject(const Method &object);
	
	//VariableObject(const function<void (Builder&)> &object) : _type(vFunction) { _f = object; }

	VariableObject(const func_ptr_t &object) : _type(vFunction) { _f = object; }

	// Clear
	void clear();

	~VariableObject()
	{
		clear();
	}

	// Copy
	VariableObject(const VariableObject &object);

	// Assign
	VariableObject& operator=(nullptr_t) {
		*this = VariableObject();
		return *this;
	}
	VariableObject& operator=(const bool &object) {
		*this = VariableObject(object);
		return *this;
	}
	VariableObject& operator=(const char &object) {
		*this = VariableObject(object);
		return *this;
	}
	VariableObject& operator=(const int32_t &object) {
		*this = VariableObject(object);
		return *this;
	}
	VariableObject& operator=(const int64_t &object) {
		*this = VariableObject(object);
		return *this;
	}
	VariableObject& operator=(const double &object) {
		*this = VariableObject(object);
		return *this;
	}
	VariableObject& operator=(const string &object) {
		*this = VariableObject(object);
		return *this;
	}
	VariableObject& operator=(const vector<VariableObject> &object) {
		*this = VariableObject(object);
		return *this;
	}

	VariableObject& operator=(const Literally &object) {
		*this = VariableObject(object);
		return *this;
	}

	VariableObject& operator=(const func_ptr_t &object) {
		*this = VariableObject(object);
		return *this;
	}

	VariableObject& operator=(const VariableObject &object);

	// Query
	void * getObject();

	bool isNull() const
	{
		switch (_type)
		{
		case vNone: return true;
		} // end switch

		return false;
	} // end function isNull

	string toString() const
	{
		switch (_type)
		{
		case vNone: return "";
		case vBool: 
			if (*_b) return "true";
			return "false";
		case vChar: return string(_c);
		case vInt: return to_string(*_int);
		case vDouble: return to_string(*_d);
		case vString: return *_s;
		case vLiterally: return _l->toString();
		case vArray: 
		case vMethod:
		case vFunction:
			break;
		} // end switch

		throw invalid_argument("Invalid type");
	} // end function getObject

	VarTypes type() const { return _type; } // end function type

	string getString() const
	{
		_ASSERT(_type == vString);
		return *_s;
	} // end function getString

	bool getBool() const
	{
		_ASSERT(_type == vBool);
		return *_b;
	} // end function getBool

	char getChar() const
	{
		_ASSERT(_type == vChar);
		return _c[0];
	} // end function getChar

	int64_t getInt() const
	{
		_ASSERT(_type == vInt);
		return *_int;
	} // end function getInt

	double getDouble() const
	{
		_ASSERT(_type == vDouble);
		return *_d;
	} // end function getDouble

	vector<VariableObject> getArray() const
	{
		_ASSERT(_type == vArray);
		return *_vv;
	} // end function getArray

	Literally getLiterally() const
	{
		_ASSERT(_type == vLiterally);
		return *_l;
	} // end function getLiterally

	Method getMethod() const;

	func_ptr_t getFunction() const
	{
		_ASSERT(_type == vFunction);
		return _f;
	} // end function getFunction

	// Conversion
	explicit operator bool() const
	{
		switch (_type)
		{
		case vNone: return true;
		case vBool: return _b != nullptr && *_b == true;
		case vChar: return _c != nullptr && *_c != NULL && *_c != '\0';
		case vInt: return _int != nullptr && *_int != 0;
		case vDouble: return _d != nullptr && *_d != 0.0;
		case vString: return _s != nullptr && !_s->empty();
		case vArray: return _vv != nullptr && !_vv->empty();
		case vMethod: return _m != nullptr;
		case vLiterally: return _l != nullptr;
		case vFunction: return _f != nullptr;
		} // end switch

		throw invalid_argument("Invalid type");
	}

private:
	VarTypes _type;

};


typedef vector<VariableObject> MixedArray;


class vUtils
{
public:
	static bool isEqual(const MixedArray &arr1, const MixedArray &arr2);

	/*
	* Javascript emulated slice
	*
	* @param T &obj
	* @param int start
	* @param int end
	*
	* @return T
	*/
	static MixedArray slice(const MixedArray &obj, const int _start, const int _end = INT32_MAX);

	/*
	  * Javascript emulated splice
	  *
	  * @param MixedArray &arr
	  * @param int start
	  * @param int deleteCount
	  * @param vector<string> &v_str
	  *
	  * @return MixedArray
	  */
	static MixedArray splice(MixedArray &arr, const int _start, const int _deleteCount = INT32_MIN, const vector<string> &v_str = {});

	static bool contains(const MixedArray &var, const VariableObject &value);

	static string tformat(const char *format);

	template<typename T, typename... Targs>
	static string tformat(const char *format, T value, Targs... Fargs);

}; // end class vUtils

#endif // !SRLTYPES_H