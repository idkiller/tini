#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <functional> 

#include <iostream>
namespace tini
{

const std::string GLOBAL = "___GLOBAL";

inline void ltrim(std::string& s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
}

inline void rtrim(std::string& s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

inline void trim(std::string& s)
{
  ltrim(s);
  rtrim(s);
}

inline int find_first_nonescaped(const std::string& str, char ch)
{
  if (!str.empty())
  {
    if (str[0] == ch) return 0;
    for (size_t i=1; i<str.length(); i++)
    {
      if (str[i] == ch && str[i-1] != '\\')
        return i;
    }
  }
  return -1;
}

inline std::string uncomment(const std::string& str)
{
  int comment_start = find_first_nonescaped(str, '#');
  if (comment_start < 0) return str;
  return str.substr(0, comment_start);
}

inline bool get_header(const std::string& str, std::string& header)
{
  if (str.length() < 2 || str.front() != '[' || str.back() != ']') return false;
  std::string expected = std::string(str.begin()+1, str.end()-1);
  trim(expected);
  if (expected.compare(header) != 0)
  {
    header = expected;
    return true;
  }
  return false;
}

inline std::pair<std::string, std::string> get_pair(const std::string& str)
{
  std::string key, value;
  std::string::size_type eq_pos = str.find('=');
  if (eq_pos == std::string::npos)
  {
    key = str;
  }
  else
  {
    key = str.substr(0, eq_pos);
    value = str.substr(eq_pos+1);
    trim(value);
  }
  trim(key);
  return std::pair<std::string, std::string>(key, value);
}

class ini
{
  public:
    ini(const std::string& str)
    {
      std::istringstream input(str);
      init(input);
    }
    ini(std::istream &input)
    {
      init(input);
    }
    inline void init (std::istream &input)
    {
      std::string line;
      std::string title(GLOBAL);
      while (std::getline(input, line))
      {
        line = uncomment(line);
        if (line.empty()) continue;
        trim(line);
        if (get_header(line, title) && Groups.find(title) == Groups.end())
        {
          Groups[title] = std::map<std::string, std::string>();
          continue;
        }

        auto pair = get_pair(line);
        Groups[title][pair.first] = pair.second;
      }
    }
    std::string to_string() const
    {
      std::stringstream str_out;
      for (auto &groups : Groups)
      {
        const std::string &title = groups.first;
        auto &pairs = groups.second;
        str_out << '[' << title << ']' << std::endl;
        for (auto &pair : pairs)
        {
          str_out << pair.first << " = " << pair.second << std::endl;
        }
      }

      return str_out.str();
    }
    std::map<std::string, std::map<std::string, std::string> > Groups;
};

std::ostream& operator << (std::ostream& os, const ini& rhs)
{
  os << rhs.to_string();
  return os;
}

}  // namespace tini
