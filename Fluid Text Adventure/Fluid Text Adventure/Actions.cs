using FluidHTN;

namespace Fluid_Text_Adventure
{
    public static class Actions
    {
        public static TaskStatus GetBottle(AIContext context)
        {
            context.CurrentScreen.Write("You pick up the bottle");
            return TaskStatus.Success;
        }

        public static TaskStatus DropBottle(AIContext context)
        {
            context.CurrentScreen.Write("You drop the bottle");
            return TaskStatus.Success;
        }

        public static TaskStatus OpenBottle(AIContext context)
        {
            context.CurrentScreen.Write("You unscrew the cork of the bottle");
            return TaskStatus.Success;
        }

        public static TaskStatus CloseBottle(AIContext context)
        {
            context.CurrentScreen.Write("You screw the cork back onto the bottle");
            return TaskStatus.Success;
        }

        public static TaskStatus SipBottle(AIContext context)
        {
            context.CurrentScreen.Write("You take a sip from the bottle");
            return TaskStatus.Success;
        }

        public static TaskStatus SipEmptyBottle(AIContext context)
        {
            context.CurrentScreen.Write("The bottle is empty");
            return TaskStatus.Success;
        }

        public static TaskStatus Failed(AIContext context)
        {
            context.CurrentScreen.Write("Sorry, I don't know how to do that yet!");
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