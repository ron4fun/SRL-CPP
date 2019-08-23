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

#include <filesystem>
#include <sstream>
#include <iostream>

#include "../SRL.h"

namespace fs = std::experimental::filesystem;

// ====================== RuleTestCase ======================
////////////////////
// RuleTestCase
///////////////////
BOOST_AUTO_TEST_SUITE(RuleTests)

struct data {
	string srl;
	vector<string> matches;
	vector<string> no_matches;
	map<string, vector<map<string, string>>> captures;
};

string applySpecialChars(const string &target) {
	return Utils::replace(Utils::replace(target, R"(\\n)", R"(\n)"), R"(\\t)", R"(\t)");
} // end function applySpecialChars

string getExpression(const string &srl, Builder &query) {
	return string("\n\nSupplied SRL Query : ") + srl + "\nGenerated Expression : " + query.getRawRegex() + "\n\n";
} // end function getExpression

data buildData(const vector<string> &lines)
{
	data result;

	VariableObject inCapture = false;

	for (register size_t i = 0; i < lines.size(); i++)
	{
		string line = lines[i];

		if (line == "" || Utils::startsWith(line, "#")) {
			continue;
		} // end if

		if (inCapture && !Utils::startsWith(line, "-")) {
			inCapture = false;
		} // end if

		if (Utils::startsWith(line, "srl: ")) {
			result.srl = Utils::substr(line, 5);
		} // end if
		else if (Utils::startsWith(line, "match: \"")) {
			result.matches.push_back(applySpecialChars(Utils::slice(line, 8, -1)));
		} // end else if
		else if (Utils::startsWith(line, "no match: \"")) {
			result.no_matches.push_back(applySpecialChars(Utils::slice(line, 11, -1)));
		} // end else if
		else if (Utils::startsWith(line, "capture for \"") && Utils::substr(line, -2, 2) == "\":") {
			inCapture = Utils::slice(line, 13, -2);
			result.captures.insert({inCapture.toString(), vector<map<string, string>>()});
		} // end else if
		else if (inCapture && Utils::startsWith(line, "-")) {
			vector<string> split = Utils::split(Utils::substr(line, 1), ": ");
			map<string, string> target;

			size_t index = atoi(split[0].c_str());
			if (result.captures.find(inCapture.toString().c_str())->second.size() > index)
				target = (result.captures.find(inCapture.toString().c_str())->second)[index];
			
			if (target.empty()) {
				(result.captures.find(inCapture.toString().c_str())->second).push_back(map<string, string>());
				//target = (result.captures.find(inCapture.toString().c_str())->second)[index];
			} // end if

			(result.captures.find(inCapture.toString().c_str())->second)[index][split[1]] = applySpecialChars(Utils::slice(split[2], 1, -1));
		} // end else if
	} // end for

	return result;
} // end function buildData

string wchar_t2string(const wchar_t *wchar)
{
	string result = "";
	size_t index = 0;
	while (wchar[index] != 0)
	{
		result += (char)wchar[index];
		++index;
	} // end while

	return result;
} // end function wchar_t2string

vector<string> getFileContents(const string &path)
{
	vector<string> result;
	string line;

	std::ifstream infile(path);

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		result.push_back(iss.str());
	} // end while

	return result;
} // end function getFileContents

void runAssertions(const data &dt)
{
	BOOST_CHECK_MESSAGE(!dt.srl.empty(), "SRL for rule is empty. Invalid rule.");

	Builder query;
	bool assertionMade = false;

	try {
		query = SRL(dt.srl);
	} // end try
	catch (const exception e) {
		BOOST_CHECK_MESSAGE(false, string("Parser error: ") + e.what() + "\n\nSupplied SRL Query : " + dt.srl + "\n\n");
	} // end catch

	for (register size_t i = 0; i < dt.matches.size(); i++)
	{
		string match = dt.matches[i];
		BOOST_CHECK_MESSAGE(query.isMatching(match),
			string("Failed asserting that this query matches '") + match + "'." + getExpression(dt.srl, query));
			
		assertionMade = true;
	} // end for

	for (register size_t i = 0; i < dt.no_matches.size(); i++)
	{
		string noMatch = dt.no_matches[i];
		BOOST_CHECK_MESSAGE(!query.isMatching(noMatch),
			string("Failed asserting that this query does not match '") + noMatch + "'." + getExpression(dt.srl, query));

		assertionMade = true;
	} // end for

	for (auto &capture : dt.captures)
	{
		string test = capture.first;
		vector<Dictionary> expected = capture.second;
		vector<Dictionary> matches;

		try {
			matches = query.getMatches(test);
		} // end try
		catch (const exception e) {
			BOOST_CHECK_MESSAGE(false,
				string("Parser error: ") + e.what() + "." + getExpression(dt.srl, query));
		} // end catch

		BOOST_CHECK_MESSAGE(matches.size() == expected.size(),
			string("Invalid match count for test ") + test + "." + getExpression(dt.srl, query));
			
		char buf[50];
		for (register size_t i = 0; i < matches.size(); i++)
		{
			auto item = expected[i];
			auto capture = matches[i];

			for (auto &key : item) {
				if (Utils::isDigit(key.first)) {
					itoa(stoi(key.first), buf, 10);
					BOOST_CHECK_MESSAGE(capture.find(buf)->second == key.second,
						string("The capture group did not return the expected results for test ") + test + "." + getExpression(dt.srl, query));

				} // end if
				else
					BOOST_CHECK_MESSAGE(capture.find(key.first)->second == key.second,
						string("The capture group did not return the expected results for test ") + test + "." + getExpression(dt.srl, query));
								
			} // end for

		} // end for

		assertionMade = true;
	} // end for

	BOOST_CHECK_MESSAGE(assertionMade, string("No assertion.Invalid rule.") + getExpression(dt.srl, query));
} // end function runAssertions

void testRules(const string &dir_path, const string &ext = ".*")
{
	for (auto &p : fs::directory_iterator(dir_path))
	{
		if (p.path().extension() == ext) {
			std::cout << ">>> Testing file: " << wchar_t2string(p.path().filename().c_str()) << std::endl;
			vector<string> lines = getFileContents(wchar_t2string(p.path().c_str()));
			runAssertions(buildData(lines));
		} // end if
	} // end for
} // end function testRules

BOOST_AUTO_TEST_CASE(FullTests)
{
	testRules("Tests\\Rules\\", ".rule");
}

BOOST_AUTO_TEST_SUITE_END()