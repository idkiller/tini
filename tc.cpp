#include "tini.hpp"
#include <string>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::string;

static int PASS, FAIL;

bool find_first_nonescaped_tc(const string& str, char ch, int expected)
{
  int result = tini::find_first_nonescaped(str, ch);
  cout << "call find_first_nonescaped with..." << endl;
  cout << "str    : " << str << endl;
  cout << "ch     : " << ch << endl;
  cout << "expect : " << expected << endl;
  cout << "result : " << result << endl;
  cout << "PASSED : " << (result == expected ? "PASS" : "FAIL") << endl;
  cout << endl;
  
  if (result == expected) PASS++;
  else FAIL++;

  return result == expected;
}

bool uncomment_tc(const string& str, const string& expected)
{
  std::string result = tini::uncomment(str);
  cout << "call uncomment with..." << endl;
  cout << "str    : " << str << endl;
  cout << "expect : " << expected << endl;
  cout << "result : " << result << endl;
  cout << "PASSED : " << (result == expected ? "PASS" : "FAIL") << endl;
  cout << endl;
  
  if (result == expected) PASS++;
  else FAIL++;

  return result == expected;
}

bool header_tc(const string& str, const string& init, const string& expected, bool expected_result)
{
  string header = init;
  bool result = tini::get_header(str, header);
  bool passfail = result == expected_result && expected == header;
  cout << "call header with..." << endl;
  cout << "str    : " << str << endl;
  cout << "expect : " << expected << endl;
  cout << "header : " << header << endl;
  cout << "result : " << result << endl;
  cout << "PASSED : " << (passfail ? "PASS" : "FAIL") << endl;
  cout << endl;
  
  if (passfail) PASS++;
  else FAIL++;

  return passfail;
}

bool ctor_string_tc(const std::string &path)
{
  std::ifstream ifs(path);
  std::stringstream buf;
  buf << ifs.rdbuf();
  tini::ini ini(buf);

  cout << "<" << path << ">" << std::endl;
  cout << ini << std::endl;

  return false;
}

int main()
{
  find_first_nonescaped_tc("", '#', -1);
  find_first_nonescaped_tc("#", '#', 0);
  find_first_nonescaped_tc("#foobar", '#', 0);
  find_first_nonescaped_tc("foobar#foo", '#', 6);
  find_first_nonescaped_tc("foobar#", '#', 6);
  find_first_nonescaped_tc("##foobar", '#', 0);
  find_first_nonescaped_tc("foobar##foo", '#', 6);
  find_first_nonescaped_tc("foobar##", '#', 6);

  uncomment_tc("", ""); 
  uncomment_tc("#", "");
  uncomment_tc("#foobar", "");
  uncomment_tc("foobar#foo", "foobar");
  uncomment_tc("foobar#", "foobar");
  uncomment_tc("##foobar", "");
  uncomment_tc("foobar##foo", "foobar"); 
  uncomment_tc("foobar##", "foobar");

  header_tc("", tini::GLOBAL, tini::GLOBAL, false);
  header_tc("[", tini::GLOBAL, tini::GLOBAL, false);
  header_tc("[foobar]", tini::GLOBAL, "foobar", true);
  header_tc("[foobar", tini::GLOBAL, tini::GLOBAL, false);
  header_tc("foobar]", tini::GLOBAL, tini::GLOBAL, false);
  header_tc("foobar[foo]", tini::GLOBAL, tini::GLOBAL, false);
  header_tc("[]", "", "", false);
  header_tc("[]", tini::GLOBAL, "", true);
  header_tc("[foobar]", "", "foobar", true);

  ctor_string_tc("test.ini");

  cout << "PASS : " << PASS << endl;
  cout << "FAIL : " << FAIL << endl;

  return 0;
}
