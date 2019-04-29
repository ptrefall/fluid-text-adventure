using System;
using System.Collections.Generic;

namespace Fluid_Text_Adventure.Screens
{
    public class IntroScreen : GameScreen
    {
        public IntroScreen()
        {
            KeywordsToAction = new Dictionary<List<string>, Func<AIContext, GameScreen>>
            {
                {
                    new List<string> { "get", "bottle" },
                    ctx => SetGoal(ctx, GoalState.GetBottle)
                },
                {
                    new List<string> { "open", "bottle" },
                    ctx => SetGoal(ctx, GoalState.OpenBottle)
                },
            };
        }

        public override GameScreen Run(AIContext context)
        {
            Console.Clear();
            context.CurrentScreen = this;
            return Intro(context);
        }

        private GameScreen Intro(AIContext context)
        {
            Write("You're standing in an empty room of white tapestries. There's no doors or windows here.");
            return Interactibles(context);
        }

        private GameScreen Interactibles(AIContext context)
        {
            var action = Promt("In front of you there is a little bottle on a table. Around the neck of the bottle there is a paper label, with the words 'DRINK ME' beautifully printed on it in large letters.");
            return PerformAction(context, action);
        }

        private GameScreen SetGoal(AIContext context, GoalState goal)
        {
            context.SetGoal(goal);
            context.Player.Think();
            return null;
        }
    }
}