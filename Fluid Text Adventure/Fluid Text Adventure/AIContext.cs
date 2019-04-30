using System;
using System.Collections.Generic;
using FluidHTN;
using FluidHTN.Contexts;
using FluidHTN.Factory;
using Fluid_Text_Adventure.Screens;

namespace Fluid_Text_Adventure
{
    public enum AIWorldState
    {
        HasBottle,
        BottleIsOpen,
        BottleIsEmpty,
        BottleIsBroken,
        HasWeapon,
        Goal
    }

    public enum GoalState
    {
        None,
        GetBottle,
        DropBottle,
        OpenBottle,
        CloseBottle,
        DrinkBottle,
        BreakBottle,
        SlashBottle,
        ThrowBottle,
        GetSword,
        DropSword,
        SlashAir,
    }

    public class AIContext : BaseContext
    {
        private byte[] _worldState = new byte[Enum.GetValues(typeof(AIWorldState)).Length];

        public override IFactory Factory { get; set; } = new DefaultFactory();
        public override List<string> MTRDebug { get; set; }
        public override List<string> LastMTRDebug { get; set; }
        public override bool DebugMTR { get; } = false;
        public override Stack<string> DecompositionLog { get; set; }
        public override bool LogDecomposition { get; } = false;
        public override byte[] WorldState => _worldState;

        public Player Player { get; }
        public GameScreen CurrentScreen { get; set; }

        public AIContext(Player player)
        {
            Player = player;
        }

        public override void Init()
        {
            base.Init();

            // Custom init of state
        }

        public bool HasGoal(GoalState goal)
        {
            return GetGoal() == goal;
        }

        public GoalState GetGoal()
        {
            return (GoalState) GetState(AIWorldState.Goal);
        }

        public void SetGoal(GoalState goal, bool setAsDirty = true, EffectType effectType = EffectType.Permanent)
        {
            SetState((int)AIWorldState.Goal, (byte)goal, setAsDirty, effectType);
        }

        public bool HasState(AIWorldState state, bool value)
        {
            return HasState((int)state, (byte)(value ? 1 : 0));
        }

        public bool HasState(AIWorldState state)
        {
            return HasState((int)state, 1);
        }

        public void SetState(AIWorldState state, bool value, EffectType type)
        {
            SetState((int)state, (byte)(value ? 1 : 0), true, type);
        }

        public byte GetState(AIWorldState state)
        {
            return GetState((int)state);
        }
    }
}