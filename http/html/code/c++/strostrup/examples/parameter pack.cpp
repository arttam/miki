#include <iostream>
#include <typeinfo>

void print()
{
	// End of tail, should do nothing
	std::cout << "\nNothing to print, end of packaged params" << std::endl;
}

template<typename T, typename... Tail>
void print(T head, Tail... tail)
{
	std::cout << "T: " << typeid(head).name() << ", V: " << head << "; ";
	print(tail...);
}

int main()
{
	print(1, 3.1415, "Some String", false);
	return 0;
}
