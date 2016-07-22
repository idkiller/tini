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

bool to_string_tc(const std::string &path)
{
  tini::ini ini;

  auto &global = ini[tini::global];
  global["global-value"] = "global value";
  auto &foobar = ini["Foobar"];
  foobar["foobar-name"] = "foobar";
  foobar["foobar-dir"] = "/foo/bar";
  foobar["foobar-integer"] = "12345";
  foobar["foobar-float"] = "0.123456";
  foobar["foobar-eqinstr"] = "foo=bar";
  foobar["foobar-eqnospace"] = "nospace";
  auto &foobar2 = ini["foobar-second"];
  foobar2["second group"] = "second";
  auto &foobar3 = ini["foobar-third"];


  std::ofstream ofs(path);
  ofs << ini;
  ofs.close();
  return true;
}

bool ctor_string_tc(const std::string &path)
{
  std::ifstream ifs(path);
  std::stringstream buf;
  buf << ifs.rdbuf();
  tini::ini ini(buf);

  cout << path << " is parsed..." << endl;

  bool passfail = true;

#define CHECK_VALUE(group, key, value) \
  if (ini[group][key] != value) {\
    cout << group << "." << key << " != " << value << endl;\
    passfail = false;\
  }\

  CHECK_VALUE(tini::global, "global-value", "global value")
  CHECK_VALUE("Foobar", "foobar-name", "foobar")
  CHECK_VALUE("Foobar", "foobar-integer", "12345")
  CHECK_VALUE("Foobar", "foobar-float", "0.123456")
  CHECK_VALUE("Foobar", "foobar-eqinstr", "foo=bar")
  CHECK_VALUE("Foobar", "foobar-eqnospace", "nospace")
  CHECK_VALUE("Foobar", "foobar-novalue", "")
  CHECK_VALUE("Foobar", "foobar-novaluenoeq", "")
  CHECK_VALUE("Foobar", "foobar-comment at last", "foobar")
  CHECK_VALUE("Foobar", "foobar-novalue comment", "")
  CHECK_VALUE("Foobar", "foobar-novaluenoeq", "")
  CHECK_VALUE("foobar-second", "second group", "second")
  CHECK_VALUE("foobar-fourth", "fourth value", "")

  if (passfail) PASS++;
  else FAIL++;

  return true;
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

  header_tc("", tini::global, tini::global, false);
  header_tc("[", tini::global, tini::global, false);
  header_tc("[foobar]", tini::global, "foobar", true);
  header_tc("[foobar", tini::global, tini::global, false);
  header_tc("foobar]", tini::global, tini::global, false);
  header_tc("foobar[foo]", tini::global, tini::global, false);
  header_tc("[]", "", "", false);
  header_tc("[]", tini::global, "", true);
  header_tc("[foobar]", "", "foobar", true);

  to_string_tc("out.ini");
  ctor_string_tc("test.ini");

  cout << "PASS : " << PASS << endl;
  cout << "FAIL : " << FAIL << endl;

  return 0;
}
