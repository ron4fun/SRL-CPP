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

#ifndef LITERALLY_H
#define LITERALLY_H

#include <string>

using namespace std;

/**
* Wrapper for literal strings that should not be split, tainted or interpreted in any way.
*/
class Literally
{
public:
	Literally(const string &str)
	{
		string temp;
		char find = '\\';
		for (auto s : str) {
			if (s != find) {
				temp += s;
			} // end if
		} // end for

		_string = temp;
	} // end cctr

	/**
	* @return  {string}
	*/
	string toString() {
		return _string;
	} // end function toString

private:
	string _string;

}; // end class Literally


#endif // !LITERALLY_H
