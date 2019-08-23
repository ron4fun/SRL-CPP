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

#ifndef INTERPRETEREXCEPTION_H
#define INTERPRETEREXCEPTION_H

#include "SRLException.h"

namespace SRLExceptions
{
	class InterpreterException : public SRLException
	{
	public:
		InterpreterException(const string &message) : SRLException(message) {}
	}; //
}

#endif // !INTERPRETEREXCEPTION_H