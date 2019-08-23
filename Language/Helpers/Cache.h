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

#ifndef CACHE_H
#define CACHE_H


#include "../../Builder.h"

typedef SRLBuilder::Builder Builder;

/**
* Temporary cache for already built SRL queries to speed up loops.
*/
class Cache
{
public:
	/**
	* Set Builder for SRL to cache.
	*
	* @param  {string} query
	* @param  {Builder} builder
	*/
	static void set(const string &query, Builder &builder) {
		_cache[query] = builder;
	} // end function set

	/**
	* Get SRL from cache, or return new Builder.
	*
	* @param  {string} query
	* @return {Builder}
	*/
	static Builder get(const string &query) {
		if (_cache.find(query) != _cache.end()) return _cache[query];
		return Builder();
	} // end function get

	/**
	* Validate if current SRL is a already in cache.
	*
	* @param  {string} query
	* @return {boolean}
	*/
	static bool	has(const string &query) {
		if (_cache.find(query) != _cache.end()) return true;
		return false;
	} // end function has

private:
	static map<string, Builder> _cache;

}; // end class Cache

map<string, Builder> Cache::_cache = {};

#endif // !CACHE_H
