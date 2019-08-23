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

#ifndef METHODMATCH_H
#define METHODMATCH_H

#include <regex>
#include <map>
#include <string>
#include "../../Utils.h"

#include "../Methods/Method.h"
#include "../Methods/AndMethod.h"
#include "../Methods/AsMethod.h"
#include "../Methods/SimpleMethod.h"
#include "../Methods/TimeMethod.h"
#include "../Methods/ToMethod.h"

#include "../../Exceptions/SyntaxException.h"

using namespace std;

// Unimplemented, all lazy, single line, unicode, first match
map<string, map<string, string>> mapper = {
	{"any character", { {"class", "SimpleMethod"}, {"method", "anyCharacter"}}},
	{"backslash",  { {"class", "SimpleMethod"}, {"method",  "backslash" }}},
	{"no character", { {"class", "SimpleMethod"}, {"method", "noCharacter" }}},
	{"multi line", { {"class", "SimpleMethod"}, {"method", "multiLine" }}},
	{"case insensitive", { {"class", "SimpleMethod"}, {"method", "caseInsensitive" }}},
	{"starts with", { {"class", "SimpleMethod"}, {"method", "startsWith" }}},
	{"start with", { {"class", "SimpleMethod"}, {"method", "startsWith" }}},
	{"begin with", { {"class", "SimpleMethod"}, {"method", "startsWith" }}},
	{"begins with", { {"class", "SimpleMethod"}, {"method", "startsWith" }}},
	{"must end", { {"class", "SimpleMethod"}, {"method", "mustEnd" }}},
	{"once or more", { {"class", "SimpleMethod"}, {"method", "onceOrMore" }}},
	{"never or more", { {"class", "SimpleMethod"}, {"method", "neverOrMore" }}},
	{"new line", { {"class", "SimpleMethod"}, {"method", "newLine" }}},
	{"whitespace", { {"class", "SimpleMethod"}, {"method", "whitespace" }}},
	{"no whitespace", { {"class", "SimpleMethod"}, {"method", "noWhitespace" }}},
	{"anything", { {"class", "SimpleMethod"}, {"method", "any" }}},
	{"tab", { {"class", "SimpleMethod"}, {"method", "tab" }}},
	{"vertical tab", { {"class", "SimpleMethod"}, {"method", "verticalTab" }}},
	{"digit", { {"class", "SimpleMethod"}, {"method", "digit" }}},
	{"no digit", { {"class", "SimpleMethod"}, {"method", "noDigit" }}},
	{"nondigit", { {"class", "SimpleMethod"}, {"method", "noDigit" }}},
	{"number", { {"class", "SimpleMethod"}, {"method", "digit" }}},
	{"letter", { {"class", "SimpleMethod"}, {"method", "letter" }}},
	{"uppercase", { {"class", "SimpleMethod"}, {"method", "uppercaseLetter" }}},
	{"once", { {"class", "SimpleMethod"}, {"method", "once" }}},
	{"twice", { {"class", "SimpleMethod"}, {"method", "twice" }}},
	{"word", { {"class", "SimpleMethod"}, {"method", "word" }}},
	{"no word", { {"class", "SimpleMethod"}, {"method", "nonWord" }}},
	{"nonword", { {"class", "SimpleMethod"}, {"method", "nonWord" }}},
	{"carriage return", { {"class", "SimpleMethod"}, {"method", "carriageReturn" }}},
	{"carriagereturn", { {"class", "SimpleMethod"}, {"method", "carriageReturn" }}},

	{"literally", { {"class", "DefaultMethod"}, {"method", "literally" }}},
	{"either of", { {"class", "DefaultMethod"}, {"method", "anyOf" }}},
	{"any of", { {"class", "DefaultMethod"}, {"method", "anyOf" }}},
	{"none of", { {"class", "DefaultMethod"}, {"method", "noneOf" }}},
	{"if followed by", { {"class", "DefaultMethod"}, {"method", "ifFollowedBy" }}},
	{"if not followed by", { {"class", "DefaultMethod"}, {"method", "ifNotFollowedBy" }}},
	{"optional", { {"class", "DefaultMethod"}, {"method", "optional" }}},
	{"until", { {"class", "DefaultMethod"}, {"method", "until" }}},
	{"raw", { {"class", "DefaultMethod"}, {"method", "raw" }}},
	{"one of", { {"class", "DefaultMethod"}, {"method", "oneOf" }}},

	{"digit from", { {"class", "ToMethod"}, {"method", "digit" }}},
	{"number from", { {"class", "ToMethod"}, {"method", "digit" }}},
	{"letter from", { {"class", "ToMethod"}, {"method", "letter" }}},
	{"uppercase letter from", { {"class", "ToMethod"}, {"method", "uppercaseLetter" }}},
	{"exactly", { {"class", "TimesMethod"}, {"method", "exactly" }}},
	{"at least", { {"class", "TimesMethod"}, {"method", "atLeast" }}},
	{"between", { {"class", "AndMethod"}, {"method", "between" }}},
	{"capture", { { "class", "AsMethod"}, {"method", "capture"}}},
} ;


/**
* Match a string part to a method. Please note that the string must start with a method.
*
* @param {string} part
* @throws {SyntaxException} If no method was found, a SyntaxException will be thrown.
* @return {Method}
*/
Method * methodMatch(const string &part) {
	string maxMatch;
	size_t maxMatchCount = 0;
	auto inMapper = &mapper;

	// Go through each mapper and check if the name matches. Then, take the highest match to avoid matching
	// 'any', if 'any character' was given, and so on.
	for (map<string, map<string, string>>::iterator it = inMapper->begin(); it != inMapper->end(); ++it)
	{
		string key = it->first;
		smatch sm;
		regex r = regex("^(" + regex_replace(key, regex(" "), ") (") + ")", regex_constants::icase);
		
		bool b = regex_search(part, sm, r);
		if (b == false) continue;
		
		size_t count = sm.size();

		if (count > maxMatchCount) {
			maxMatchCount = count;
			maxMatch = key;
		} // end if
	} // end for

	if (!maxMatch.empty()) {
		// We've got a match. Create the desired object and populate it.
		auto item = mapper.find(maxMatch)->second;
		
		string cls = item.find("class")->second;
		string mthd = item.find("method")->second;

		if (cls == "DefaultMethod") return new Method(maxMatch, mthd);
		if (cls == "SimpleMethod") return new SimpleMethod(maxMatch, mthd);
		if (cls == "TimesMethod") return new TimeMethod(maxMatch, mthd);
		if (cls == "AndMethod") return new AndMethod(maxMatch, mthd);
		if (cls == "AsMethod") return new AsMethod(maxMatch, mthd);
		if (cls == "ToMethod") return new ToMethod(maxMatch, mthd);
	} // endif

	throw SyntaxException("Invalid method: " + part);
}



#endif // !METHODMATCH_H