#pragma once
#include "Screens/GameScreen.h"
#include "Screens/EnlightenedScreen.h"
#include "Tasks/Task.h"
#include "AIContext.h"

using namespace FluidHTN;

class Actions
{
public:
	static TaskStatus GetBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You pick up the bottle", false);
		return TaskStatus::Success;
	}
	static TaskStatus DropBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You drop the bottle", false);
		return TaskStatus::Success;
	}

	static TaskStatus OpenBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You unscrew the cork of the bottle", false);
		return TaskStatus::Success;
	}

	static TaskStatus CloseBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You screw the cork back onto the bottle", false);
		return TaskStatus::Success;
	}

	static TaskStatus SipBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You take a sip from the bottle", false);
		context.CurrentScreen()->Write("As the liquid runs down your throat and fills your belly, something weird starts happening. Your vision blur!");
		context.CurrentScreen() = MakeSharedPtr<EnlightenedScreen>();
		return TaskStatus::Success;
	}

	static TaskStatus ThrowFullBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You throw the bottle at the wall, it shatters!", false);
		context.CurrentScreen()->Write("The liquid splashes all over the place, even on you!");
		context.CurrentScreen() = MakeSharedPtr<EnlightenedScreen>();
		return TaskStatus::Success;
	}

	static TaskStatus SlashBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("You swing your sword furiously at the bottle, it shatters!", false);
		context.CurrentScreen()->Write("The liquid splashes all over the place, even on you!");
		context.CurrentScreen() = MakeSharedPtr<EnlightenedScreen>();
		return TaskStatus::Success;
	}

	static TaskStatus SipEmptyBottle(AIContext& context)
	{
		context.CurrentScreen()->Write("The bottle is empty", false);
		return TaskStatus::Success;
	}

	static TaskStatus GetSword(AIContext& context)
	{
		context.CurrentScreen()->Write("You pick up the sword", false);
		return TaskStatus::Success;
	}

	static TaskStatus DropSword(AIContext& context)
	{
		context.CurrentScreen()->Write("You drop the sword", false);
		return TaskStatus::Success;
	}

	static TaskStatus Write(AIContext& context, const std::string& text)
	{
		context.CurrentScreen()->Write(text, false);
		return TaskStatus::Success;
	}

	static TaskStatus Failed(AIContext& context)
	{
		context.CurrentScreen()->Write("Sorry, I don't know how to do that yet!", false);
		return TaskStatus::Success;
	}

	static void TryCompleteGoal(AIContext& context, GoalState endOfGoal, EffectType type)
	{
		if (context.HasGoal(endOfGoal))
		{
			context.SetGoal(GoalState::None, true, type);
		}
	}

	static void ChangeGoal(AIContext& context, EffectType type, GoalState goal)
	{
		context.SetGoal(goal, true, type);
	}
};
