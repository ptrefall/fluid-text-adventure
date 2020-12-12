#pragma once
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include "pch.h"
#include "DebugInterfaces/DecompositionLogEntry.h"
#include <functional>
#include<tuple>
#include <iostream>

//http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

inline void StringSplit(const std::string& str, std::vector<std::string>& cont, char delim = ' ')
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}

inline std::string StringToLower(const std::string& input)
{
	auto output = input;
	std::transform(output.begin(), output.end(), output.begin(),
		[](unsigned char c) { return std::tolower(c); });

	return output;
}

typedef std::function<SharedPtr<class GameScreen>(class AIContext&)> ScreenFuncType;
typedef std::tuple<std::vector<std::string>, ScreenFuncType > KeywordsToActionEntry;
