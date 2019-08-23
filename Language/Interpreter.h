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

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <regex>
#include <string>
#include "../Utils.h"

#include "Helpers\Cache.h"
#include "Helpers\Literally.h"
#include "Helpers\parseParentheses.h"
#include "Helpers\buildQuery.h"
#include "Helpers\methodMatch.h"

#include "../Exceptions/InterpreterException.h"

using namespace std;


class Interpreter
{
public:
	/**
	* @constructor
	* @param  {string} query
	*/
	Interpreter(const string &query)
	{
		rawQuery = Utils::replace(Utils::trim(query), ";", "");

		if (Cache::has(rawQuery)) {
			builder = Cache::get(rawQuery).clone();
		} // end if
		else {
			build();
		} // end else
	} // !contructor

	~Interpreter()
	{
		//
	} // end destrcttr

	/**
	* Resolve and then build the query.
	*/
	void build()
	{
		resolvedQuery = resolveQuery(parseParentheses(rawQuery));
		
		builder = Builder();
		builder = buildQuery(resolvedQuery, builder);

		// Add built query to cache, to avoid rebuilding the same query over and over.
		Cache::set(rawQuery, builder);
	} // end function build

	/**
	* Resolve the query array recursively and insert Methods.
	*
	* @param MixedArray $query
	* @return MixedArray
	* @throws InterpreterException
	*/
	MixedArray resolveQuery(MixedArray &query)
	{
		// Using for, since the array will be altered. Foreach would change behaviour.
		for (register size_t i = 0; i < query.size(); i++)
		{
			auto item = query[i];

			if (item.type() == vString)
			{
				// Remove commas and remove item if empty.
				string temp = Utils::trim(item.getString());
				*(string *)item.getObject() = *(string *)query[i].getObject() = temp = Utils::replace(temp, ",", " ");

				if (temp.empty()) continue;

				try
				{
					// A string can be interpreted as a method. Let's try resolving the method then.
					Method * method = methodMatch(temp);

					// If anything was left over (for example parameters), grab them and insert them.
					string leftOver = Utils::iReplace(temp, method->getOrigin(), "");
					query[i] = VariableObject(*method);
					if (!leftOver.empty()) {
						vUtils::splice(query, (int)i + 1, 0, { Utils::trim(leftOver) });
					} // end if

					delete method; // destroy method
				} // end try
				catch (const exception e)
				{
					// There could be some parameters, so we'll split them and try to parse them again
					smatch sm;
					regex r = regex("(.*?)[\\s]+(.*)");

					bool matches = regex_search(temp, sm, r);

					if (matches){
						size_t count = sm.size();
						
						query[i] = Utils::trim(sm[1]);

						if (count > 1) {
							vUtils::splice(query, (int)i + 1, 0, { Utils::trim(sm[2]) });
						} // end if
					} // end if
				} // end catch
			} // end if
			else if (item.type() == vArray) {
				query[i] = resolveQuery(item.getArray());
			} // end else if
			else if (!(item.type() == vLiterally)) {
				throw InterpreterException("Unexpected statement: " + (*(Literally *)(item.getObject())).toString());
			} // end else if

		} // end if

		MixedArray result;
		for (register size_t i = 0; i < query.size(); i++)
		{
			VariableObject item = query[i];
			if (item) result.push_back(item);
		} // end for

		return result;
	} // end function resolveQuery

	/**
	* Return the builder object.
	*
	* @return {Builder}
	*/
	Builder getBuilder()
	{
		return builder;
	} // end function getBuilder

	/**
	* Return the built RegExp object.
	*
	* @return {RegExp}
	*/
	regex get() { return builder.get(); }

private:
	string rawQuery;
	MixedArray resolvedQuery;
	Builder builder;

}; // end class Interpreter


#endif // !INTERPRETER_H