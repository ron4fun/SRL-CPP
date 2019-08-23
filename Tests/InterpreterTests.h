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

#include <boost/test/included/unit_test.hpp>

#include "../SRL.h"

// ====================== InterpreterTestCase ======================
////////////////////
// InterpreterTestCase
///////////////////
BOOST_AUTO_TEST_SUITE(InterpreterTests)

BOOST_AUTO_TEST_CASE(Parser)
{
	Builder query;

	query = SRL("aNy Character ONCE or more literAlly \"fO/o\"");
	BOOST_CHECK(query.getRawRegex() == "\\w+(?:fO/o)");

	query = SRL("begin with literally \"http\", optional \"s\", literally \"://\", optional \"www.\", anything once or more, literally \".com\", must end");
	BOOST_CHECK(query.getRawRegex() == "^(?:http)(?:(?:s))?(?:://)(?:(?:www\\.))?.+(?:\\.com)$");
	BOOST_CHECK(query.isMatching("http://www.ebay.com"));
	BOOST_CHECK(query.isMatching("https://google.com"));
	BOOST_CHECK(!query.isMatching("htt://google.com"));
	BOOST_CHECK(!query.isMatching("http://.com"));

	query = SRL("begin with capture (digit from 0 to 8 once or more) if followed by \"foo\"");
	BOOST_CHECK(query.getRawRegex() == "^([0-8]+)(?=(?:foo))");
	BOOST_CHECK(query.isMatching("142foo"));
	BOOST_CHECK(!query.isMatching("149foo"));
	BOOST_CHECK(!query.isMatching("14bar"));
	BOOST_CHECK(query.getMatch("142foo")[1] == "142");

	query = SRL("literally \"colo\", optional \"u\", literally \"r\"");
	BOOST_CHECK(query.isMatching("color"));
	BOOST_CHECK(query.isMatching("colour"));

	query = SRL("starts with number from 0 to 5 between 3 and 5 times, must end");
	BOOST_CHECK(query.isMatching("015"));
	BOOST_CHECK(!query.isMatching("444444"));
	BOOST_CHECK(!query.isMatching("1"));
	BOOST_CHECK(!query.isMatching("563"));

	query = SRL("starts with digit exactly 2 times, letter at least 3 time");
	BOOST_CHECK(query.getRawRegex() == "^[0-9]{2}[a-z]{3,}");
	BOOST_CHECK(query.isMatching("12abc"));
	BOOST_CHECK(query.isMatching("12abcd"));
	BOOST_CHECK(!query.isMatching("123abc"));
	BOOST_CHECK(!query.isMatching("1a"));
	BOOST_CHECK(!query.isMatching(""));
}

BOOST_AUTO_TEST_CASE(Email)
{
	Builder query = SRL(R"(begin with any of (digit, letter, one of "._%+-") once or more, literally "@", either of(digit, letter, one of ".-") once or more, literally ".", letter at least 2, must end, case insensitive)");
	BOOST_CHECK(query.isMatching("sample@example.com"));
	BOOST_CHECK(query.isMatching("super-He4vy.add+ress@top-Le.ve1.domains"));
	BOOST_CHECK(!query.isMatching("sample.example.com"));
	BOOST_CHECK(!query.isMatching("missing@tld"));
	BOOST_CHECK(!query.isMatching("hav ing@spac.es"));
	BOOST_CHECK(!query.isMatching("no@pe.123"));
	BOOST_CHECK(!query.isMatching("invalid@email.com123"));
}

BOOST_AUTO_TEST_CASE(CaptureGroup)
{
	Builder query = SRL("literally \"color:\", whitespace, capture (letter once or more), literally \".\", all");

	const string target = "Favorite color: green. Another color: yellow.";

	auto matches = query.getMatches(target);

	BOOST_CHECK(matches[0]["0"] == "green");
	BOOST_CHECK(matches[1]["0"] == "yellow");
}

BOOST_AUTO_TEST_CASE(Parentheses)
{
	Builder query;

	query = SRL("begin with (literally \"foo\", literally \"bar\") twice must end");
	BOOST_CHECK(query.getRawRegex() == "^(?:(?:foo)(?:bar)){2}$");
	BOOST_CHECK(query.isMatching("foobarfoobar"));
	BOOST_CHECK(!query.isMatching("foobar"));

	query = SRL("begin with literally \"bar\", (literally \"foo\", literally \"bar\") twice must end");
	BOOST_CHECK(query.getRawRegex() == "^(?:bar)(?:(?:foo)(?:bar)){2}$");
	BOOST_CHECK(query.isMatching("barfoobarfoobar"));

	query = SRL("(literally \"foo\") twice");
	BOOST_CHECK(query.getRawRegex() == "(?:(?:foo)){2}");
	BOOST_CHECK(query.isMatching("foofoo"));
	BOOST_CHECK(!query.isMatching("foo"));
}

BOOST_AUTO_TEST_SUITE_END()