#pragma once
#include "Items/Item.h"
#include "Screens/GameScreen.h"
#include "Player.h"


class IntroScreen : public GameScreen
{
public:
	IntroScreen();
	

protected:
	virtual std::shared_ptr<GameScreen> OnRun(AIContext& context) override;


private:
	std::shared_ptr<GameScreen> Intro(AIContext& context)
	{
		Write("You're standing in an empty room of white tapestries. There's no doors or windows here.");
		for (auto item : _Items)
			Write(item->Description());

		return Interactibles(context);
	}

	std::shared_ptr<GameScreen> Interactibles(AIContext& context)
	{
		auto action = Promt("Now, what to do..?");
		return PerformAction(context, action);
	}

	std::shared_ptr<GameScreen> SetGoal(AIContext& context, GoalState goal)
	{
		context.SetGoal(goal);
		context.Player()->Think(*GetDomain());
		return context.CurrentScreen();
	}

	static std::shared_ptr<Domain> DefineDomain(std::vector<std::shared_ptr<Item>>& itemsInScreen);

};
