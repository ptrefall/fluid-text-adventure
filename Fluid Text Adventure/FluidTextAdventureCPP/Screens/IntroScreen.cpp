#include "StdIncludes.h"
#include "AIContext.h"
#include "CoreIncludes/BaseDomainBuilder.h"
#include "Items/Bottle.h"
#include "Items/Sword.h"
#include "Actions.h"
#include "IntroScreen.h"

IntroScreen::IntroScreen()
{
	_Items =
	{
        MakeSharedPtr<Bottle>([this](AIContext& ctx, GoalState gs) { return this->SetGoal(ctx, gs); }),
		MakeSharedPtr<Sword>(([this](AIContext& ctx, GoalState gs) { return this->SetGoal(ctx, gs); })),
	};


	GameScreen::Initialize(DefineDomain(_Items));
}
SharedPtr<GameScreen> IntroScreen::OnRun(AIContext& context)
{
	if (_FirstRun)
	{
		Console::Clear();
		context.CurrentScreen() = shared_from_this();
		return Intro(context);
	}
	else
	{
		auto action = Console::ReadLine();
		Console::WriteLine();
		return PerformAction(context, action);
	}
}
SharedPtr<Domain> IntroScreen::DefineDomain(std::vector<SharedPtr<Item>>& itemsInScreen)
{
    BaseDomainBuilder itemDomainBuilder("Item Sub-domains");
    for (auto item : itemsInScreen)
    {
        itemDomainBuilder.Splice(*(item->Domain()));
    }

    BaseDomainBuilder builder("Intro Screen Domain");
    builder.Splice(*(itemDomainBuilder.Build()));
    builder.AddAction("Failed");
    builder.AddCondition("Failed to address goal",
                         [](IContext& ctx) { return (static_cast<AIContext&>(ctx).HasGoal(GoalState::None) == false); });
    builder.AddOperator([](IContext& ctx) { return Actions::Failed(static_cast<AIContext&>(ctx)); });
    builder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
        static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
    });
    builder.End();
    builder.AddAction("Idle");
    builder.AddOperator([](IContext&) { return TaskStatus::Continue; });
    builder.End();
    return builder.Build();
}
