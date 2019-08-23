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

#ifndef METHOD_H
#define METHOD_H

#include <memory>
#include <utility>
#include <functional>
#include "../../Exceptions/SyntaxException.h"
#include "../../Exceptions/ImplementationException.h"
#include "../Helpers/Literally.h"

#include "../../Builder.h"

#include "../../Utils.h"
#include "../SRLTypes.h"

typedef SRLBuilder::Builder Builder;


class Method 
{
public:
	Method(const string &origin, const string &methodName)
	{
		_origin = origin;	
		_methodName = methodName;
	} // end cctr

	MixedArray& getExecutedCallbacks() {
		return _executedCallbacks;
	} // end function getExecutedCallbacks

	string getOrigin() const
	{
		return _origin;
	} // end function getOrigin

	Builder& applyMethod(Builder &builder, const string &methodName, MixedArray &parameters) const
	{
		if (memcmp(methodName.c_str(), "capture", methodName.size() | 7) == 0) {
			if ((int)parameters.size() > 0) {
				if (parameters.size() > 2) {
					if (parameters[0].type() == vString) return builder.capture(parameters[0].getString(), parameters[2].getString());
					if (parameters[0].type() == vFunction) return builder.capture(parameters[0].getFunction(), parameters[2].getString());
				} // end if
				if (parameters[0].type() == vString) return builder.capture(parameters[0].getString());
				if (parameters[0].type() == vFunction) return builder.capture(parameters[0].getFunction());
			} // end if
			throw SyntaxException("Capture syntax wrongly formed.");
		} // end if
		else if (memcmp(methodName.c_str(), "anyCharacter", methodName.size() | 12) == 0) {
			return builder.anyCharacter();
		} // end if
		else if (memcmp(methodName.c_str(), "backslash", methodName.size() | 9) == 0) {
			return builder.backslash();
		} // end else if
		else if (memcmp(methodName.c_str(), "noCharacter", methodName.size() | 11) == 0) {
			return builder.noCharacter();
		} // end else if
		else if (memcmp(methodName.c_str(), "multiLine", methodName.size() | 9) == 0) {
			// pass
		} // end else if
		else if (memcmp(methodName.c_str(), "caseInsensitive", methodName.size() | 15) == 0) {
			return builder.caseInsensitive();
		} // end else if
		else if (memcmp(methodName.c_str(), "startsWith", methodName.size() | 10) == 0) {
			return builder.startsWith();
		} // end else if
		else if (memcmp(methodName.c_str(), "mustEnd", methodName.size() | 7) == 0) {
			return builder.mustEnd();
		} // end else if
		else if (memcmp(methodName.c_str(), "onceOrMore", methodName.size() | 10) == 0) {
			return builder.onceOrMore();
		} // end else if
		else if (memcmp(methodName.c_str(), "neverOrMore", methodName.size() | 11) == 0) {
			return builder.neverOrMore();
		} // end else if
		else if (memcmp(methodName.c_str(), "newLine", methodName.size() | 7) == 0) {
			return builder.newLine();
		} // end else if
		else if (memcmp(methodName.c_str(), "whitespace", methodName.size() | 10) == 0) {
			return builder.whitespace();
		} // end else if
		else if (memcmp(methodName.c_str(), "noWhitespace", methodName.size() | 12) == 0) {
			return builder.noWhitespace();
		} // end else if
		else if (memcmp(methodName.c_str(), "any", methodName.size() | 3) == 0) {
			return builder.any();
		} // end else if
		else if (memcmp(methodName.c_str(), "tab", methodName.size() | 3) == 0) {
			return builder.tab();
		} // end else if
		else if (memcmp(methodName.c_str(), "verticalTab", methodName.size() | 11) == 0) {
			return builder.verticalTab();
		} // end else if
		else if (memcmp(methodName.c_str(), "digit", methodName.size() | 5) == 0) {
			if ((int)parameters.size() == 3) return builder.digit(atoi(parameters[0].getString().c_str()), atoi(parameters[2].getString().c_str()));
			return builder.digit();
		} // end else if
		else if (memcmp(methodName.c_str(), "noDigit", methodName.size() | 7) == 0) {
			return builder.noDigit();
		} // end else if
		else if (memcmp(methodName.c_str(), "letter", methodName.size() | 6) == 0) {
			return builder.letter();
		} // end else if
		else if (memcmp(methodName.c_str(), "uppercaseLetter", methodName.size() | 15) == 0) {
			return builder.uppercaseLetter();
		} // end else if
		else if (memcmp(methodName.c_str(), "once", methodName.size() | 4) == 0) {
			return builder.once();
		} // end else if
		else if (memcmp(methodName.c_str(), "twice", methodName.size() | 5) == 0) {
			return builder.twice();
		} // end else if
		else if (memcmp(methodName.c_str(), "word", methodName.size() | 4) == 0) {
			return builder.word();
		} // end else if
		else if (memcmp(methodName.c_str(), "nonWord", methodName.size() | 7) == 0) {
			return builder.nonWord();
		} // end else if
		else if (memcmp(methodName.c_str(), "carriageReturn", methodName.size() | 14) == 0) {
			return builder.newLine(); // 
		} // end else if

		else if (memcmp(methodName.c_str(), "literally", methodName.size() | 9) == 0) {
			if ((int)parameters.size() > 0) return builder.literally(parameters[0].getString());
			throw SyntaxException("Literally syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "anyOf", methodName.size() | 5) == 0) {
			if ((int)parameters.size() > 0) {
				if (parameters[0].type() == vFunction) return builder.anyOf(parameters[0].getFunction());
				return builder.anyOf(parameters[0].getString());
			} // end if
			throw SyntaxException("AnyOf syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "noneOf", methodName.size() | 6) == 0) {
			if ((int)parameters.size() > 0) return builder.noneOf(parameters[0].getString());
			throw SyntaxException("NoneOf syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "ifFollowedBy", methodName.size() | 12) == 0) {
			if ((int)parameters.size() > 0) {
				if (parameters[0].type() == vFunction) return builder.ifFollowedBy(parameters[0].getFunction());
				return builder.ifFollowedBy(parameters[0].getString());
			} // end if
			throw SyntaxException("IfFollowedBy syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "ifNotFollowedBy", methodName.size() | 15) == 0) {
			if ((int)parameters.size() > 0) {
				if (parameters[0].type() == vFunction) return builder.ifNotFollowedBy(parameters[0].getFunction());
				return builder.ifNotFollowedBy(parameters[0].getString());
			} // end if
			throw SyntaxException("IfNotFollowedBy syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "optional", methodName.size() | 8) == 0) {
			if (parameters.empty()) return builder.optional();
			if ((int)parameters.size() > 0) {
				if (parameters[0].type() == vFunction) return builder.optional(parameters[0].getFunction());
				return builder.optional(parameters[0].getString());
			} // end if
			throw SyntaxException("Optional syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "until", methodName.size() | 5) == 0) {
			if ((int)parameters.size() > 0) {
				if (parameters[0].type() == vFunction) return builder.until(parameters[0].getFunction());
				return builder.until(parameters[0].getString());
			} // end if
			throw SyntaxException("Until syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "raw", methodName.size() | 3) == 0) {
			if ((int)parameters.size() > 0) return builder.raw(parameters[0].getString());
			throw SyntaxException("Raw syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "oneOf", methodName.size() | 5) == 0) {
			if ((int)parameters.size() > 0) return builder.oneOf(parameters[0].getString());
			throw SyntaxException("OneOf syntax wrongly formed.");
		} // end else if

		else if (memcmp(methodName.c_str(), "exactly", methodName.size() | 7) == 0) {
			if ((int)parameters.size() > 0) return builder.exactly(atoi(parameters[0].getString().c_str()));
			throw SyntaxException("Exactly syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "atLeast", methodName.size() | 7) == 0) {
			if ((int)parameters.size() > 0) return builder.atLeast(atoi(parameters[0].getString().c_str()));
			throw SyntaxException("AtLeast syntax wrongly formed.");
		} // end else if
		else if (memcmp(methodName.c_str(), "between", methodName.size() | 7) == 0) {
			if ((int)parameters.size() >= 3)
				return builder.between(atoi(parameters[0].getString().c_str()), atoi(parameters[2].getString().c_str()));

			throw SyntaxException("Between syntax wrongly formed.");
		} // end else if
				
		return builder;
	} // end function applyMethod

	//==================================================================
	//  operator==
	//==================================================================
	//
	bool operator==(const Method &value) const
	{
		return _origin == value._origin && _methodName == value._methodName; // &&
			/*vUtils::isEqual(_parameters, value._parameters) &&
			_executedCallbacks == value._executedCallbacks;*/
	} // end function operator==

	//==================================================================
	//  operator!=
	//==================================================================
	//
	bool operator!=(const Method &value) const
	{
		return !(*this == value);
	} // end function operator!=

	Builder& callMethodOn(Builder &builder) const;

	virtual Method& setParameters(const MixedArray &parameters);

private:
	string _origin;

	string _methodName;

	MixedArray _parameters;

	MixedArray _executedCallbacks;

}; // end class Method


#endif // !METHOD_H
