#pragma once
#include "Items/Item.h"
#include "Screens/GameScreen.h"
#include "Player.h"


class IntroScreen : public GameScreen
{
public:
	IntroScreen();
	

protected:
	virtual SharedPtr<GameScreen> OnRun(AIContext& context) override;


private:
	SharedPtr<GameScreen> Intro(AIContext& context)
	{
		Write("You're standing in an empty room of white tapestries. There's no doors or windows here.");
		for (auto item : _Items)
			Write(item->Description());

		return Interactibles(context);
	}

	SharedPtr<GameScreen> Interactibles(AIContext& context)
	{
		auto action = Promt("Now, what to do..?");
		return PerformAction(context, action);
	}

	SharedPtr<GameScreen> SetGoal(AIContext& context, GoalState goal)
	{
		context.SetGoal(goal);
		context.Player()->Think(*GetDomain());
		return context.CurrentScreen();
	}

	static SharedPtr<Domain> DefineDomain(std::vector<SharedPtr<Item>>& itemsInScreen);

};
