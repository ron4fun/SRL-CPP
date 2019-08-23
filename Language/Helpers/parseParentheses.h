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

#ifndef PARSEPARENTHESES_H
#define PARSEPARENTHESES_H

#include <string>
#include "../../Utils.h"
#include "../SRLTypes.h"

#include "../../Exceptions/SyntaxException.h"
#include "Literally.h"


using namespace std;


/**
* Replace all "literal strings" with a Literally object to simplify parsing later on.
*
* @param  {string} string
* @param  {size_t} openPos
* @param  {vector<map<string, int>>} stringPositions
* @return {MixedArray}
* @throws {SyntaxException}
*/
MixedArray createLiterallyObjects(const string &query, const size_t openPos, const vector<map<string, int>> &stringPositions) {
	string firstRaw = query.substr(0, openPos);
	MixedArray result = { Utils::trim(firstRaw) };
	size_t pointer = 0;


	for (register size_t i = 0; i < stringPositions.size(); i++)
	{
		map<string, int> stringPosition = stringPositions[i];
	
		if (stringPosition.find("end") == stringPosition.end()) {
			throw SyntaxException("Invalid string ending found.");
		} // end if

		size_t start = stringPosition.find("start")->second;
		size_t end = stringPosition.find("end")->second;
		if (end < firstRaw.size()) {
			// At least one string exists in first part, create a new object.

			// Remove the last part, since this wasn't parsed.
			result.pop_back();

			// Add part between pointer and string occurrence.
			result.push_back(Utils::trim(firstRaw.substr(pointer, start - pointer)));

			// Add the string as object.
			result.push_back(Literally(firstRaw.substr(
				start + 1,
				end - start
				)));

			result.push_back(Utils::trim(firstRaw.substr(end + 2)));

			pointer = end + 2;
		} // end if

	} // end for


	return result;
} // end function createLiterallyObjects


/**
* Parse parentheses and return multidimensional array containing the structure of the input string.
* It will parse ( and ) and supports nesting, escaping using backslash and strings using ' or ".
*
* @param  {string} query
* @return {MixedArray}
* @throws {SyntaxException}
*/
MixedArray parseParentheses(const string &_query)
{
	string query = _query;

	int openCount = 0;
	VariableObject openPos = false;
	VariableObject closePos = false;
	VariableObject inString = false;
	VariableObject backslash = false;
	vector<map<string, int>> stringPositions = {};
	size_t stringLength = query.size();

	if (query[0] == '(' && query[stringLength - 1] == ')') {
		query = Utils::slice(query, 1, -1);
	}

	for (register size_t i = 0; i < stringLength; i++) {
		string c = string(1, query[i]);

		if (inString) {
			if (
				c == inString.getString() &&
				(query[i - 1] != '\\' || (query[i - 1] == '\\' && query[i - 2] == '\\'))
				) {
				// We're no more in the string. Either the ' or " was not escaped, or it was but the backslash
				// before was escaped as well.
				inString = false;

				// Also, to create a "Literally" object later on, save the string end position.
				stringPositions[stringPositions.size() - 1].insert({ "end", i - 1 });
			} // end if

			continue;
		} // end if

		if (backslash) {
			// Backslash was defined in the last char. Reset it and continue, since it only matches one character.
			backslash = false;
			continue;
		} // end if

		switch (c[0]) {
		case '\\':
			// Set the backslash flag. This will skip one character.
			backslash = true;
			break;
		case '\"':
		case '\'':
			// Set the string flag. This will tell the parser to skip over this string.
			inString = c;
			// Also, to create a "Literally" object later on, save the string start position.
			stringPositions.push_back({{ "start",  i }});
			break;
		case '(':
			// Opening parenthesis, increase the count and set the pointer if it's the first one.
			openCount++;
			if (openPos.type() == vBool && openPos.getBool() == false) {
				openPos = (int64_t)i;
			} // end if
			break;
		case ')':
			// Closing parenthesis, remove count
			openCount--;
			if (openCount == 0) {
				// If this is the matching one, set the closing pointer and break the loop, since we don't
				// want to match any following pairs. Those will be taken care of in a later recursion step.
				closePos = (int64_t)i;
				goto endloop;
			} // end if

			break;
		} // end switch

	} // end for

endloop:;

	if (openCount != 0) {
		throw SyntaxException("Non-matching parenthesis found.");
	} // end if

	if (closePos.type() == vBool && closePos.getBool() == false) {
		// No parentheses found. Use end of string.
		openPos = closePos = (int64_t)stringLength;
	} // end if

	MixedArray result = createLiterallyObjects(query, (size_t)openPos.getInt(), stringPositions);

	if (openPos.getInt() != closePos.getInt()) {

		MixedArray a = parseParentheses(query.substr((size_t)openPos.getInt() + 1, (size_t)closePos.getInt() - (size_t)openPos.getInt() - 1));
		MixedArray b = parseParentheses(query.substr((size_t)closePos.getInt() + 1));

		result.reserve(a.size() + b.size());
		// Parentheses found.
		// First part is definitely without parentheses, since we'll match the first pair.
		MixedArray row1 = MixedArray(a.begin(), a.end());
		result.insert(result.end(), {row1});
		// This is the inner part of the parentheses pair. There may be some more nested pairs, so we'll check them.
		result.insert(result.end(), b.begin(), b.end());
		// Last part of the string wasn't checked at all, so we'll have to re-check it.
	}

	MixedArray temp;
	for (register size_t i = 0; i < result.size(); i++)
	{
		auto item = result[i];

		if (item.type() != vString || !item.getString().empty())
		{
			temp.push_back(item);
		}
	} // end for
	
	return temp;
} // end function parseParentheses


#endif // !PARSEPARENTHESES_H