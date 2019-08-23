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

#ifndef BUILDER_H
#define BUILDER_H

#pragma warning(disable:4996) // _CRT_SECURE_NO_WARNING


#include <map>
#include <stdexcept>
#include <regex>
#include <iostream>
#include <functional>
#include "Exceptions\NotImplementedException.h"
#include "Exceptions\BuilderException.h"
#include "Exceptions\ImplementationException.h"
#include "Exceptions\SyntaxException.h"
#include "Utils.h"

using namespace std;
using namespace SRLExceptions;


namespace SRLBuilder
{

	const string NON_LITERAL_CHARACTERS = "[]\\^$.|?*+-(){}"; // "[\\^$.|?*+- ()/";

	const int METHOD_TYPE_BEGIN = 0b00001;
	const int METHOD_TYPE_CHARACTER = 0b00010;
	const int METHOD_TYPE_GROUP = 0b00100;
	const int METHOD_TYPE_QUANTIFIER = 0b01000;
	const int METHOD_TYPE_ANCHOR = 0b10000;
	const int METHOD_TYPE_UNKNOWN = 0b11111;
	const int METHOD_TYPES_ALLOWED_FOR_CHARACTERS = METHOD_TYPE_BEGIN | METHOD_TYPE_ANCHOR | METHOD_TYPE_GROUP | METHOD_TYPE_QUANTIFIER | METHOD_TYPE_CHARACTER;

