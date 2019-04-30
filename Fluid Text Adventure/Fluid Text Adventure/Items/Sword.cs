using System;
using System.Collections.Generic;
using FluidHTN;
using FluidHTN.Compounds;
using Fluid_Text_Adventure.Screens;

namespace Fluid_Text_Adventure
{
    public class Sword : Item
    {
        public override string Description =>
            "On the ground lies a rusty, old sword.";


        public Sword(Func<AIContext, GoalState, GameScreen> SetGoal) : base(DefineDomain())
        {
            KeywordsToAction = new Dictionary<List<string>, Func<AIContext, GameScreen>>
            {
                {
                    new List<string> { "get", "sword" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.GetSword)
                },
                {
                    new List<string> { "slash" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.SlashAir)
                },
            };
        }

        private static Domain<AIContext> DefineDomain()
        {
            var getSwordDomain = new DomainBuilder<AIContext>("Get Sword Sub-domain")
                .Select("Get Sword")
                    .Condition("GOAL: Get Sword", (ctx => ctx.HasGoal(GoalState.GetSword)))
                    .Action("Get Sword")
                        .Condition("Has NOT Weapon", ctx => ctx.HasState(AIWorldState.HasWeapon, false))
                        .Do(Actions.GetSword)
                        .Effect("Has Weapon", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.HasWeapon, true, type))
                        .Effect("Try Complete Goal", EffectType.PlanAndExecute, (ctx, type) => Actions.TryCompleteGoal(ctx, GoalState.GetSword, type))
                    .End()
                .End()
                .Build();

            var slashAirActionDomain = new DomainBuilder<AIContext>("Slash Air Action Sub-domain")
                .Action("Slash Air")
                    .Condition("GOAL: Slash Air", (ctx => ctx.HasGoal(GoalState.SlashAir)))
                    .Condition("Has Weapon", ctx => ctx.HasState(AIWorldState.HasWeapon))
                    .Do((ctx => Actions.Write(ctx, "You slash your sword through the air elegantly!")))
                    .Effect("Try Complete Goal", EffectType.PlanAndExecute, (ctx, type) => Actions.TryCompleteGoal(ctx, GoalState.SlashAir, type))
                .End()
                .Build();

            return new DomainBuilder<AIContext>("Sword sub-domain")
                .Splice(getSwordDomain)
                .Action("Already has sword")
                    .Condition("GOAL: Get Sword", (ctx => ctx.HasGoal(GoalState.GetSword)))
                    .Condition("Has Weapon", ctx => ctx.HasState(AIWorldState.HasWeapon))
                    .Do((ctx => Actions.Write(ctx, "But you're already wielding the sword!")))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Select("Slash")
                    .Condition("GOAL: Slash Air", (ctx => ctx.HasGoal(GoalState.SlashAir)))
                    .Splice(slashAirActionDomain)
                    .Sequence("Pick up sword, then slash with it")
                        .Action("Temporary change goal")
                            .Effect("Get Sword Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.GetSword))
                        .End()
                        .Splice(getSwordDomain)
                        .Action("Temporary change goal")
                            .Effect("Slash Air Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.SlashAir))
                        .End()
                        .Splice(slashAirActionDomain)
                    .End()
                .End()
                .Build();
        }
    }
}