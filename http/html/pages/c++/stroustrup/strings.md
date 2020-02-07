### Strings

#### Using literals:
```cpp
#include <string>

int main()
{
	using namespace std::literals::string_literals;

	auto s = "SomeString"s;
	std::cout << "String length: " << s.length() << ", value: " << s << std::endl;

	return 0;
}
```

#### Same for string_view

```cpp
    using namespace std::literals::string_view_literals;
    auto sv = "This Is StringView"sv;
```