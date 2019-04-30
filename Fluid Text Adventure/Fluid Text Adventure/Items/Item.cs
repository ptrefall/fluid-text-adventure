using System;
using System.Collections.Generic;
using FluidHTN;
using Fluid_Text_Adventure.Screens;

namespace Fluid_Text_Adventure
{
    public abstract class Item
    {
        public Domain<AIContext> Domain { get; private set; }
        public Dictionary<List<string>, Func<AIContext, GameScreen>> KeywordsToAction;

        public abstract string Description { get; }

        protected Item(Domain<AIContext> domain)
        {
            Domain = domain;
        }
    }
}