#pragma once
#include "Effects/Effect.h"
#include "WorldState.h"
#include "Contexts/BaseContext.h"

using namespace FluidHTN;

enum class AIWorldState
{
	HasBottle,
	BottleIsOpen,
	BottleIsEmpty,
	BottleIsBroken,
	HasWeapon,
	Goal
};
enum class GoalState
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
};

class AIWorldStateType : public IWorldState
{
protected:
    WORLDSTATEPROPERTY_VALUE_TYPE _worldState[(int)(AIWorldState::Goal)+1] = { 0 };

public:
    bool HasState(WORLDSTATEPROPERTY_ID_TYPE state, WORLDSTATEPROPERTY_VALUE_TYPE value) override
    {
        return (_worldState[value] == value);
    }


    WORLDSTATEPROPERTY_VALUE_TYPE& GetState(WORLDSTATEPROPERTY_ID_TYPE state) override
    {
        return _worldState[state];
    }


    void SetState(WORLDSTATEPROPERTY_ID_TYPE state, WORLDSTATEPROPERTY_VALUE_TYPE value) override
    {
        _worldState[state] = value;
    }


    int GetMaxPropertyCount() override
    {
        return (int)(AIWorldState::Goal)+1;
    }

};

class AIContext : public BaseContext
{
private:
    std::shared_ptr<class Player> _Player;
    std::shared_ptr<class GameScreen>  _CurrentScreen;
    std::shared_ptr<AIWorldStateType> _AIWorldState;
public:
    AIContext() = delete;
    AIContext(const std::shared_ptr<Player>& p)
    {
        _Player = p;
        _DebugMTR = false;
        _LogDecomposition = true;
        _WorldState = std::make_shared<AIWorldStateType>();
    }
    std::shared_ptr<Player>& Player() { return _Player; }
    std::shared_ptr<GameScreen>& CurrentScreen(){ return _CurrentScreen; }

    virtual void Init() override
    {
        BaseContext::Init();
        // Custom init of state
    }

    bool HasGoal(GoalState goal)
    {
        return GetGoal() == goal;
    }

    GoalState GetGoal()
    {
        return (GoalState)BaseContext::GetState((WORLDSTATEPROPERTY_ID_TYPE)AIWorldState::Goal);
    }

    void SetGoal(GoalState goal, bool setAsDirty = true, EffectType effectType = EffectType::Permanent)
    {
        SetState((int)AIWorldState::Goal, (uint8_t)goal, setAsDirty, effectType);
    }

    bool HasStateAIWS(AIWorldState state, bool value)
    {
        WORLDSTATEPROPERTY_VALUE_TYPE val = (value ? 1 : 0);
        return HasState((WORLDSTATEPROPERTY_ID_TYPE)state, val);
    }

    bool HasStateAIWS(AIWorldState state)
    {
        return HasStateAIWS(state, 1);
    }

    void SetStateAIWS(AIWorldState state, bool value, EffectType type)
    {
        SetState((int)state, (uint8_t)(value ? 1 : 0), true, type);
    }

    WORLDSTATEPROPERTY_VALUE_TYPE GetState(AIWorldState state) 
    {
        return BaseContext::GetState((WORLDSTATEPROPERTY_ID_TYPE)state);
    }
};
