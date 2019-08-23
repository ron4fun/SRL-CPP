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

#include "Method.h"
#include "../Helpers/SubProcess.h"


Builder& Method::callMethodOn(Builder &builder) const
{
	string methodName = _methodName;
	MixedArray parameters = _parameters;

	try {
		builder = applyMethod(builder, methodName, parameters);

		for (register size_t i = 0; i < parameters.size(); i++) {
			VariableObject parameter = parameters[i];

			if (
				parameter.type() == vFunction &&
				!vUtils::contains(_executedCallbacks, (int)i)
				) {
				// Callback wasn't executed, but expected to. Assuming parentheses without method, so let's "and" it.
				builder.group(parameter.getFunction());
			} // end if
		} // end for

	} // end try
	catch (ImplementationException &e) {
		throw SyntaxException(e.what());
	} // end catch
	catch (const exception e) {
		throw SyntaxException(methodName + " does not allow the use of sub-queries.");
	} // end catch

	return builder;
} // end function callMethodOn


Method& Method::setParameters(const MixedArray &parameters)
{
	for (register size_t i = 0; i < parameters.size(); i++) {
		VariableObject parameter = parameters[i];

		if (parameter.type() == vLiterally) {
			_parameters.push_back(parameter.getLiterally().toString());
		} // end if
		else if (parameter.type() == vArray) {
			 // Assuming the user wanted to start a sub-query. This means, we'll create a callback for them.
			_parameters.push_back(
				VariableObject([this, i, parameter](Builder &builder)
				{
					this->getExecutedCallbacks().push_back((int)i);
					SubProcess::callBuildQuery(parameter.getArray(), builder);
				}) // end function
			);
		} // end else if
		else {
			_parameters.push_back(parameter);
		} // end else

	} // end for

	return *this;
} // end function setParameters

