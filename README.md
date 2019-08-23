# SRL-CPP [![License](http://img.shields.io/badge/license-MIT-green.svg)](https://github.com/ron4fun/SRL-CPP/blob/master/LICENSE)
SRL-CPP is a Simple Regex Language builder library written C++11 that provides an easy to use interface for constructing both simple and complex regex expressions.

It also supports **capture names**.

**CHARACTERS Methods:**

    raw(string)	// Add raw Regular Expression to current expression.
	oneOf(string)	// Literally match one of these characters.
	noneOf(string)	// Literally match a character that is not one of these characters.
	literally(string)	// Literally match all of these characters in that order.
	digit(int,int)	// Match any digit (in given span). Default will be a digit 
					   between 0 and 9.
	number(int,int)	// Match any number (in given span). Default will be a number 
					   between 0 and 9.
	noDigit()	// Match any non-digit character (in given span). Default will be any 
				   character not between 0 and 9.
	uppercaseLetter(char,char)	// Match any uppercase letter (between A to Z).
	letter(char,char)	// Match any lowercase letter (between a to z).
	
**GROUPS Methods:**

	anyOf(Closure|Builder|string) // Match any of these conditions.
	eitherOf(Closure|Builder|string)	// Match any of these conditions.
	group(Closure|Builder|string)	// Match all of these conditions, but in a non 
									   capture group.
	and(Closure|Builder|string) // Match all of these conditions, Basically reverts 
								   back to the default mode, if coming from anyOf, etc.
	ifFollowedBy(Closure|Builder|string)	// Positive lookahead. Match the previous 
											   condition only if followed by given 
											   conditions.
	ifNotFollowedBy(Closure|Builder|string)	// Negative lookahead. Match the previous 
											   condition only if NOT followed by given 
											   conditions.
	capture(Closure|Builder|string, null|string) // Create capture group of given 
													conditions.
	
**QUANTIFIERS Methods:**

	optional(null|Closure|Builder|string)	// Make the last or given condition optional.
	between(int,int) // Previous match must occur so often.
	atLeast(int)	// Previous match must occur at least this often.
	once()	// Previous match must occur exactly once.
	twice() // Previous match must occur exactly twice.
	exactly(int)	// Previous match must occur exactly this often.
	lazy()	// Match less chars instead of more (lazy).
	until(Closure|Builder|string)	// Match up to the given condition.

**MODIFIER MAPPER Methods:**
	
	addUniqueModifier(int)	// Add a specific unique modifier. This will ignore all 
							   modifiers already set.
	ECMAScript()	// Add ECMA script modifier.
	caseInsensitive()	// Add case-insensitive modifier.
 
**SIMPLE MAPPER Methods:**

	startsWith()
	beginWith()
	mustEnd()
	onceOrMore()
	neverOrMore()
	any()
	backslash()
	tab()
	verticalTab()
	newLine()
	whitespace()
	noWhitespace()
	anyCharacter()
	noCharacter()
	word()
	nonWord()

**INTERNAL Methods:**

	escape(char)	// Escape specific character.
	getRawRegex()	// Get the raw regular expression string.
	getModifiers()	// Get the union of all set modifiers.
	removeModifier(int) // Remove specific flag.
	add(string)	// Add condition to the expression query.
	get()	// Build and return the resulting RegExp object. This will apply all 
			   the modifiers.
	isValid()	// Validate regular expression.
	clone()	// Clone a new builder object.
	isMatchContained(string)	// Check if the target string contains any substring 
								   that matches the regex pattern.
	isExactMatch(string)	// Check if the target string matches the pattern completely.
	isMatching(string)	// Check if the target string matches the pattern completely 
						   or partly.
	replace(string,string)	// Search and replace patterns within a string of text.
	getMatch(string)	// Get the first substring contained in the target string that 
						   matches the regex pattern.
	getMatches(string)	// Get all matches.
    
**Usage Examples.**

The SRL function accepts a Simple Regex Language string as input, and return the builder for the query. 

```c++
#include "SRL.h"

int main()
{
	Builder query = SRL().literally("colo").optional((string)"u").literally("r").anyOf(
		[](Builder& builder) {
		builder.literally(":"). and (
			[](Builder& builder) {
			builder.literally(" is");
		});
	}).whitespace().capture(
		[](Builder& builder) {
		builder.letter().onceOrMore();
	}).literally(".");
		
    auto matches = query.getMatches("my favorite colour is green. And my favorite color: yellow.");
	
    bool match_1 = matches[0]["0"];
	bool match_2 = matches[1]["0"];  
  
	query = SRL("literally \"color:\", whitespace, capture (letter once or more), literally \".\", all");

	matches = query.getMatches("my favorite colour is green. And my favorite color: yellow.");
	
    bool match_3 = matches[0]["0"];
	bool match_4 = matches[1]["0"];

	cout << match_1 == match_3 << ": " << match_1 << endl; 
	cout << match_2 == match_4 << ": " << match_2 << endl; 

    return 0;
}
```

**Tested Enviroments:**
     
    Visual Studio 2015.
    Visual Studio 2017.

 **Unit Tests:**

	To run the unit tests, you should have [boost library](http://www.boost.org/) 
	installed in your workstation.
	
	For Visual Studio, set environment variable BOOST_ROOT to your boost root folder 
	(where the binary is).
	Also on 64 bit you might need to compile with additional command /bigobj.

You can also check out how to write Rules, see: [Test-Rules](https://github.com/SimpleRegex/Test-Rules).

###License

This "Software" is Licensed Under  **`MIT License (MIT)`** .

#### Tip Jar
* :dollar: **Bitcoin**: `1Mcci95WffSJnV6PsYG7KD1af1gDfUvLe6`


Conclusion
--------------------------------------------------

   Special thanks to [Simple Regex](https://simple-regex.com/) for making this library available to port from in the first place.
