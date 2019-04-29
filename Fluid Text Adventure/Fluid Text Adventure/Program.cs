using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FluidHTN;
using Fluid_Text_Adventure.Screens;
using TaskStatus = FluidHTN.TaskStatus;

namespace Fluid_Text_Adventure
{
    class Program
    {
        static void Main(string[] args)
        {
            var domain = DefineDomain();
            var player = new Player(domain);

            var intro = new IntroScreen();
            intro.Run(player.Context);
            Console.WriteLine("The End!");
            Console.ReadKey();
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

            return new DomainBuilder<AIContext>("TextAdventurePlanner")
                .Splice(getBottleDomain)
                .Splice(openBottleDomain)
                .Action("Idle")
                    .Condition("Failed to address goal", (ctx => ctx.HasGoal(GoalState.None) == false))
                    .Do(Actions.Failed)
                    .Effect("Complete Goal", EffectType.PlanAndExecute, (ctx, type) => ctx.SetGoal(GoalState.None, true, type))
                .End()
                .Action("Idle")
                    .Do(ctx => TaskStatus.Continue)
                .End()
                .Build();
        }
    }
}
