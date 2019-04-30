using System;
using System.Collections.Generic;
using FluidHTN;

namespace Fluid_Text_Adventure.Screens
{
    public class IntroScreen : GameScreen
    {
        public IntroScreen()
        {
            Items = new List<Item>
            {
                new Bottle(SetGoal),
                new Sword(SetGoal),
            };

            KeywordsToAction = new Dictionary<List<string>, Func<AIContext, GameScreen>>
            {
            };

            base.Initialize(DefineDomain(Items));
        }

        protected override GameScreen OnRun(AIContext context)
        {
            if (FirstRun)
            {
                Console.Clear();
                context.CurrentScreen = this;
                return Intro(context);
            }
            else
            {
                var action = Console.ReadLine();
                Console.WriteLine();
                return PerformAction(context, action);
            }
        }

        private GameScreen Intro(AIContext context)
        {
            Write("You're standing in an empty room of white tapestries. There's no doors or windows here.");
            foreach(var item in Items)
                Write(item.Description);

            return Interactibles(context);
        }

        private GameScreen Interactibles(AIContext context)
        {
            var action = Promt("Now, what to do..?");
            return PerformAction(context, action);
        }

        private GameScreen SetGoal(AIContext context, GoalState goal)
        {
            context.SetGoal(goal);
            context.Player.Think(Domain);
            return context.CurrentScreen;
        }

        private static Domain<AIContext> DefineDomain(List<Item> itemsInScreen)
        {
            var itemDomainBuilder = new DomainBuilder<AIContext>("Item Sub-domains");
            foreach (var item in itemsInScreen)
            {
                itemDomainBuilder.Splice(item.Domain);
            }

            return new DomainBuilder<AIContext>("Intro Screen Domain")
                .Splice(itemDomainBuilder.Build())
                .Action("Failed")
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