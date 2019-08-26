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

#ifndef BUILDQUERY_H
#define BUILDQUERY_H

#include <string>
#include "../Methods/Method.h"
#include "../../Builder.h"
#include "../../Builder/NonCapture.h"
#include "../../Utils.h"

using namespace std;


/**
* After the query was resolved, it can be built and thus executed.
*
* @param MixedArray $query
* @param Builder|null $builder If no Builder is given, the default Builder will be taken.
* @return Builder
* @throws SyntaxException
*/
Builder& buildQuery(MixedArray &query, Builder &builder)
{
	for (size_t i = 0; i < query.size(); i++)
	{
		VariableObject method = query[i];

		if (method.type() == vArray) {
			builder.and(buildQuery(method.getArray(), NonCapture()));
			continue;
		} // end if

		if (method.type() != vMethod) {
			// At this point, there should only be methods left, since all parameters are already taken care of.
			// If that's not the case, something didn't work out.
			throw SyntaxException("Unexpected statement: " + method.toString());
		} // end if

		MixedArray parameters;
		// If there are parameters, walk through them and apply them if they don't start a new method.
		while (i + 1 < query.size() && query[i + 1] && !(query[i + 1].type() == vMethod)) {
			parameters.push_back(query[i + 1]);

			// Since the parameters will be appended to the method object, they are already parsed and can be
			// removed from further parsing. Don't use unset to keep keys incrementing.
			vUtils::splice(query, (int)i + 1, 1);
		} // end while

		try {
			// Now, append that method to the builder object.
			(*(Method *)(method.getObject())).setParameters(parameters).callMethodOn(builder);
		} // end try
		catch (const exception e) {
			size_t lastIndex = parameters.size() - 1;
			if (parameters[lastIndex].type() == vArray) {
				if (lastIndex != 0) {
					(*(Method *)(method.getObject())).setParameters(vUtils::slice(parameters, 0, (int)lastIndex));
				} // end if
				
				(*(Method *)(method.getObject())).callMethodOn(builder);
				builder.and(buildQuery(parameters[lastIndex].getArray(), NonCapture()));
			} // end if
			else {
				throw SyntaxException("Invalid parameter given for " + (*(Method *)(method.getObject())).getOrigin());
			} // end else
		} // end catch
	} // end for

	return builder;
} // end function buildQuery


#endif // !BUILDQUERY_H
