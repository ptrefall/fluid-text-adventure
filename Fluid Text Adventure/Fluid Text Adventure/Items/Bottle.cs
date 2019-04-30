using System;
using System.Collections.Generic;
using FluidHTN;
using Fluid_Text_Adventure.Screens;

namespace Fluid_Text_Adventure
{
    public class Bottle : Item
    {
        public override string Description =>
            "In front of you there is a little bottle on a table. Around the neck of the bottle there is a paper label, with the words 'DRINK ME' beautifully printed on it in large letters.";

        public Bottle(Func<AIContext, GoalState, GameScreen> SetGoal) : base(DefineDomain())
        {
            KeywordsToAction = new Dictionary<List<string>, Func<AIContext, GameScreen>>
            {
                {
                    new List<string> { "get", "bottle" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.GetBottle)
                },
                {
                    new List<string> { "open", "bottle" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.OpenBottle)
                },
                {
                    new List<string> { "drink", "bottle" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.DrinkBottle)
                },
                {
                    new List<string> { "break", "bottle" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.BreakBottle)
                },
                {
                    new List<string> { "throw", "bottle" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.ThrowBottle)
                },
                {
                    new List<string> { "slash", "bottle" },
                    ctx => SetGoal?.Invoke(ctx, GoalState.SlashBottle)
                },
            };
        }

        private static Domain<AIContext> DefineDomain()
        {
            var getBottleDomain = new DomainBuilder<AIContext>("Get Bottle Sub-domain")
                .Select("Get Bottle")
                    .Condition("GOAL: Get Bottle", (ctx => ctx.HasGoal(GoalState.GetBottle)))
                    .Action("Get Bottle")
                        .Condition("Has NOT Bottle", ctx => ctx.HasState(AIWorldState.HasBottle, false))
                        .Do(Actions.GetBottle)
                        .Effect("Has Bottle", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.HasBottle, true, type))
                        .Effect("Try Complete Goal", EffectType.PlanAndExecute, (ctx, type) => Actions.TryCompleteGoal(ctx, GoalState.GetBottle, type))
                    .End()
                .End()
                .Build();

            var openBottleActionDomain = new DomainBuilder<AIContext>("Open Bottle Action Sub-domain")
                .Action("Open Bottle")
                    .Condition("Has Bottle", ctx => ctx.HasState(AIWorldState.HasBottle))
                    .Condition("Bottle is NOT Open", ctx => ctx.HasState(AIWorldState.BottleIsOpen, false))
                    .Do(Actions.OpenBottle)
                    .Effect("Open Bottle", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.BottleIsOpen, true, type))
                    .Effect("Try Complete Goal", EffectType.PlanAndExecute, (ctx, type) => Actions.TryCompleteGoal(ctx, GoalState.OpenBottle, type))
                .End()
                .Build();

