#include "StdIncludes.h"
#include "Actions.h"
#include "Items/Item.h"
#include "GameScreen.h"
#include "Screens/EnlightenedScreen.h"

SharedPtr<GameScreen> GameScreen::PerformAction(AIContext context, std::string& action)
{
	action = StringToLower(action);
	std::vector<std::string> parameters;
	StringSplit(action, parameters, ' ');
	parameters = SpliceKnownCommands(parameters);
	auto func = FindBestMatch(parameters);
	SharedPtr<GameScreen> result;
	if (func)
	{
		result = func(context);

	}
	if (result == nullptr)
	{
		Actions::Failed(context);
		return context.CurrentScreen();
	}

	return result;
}

ScreenFuncType GameScreen::FindBestMatch(std::vector<std::string>& parameters)
{

	ScreenFuncType bestMatch;
	int bestScore = 0;
	for (auto kvp : KeywordsToAction)
	{
		auto KvpKeys = std::get<0>(kvp);
		if (!IsEqual(KvpKeys[0], parameters[0]))
		{
			continue;
		}

		auto score = 0;
		for (auto key : KvpKeys)
		{
			for (auto parameter : parameters)
			{
				if (IsEqual(key, parameter))
				{
					score++;
				}
			}
		}

		if (KvpKeys.size() == parameters.size())
		{
			score *= 10;
		}

		if (score > bestScore)
		{
			bestScore = score;
			bestMatch = std::get<1>(kvp);
		}
	}

	for (auto item : _Items)
	{
		for(auto kvp : item->KeywordsToAction())
		{
			auto KvpKeys = std::get<0>(kvp);
			if (!IsEqual(KvpKeys[0], parameters[0]))
			{
				continue;
			}

			auto score = 0;
			for (auto key : KvpKeys)
			{
				for (auto parameter : parameters)
				{
					if (IsEqual(key, parameter))
					{
						score++;
					}
				}
			}

			if (KvpKeys.size() == parameters.size())
				score *= 10;

			if (score > bestScore)
			{
				bestScore = score;
				bestMatch = std::get<1>(kvp);
			}
		}
	}

	if (bestScore == 1 && parameters.size() == 1)
	{
		return bestMatch;
	}

	return (bestScore > 1 ? bestMatch : nullptr);
}
SharedPtr<GameScreen> EnlightenedScreen::OnRun(AIContext& context)
{
	if (_FirstRun)
	{
		context.CurrentScreen() = shared_from_this();
		return Intro(context);
	}
	else
	{
		throw std::exception("Unexpected turn of events!");
	}
}
