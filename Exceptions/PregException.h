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

#ifndef PREGEXCEPTION_H
#define PREGEXCEPTION_H

#include "SRLException.h"
#include <map>
#include "../Utils.h"

using namespace std;

namespace SRLExceptions
{

	enum PregExceptionEnum
	{
		InternalError,
		BacktrackLimitError,
		RecursionLimitError,
		BadUtf8Error,
		BadUtf8OffsetError,
		JitStacklimitError
	};

	class PregException : public SRLException
	{
	private:
		static const string PREG_INTERNAL_ERROR;
		static const string PREG_BACKTRACK_LIMIT_ERROR;
		static const string PREG_RECURSION_LIMIT_ERROR;
		static const string PREG_BAD_UTF8_ERROR;
		static const string PREG_BAD_UTF8_OFFSET_ERROR;
		static const string PREG_JIT_STACKLIMIT_ERROR;

		static map<PregExceptionEnum, string> messages;

	public:	

		PregException(PregExceptionEnum &type) : SRLException(messages[type])
		{}

	}; // end class

	const string PregException::PREG_INTERNAL_ERROR = "Internal PCRE error.";
	const string PregException::PREG_BACKTRACK_LIMIT_ERROR = "Backtrack limit exhausted.";
	const string PregException::PREG_RECURSION_LIMIT_ERROR = "Recursion limit exhausted.";
	const string PregException::PREG_BAD_UTF8_ERROR = "Malformed UTF-8 data.";
	const string PregException::PREG_BAD_UTF8_OFFSET_ERROR = "Offset did not correspond to the begin of a valid UTF-8 code point.";
	const string PregException::PREG_JIT_STACKLIMIT_ERROR = "PCRE function failed due to limited JIT stack space.";


	map<PregExceptionEnum, string> PregException::messages =
								{
									{ PregExceptionEnum::InternalError, PREG_INTERNAL_ERROR },
									{ PregExceptionEnum::BacktrackLimitError, PREG_BACKTRACK_LIMIT_ERROR },
									{ PregExceptionEnum::RecursionLimitError, PREG_RECURSION_LIMIT_ERROR },
									{ PregExceptionEnum::BadUtf8Error, PREG_BAD_UTF8_ERROR },
									{ PregExceptionEnum::BadUtf8OffsetError, PREG_BAD_UTF8_OFFSET_ERROR },
									{ PregExceptionEnum::JitStacklimitError, PREG_JIT_STACKLIMIT_ERROR }
								};

}

#endif // !PREGEXCEPTION_H