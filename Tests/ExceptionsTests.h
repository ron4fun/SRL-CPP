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

// ====================== ExceptionsTestCase ======================
////////////////////
// ExceptionsTestCase
///////////////////
BOOST_AUTO_TEST_SUITE(ExceptionsTests)

BOOST_AUTO_TEST_CASE(RawMethod)
{
	Builder query = SRL("Literally \"a\"");

	BOOST_CHECK_THROW(query.raw(")"), BuilderException);
	BOOST_CHECK(query.isExactMatch("a"));
}

BOOST_AUTO_TEST_CASE(LazyMethod)
{
	Builder query = SRL("Literally \"a\"");

	BOOST_CHECK_THROW(query.lazy(), ImplementationException);
}

BOOST_AUTO_TEST_SUITE_END()