	const map<string, mapperStruct> simpleMapper = {
		{"startsWith", mapperStruct("^", METHOD_TYPE_ANCHOR, METHOD_TYPE_BEGIN)},
		{ "mustEnd", mapperStruct("$", METHOD_TYPE_ANCHOR, METHOD_TYPE_CHARACTER | METHOD_TYPE_QUANTIFIER | METHOD_TYPE_GROUP) },
		{ "onceOrMore", mapperStruct("+", METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP) },
		{ "neverOrMore", mapperStruct("*", METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP) },
		{ "any", mapperStruct(".", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },

		{ "backslash", mapperStruct(R"(\\)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "tab", mapperStruct(R"(\t)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "verticalTab", mapperStruct(R"(\v)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "newLine", mapperStruct(R"(\n)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "carriageReturn", mapperStruct(R"(\r)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },

		{ "whitespace", mapperStruct(R"(\s)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "noWhitespace", mapperStruct(R"(\S)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "anyCharacter", mapperStruct(R"(\w)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "noCharacter", mapperStruct(R"(\W)", METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS) },
		{ "word", mapperStruct(R"(\b)", METHOD_TYPE_CHARACTER, METHOD_TYPE_BEGIN) },
		{ "nonWord", mapperStruct(R"(\B)", METHOD_TYPE_CHARACTER, METHOD_TYPE_BEGIN) },
	
	}; // end 


	class Builder
	{
		//==================================================================
		//  Friends of class
		//==================================================================
		//
		friend ostream &operator<<(ostream &output, Builder &value)
		{
			output << value.getRawRegex();
			return output; // enables cout << a << b << c;
		} // end function &operator<<


	public:
		Builder(){}

		~Builder () {
			if (_result != nullptr) {
				delete _result;
				_result = nullptr;
			} // end if
		} // end destructor
		
		Builder(Builder &value)
		{
			_regEx = value._regEx;
			_modifiers = value._modifiers;
			_lastMethodType = value._lastMethodType;
			_group = value._group;
			_implodeString = value._implodeString;
			_captureNames = value._captureNames;

			_clearResult();
		} // end cctor

		//==================================================================
		//  to string operator
		//==================================================================
		//
		explicit operator string() const
		{
			return getRawRegex();
		} // end function operator string()

		// 
		//==================================================================
		//  assignment operator
		//==================================================================
		//
		const Builder &operator=(Builder &value) 
		{
			if (*this != value)
			{
				_regEx = value._regEx;
				_modifiers = value._modifiers;
				_lastMethodType = value._lastMethodType;
				_group = value._group;
				_implodeString = value._implodeString;
				_captureNames = value._captureNames;

				_clearResult();
			} // end if

			return *this;
		} // end function assignment operator

		//==================================================================
		//  operator==
		//==================================================================
		//
		bool operator==(Builder &value) const
		{
			return _regEx == value._regEx && _modifiers == value._modifiers &&
				_lastMethodType == value._lastMethodType && _group == value._group && 
				_implodeString == value._implodeString;
		} // end function operator==
		
		//==================================================================
		//  operator!=
		//==================================================================
		//
		bool operator!=(Builder &value) const
		{
			return !(*this == value);
		} // end function operator!=

		/**********************************************************/
		/*                     CHARACTERS                         */
		/**********************************************************/

		/**
		* Add raw Regular Expression to current expression.
		*
		* @param  {string} regularExpression
		* @throws {BuilderException}
		* @return {Builder}
		*/
		Builder& raw(const string &regularExpression) {
			_lastMethodType = METHOD_TYPE_UNKNOWN;
			add(regularExpression);

			if (!isValid()) {
				_revertLast();
				throw BuilderException("Adding raw would invalidate this regular expression. Reverted.");
			} // end if

			return *this;
		} // end function raw;

		/**
		* Literally match one of these characters.
		*
		* @param  {string} chars
		* @return {Builder}
		*/
		Builder& oneOf(const string &chars) {
			string temp;

			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			for (register size_t i = 0;i < chars.size(); i++) {
				temp += escape(chars[i]);
			} //end if
			
			return add(Utils::format<char *>("[%s]", 1, temp.c_str()));
		} // end function oneOf

		/**
		* Literally match a character that is not one of these characters.
		*
		* @param  {string} chars
		* @return {Builder}
		*/
		Builder& noneOf(const string &chars) {
			string temp;

			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			for (register size_t i = 0; i < chars.size(); i++) {
				temp += escape(chars[i]);
			} //end if

			return add(Utils::format<char *>("[^%s]", 1, temp.c_str()));
		} // end function noneOf

		/**
		* Literally match all of these characters in that order.
		*
		* @param  {string} chars One or more characters
		* @return {Builder}
		*/
		Builder& literally(const string &chars) {
			string temp;

			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			for (register size_t i = 0;i < chars.size(); i++) {
				temp += escape(chars[i]);
			} //end if

			return add(Utils::format<char *>("(?:%s)", 1, temp.c_str())); // "(? : %s)"
		} // end function literally

		/**
		* Match any digit (in given span). Default will be a digit between 0 and 9.
		*
		* @param  {number} min
		* @param  {number} max
		* @return {Builder}
		*/
		Builder& digit(const int min = 0, const int max = 9) {
			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return add(Utils::format<int>("[%d-%d]", 2, min, max));
		} // end function digit

		/**
		* Match any number (in given span). Default will be a number between 0 and 9.
		*
		* @param  {number} min
		* @param  {number} max
		* @return {Builder}
		*/
		Builder& number(const int min = 0, const int max = 9) { return digit(min, max); } // end function number

		/**
		* Match any non-digit character (in given span). Default will be any character not between 0 and 9.
		*
		* @return {Builder}
		*/
		Builder& noDigit() {
			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return add("[^0-9]");
		} // end function noDigit

		/**
		* Match any uppercase letter (between A to Z).
		*
		* @param  {char} min
		* @param  {char} max
		* @return {Builder}
		*/
		Builder& uppercaseLetter(const char min = 'A', const char max = 'Z') {
			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return add(Utils::format<char>("[%c-%c]", 2, min, max));
		} // end function uppercaseLetter

		/**
		* Match any lowercase letter (between a to z).
		* @param  {char} min
		* @param  {char} max
		* @return {Builder}
		*/
		Builder& letter(const char min = 'a', const char max = 'z') {
			_validateAndAddMethodType(METHOD_TYPE_CHARACTER, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return add(Utils::format<char>("[%c-%c]", 2, min, max));
		} // end function letter

		/**********************************************************/
		/*                        GROUPS                          */
		/**********************************************************/

		/**
		* Match any of these conditions.
		*
		* @param  {Closure|Builder|string} conditions Anonymous function with its Builder as first parameter.
		* @return {Builder}
		*/
		Builder& anyOf(const string &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?:%s)", "|"), conditions);
		} // end function anyOf

		Builder& anyOf(Builder &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?:%s)", "|"), conditions);
		} // end function anyOf

		Builder& anyOf(function<void (Builder&)> func)
		{
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?:%s)", "|"), func);
		} // end function anyOf


		/**
		* Match any of these conditions.
		*
		* @param  {Closure|Builder|string} conditions Anonymous function with its Builder as first parameter.
		* @return {Builder}
		*/
		Builder& eitherOf(const string &conditions) {return anyOf(conditions);} // end function eitherOf
		Builder& eitherOf(Builder &conditions) { return anyOf(conditions); } // end function eitherOf
		Builder& eitherOf(function<void (Builder&)> func) { return anyOf(func);} // end function eitherOf

		/**
		* Match all of these conditions, but in a non capture group.
		*
		* @param  {Closure|Builder|string} conditions Anonymous function with its Builder as a first parameter.
		* @return {Builder}
		*/
		Builder& group(const string &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?:%s)"), conditions);
		} // end function group

		Builder& group(Builder &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?:%s)"), conditions);
		} // end function group

		Builder& group(function<void (Builder&)> func)
		{
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?:%s)"), func);
		} // end function group

		/**
		* Match all of these conditions, Basically reverts back to the default mode, if coming from anyOf, etc.
		*
		* @param  {Closure|Builder|string} conditions
		* @return {Builder}
		*/
		Builder& and(const string &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder(), conditions);
		} // end function and

		Builder& and(Builder &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder(), conditions);
		} // end function and

		Builder& and(function<void (Builder&)> func)
		{
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder(), func);
		} // end function and

		/**
		* Positive lookahead. Match the previous condition only if followed by given conditions.
		*
		* @param  {Closure|Builder|string} condition Anonymous function with its Builder as a first parameter.
		* @return {Builder}
		*/
		Builder& ifFollowedBy(const string &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?=%s)"), conditions);
		} // end function ifFollowedBy

