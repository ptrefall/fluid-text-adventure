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
	static WORD convertColorToWin32(FluidHTN::ConsoleColor color)
	{
        switch (color)
        {
            case FluidHTN::ConsoleColor::Black:
                return 0;
            case FluidHTN::ConsoleColor::White:
                return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
            case FluidHTN::ConsoleColor::Blue:
                return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case FluidHTN::ConsoleColor::DarkBlue:
                return FOREGROUND_BLUE;
            case FluidHTN::ConsoleColor::Green:
                return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case FluidHTN::ConsoleColor::DarkGreen:
                return FOREGROUND_GREEN;
            case FluidHTN::ConsoleColor::Red:
                return FOREGROUND_RED | FOREGROUND_INTENSITY;
            case FluidHTN::ConsoleColor::DarkRed:
				return FOREGROUND_RED;
            case FluidHTN::ConsoleColor::Yellow:
                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case FluidHTN::ConsoleColor::DarkYellow:
                return FOREGROUND_RED | FOREGROUND_GREEN;
            default:
                return 0xFF;
			
		}

	};
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
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), convertColorToWin32(color));
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
