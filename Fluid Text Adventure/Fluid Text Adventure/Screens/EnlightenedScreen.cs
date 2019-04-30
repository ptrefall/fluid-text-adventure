using System;
using System.Collections.Generic;

namespace Fluid_Text_Adventure.Screens
{
    public class EnlightenedScreen : GameScreen
    {
        public EnlightenedScreen()
        {
            KeywordsToAction = new Dictionary<List<string>, Func<AIContext, GameScreen>>
            {
            };

            base.Initialize(null);
        }

        protected override GameScreen OnRun(AIContext context)
        {
            if (FirstRun)
            {
                context.CurrentScreen = this;
                return Intro(context);
            }
            else
            {
                throw new Exception("Unexpected turn of events!");
            }
        }

        private GameScreen Intro(AIContext context)
        {
            Write("The room seems to change around you.");
            Write("The walls disappear.");
            Write("The ceiling disappears.");
            Write("The floor disappears!");
            Write("You fall into the darkness...");
            return null;
        }
    }
}