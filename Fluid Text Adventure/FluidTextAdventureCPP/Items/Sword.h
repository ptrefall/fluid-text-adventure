#pragma once
#include "Items/Item.h"

class AIContext;

class Sword : public Item
{
    std::string _description = "On the ground lies a rusty, old sword.";

public :
    virtual const std::string& Description() override { return _description; }

    Sword(std::function<SharedPtr<GameScreen>(AIContext&, GoalState)> SetGoal) : Item(DefineDomain())
    {
        _KeywordsToAction = {
            {{"get", "sword"},
             [=](AIContext& ctx) -> SharedPtr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::GetSword);
                 }
                 return nullptr;
             }},
            {{"slash", "sword"},
             [=](AIContext& ctx) -> SharedPtr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::SlashAir);
                 }
                 return nullptr;
             }},
            {{"slash"},
             [=](AIContext& ctx) -> SharedPtr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::SlashAir);
                 }
                 return nullptr;
             }},
            {{"drop", "sword"},
             [=](AIContext& ctx) -> SharedPtr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::DropSword);
                 }
                 return nullptr;
             }},
        };
    }

private:
    static SharedPtr<FluidHTN::Domain> DefineDomain();
    
};
