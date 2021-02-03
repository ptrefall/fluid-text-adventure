#pragma once
#include "Console.h"


class GameScreen: public std::enable_shared_from_this<GameScreen>
{
protected:
	SharedPtr<class Domain> _Domain;
	std::vector<SharedPtr<class Item>> _Items;
	std::vector<KeywordsToActionEntry> KeywordsToAction;

	bool _FirstRun = true;

	virtual SharedPtr<GameScreen> OnRun(AIContext& context) = 0;
public:
	SharedPtr<Domain> GetDomain() { return _Domain; }
	std::vector<SharedPtr<Item>>& Items() { return _Items; }
	bool FirstRun() { return _FirstRun; }


	void Initialize(SharedPtr<Domain> domain)
	{
		_Domain = domain;
	}


	SharedPtr<GameScreen> Run(AIContext& context)
	{
		auto result = OnRun(context);
		if (_FirstRun) _FirstRun = false;
		return result;
	}

	void Write(std::string text, bool readLineAtEnd = true)
	{
		Console::Write(text);
		Console::WriteLine();
		if (readLineAtEnd)
		{
			Console::WriteLine("[Enter]");
			Console::ReadLine();
		}
	}
	std::string Promt(std::string text)
	{
		std::string result;
		Console::WriteLine(text);
		result = Console::ReadLine();
		Console::WriteLine();
		return result;
	}

	SharedPtr<GameScreen> PerformAction(AIContext context, std::string& action);
	

private:
	std::vector<std::string> ToLower(std::vector<std::string>& parameters)
	{
		for (auto i = 0; i < parameters.size(); i++)
		{
			parameters[i] = StringToLower(parameters[i]);
		}

		return parameters;
	}

	std::vector<std::string> SpliceKnownCommands(std::vector<std::string>& parameters)
	{
		std::vector<std::string> result;

		// corner case of one-word commands.
		if(parameters.size() == 1)
        {
            result = parameters;
        }
		for (int i = 0; i < parameters.size() - 1; i++)
		{
			if (parameters[i] == "pick" && parameters[i + 1] == "up")
			{
				result.push_back("pick up"s);
				i++;
			}
			else
			{
				result.push_back(parameters[i]);
			}
		}
		parameters = result;
		return parameters;
	}

	ScreenFuncType FindBestMatch(std::vector<std::string>& parameters);
	
	bool IsEqual(std::string& a, std::string& b)
	{
		if (a == b) return true;

		if (a == "get")
		{
			if (b == "pick up") return true;
			if (b == "fetch") return true;
			if (b == "hold") return true;
			if (b == "wield") return true;
			if (b == "gather") return true;
			if (b == "acquire") return true;
			if (b == "take") return true;
		}

		if (a == "slash")
		{
			if (b == "swing") return true;
			if (b == "cut") return true;
			if (b == "attack") return true;
			if (b == "hit") return true;
			if (b == "hack") return true;
			if (b == "slice") return true;
			if (b == "slit") return true;
		}

		if (a == "drink")
		{
			if (b == "sip") return true;
			if (b == "gulp") return true;
			if (b == "swallow") return true;
			if (b == "quaff") return true;
			if (b == "taste") return true;
			if (b == "consume") return true;
			if (b == "drain") return true;
			if (b == "slurp") return true;
			if (b == "down") return true;
		}

		if (a == "open")
		{
			if (b == "unscrew") return true;
			if (b == "uncork") return true;
		}

		if (a == "throw")
		{
			if (b == "whirl") return true;
			if (b == "fling") return true;
			if (b == "hurl") return true;
			if (b == "lob") return true;
			if (b == "thrust") return true;
		}

		if (a == "break")
		{
			if (b == "destroy") return true;
			if (b == "shatter") return true;
			if (b == "crack") return true;
			if (b == "crush") return true;
			if (b == "demolish") return true;
			if (b == "fracture") return true;
			if (b == "ruin") return true;
			if (b == "smash") return true;
			if (b == "wreck") return true;
		}

		return false;
	}
};
