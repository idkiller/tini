# tini
c++11 tiny ini subset parser in one header.

## example code

write ini
```c++
tini::ini ini;

auto &global = ini[tini::global];
global["global-value"] = "global value";
auto &foobar = ini["Foobar"];
foobar["foobar-name"] = "foobar";
foobar["foobar-dir"] = "/foo/bar";
foobar["foobar-integer"] = "12345";

std::ofstream ofs("foobar.ini");
ofs << ini;
ofs.close();
```

read ini
```c++
std::ifstream ifs("foobar.ini");
std::stringstream buf;
buf << ifs.rdbuf();
tini::ini ini(buf);

std::cout << ini["Foobar"]["foobar-name"] << std::endl;
```

