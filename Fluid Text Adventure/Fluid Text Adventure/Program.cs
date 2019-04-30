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
            var player = new Player();

            var intro = new IntroScreen();
            var nextScreen = intro.Run(player.Context);
            while (nextScreen != null)
            {
                nextScreen = nextScreen.Run(player.Context);
            }

            Console.WriteLine("The End!");
            Console.ReadKey();
        }
    }
}
