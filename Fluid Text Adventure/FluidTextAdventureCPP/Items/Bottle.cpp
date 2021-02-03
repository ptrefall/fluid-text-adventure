#include "StdIncludes.h"
#include "CoreIncludes/BaseDomainBuilder.h"
#include "Actions.h"
#include "Bottle.h"

SharedPtr<FluidHTN::Domain> Bottle::DefineDomain()
{
    SharedPtr<FluidHTN::Domain> getBottleDomain;
    SharedPtr<FluidHTN::Domain> dropBottleDomain;
    SharedPtr<FluidHTN::Domain> openBottleActionDomain;
    SharedPtr<FluidHTN::Domain> openBottleDomain;
    SharedPtr<FluidHTN::Domain> drinkBottleActionDomain;
    SharedPtr<FluidHTN::Domain> drinkBottleDomain;
    SharedPtr<FluidHTN::Domain> throwBottleDomain;
    SharedPtr<FluidHTN::Domain> cutBottleDomain;
    SharedPtr<FluidHTN::Domain> breakBottleDomain;
    {
        BaseDomainBuilder getBottleDomainBuilder("Get Bottle Sub-domain");
        getBottleDomainBuilder.AddSelector("Get Bottle");
        getBottleDomainBuilder.AddCondition("GOAL: Get Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasGoal(GoalState::GetBottle);
        });
        getBottleDomainBuilder.AddAction("Get Bottle");
        getBottleDomainBuilder.AddCondition("Has NOT Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, false);
        });
        getBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetBottle(static_cast<AIContext&>(ctx)); });
        getBottleDomainBuilder.AddEffect("Has Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasBottle, true, type);
        });
        getBottleDomainBuilder.AddEffect("Try Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::GetBottle, type);
        });
        getBottleDomainBuilder.End();
        getBottleDomainBuilder.End();
        getBottleDomain =  getBottleDomainBuilder.Build();
    }

    {
        BaseDomainBuilder dropBottleDomainBuilder("Drop Bottle Sub-domain");
        dropBottleDomainBuilder.AddSelector("Drop Bottle");
        dropBottleDomainBuilder.AddCondition("GOAL: Drop Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasGoal(GoalState::DropBottle);
        });
        dropBottleDomainBuilder.AddAction("Drop Bottle");
        dropBottleDomainBuilder.AddCondition("Has Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
        });
        dropBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::DropBottle(static_cast<AIContext&>(ctx)); });
        dropBottleDomainBuilder.AddEffect("Has NOT Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasBottle, true, type);
        });
        dropBottleDomainBuilder.AddEffect("Try Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::DropBottle, type);
        });
        dropBottleDomainBuilder.End();
        dropBottleDomainBuilder.End();
        dropBottleDomain = dropBottleDomainBuilder.Build();
    }

    {
        BaseDomainBuilder openBottleActionDomainBuilder("Open Bottle Action Sub-domain");
        openBottleActionDomainBuilder.AddAction("Open Bottle");
        openBottleActionDomainBuilder.AddCondition("Has Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
        });
        openBottleActionDomainBuilder.AddCondition("Bottle is NOT Open", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsOpen, false);
        });
        openBottleActionDomainBuilder.AddOperator([](IContext& ctx) { return Actions::OpenBottle(static_cast<AIContext&>(ctx)); });
        openBottleActionDomainBuilder.AddEffect("Open Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::BottleIsOpen, true, type);
        });
        openBottleActionDomainBuilder.AddEffect(
            "Try Complete Goal",
            EffectType::PlanAndExecute,
            [](IContext& ctx, EffectType type) {
                Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::OpenBottle, type);
            });
        openBottleActionDomainBuilder.End();
        openBottleActionDomain = openBottleActionDomainBuilder.Build();
    }
    {
        BaseDomainBuilder openBottleDomainBuilder("Open Bottle Sub-domain");
        openBottleDomainBuilder.AddSelector("Open Bottle");
        openBottleDomainBuilder.AddCondition("GOAL: Open Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasGoal(GoalState::OpenBottle);
        });
        openBottleDomainBuilder.AddSelector("Already Has Bottle?");
        openBottleDomainBuilder.Splice(*openBottleActionDomain);
        openBottleDomainBuilder.AddSequence("Get bottle and open it");
        openBottleDomainBuilder.AddAction("Temporary change goal");
        openBottleDomainBuilder.AddEffect("Get Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
            Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetBottle);
        });
        openBottleDomainBuilder.End();
        openBottleDomainBuilder.Splice(*getBottleDomain);
        openBottleDomainBuilder.AddAction("Temporary change goal");
        openBottleDomainBuilder.AddEffect("Open Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
            Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::OpenBottle);
        });
        openBottleDomainBuilder.End();
        openBottleDomainBuilder.Splice(*openBottleActionDomain);
        openBottleDomainBuilder.End();
        openBottleDomainBuilder.End();
        openBottleDomainBuilder.End();
        openBottleDomain = openBottleDomainBuilder.Build();
    }
    {
        BaseDomainBuilder drinkBottleActionDomainBuilder("Drink Bottle Action Sub-domain");
        drinkBottleActionDomainBuilder.AddAction("Drink Bottle");
        drinkBottleActionDomainBuilder.AddCondition("Has Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
        });
        drinkBottleActionDomainBuilder.AddCondition("Bottle is Open", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsOpen, true);
        });
        drinkBottleActionDomainBuilder.AddCondition("Bottle is NOT Empty", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsEmpty, false);
        });
        drinkBottleActionDomainBuilder.AddOperator([](IContext& ctx) { return Actions::SipBottle(static_cast<AIContext&>(ctx)); });
        drinkBottleActionDomainBuilder.AddEffect("Drink Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::BottleIsEmpty, true, type);
        });
        drinkBottleActionDomainBuilder.AddEffect(
            "Try Complete Goal",
            EffectType::PlanAndExecute,
            [](IContext& ctx, EffectType type) {
                Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::DrinkBottle, type);
            });
        drinkBottleActionDomainBuilder.End();
        drinkBottleActionDomain = drinkBottleActionDomainBuilder.Build();
    }

    {
        BaseDomainBuilder drinkBottleDomainBuilder("Drink Bottle Sub-domain");
        drinkBottleDomainBuilder.AddSelector("Drink Bottle");
        drinkBottleDomainBuilder.AddCondition("GOAL: Drink Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasGoal(GoalState::DrinkBottle);
        });
        drinkBottleDomainBuilder.AddSelector("Already Has Opened Bottle?");
        drinkBottleDomainBuilder.Splice(*drinkBottleActionDomain);
        drinkBottleDomainBuilder.AddSequence("Get bottle, open it, then drink it!");
        drinkBottleDomainBuilder.AddAction("Temporary change goal");
        drinkBottleDomainBuilder.AddEffect("Open Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
            Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::OpenBottle);
        });
        drinkBottleDomainBuilder.End();
        drinkBottleDomainBuilder.Splice(*openBottleDomain);
        drinkBottleDomainBuilder.AddAction("Temporary change goal");
        drinkBottleDomainBuilder.AddEffect("Drink Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
            Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::DrinkBottle);
        });
        drinkBottleDomainBuilder.End();
        drinkBottleDomainBuilder.Splice(*drinkBottleActionDomain);
        drinkBottleDomainBuilder.End();
        drinkBottleDomainBuilder.End();
        drinkBottleDomainBuilder.End();
        drinkBottleDomain = drinkBottleDomainBuilder.Build();
    }

    {
        BaseDomainBuilder throwBottleDomainBuilder("Throw Bottle Sub-domain");
        throwBottleDomainBuilder.AddAction("Throw the bottle");
        throwBottleDomainBuilder.AddCondition("Has Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, true);
        });
        throwBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::ThrowFullBottle(static_cast<AIContext&>(ctx)); });
        throwBottleDomainBuilder.AddEffect("Break Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::BottleIsBroken, true, type);
        });
        throwBottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
        });
        throwBottleDomainBuilder.End();
        throwBottleDomain = throwBottleDomainBuilder.Build();
    }
    {
        BaseDomainBuilder cutBottleDomainBuilder("Cut Bottle Sub-domain");
        cutBottleDomainBuilder.AddAction("Cut the bottle?");
        cutBottleDomainBuilder.AddCondition("Has NOT Bottle", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, false);
        });
        cutBottleDomainBuilder.AddCondition("Has Weapon", [](IContext& ctx) {
            return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon, true);
        });
        cutBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::SlashBottle(static_cast<AIContext&>(ctx)); });
        cutBottleDomainBuilder.AddEffect("Break Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::BottleIsBroken, true, type);
        });
        cutBottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
            static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
        });
        cutBottleDomainBuilder.End();
        cutBottleDomain = cutBottleDomainBuilder.Build();
    }
    // clang-format off
    {
        BaseDomainBuilder breakBottleDomainBuilder("Break Bottle Sub-domain");
        breakBottleDomainBuilder.AddSelector("Break Bottle");
			breakBottleDomainBuilder.AddCondition("GOAL: Break Bottle", [](IContext& ctx) {
				return static_cast<AIContext&>(ctx).HasGoal(GoalState::BreakBottle);
			});
			breakBottleDomainBuilder.AddCondition("Bottle NOT already broken", [](IContext& ctx) {
				return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsBroken, false);
			});
			breakBottleDomainBuilder.Splice(*throwBottleDomain);
			breakBottleDomainBuilder.Splice(*cutBottleDomain);
			breakBottleDomainBuilder.AddRandomSelector("Select random");
				breakBottleDomainBuilder.AddSequence("Get bottle and throw it");
					breakBottleDomainBuilder.AddAction("Temporary change goal");
						breakBottleDomainBuilder.AddEffect("Get Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
							Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetBottle);
						});
						breakBottleDomainBuilder.End();
						breakBottleDomainBuilder.Splice(*getBottleDomain);
						breakBottleDomainBuilder.AddAction("Temporary change goal");
							breakBottleDomainBuilder.AddEffect("Break Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
								Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::BreakBottle);
							});
						breakBottleDomainBuilder.End();
						breakBottleDomainBuilder.Splice(*throwBottleDomain);
				breakBottleDomainBuilder.End();
				breakBottleDomainBuilder.AddSequence("Get sword and cut bottle");
					breakBottleDomainBuilder.AddAction("Get Sword");
						breakBottleDomainBuilder.AddCondition("Has NOT Weapon", [](IContext& ctx) {
							return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon, false);
						});
						breakBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetSword(static_cast<AIContext&>(ctx)); });
							breakBottleDomainBuilder.AddEffect("Has Weapon", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
								static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, true, type);
							});
						breakBottleDomainBuilder.End();
						breakBottleDomainBuilder.Splice(*cutBottleDomain);
				breakBottleDomainBuilder.End();
			breakBottleDomainBuilder.End(); // end random
        breakBottleDomainBuilder.End(); //end outer selector
        breakBottleDomainBuilder.AddSelector("Throw Bottle");
			breakBottleDomainBuilder.AddCondition("GOAL: Throw Bottle", [](IContext& ctx) {
				return static_cast<AIContext&>(ctx).HasGoal(GoalState::ThrowBottle);
			});
			breakBottleDomainBuilder.AddCondition("Bottle NOT already broken", [](IContext& ctx) {
				return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsBroken, false);
			});
			breakBottleDomainBuilder.Splice(*throwBottleDomain);
			breakBottleDomainBuilder.AddSequence("Get bottle and throw it");
				breakBottleDomainBuilder.AddAction("Temporary change goal");
					breakBottleDomainBuilder.AddEffect("Get Bottle Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
							Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetBottle);
					});
				breakBottleDomainBuilder.End();
				breakBottleDomainBuilder.Splice(*getBottleDomain);
				breakBottleDomainBuilder.AddAction("Temporary change goal");
					breakBottleDomainBuilder.AddEffect("Throw Bottle Goal", EffectType::PlanOnly,[](IContext& ctx, EffectType type) {
							Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::ThrowBottle);
					});
				breakBottleDomainBuilder.End();
				breakBottleDomainBuilder.Splice(*throwBottleDomain);
            breakBottleDomainBuilder.End();
		breakBottleDomainBuilder.End(); // throw bottle selector
		breakBottleDomainBuilder.AddSelector("Slash Bottle");
            breakBottleDomainBuilder.AddCondition("GOAL: Slash Bottle", [](IContext& ctx) {
				return static_cast<AIContext&>(ctx).HasGoal(GoalState::SlashBottle);
			});
            breakBottleDomainBuilder.AddCondition("Bottle NOT already broken", [](IContext& ctx) {
				return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::BottleIsBroken, false);
			});
            breakBottleDomainBuilder.Splice(*cutBottleDomain);
            breakBottleDomainBuilder.AddSequence("Get sword and cut bottle");
				breakBottleDomainBuilder.AddAction("Get Sword");
					breakBottleDomainBuilder.AddCondition("Has NOT Weapon", [](IContext& ctx) {
                         return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon, false);
					 });
					breakBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetSword(static_cast<AIContext&>(ctx)); });
					breakBottleDomainBuilder.AddEffect("Has Weapon", EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
                         static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, true, type);
					});
				breakBottleDomainBuilder.End();
				breakBottleDomainBuilder.Splice(*cutBottleDomain);
            breakBottleDomainBuilder.End();
            breakBottleDomainBuilder.AddSequence("Set down bottle and slash it");
				breakBottleDomainBuilder.AddAction("Drop bottle");
					breakBottleDomainBuilder.AddCondition("Has Bottle",[](IContext& ctx) {
								 return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasBottle, false);
					 });
					breakBottleDomainBuilder.AddOperator([](IContext& ctx) { return Actions::DropBottle(static_cast<AIContext&>(ctx)); });
					breakBottleDomainBuilder.AddEffect("Has NOT Bottle", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
						 static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasBottle, true, type);
					});
				breakBottleDomainBuilder.End();
				breakBottleDomainBuilder.Splice(*cutBottleDomain);
            breakBottleDomainBuilder.End();
		breakBottleDomainBuilder.End();
        breakBottleDomain = breakBottleDomainBuilder.Build();
    }
    {
		BaseDomainBuilder bottleDomainBuilder("Bottle sub-domain");
		bottleDomainBuilder.AddAction("Bottle is broken");
			bottleDomainBuilder.AddCondition("GOAL: * Bottle", [](IContext& ctx) {
				return ( static_cast<AIContext&>(ctx).GetGoal() > GoalState::None &&  static_cast<AIContext&>(ctx).GetGoal() <= GoalState::ThrowBottle);
			});
			bottleDomainBuilder.AddCondition("Bottle is broken",[](IContext& ctx) { 
				return  static_cast<AIContext&>(ctx).HasState(AIWorldState::BottleIsBroken);
			});
			bottleDomainBuilder.AddOperator( [](IContext& ctx) {return Actions::Write(static_cast<AIContext&>(ctx), "But the bottle lies shattered on the ground!");});
			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
			 });
		bottleDomainBuilder.End();
		bottleDomainBuilder.Splice(*getBottleDomain);
		bottleDomainBuilder.Splice(*openBottleDomain);
		bottleDomainBuilder.Splice(*drinkBottleDomain);
		bottleDomainBuilder.Splice(*breakBottleDomain);
		bottleDomainBuilder.Splice(*dropBottleDomain);
		bottleDomainBuilder.AddAction("Already has bottle");
			bottleDomainBuilder.AddCondition("GOAL: Get Bottle", [](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::GetBottle);
			 });
			bottleDomainBuilder.AddCondition("Has Bottle", [](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::HasBottle);
			});
			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you already hold the bottle!");});
			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
			});
			bottleDomainBuilder.End();
		bottleDomainBuilder.AddAction("Already opened bottle");
			bottleDomainBuilder.AddCondition("GOAL: Open Bottle", [](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::OpenBottle);
			});
			bottleDomainBuilder.AddCondition("Bottle is open",[](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::BottleIsOpen);
			});
			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you already opened the bottle!");});
			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
			 });
			bottleDomainBuilder.End();
		bottleDomainBuilder.AddAction("Already emptied bottle");
			bottleDomainBuilder.AddCondition("GOAL: Drink Bottle",[](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::DrinkBottle);
			});
			bottleDomainBuilder.AddCondition("Bottle is empty",  [](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::BottleIsEmpty);
			});
			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But the bottle is empty!");});
			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
			 });
			bottleDomainBuilder.End();
		bottleDomainBuilder.AddAction("Not holding bottle to drop");
			bottleDomainBuilder.AddCondition("GOAL: Drop Bottle",[](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasGoal(GoalState::DropBottle);
			});
			bottleDomainBuilder.AddCondition("Has NOT Bottle",[](IContext& ctx) { 
				 return  static_cast<AIContext&>(ctx).HasState(AIWorldState::HasBottle);
			});
			bottleDomainBuilder.AddOperator( [](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you're not holding the bottle!");});
			bottleDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
				 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
			 });
			bottleDomainBuilder.End();
		return bottleDomainBuilder.Build();
		}
}
