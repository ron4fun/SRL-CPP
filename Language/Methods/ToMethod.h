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

#ifndef TOMETHOD_H
#define TOMETHOD_H


#include "Method.h"

/**
* Method having simple parameter(s) ignoring "to".
*/
class ToMethod : public Method
{

	using base_type = Method;

public:
	ToMethod(const string &origin, const string &methodName)
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

			if (lower != "to") parameters.push_back(parameter);
		} // end for

		return base_type::setParameters(parameters);
	} // end setParameters

};


#endif // ! TOMETHOD_H