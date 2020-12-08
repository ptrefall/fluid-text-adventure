#pragma once

class Console final
{
	static	WORD _wStartConsoleAttributes;
	static WORD getCurrentAttributes()
	{
		CONSOLE_SCREEN_BUFFER_INFO info;
		if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		{
			return 0;
		}
		return  info.wAttributes;
	}
public:
	static bool Init()
	{
		_wStartConsoleAttributes = getCurrentAttributes();
		return (_wStartConsoleAttributes != 0);
	}
	static void Reset()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _wStartConsoleAttributes);
	}
	static void ForegroundColor(FluidHTN::ConsoleColor color)
	{
		WORD currAttr = getCurrentAttributes();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), currAttr | (WORD)color);
	}
	static void Write(const std::string& msg)
	{
		std::cout << msg;
	}
	static void WriteLine(const std::string& line = ""s)
	{
		std::cout << line << std::endl;
	}
	static std::string ReadLine()
	{
		std::string line;
		std::getline(std::cin, line);
		return line;
	}
	static char ReadKey()
	{ 
		return ::getchar();
	}
	static void Clear()
	{
		COORD coordScreen = { 0, 0 };    // home for the cursor
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD dwConSize;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// Get the number of character cells in the current buffer.
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		{
			return;
		}

		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		// Fill the entire screen with blanks.
		if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
			(TCHAR)' ',      // Character to write to the buffer
			dwConSize,       // Number of cells to write
			coordScreen,     // Coordinates of first cell
			&cCharsWritten)) // Receive number of characters written
		{
			return;
		}

		// Get the current text attribute.
		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		{
			return;
		}

		// Set the buffer's attributes accordingly.
		if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
			csbi.wAttributes, // Character attributes to use
			dwConSize,        // Number of cells to set attribute
			coordScreen,      // Coordinates of first cell
			&cCharsWritten))  // Receive number of characters written
		{
			return;
		}

		// Put the cursor at its home coordinates.
		SetConsoleCursorPosition(hConsole, coordScreen);
	}

};
