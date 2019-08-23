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

#ifndef BUILDEREXCEPTION_H
#define BUILDEREXCEPTION_H

#include "SRLException.h"

namespace SRLExceptions
{
	class BuilderException : public SRLException
	{
	public:
		BuilderException(const string &message) : SRLException(message) {}
	}; //
}

#endif // !BUILDEREXCEPTION_H
