// cpwd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
#include <nowide/iostream.hpp>
#include <nowide/args.hpp>
#include <filesystem>

int main(int argc, char** argv)
{
	nowide::args a(argc, argv);
	//https://en.cppreference.com/w/cpp/filesystem/current_path
	std::error_code ec;
	auto cwd = std::filesystem::current_path(ec);
	std::wstring cwd_wstr = cwd.native();
	if (ec) {
		nowide::cerr << "error!\n";
		return 1;
	}

	nowide::cerr << u8"The current path is:\n" << std::flush;
	nowide::cout << cwd.u8string() << u8"\n" << std::flush;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
