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

#ifndef NEGATIVELOOKAHEAD_H
#define NEGATIVELOOKAHEAD_H

#include "../Builder.h"

typedef SRLBuilder::Builder Builder;

class NegativeLookahead : public Builder
{
public:
	NegativeLookahead()
	{
		/** @var {string} _group Desired match group. */
		_group = "(?!%s)";
	} // end cctr
};


#endif // !NEGATIVELOOKAHEAD_H