		Builder& ifFollowedBy(Builder &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?=%s)"), conditions);
		} // end function ifFollowedBy

		Builder& ifFollowedBy(function<void (Builder&)> func)
		{
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?=%s)"), func);
		} // end function ifFollowedBy

		/**
		* Negative lookahead. Match the previous condition only if NOT followed by given conditions.
		*
		* @param  {Closure|Builder|string} condition Anonymous function with its Builder as a first parameter.
		* @return {Builder}
		*/
		Builder& ifNotFollowedBy(const string &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?!%s)"), conditions);
		} // end function ifNotFollowedBy

		Builder& ifNotFollowedBy(Builder &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?!%s)"), conditions);
		} // end function ifNotFollowedBy

		Builder& ifNotFollowedBy(function<void (Builder&)> func)
		{
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(?!%s)"), func);
		} // end function ifNotFollowedBy

		/**
		* Create capture group of given conditions.
		*
		* @param  {Closure|Builder|string} condition Anonymous function with its Builder as a first parameter.
		* @param  {String} name
		* @return {Builder}
		*/
		Builder& capture(const string &conditions, const string &name = "") {
			if (!name.empty()) _captureNames.push_back(name);
			
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(%s)"), conditions);
		} // end function capture

		Builder& capture(Builder &conditions, const string &name = "") {
			if (!name.empty()) _captureNames.push_back(name);

			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(%s)"), conditions);
		} // end function capture

		Builder& capture(function<void(Builder&)> func, const string &name = "") {
			if (!name.empty()) _captureNames.push_back(name);

			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder()._extends("(%s)"), func);
		} // end function capture

		/**********************************************************/
		/*                      QUANTIFIERS                       */
		/**********************************************************/

		/**
		* Make the last or given condition optional.
		*
		* @param  {null|Closure|Builder|string} conditions Anonymous function with its Builder as a first parameter.
		* @return {Builder}
		*/
		Builder& optional(const string &conditions="") {
			_validateAndAddMethodType(METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP);

			if (conditions.empty()) {
				return add("?");
			} // end if

			return _addClosure(Builder()._extends("(?:%s)?"), conditions);
		} // end function optional

		Builder& optional(Builder &conditions) {
			_validateAndAddMethodType(METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP);

			return _addClosure(Builder()._extends("(?:%s)?"), conditions);
		} // end function optional

		Builder& optional(function<void (Builder&)> func)
		{
			_validateAndAddMethodType(METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP);

			return _addClosure(Builder()._extends("(?:%s)?"), func);
		} // end function optional

		/**
		* Previous match must occur so often.
		*
		* @param  {number} min
		* @param  {number} max
		* @return {Builder}
		*/
		Builder& between(const int min, const int max) {
			_validateAndAddMethodType(METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP);

			return add(Utils::format<int>("{%d,%d}", 2, min, max));
		} // end function between

		/**
		* Previous match must occur at least this often.
		*
		* @param  {number} min
		* @return {Builder}
		*/
		Builder& atLeast(const int min) {
			_validateAndAddMethodType(METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP);

			return add(Utils::format<int>("{%d,}", 1, min));
		} // end function atLeast

		/**
		* Previous match must occur exactly once.
		*
		* @return {Builder}
		*/
		Builder& once() {
			return exactly(1);
		} // end function once

		/**
		* Previous match must occur exactly twice.
		*
		* @return {Builder}
		*/
		Builder& twice() {
			return exactly(2);
		} // end function twice

		/**
		* Previous match must occur exactly this often.
		*
		* @param  {number} count
		* @return {Builder}
		*/
		Builder& exactly(const int count) {
			_validateAndAddMethodType(METHOD_TYPE_QUANTIFIER, METHOD_TYPE_CHARACTER | METHOD_TYPE_GROUP);

			return add(Utils::format<int>("{%d}", 1, count));
		} // end function exactly

		/**
		* Match less chars instead of more (lazy).
		*
		* @return {Builder}
		* @throws {ImplementationException}
		*/
		Builder& lazy() {
			const string chars = "+*}?";
			const string raw = getRawRegex();
			const string last = Utils::substr(raw, -1);
			const int lastMethodType = _lastMethodType;
			_lastMethodType = METHOD_TYPE_QUANTIFIER;

			if (!Utils::contains(chars, last)) 
			{
				if (last == ")" && Utils::contains(chars, Utils::substr(raw, -2, 1)))
				{
					const string target = lastMethodType == METHOD_TYPE_GROUP ? Utils::slice(_revertLast(), 0, -1) + "?)" : "?";
					return add(target);
				} // end if

				throw ImplementationException("Cannot apply laziness at this point. Only applicable after quantifier.");
			} // end if

			return add("?");
		} // end function lazy


		/**
		* Match up to the given condition.
		*
		* @param  {Closure|Builder|string} toCondition
		* @return {Builder}
		*/
		Builder& until(const string &toCondition) {
			lazy();
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder(), toCondition);
		} // end function until

		Builder& until(Builder &toCondition) {
			lazy();
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder(), toCondition);
		} // end function until

		Builder& until(function<void (Builder&)> func)
		{
			lazy();
			_validateAndAddMethodType(METHOD_TYPE_GROUP, METHOD_TYPES_ALLOWED_FOR_CHARACTERS);

			return _addClosure(Builder(), func);
		} // end function until

		/**********************************************************/
		/*                   MODIFIER MAPPER                      */
		/**********************************************************/

		/**
		* Add a specific unique modifier. This will ignore all modifiers already set.
		*
		* @param  {int} modifier
		* @return {Builder}
		*/
		Builder& addUniqueModifier(const regex_constants::syntax_option_type modifier) {
			_clearResult();

			if (!Utils::contains(_modifiers, modifier)) {
				_modifiers.push_back(modifier);
			} // end if

			return *this;
		} // end function addUniqueModifier

		Builder& ECMAScript() {
			return addUniqueModifier(regex_constants::ECMAScript);
		} // end function ECMAScript

		Builder& caseInsensitive() {
			return addUniqueModifier(regex_constants::icase);
		} // end function caseInsensitive

		/**********************************************************/
		/*                   SIMPLE MAPPER                        */
		/**********************************************************/

		Builder& startsWith() {
			return _addFromMapper("startsWith");
		} // end function startsWith

		Builder& beginWith() { return startsWith();  } // end function beginWith

		Builder& mustEnd() {
			return _addFromMapper("mustEnd");
		} // end function mustEnd

		Builder& onceOrMore() {
			return _addFromMapper("onceOrMore");
		} // end function onceOrMore

		Builder& neverOrMore() {
			return _addFromMapper("neverOrMore");
		} // end function neverOrMore

		Builder& any() {
			return _addFromMapper("any");
		} // end function nonWord

		Builder& backslash() {
			return _addFromMapper("backslash");
		} // end function backslash

		Builder& tab() {
			return _addFromMapper("tab");
		} // end function tab

		Builder& verticalTab() {
			return _addFromMapper("verticalTab");
		} // end function verticalTab

		Builder& newLine() {
			return _addFromMapper("newLine");
		} // end function newLine

		Builder& whitespace() {
			return _addFromMapper("whitespace");
		} // end function whitespace

		Builder& noWhitespace() {
			return _addFromMapper("noWhitespace");
		} // end function noWhitespace

		Builder& anyCharacter() {
			return _addFromMapper("anyCharacter");
		} // end function anyCharacter

		Builder& noCharacter() {
			return _addFromMapper("noCharacter");
		} // end function noCharacter

		Builder& word() {
			return _addFromMapper("word");
		} // end function word

		Builder& nonWord() {
			return _addFromMapper("nonWord");
		} // end function nonWord

		/**********************************************************/
		/*                   INTERNAL METHODS                     */
		/**********************************************************/

		/**
		* Escape specific character.
		*
		* @param  {char} character
		* @return {string}
		*/
		string escape(const char character) {
			string temp;
			if (Utils::contains(NON_LITERAL_CHARACTERS, character)) temp += "\\";
			temp += character;
			return temp;
		} // end function escape

		/**
		* Get the raw regular expression string.
		*
		* @return string
		*/
		string getRawRegex() const {
			string temp = Utils::join(_regEx, _implodeString.c_str());
			return Utils::replace(_group, "%s", temp);
		} // end function getRawRegex

		/**
		* Get the union of all set modifiers.
		*
		* @return {int}
		*/
		regex_constants::syntax_option_type getModifiers() {
			regex_constants::syntax_option_type value = _modifiers[0];
			for (register size_t i = 1; i < _modifiers.size(); i++) value |= _modifiers[i];
			
			return value;
		} // end function getModifiers

		/**
		* Remove specific flag.
		*
		* @param  {int} flag
		* @return {Builder}
		*/
		Builder& removeModifier(const regex_constants::syntax_option_type flag) {
			_modifiers = Utils::remove(_modifiers, flag);
			_clearResult();

			return *this;
		} // end function removeModifier

		/**
		* Add condition to the expression query.
		*
		* @param  {string} condition
		* @return {Builder}
		*/
		Builder& add(const string &condition) {
			_clearResult(); // Reset result to make up a new one.
			_regEx.push_back(condition);
			return *this;
		} // end function add

		/**
		* Build and return the resulting RegExp object. This will apply all the modifiers.
		*
		* @return {RegExp}
		* @throws {SyntaxException}
		*/
		regex get() {
			if (isValid()) {
				return *_result;
			} // end if

			throw SyntaxException("Generated expression seems to be invalid.");
		} // end function get

		/**
		* Validate regular expression.
		*
		* @return {boolean}
		*/
		bool isValid() {
			if (_result != nullptr) {
				return true;
			} // end if
			else {
				try {
					if (_modifiers.empty())
						_result = new regex(getRawRegex().c_str());
					else
						_result = new regex(getRawRegex().c_str(), getModifiers());
					return true;
				} // end try
				catch (exception &) {
					return false;
				} // end catch
			} // end else
		} // end function isValid

		/**
		* Clone a new builder object.
		*
		* @return {Builder}
		*/
		Builder clone() const {
			Builder clone = Builder();

			// Copy deeply
			clone._regEx = vector<string>(_regEx);
			clone._modifiers = _modifiers;
			clone._lastMethodType = _lastMethodType;
			clone._group = _group;

			return clone;
		} // end function clone

		/**
		* Check if the target string contains any substring that matches the regex pattern.
		*
		* @param  {string} target
		* @return {boolean}
		*/
		bool isMatchContained(const string &target) {
			return regex_search(target, get());
		} // end function isMatchContained

		/**
		* Check if the target string matches the pattern completely.
		*
		* @param  {string} target
		* @return {boolean}
		*/
		bool isExactMatch(const string &target) {
			return regex_match(target, get());
		} // end function isExactMatch


		/**
		* Check if the target string matches the pattern completely or partly.
		*
		* @param  {string} target
		* @return {boolean}
		*/
		bool isMatching(const string &target) {
			return isMatchContained(target) || isExactMatch(target);
		} // end function isExactMatch

		/**
		* Search and replace patterns within a string of text.
		*
		* @param  {string} target
		* @param  {string} replacement_pattern_str
		* @return {string}
		*/
		string replace(const string &target, const string &replacement_pattern_str) {
			regex r = get();
			return regex_replace(target, r, replacement_pattern_str);
		} // end function replace

		/**
		* Get the first substring contained in the target string that matches the regex pattern.
		*
		* @param  {string} target
		* @return {vector<string>}
		*/
		vector<string> getMatch(const string &target) {
			smatch sm;
			vector<string> result;
			regex r = get();

			bool b = regex_search(target, sm, r);
			if (b == false) return vector<string>();

			for (register size_t i = 0; i < sm.size(); i++) {
				result.push_back(sm[i]);
			} // end for

			return result;
		} // end function getMatch

		/**
		* Get all matches.
		* @param  {string} target
		* @return {vector<Dictionary>}
		*/
		vector<Dictionary> getMatches(const string &target) {
			vector<Dictionary> result;
			vector<string> arr;
			smatch sm;
			regex r = get();
			
			size_t last_pos = 0;
			string::const_iterator searchStart( target.cbegin() );
			while ( regex_search(searchStart, target.cend(), sm, r) )
			{
				for (register size_t i = 0; i < sm.size(); i++) {					
					arr.push_back(sm[i]);
				} // end for

				result.push_back(_mapCaptureIndexToName(arr));

				arr.clear(); // empty array

				searchStart = sm.suffix().first;
				last_pos = (target.size() - sm.suffix().length());
			} // end while	

			return result;
		} // end function getMatches

	protected:
		/**
		* Clears the regex variable if any.
		*/
		void _clearResult() {
			if (_result != nullptr) {
				delete _result;
				_result = nullptr;
			} // end if
		} // end function _clearResult

		/**
		* Map capture index to name.
		* Then to resolve to return: ["0": "aa ", "1": "aa", [captureName]: "aa" ]
		*
		* @param {vector<string>} result
		* @return {Dictionary}
		*/
		Dictionary _mapCaptureIndexToName(const vector<string> &arr) const
		{
			Dictionary result;
			vector<string> _arr = Utils::slice(arr, 1);
			vector<string> names = _captureNames;

			size_t count = 0;
			for (register size_t i = 0; i < _arr.size(); ++i)
			{
				string current = _arr[i];

				if (i < names.size() && !(names[i].empty())) {
					result[names[i]] = current;
				} // end if
				else {
					char buf[50];
					result[itoa((int)count, buf, 10)] = current;
					count += 1;
				} // end else
			} // end for

			return result;
		} // end function _mapCaptureIndexToName

		/**
		* Validate method call. This will throw an exception if the called method makes no sense at this point.
		* Will add the current type as the last method type.
		*
		* @param  {number} type
		* @param  {number} allowed
		* @param  {string} methodName
		*/
		void _validateAndAddMethodType(const int type, const int allowed, const string &methodName="") {
			if (allowed & _lastMethodType) {
				_lastMethodType = type;
				return;
			} // end if

			string message;

			switch (_lastMethodType)
			{
			case METHOD_TYPE_BEGIN:
				message = "at the beginning";
				break;
			case METHOD_TYPE_CHARACTER:
				message = "after a literal character";
				break;
			case METHOD_TYPE_GROUP:
				message = "after a group";
				break;
			case METHOD_TYPE_QUANTIFIER:
				message = "after a quantifier";
				break;
			case METHOD_TYPE_ANCHOR:
				message = "after an anchor";
				break;
			default:
				message = "here";
				break;
			}

			throw ImplementationException(string("Method ") + methodName + string(" is not allowed ") + message);
		}

		/**
		* Add the value form simple mapper to the regular expression.
		*
		* @param  {string} name
		* @return {Builder}
		* @throws {BuilderException}
		*/
		Builder& _addFromMapper(const string &name) {
			mapperStruct item;
			
			if (!Utils::contains(simpleMapper, name, item)) {
				throw BuilderException("Unknown mapper.");
			} // end if

			_validateAndAddMethodType(item.type, item.allowed, name);
			return add(item.add);
		} // end function _addFromMapper

		/**
		* Build the given Closure or string and append it to the current expression.
		*
		* @param  {Builder} builder
		* @param  {Closure|Builder|string} conditions Either a closure, literal character string or another Builder instance.
		*/
		Builder& _addClosure(Builder &builder, const string &conditions) 
		{
			builder.literally(conditions);
			return add(builder.getRawRegex());
		} // end function _addClosure

		Builder& _addClosure(Builder &builder, Builder &conditions)
		{
			builder.raw(conditions.getRawRegex());
			return add(builder.getRawRegex());
		} // end function _addClosure

		Builder& _addClosure(Builder &builder, function<void (Builder&)> func)
		{
			func(builder);
			return add(builder.getRawRegex());
		} // end function _addClosure

		/**
		* Get and remove last added element.
		*
		* @return  {string}
		*/
		string _revertLast() {
			string last_el = _regEx[_regEx.size() - 1];
			_regEx.pop_back();
			return last_el;
		} // end function _revertLast

		/**
		* Extends self to match more cases.
		*
		* @param  {string} group
		* @param  {string} implodeString
		* @return {Builder}
		*/
		Builder& _extends(const string &group, const string &implodeString = "") {
			_group = group;
			_implodeString = implodeString;
			return *this;
		} // end function _extends

	protected:
		/** @var {array} _regEx Regular Expression being built. */
		vector<string> _regEx;

		/** @var {array} _modifiers Raw modifier to apply on. */
		vector<regex_constants::syntax_option_type> _modifiers = {};

		/** @var {number} _lastMethodType Type of last method, to avoid invalid builds. */
		int _lastMethodType = METHOD_TYPE_BEGIN;

		/** @var {RegExp|null} _result Regular Expression Object built. */
		regex *_result = nullptr;

		/** @var {string} _group Desired group, if any */
		string _group = "%s";

		/** @var {string} _implodeString String to join with. */
		string _implodeString = "";

		/** @var {array} _captureNames Save capture names to map */
		vector<string> _captureNames;
		
	}; // end class Builder

} // end namespace SRL

#endif // !BUILDER_H
