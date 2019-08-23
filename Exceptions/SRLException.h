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

#ifndef SRLEXCEPTION_H
#define SRLEXCEPTION_H


#include <string>
#include <exception>

using namespace std;

namespace SRLExceptions
{
	class SRLException : public exception
	{
	protected:
		SRLException() { }

		SRLException(const string &message) : exception(message.c_str()) { }

		//SRLException(const string &message, exception &innerException) : exception(message, innerException) { }

	}; // end class SRLException
}


#endif // !SRLEXCEPTION_H