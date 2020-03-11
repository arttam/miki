### Regular expressions

```cpp
#include <iostream>
#include <regex>
#include <string>

int main()
{
	using namespace std::literals::string_literals;
	std::regex pat{R"(\w{2}\s*\d{5}(-\d{4})?)"};
	for (const auto testCase : {"AA12345"s, "BB12345-7890"s}) {
		std::smatch matches;
		if (std::regex_search(testCase, matches, pat)) {
			std::cout << "Found match: " << matches[0] << std::endl;
			if (matches.size() > 1) {
				for (const auto& match : matches) {
					std::cout << "\t" << match << std::endl;
				}
			}
		}
	}

	return 0;
}
```

Gives
```bash
./strings
Found match: AA12345
	AA12345

Found match: BB12345-7890
	BB12345-7890
	-7890
```

#### Skip subpattern

```cpp
	std::regex noSkipPattern{R"((\s|:|,)*(\d*))"};
	std::regex skipPattern{R"((?:\s|:|,)*(\d*))"};

	for (const auto testCase : {":228"s, " 339"s}) {
		std::smatch matches;
		std::cout << "No skip first: ";
		if (std::regex_search(testCase, matches, noSkipPattern)) {
			std::copy(matches.begin(),
			          matches.end(),
			          std::ostream_iterator<std::string>(std::cout, ", "));
		}
		std::cout << "\nSame with skip subpattern: ";
		if (std::regex_search(testCase, matches, skipPattern)) {
			std::copy(matches.begin(),
			          matches.end(),
			          std::ostream_iterator<std::string>(std::cout, ", "));
		}
		std::cout << std::endl;
	}
```

Gives
```bash
./strings
No skip first: :228, :, 228,
Same with skip subpattern: :228, 228,
No skip first:  339,  , 339,
Same with skip subpattern:  339, 339,
```
