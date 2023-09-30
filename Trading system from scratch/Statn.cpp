# include <iostream>
# include <string>

int main()
{
	// Captured variable
	std::string cVar("Hello");

	// Stored lambda function, with captured variable
	auto hello = [&cVar](const std::string& s)
	{ // Return type auitomatically deduced 

		std::cout << cVar << " " << s << '\n';
	};
	// Call the stored lambda function
	hello (std::string("C"));
	hello (std::string("C++"));

	return 0;
}