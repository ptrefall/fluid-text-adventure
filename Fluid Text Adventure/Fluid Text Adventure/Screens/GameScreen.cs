using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using FluidHTN;

namespace Fluid_Text_Adventure.Screens
{
    public abstract class GameScreen
    {
        protected Domain<AIContext> Domain { get; private set; }
        protected List<Item> Items;
        protected Dictionary<List<string>, Func<AIContext, GameScreen>> KeywordsToAction;
        protected bool FirstRun { get; private set; } = true;

        public void Initialize(Domain<AIContext> domain)
        {
            Domain = domain;
        }

        protected abstract GameScreen OnRun(AIContext context);

        public GameScreen Run(AIContext context)
        {
            var result = OnRun(context);
            if (FirstRun) FirstRun = false;
            return result;
        }

        public void Write(string text, bool readLineAtEnd = true)
        {
            Console.Write(text);
            Console.WriteLine();
            if (readLineAtEnd)
            {
                Console.WriteLine("[Enter]");
                Console.ReadLine();
            }
        }

        public string Promt(string text)
        {
            Console.WriteLine(text);
            var result = Console.ReadLine();
            Console.WriteLine();
            return result;
        }

        public GameScreen PerformAction(AIContext context, string action)
        {
            action = action.ToLower();
            var parameters = action.Split(' ').ToList();
            parameters = SpliceKnownCommands(parameters);
            var func = FindBestMatch(parameters);
            var result = func?.Invoke(context);
            if (result == null)
            {
                Actions.Failed(context);
                return context.CurrentScreen;
            }

            return result;
        }

        private List<string> ToLower(List<string> parameters)
        {
            for (var i = 0; i < parameters.Count; i++)
            {
                parameters[i] = parameters[i].ToLower();
            }

            return parameters;
        }

        private List<string> SpliceKnownCommands(List<string> parameters)
        {
            for (var i = 0; i < parameters.Count-1; i++)
            {
                if (parameters[i] == "pick" && parameters[i + 1] == "up")
                {
                    parameters[i] = "pick up";
                    parameters.RemoveAt(i+1);
                }
            }

            return parameters;
        }

        private Func<AIContext, GameScreen> FindBestMatch(List<string> parameters)
        {
            if (KeywordsToAction == null)
                return null;

            Func<AIContext, GameScreen> bestMatch = null;
            int bestScore = 0;
            foreach (var kvp in KeywordsToAction)
            {
                if (!IsEqual(kvp.Key[0], parameters[0]))
                    continue;

                var score = 0;
                foreach (var key in kvp.Key)
                {
                    foreach (var parameter in parameters)
                    {
                        if (IsEqual(key, parameter))
                        {
                            score++;
                        }
                    }
                }

                if (kvp.Key.Count == parameters.Count)
                    score *= 10;

                if (score > bestScore)
                {
                    bestScore = score;
                    bestMatch = kvp.Value;
                }
            }

            foreach (var item in Items)
            {
                foreach (var kvp in item.KeywordsToAction)
                {
                    if (!IsEqual(kvp.Key[0], parameters[0]))
                        continue;

                    var score = 0;
                    foreach (var key in kvp.Key)
                    {
                        foreach (var parameter in parameters)
                        {
                            if (IsEqual(key, parameter))
                            {
                                score++;
                            }
                        }
                    }

                    if (kvp.Key.Count == parameters.Count)
                        score *= 10;

                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestMatch = kvp.Value;
                    }
                }
            }

            if(bestScore == 1 && parameters.Count == 1)
                return bestMatch;

            return bestScore > 1 ? bestMatch : null;
        }

        private bool IsEqual(string a, string b)
        {
            if (a == b) return true;

            if (a == "get")
            {
                if (b == "pick up") return true;
                if (b == "fetch") return true;
                if (b == "hold") return true;
                if (b == "wield") return true;
                if (b == "gather") return true;
                if (b == "acquire") return true;
                if (b == "take") return true;
            }

            if (a == "slash")
            {
                if (b == "swing") return true;
                if (b == "cut") return true;
                if (b == "attack") return true;
                if (b == "hit") return true;
                if (b == "hack") return true;
                if (b == "slice") return true;
                if (b == "slit") return true;
            }

            if (a == "drink")
            {
                if (b == "sip") return true;
                if (b == "gulp") return true;
                if (b == "swallow") return true;
                if (b == "quaff") return true;
                if (b == "taste") return true;
                if (b == "consume") return true;
                if (b == "drain") return true;
                if (b == "slurp") return true;
                if (b == "down") return true;
            }

            if (a == "open")
            {
                if (b == "unscrew") return true;
                if (b == "uncork") return true;
            }

            if (a == "throw")
            {
                if (b == "whirl") return true;
                if (b == "fling") return true;
                if (b == "hurl") return true;
                if (b == "lob") return true;
                if (b == "thrust") return true;
            }

            if (a == "break")
            {
                if (b == "destroy") return true;
                if (b == "shatter") return true;
                if (b == "crack") return true;
                if (b == "crush") return true;
                if (b == "demolish") return true;
                if (b == "fracture") return true;
                if (b == "ruin") return true;
                if (b == "smash") return true;
                if (b == "wreck") return true;
            }

            return false;
        }
    }
}