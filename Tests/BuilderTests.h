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

#include <iostream>
#include <ostream>
#include "../SRL.h"

// ====================== BuilderTestCase ======================
////////////////////
// BuilderTestCase
///////////////////
BOOST_AUTO_TEST_SUITE(BuilderTests)

BOOST_AUTO_TEST_CASE(SimplePhoneNumberFormat)
{
	Builder regex = SRL().startsWith().literally("+").digit().between(1, 3).literally(" ")
		.digit().between(3, 4).literally("-").digit().onceOrMore().mustEnd();

	BOOST_CHECK(regex.isMatching("+49 123-45"));
	BOOST_CHECK(regex.isMatching("+492 1235-4"));
	BOOST_CHECK(!regex.isMatching("+49 123 45"));
	BOOST_CHECK(!regex.isMatching("49 123-45"));
	BOOST_CHECK(!regex.isMatching("a+49 123-45"));
	BOOST_CHECK(!regex.isMatching("+49 123-45b"));
}

BOOST_AUTO_TEST_CASE(SimpleEmailFormat)
{
	Builder regex = SRL().startsWith().anyOf(
		[](Builder& builder) {
			builder.digit().letter().oneOf("._%+-");
		}).onceOrMore().literally("@").anyOf(
		[](Builder& builder) {
			builder.digit().letter().oneOf(".-");
		}).onceOrMore().literally(".").letter().atLeast(2)
			.mustEnd().caseInsensitive();

	BOOST_CHECK(regex.getMatch("sample@example.com")[0] == "sample@example.com");
	BOOST_CHECK(regex.getMatch("super-He4vy.add+ress@top-Le.ve1.domains")[0] == "super-He4vy.add+ress@top-Le.ve1.domains");
	BOOST_CHECK(!regex.isMatching("sample.example.com"));
	BOOST_CHECK(!regex.isMatching("missing@tld"));
	BOOST_CHECK(!regex.isMatching("hav ing@spac.es"));
	BOOST_CHECK(!regex.isMatching("no@pe.123"));
	BOOST_CHECK(!regex.isMatching("invalid@email.com123"));
}

BOOST_AUTO_TEST_CASE(CaptureGroup)
{
	Builder regex = SRL().literally("colo").optional((string)"u").literally("r").anyOf(
		[](Builder& builder) {
		builder.literally(":"). and (
			[](Builder& builder) {
			builder.literally(" is");
		});
	}).whitespace().capture(
		[](Builder& builder) {
		builder.letter().onceOrMore();
	}).literally(".");

	BOOST_CHECK(regex.isMatching("my favorite color: blue."));
	BOOST_CHECK(regex.isMatching("my favorite colour is green."));
	BOOST_CHECK(!regex.isMatching("my favorite colour is green!"));

	const string testcase = "my favorite colour is green. And my favorite color: yellow.";
	auto matches = regex.getMatch(testcase);
	BOOST_CHECK(matches[1] == "green");
}

BOOST_AUTO_TEST_CASE(MoreMethods)
{
	Builder regex = SRL().noWhitespace().literally("a").ifFollowedBy(
		[](Builder& builder) {
			builder.noCharacter();
	}).tab().mustEnd();

	string target = R"(
        ba	
        aaabbb
        )";
	
	BOOST_CHECK(regex.isMatching(target));

	Builder regex2 = SRL().startsWith().literally("a").newLine().whitespace().onceOrMore().literally("b").mustEnd();

	string target2 = R"(a
        b)";
			
	BOOST_CHECK(regex2.isMatching(target2));
}

BOOST_AUTO_TEST_CASE(Replace)
{
	Builder regex = SRL().capture(
		[](Builder& builder) {
		builder.anyCharacter().onceOrMore();
	}).whitespace().capture(
		[](Builder& builder) {
		builder.digit().onceOrMore();
	}).literally(", ").capture(
		[](Builder& builder) {
		builder.digit().onceOrMore();
	}).caseInsensitive();

	BOOST_CHECK(regex.replace("April 15, 2003", "$1 1, $3") == "April 1, 2003");
}

BOOST_AUTO_TEST_CASE(Lazyness)
{
	Builder regex = SRL().capture(
		[](Builder& builder) {
		builder.literally(",").twice().whitespace().optional().lazy();
	});

	auto matches = regex.getMatch(",, ");
	BOOST_CHECK(matches[1] == ",,");
	BOOST_CHECK(matches[1] != ",, ");
	
	regex = SRL().literally(",").atLeast(1).lazy();

	matches = regex.getMatch(",,,,,");
	BOOST_CHECK(matches[0] == ",");
	BOOST_CHECK(matches[0] != ",,,,,");
}

BOOST_AUTO_TEST_CASE(Raw)
{
	Builder regex = SRL().literally("foo").raw("b[a-z]r").raw(R"(\d+)");

	BOOST_CHECK(regex.isExactMatch("foobzr123"));
	BOOST_CHECK(regex.isExactMatch("foobar1"));
	BOOST_CHECK(!regex.isExactMatch("fooa"));
	BOOST_CHECK(!regex.isExactMatch("foobar"));
}

BOOST_AUTO_TEST_CASE(RemoveModifier)
{
	Builder regex = SRL().literally("foO").caseInsensitive();

	BOOST_CHECK(regex.isMatching("FOO"));

	regex.removeModifier(regex_constants::icase);

	BOOST_CHECK(regex.isMatching("foO"));
}

BOOST_AUTO_TEST_SUITE_END()