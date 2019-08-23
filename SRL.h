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

#ifndef SRL_H
#define SRL_H

#include <string>
#include "Builder.h"
#include "Language\Interpreter.h"

using namespace std;

typedef SRLBuilder::Builder Builder;

/**
* SRL facade for SRL Builder and SRL Language.
*
* @param  {string} query
* @return {Builder}
*/
Builder SRL(const string &query="") 
{
	if (!query.empty()) return Interpreter(query).getBuilder();
	return Builder();
} // end function SRL


#endif // !SRL_H