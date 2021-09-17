// cpwd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
#include <nowide/iostream.hpp>
#include <nowide/args.hpp>
#include <filesystem>
#include <Windows.h>
#include <type_traits>

int main(int argc, char** argv)
{
	nowide::args a(argc, argv);

	std::error_code ec;
	auto cwd = std::filesystem::current_path(ec);
	const std::wstring cwd_wstr = cwd.native();
	if (ec) {
		nowide::cerr << "error!\n";
		return 1;
	}

	const HWND console_window = GetConsoleWindow();
	static_assert(static_cast<HWND>(NULL) == nullptr);

	if (console_window == nullptr) {
		nowide::cerr << "cannot get ConsoleWindow.\n";
		return 1;
	}

	if (!OpenClipboard(console_window)) {
		nowide::cerr << "cannot open Clipboard.\n";
		return 1;
	}

	if (!EmptyClipboard()) {
		(void)CloseClipboard();
		nowide::cerr << "Clipboard could not be emptied.\n";
		return 1;
	}

	//CF_UNICODETEXT

	static_assert(std::is_same_v<SIZE_T, std::size_t>);
	static_assert(std::is_same_v<decltype(cwd_wstr.size()), std::size_t>);
	static_assert(std::is_same_v<decltype(cwd_wstr.size()), SIZE_T>);
	
	std::size_t mem_size = (cwd_wstr.size() + 1) * sizeof(decltype(cwd_wstr)::value_type); // plus one for L'\0' NUL character
	HGLOBAL mem_handle = GlobalAlloc(GMEM_MOVEABLE, mem_size);
	static_assert(static_cast<HGLOBAL>(NULL) == nullptr);
	if (mem_handle == nullptr) {
		(void)CloseClipboard();
		nowide::cerr << "Global memory could not be allocated.\n";
		return 1;
	}

	auto mem_ptr = GlobalLock(mem_handle);
	static_assert(static_cast<decltype(mem_ptr)>(NULL) == nullptr);
	if (mem_ptr == nullptr) {
		(void)GlobalFree(mem_handle);
		(void)CloseClipboard();
		nowide::cerr << "Global memory could not be accessed.\n";
		return 1;
	}
	auto wchar_ptr = cwd_wstr.c_str();
	memcpy(mem_ptr, wchar_ptr, mem_size);

	if (GlobalUnlock(mem_handle)!=0 || GetLastError()!=NO_ERROR) {
		(void)CloseClipboard();
		nowide::cerr << "Global memory could not be unlocked.\n";
		return 1;
	}


	if (SetClipboardData(CF_UNICODETEXT, mem_handle) == nullptr) {
		(void)GlobalFree(mem_handle);
		(void)CloseClipboard();
		nowide::cerr << "Clipboard content could not be set.\n";
		return 1;
	}

	if (!CloseClipboard()) {
		nowide::cerr << "Clipboard could not be closed.\n";
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
