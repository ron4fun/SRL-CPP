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

#ifndef TIMEMETHOD_H
#define TIMEMETHOD_H


#include "Method.h"

/**
* Method having one or two parameters. First is simple, ignoring second "time" or "times". Will throw SyntaxException if more parameters provided.
*/
class TimeMethod : public Method
{

	using base_type = Method;

public:
	TimeMethod(const string &origin, const string &methodName)
		: base_type(origin, methodName)
	{}

	virtual Method& setParameters(const MixedArray &__parameters)
	{
		MixedArray parameters;

		for (register size_t i = 0; i < __parameters.size(); i++) {
			VariableObject parameter = __parameters[i];

			if (parameter.type() != vString) {
				parameters.push_back(parameter);
			} // end if

			string lower = Utils::strToLower(parameter.getString());

			if (lower != "times" && lower != "time") parameters.push_back(parameter);
		} // end for

		if (parameters.size() > 1) {
			throw SyntaxException("Invalid parameter.");
		}

		return base_type::setParameters(parameters);
	} // end setParameters

};


#endif // ! TIMEMETHOD_H