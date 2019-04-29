using System;
using System.Collections.Generic;
using System.Linq;

namespace Fluid_Text_Adventure.Screens
{
    public abstract class GameScreen
    {
        protected Dictionary<List<string>, Func<AIContext, GameScreen>> KeywordsToAction;

        public abstract GameScreen Run(AIContext context);

        public void Write(string text)
        {
            Console.Write(text);
            Console.WriteLine();
            Console.WriteLine("[Enter]");
            Console.ReadLine();
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
            return func?.Invoke(context);
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
            if (KeywordsToAction == null || KeywordsToAction.Count == 0)
                return null;

            Func<AIContext, GameScreen> bestMatch = null;
            int bestScore = 0;
            foreach (var kvp in KeywordsToAction)
            {
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

                if (score > bestScore)
                {
                    bestScore = score;
                    bestMatch = kvp.Value;
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
            }

            return false;
        }
    }
}