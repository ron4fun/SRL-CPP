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

#ifndef SIMPLEMETHOD_H
#define SIMPLEMETHOD_H


#include "Method.h"

/**
* Method having no parameters. Will throw SyntaxException if a parameter is provided.
*/
class SimpleMethod : public Method
{

	using base_type = Method;

public:
	SimpleMethod(const string &origin, const string &methodName)
		: base_type(origin, methodName)
	{}

	virtual Method& setParameters(const MixedArray &parameters)
	{
		if (parameters.size() != 0) {
			throw SyntaxException("Invalid parameters.");
		}

		return *this;
	} // end setParameters

};


#endif // ! SIMPLEMETHOD_H