            var openBottleDomain = new DomainBuilder<AIContext>("Open Bottle Sub-domain")
                .Select("Open Bottle")
                    .Condition("GOAL: Open Bottle", (ctx => ctx.HasGoal(GoalState.OpenBottle)))
                    .Select("Already Has Bottle?")
                        .Splice(openBottleActionDomain)
                        .Sequence("Get bottle and open it")
                            .Action("Temporary change goal")
                                .Effect("Get Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.GetBottle))
                            .End()
                            .Splice(getBottleDomain)
                            .Action("Temporary change goal")
                                .Effect("Open Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.OpenBottle))
                            .End()
                            .Splice(openBottleActionDomain)
                        .End()
                    .End()
                .End()
                .Build();

            var drinkBottleActionDomain = new DomainBuilder<AIContext>("Drink Bottle Action Sub-domain")
                .Action("Drink Bottle")
                    .Condition("Has Bottle", ctx => ctx.HasState(AIWorldState.HasBottle))
                    .Condition("Bottle is Open", ctx => ctx.HasState(AIWorldState.BottleIsOpen))
                    .Condition("Bottle is NOT Empty", ctx => ctx.HasState(AIWorldState.BottleIsEmpty, false))
                    .Do(Actions.SipBottle)
                    .Effect("Drink Bottle", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.BottleIsEmpty, true, type))
                    .Effect("Try Complete Goal", EffectType.PlanAndExecute, (ctx, type) => Actions.TryCompleteGoal(ctx, GoalState.DrinkBottle, type))
                .End()
                .Build();

            var drinkBottleDomain = new DomainBuilder<AIContext>("Drink Bottle Sub-domain")
                .Select("Drink Bottle")
                    .Condition("GOAL: Drink Bottle", (ctx => ctx.HasGoal(GoalState.DrinkBottle)))
                    .Select("Already Has Opened Bottle?")
                        .Splice(drinkBottleActionDomain)
                        .Sequence("Get bottle, open it, then drink it!")
                            .Action("Temporary change goal")
                                .Effect("Open Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.OpenBottle))
                            .End()
                            .Splice(openBottleDomain)
                            .Action("Temporary change goal")
                                .Effect("Drink Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.DrinkBottle))
                            .End()
                            .Splice(drinkBottleActionDomain)
                        .End()
                    .End()
                .End()
                .Build();

            var throwBottleDomain = new DomainBuilder<AIContext>("Throw Bottle Sub-domain")
                .Action("Throw the bottle")
                    .Condition("Has Bottle", ctx => ctx.HasState(AIWorldState.HasBottle))
                    .Do(Actions.ThrowFullBottle)
                    .Effect("Break Bottle", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.BottleIsBroken, true, type))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Build();

            var cutBottleDomain = new DomainBuilder<AIContext>("Cut Bottle Sub-domain")
                .Action("Cut the bottle?")
                    .Condition("Has NOT Bottle", ctx => ctx.HasState(AIWorldState.HasBottle, false))
                    .Condition("Has Weapon", ctx => ctx.HasState(AIWorldState.HasWeapon))
                    .Do(Actions.SlashBottle)
                    .Effect("Break Bottle", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.BottleIsBroken, true, type))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Build();

            var breakBottleDomain = new DomainBuilder<AIContext>("Break Bottle Sub-domain")
                .Select("Break Bottle")
                    .Condition("GOAL: Break Bottle", (ctx => ctx.HasGoal(GoalState.BreakBottle)))
                    .Condition("Bottle NOT already broken", ctx => ctx.HasState(AIWorldState.BottleIsBroken, false))
                    .Splice(throwBottleDomain)
                    .Splice(cutBottleDomain)
                    .RandomSelect<DomainBuilder<AIContext>, AIContext>("Select random")
                        .Sequence("Get bottle and throw it")
                            .Action("Temporary change goal")
                                .Effect("Get Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.GetBottle))
                            .End()
                            .Splice(getBottleDomain)
                            .Action("Temporary change goal")
                                .Effect("Break Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.BreakBottle))
                            .End()
                            .Splice(throwBottleDomain)
                        .End()
                        .Sequence("Get sword and cut bottle")
                            .Action("Get Sword")
                                .Condition("Has NOT Weapon", ctx => ctx.HasState(AIWorldState.HasWeapon, false))
                                .Do(Actions.GetSword)
                                .Effect("Has Weapon", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.HasWeapon, true, type))      
                            .End()
                            .Splice(cutBottleDomain)
                        .End()
                    .End()
                .End()
                .Select("Throw Bottle")
                    .Condition("GOAL: Throw Bottle", (ctx => ctx.HasGoal(GoalState.ThrowBottle)))
                    .Condition("Bottle NOT already broken", ctx => ctx.HasState(AIWorldState.BottleIsBroken, false))
                    .Splice(throwBottleDomain)
                    .Sequence("Get bottle and throw it")
                        .Action("Temporary change goal")
                            .Effect("Get Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.GetBottle))
                        .End()
                        .Splice(getBottleDomain)
                        .Action("Temporary change goal")
                            .Effect("Throw Bottle Goal", EffectType.PlanOnly, (ctx, type) => Actions.ChangeGoal(ctx, type, GoalState.ThrowBottle))
                        .End()
                        .Splice(throwBottleDomain)
                    .End()
                .End()
                .Select("Slash Bottle")
                    .Condition("GOAL: Slash Bottle", (ctx => ctx.HasGoal(GoalState.SlashBottle)))
                    .Condition("Bottle NOT already broken", ctx => ctx.HasState(AIWorldState.BottleIsBroken, false))
                    .Splice(cutBottleDomain)
                    .Sequence("Get sword and cut bottle")
                        .Action("Get Sword")
                            .Condition("Has NOT Weapon", ctx => ctx.HasState(AIWorldState.HasWeapon, false))
                            .Do(Actions.GetSword)
                            .Effect("Has Weapon", EffectType.PlanAndExecute, (ctx, type) => ctx.SetState(AIWorldState.HasWeapon, true, type))
                        .End()
                        .Splice(cutBottleDomain)
                    .End()
                .End()
                .Build();

            return new DomainBuilder<AIContext>("Bottle sub-domain")
                .Action("Bottle is broken")
                .Condition("GOAL: * Bottle", (ctx => ctx.GetGoal() > GoalState.None && ctx.GetGoal() <= GoalState.ThrowBottle))
                    .Condition("Bottle is broken", ctx => ctx.HasState(AIWorldState.BottleIsBroken))
                    .Do((ctx => Actions.Write(ctx, "But the bottle lies shattered on the ground!")))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Splice(getBottleDomain)
                .Splice(openBottleDomain)
                .Splice(drinkBottleDomain)
                .Splice(breakBottleDomain)
                .Action("Already has bottle")
                    .Condition("GOAL: Get Bottle", (ctx => ctx.HasGoal(GoalState.GetBottle)))
                    .Condition("Has Bottle", ctx => ctx.HasState(AIWorldState.HasBottle))
                    .Do((ctx => Actions.Write(ctx, "But you already hold the bottle!")))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Action("Already opened bottle")
                    .Condition("GOAL: Open Bottle", (ctx => ctx.HasGoal(GoalState.OpenBottle)))
                    .Condition("Bottle is open", ctx => ctx.HasState(AIWorldState.BottleIsOpen))
                        .Do((ctx => Actions.Write(ctx, "But you already opened the bottle!")))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Action("Already emptied bottle")
                    .Condition("GOAL: Drink Bottle", (ctx => ctx.HasGoal(GoalState.DrinkBottle)))
                    .Condition("Bottle is empty", ctx => ctx.HasState(AIWorldState.BottleIsEmpty))
                        .Do((ctx => Actions.Write(ctx, "But the bottle is empty!")))
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Build();
        }
    }
}