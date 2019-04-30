using FluidHTN;
using Fluid_Text_Adventure.Screens;

namespace Fluid_Text_Adventure
{
    public static class Actions
    {
        public static TaskStatus GetBottle(AIContext context)
        {
            context.CurrentScreen.Write("You pick up the bottle", false);
            return TaskStatus.Success;
        }

        public static TaskStatus DropBottle(AIContext context)
        {
            context.CurrentScreen.Write("You drop the bottle", false);
            return TaskStatus.Success;
        }

        public static TaskStatus OpenBottle(AIContext context)
        {
            context.CurrentScreen.Write("You unscrew the cork of the bottle", false);
            return TaskStatus.Success;
        }

        public static TaskStatus CloseBottle(AIContext context)
        {
            context.CurrentScreen.Write("You screw the cork back onto the bottle", false);
            return TaskStatus.Success;
        }

        public static TaskStatus SipBottle(AIContext context)
        {
            context.CurrentScreen.Write("You take a sip from the bottle", false);
            context.CurrentScreen.Write("As the liquid runs down your throat and fills your belly, something weird starts happening. Your vision blur!");
            context.CurrentScreen = new EnlightenedScreen();
            return TaskStatus.Success;
        }

        public static TaskStatus ThrowFullBottle(AIContext context)
        {
            context.CurrentScreen.Write("You throw the bottle at the wall, it shatters!", false);
            context.CurrentScreen.Write("The liquid splashes all over the place, even on you!");
            context.CurrentScreen = new EnlightenedScreen();
            return TaskStatus.Success;
        }

        public static TaskStatus SlashBottle(AIContext context)
        {
            context.CurrentScreen.Write("You swing your sword furiously at the bottle, it shatters!", false);
            context.CurrentScreen.Write("The liquid splashes all over the place, even on you!");
            context.CurrentScreen = new EnlightenedScreen();
            return TaskStatus.Success;
        }

        public static TaskStatus SipEmptyBottle(AIContext context)
        {
            context.CurrentScreen.Write("The bottle is empty", false);
            return TaskStatus.Success;
        }

        public static TaskStatus GetSword(AIContext context)
        {
            context.CurrentScreen.Write("You pick up the sword", false);
            return TaskStatus.Success;
        }

        public static TaskStatus Write(AIContext context, string text)
        {
            context.CurrentScreen.Write(text, false);
            return TaskStatus.Success;
        }

        public static TaskStatus Failed(AIContext context)
        {
            context.CurrentScreen.Write("Sorry, I don't know how to do that yet!", false);
            return TaskStatus.Success;
        }

        public static void TryCompleteGoal(AIContext context, GoalState endOfGoal, EffectType type)
        {
            if (context.HasGoal(endOfGoal))
            {
                context.SetGoal(GoalState.None, true, type);
            }
        }

        public static void ChangeGoal(AIContext context, EffectType type, GoalState goal)
        {
            context.SetGoal(goal, true, type);
        }
    }
}