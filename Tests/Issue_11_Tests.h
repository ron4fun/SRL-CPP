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

// ====================== Issue11TestCase ======================
////////////////////
// Issue11TestCase
///////////////////
BOOST_AUTO_TEST_SUITE(Issue11Tests)

BOOST_AUTO_TEST_CASE(NumericalQuantifiesAndNonCapturingGroup)
{
	Builder query = SRL("digit, exactly 5 times, (letter, twice) optional");
	BOOST_CHECK(query.isMatching("12345"));
	BOOST_CHECK(query.isMatching("12345aa"));
}

BOOST_AUTO_TEST_CASE(ComplicatedCase)
{
	Builder query = SRL("begin with, digit, exactly 5 times, ( literally '-', digit, exactly 4 times ), optional, must end");
	BOOST_CHECK(query.isMatching("12345-1234"));
}

BOOST_AUTO_TEST_SUITE_END()