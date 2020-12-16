#include "StdIncludes.h"
#include "CoreIncludes/BaseDomainBuilder.h"
#include "Actions.h"
#include "Sword.h"

SharedPtr<FluidHTN::Domain> Sword::DefineDomain()
{

    SharedPtr<FluidHTN::Domain> getSwordDomain;
    SharedPtr<FluidHTN::Domain> dropSwordDomain;
    SharedPtr<FluidHTN::Domain> slashAirActionDomain;
    // clang-format off
    {
    BaseDomainBuilder getSwordDomainBuilder("Get Sword Sub-domain");
        getSwordDomainBuilder.AddSelector("Get Sword");
			getSwordDomainBuilder.AddCondition("GOAL: Get Sword",
											   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::GetSword); });
			getSwordDomainBuilder.AddAction("Get Sword");
                getSwordDomainBuilder.AddCondition("Has NOT Weapon",[](IContext& ctx) {
                     return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon,false);
				});
				getSwordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::GetSword(static_cast<AIContext&>(ctx)); });
                getSwordDomainBuilder.AddEffect("Has Weapon", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
                     static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, true, type);
				});
                getSwordDomainBuilder.AddEffect("Try Complete Goal",
                        EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
                         Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::GetSword,type);
				});
			getSwordDomainBuilder.End();
		getSwordDomainBuilder.End();

        getSwordDomain = getSwordDomainBuilder.Build();
    }
	{
            BaseDomainBuilder dropSwordDomainBuilder("Drop Sword Sub-domain");
			  dropSwordDomainBuilder.AddSelector("Drop Sword");
				  dropSwordDomainBuilder.AddCondition("GOAL: Drop Sword",  
											   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::DropSword); });
				  dropSwordDomainBuilder.AddAction("Drop Sword");
					  dropSwordDomainBuilder.AddCondition("Has Weapon",[](IContext& ctx) {
						 return static_cast<AIContext&>(ctx).HasState(AIWorldState::HasWeapon);
					  });
					  dropSwordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::DropSword(static_cast<AIContext&>(ctx)); });
					  dropSwordDomainBuilder.AddEffect("Has NOT Weapon",
                                          EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
                                           static_cast<AIContext&>(ctx).SetStateAIWS(AIWorldState::HasWeapon, false, type);
					   });
						dropSwordDomainBuilder.AddEffect("Try Complete Goal",
								EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
								 Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::DropSword,type);
						});
				  dropSwordDomainBuilder.End();
			  dropSwordDomainBuilder.End();
			dropSwordDomain = dropSwordDomainBuilder.Build();
	}
    {
	    BaseDomainBuilder slashAirActionDomainBuilder("Slash Air Action Sub-domain");
        slashAirActionDomainBuilder.AddAction("Slash Air");
		   slashAirActionDomainBuilder.AddCondition("GOAL: Slash Air", 
						   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::SlashAir); });
			slashAirActionDomainBuilder.AddCondition("Has Weapon",[](IContext& ctx) {
				 return static_cast<AIContext&>(ctx).HasState(AIWorldState::HasWeapon);
			});
			slashAirActionDomainBuilder.AddOperator([](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "You slash your sword through the air elegantly!");});
			slashAirActionDomainBuilder.AddEffect("Try Complete Goal",
					EffectType::PlanAndExecute,[](IContext& ctx, EffectType type) {
					 Actions::TryCompleteGoal(static_cast<AIContext&>(ctx), GoalState::SlashAir,type);
			 });
		slashAirActionDomainBuilder.End();
        slashAirActionDomain = slashAirActionDomainBuilder.Build();

	}
    {
        BaseDomainBuilder swordDomainBuilder("Sword sub-domain");
            swordDomainBuilder.Splice(*getSwordDomain);
            swordDomainBuilder.Splice(*dropSwordDomain);
            swordDomainBuilder.AddAction("Already has sword");
				swordDomainBuilder.AddCondition("GOAL: Get Sword", 
						   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::GetSword); });
				swordDomainBuilder.AddCondition("Has Weapon", [](IContext& ctx) {
					 return static_cast<AIContext&>(ctx).HasState(AIWorldState::HasWeapon);
				});
				swordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you're already wielding the sword!");});
				swordDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
					 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
				 });
            swordDomainBuilder.End();
            swordDomainBuilder.AddAction("Not holding sword to drop");
				swordDomainBuilder.AddCondition("GOAL: Drop Sword",
							   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::DropSword); });
				swordDomainBuilder.AddCondition("Has NOT Weapon", [](IContext& ctx) {
						 return static_cast<AIContext&>(ctx).HasStateAIWS(AIWorldState::HasWeapon,false);
				});
				swordDomainBuilder.AddOperator([](IContext& ctx) { return Actions::Write(static_cast<AIContext&>(ctx), "But you're not holding a sword!");});
				swordDomainBuilder.AddEffect("Complete Goal", EffectType::PlanAndExecute, [](IContext& ctx, EffectType type) {
					 static_cast<AIContext&>(ctx).SetGoal(GoalState::None, true, type);
				 });
            swordDomainBuilder.End();
            swordDomainBuilder.AddSelector("Slash");
				swordDomainBuilder.AddCondition("GOAL: Slash Air", 
							   [](IContext& ctx) { return static_cast<AIContext&>(ctx).HasGoal(GoalState::SlashAir); });
				swordDomainBuilder.Splice(*slashAirActionDomain);
				swordDomainBuilder.AddSequence("Pick up sword, then slash with it");
				swordDomainBuilder.AddAction("Temporary change goal");
					swordDomainBuilder.AddEffect("Get Sword Goal", EffectType::PlanOnly, [](IContext& ctx, EffectType type) {
						 Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetSword);
					});
				swordDomainBuilder.End();
				swordDomainBuilder.Splice(*getSwordDomain);
				swordDomainBuilder.AddAction("Temporary change goal");
						swordDomainBuilder.AddEffect("Slash Air Goal", EffectType::PlanOnly,  [](IContext& ctx, EffectType type) {
						 Actions::ChangeGoal(static_cast<AIContext&>(ctx), type, GoalState::GetSword);
						});
				swordDomainBuilder.End();
				swordDomainBuilder.Splice(*slashAirActionDomain);
            swordDomainBuilder.End();
		swordDomainBuilder.End();

		return swordDomainBuilder.Build();
	}
}
