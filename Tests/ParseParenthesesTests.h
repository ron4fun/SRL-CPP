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

#include "../Language/Helpers/parseParentheses.h"


// ====================== DefaultTestCase ======================
////////////////////
// DefaultTestCase
///////////////////
BOOST_AUTO_TEST_SUITE(ParseParenthesesTests)

BOOST_AUTO_TEST_CASE(Default)
{
	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo (bar) baz"), MixedArray({ "foo", MixedArray({ "bar" }), "baz" })));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("(foo (bar) baz)"), MixedArray({"foo", MixedArray({ "bar" }), "baz"})));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo (bar)"), {"foo", MixedArray({ "bar" })}));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("(foo)bar"), { MixedArray({"foo"}), "bar"}));
	
	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo (0)"), {"foo", MixedArray({ "0" })}));
	
	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo (bar (nested)) baz"), { "foo", MixedArray({"bar", MixedArray({"nested"})}), "baz" }));
	
	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo boo (bar (nested) something) baz (bar (foo foo))"),
	{ "foo boo", MixedArray({"bar", MixedArray({"nested"}), "something"}), "baz", MixedArray({"bar", MixedArray({"foo foo"})}) }));

}

BOOST_AUTO_TEST_CASE(Escaping)
{
	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo (bar \"(bla)\") baz"), { "foo", MixedArray({"bar", Literally("(bla)")}), "baz" }));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("sample \"foo\" bar"), { "sample", Literally("foo"), "bar" }));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("sample \"foo\""), { "sample", Literally("foo") }));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("bar \"(b\\\"la)\" baz"), {"bar", Literally("(b\\\"la)"), "baz"}));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo \"ba'r\" baz"), {"foo", Literally("ba'r"), "baz"}));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo (bar \"(b\\'la)\") baz"), 
	{ "foo", MixedArray({"bar", Literally("(b\\'la)")}), "baz" }));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("bar \"b\\\\\" (la) baz"), { "bar", Literally("b\\\\"), MixedArray({"la"}), "baz" }));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("\"fizz\" and \"buzz\" (with) \"bar\""),
	{ Literally("fizz"), "and", Literally("buzz"), MixedArray({"with"}), Literally("bar") }));

	BOOST_CHECK(vUtils::isEqual(parseParentheses("foo \\'boo (bar (nes\"ted) s\\'om\\'\")ething) baz (bar (foo foo))"),
	{ "foo \\'boo", MixedArray({"bar", MixedArray({"nes", Literally("ted) s\\'om\\'")}), "ething"}), "baz", MixedArray({"bar", MixedArray({"foo foo"})}) }));

}

BOOST_AUTO_TEST_CASE(Empty)
{
	BOOST_CHECK(vUtils::isEqual(parseParentheses(""), {}));
}

BOOST_AUTO_TEST_SUITE_END()
