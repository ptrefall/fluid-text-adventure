#pragma once
#include "Items/Item.h"

class AIContext;

class Bottle : public Item
{
    std::string _description = 
        "In front of you there is a little bottle on a table. Around the neck of the bottle there is a paper label, with "s
        "the words 'DRINK ME' beautifully printed on it in large letters."s;

public :
    virtual const std::string& Description() override { return _description; }

	Bottle(std::function<std::shared_ptr<GameScreen>(AIContext&, GoalState)> SetGoal) : Item(DefineDomain())
	{
        _KeywordsToAction = {
            {{"get", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::GetBottle);
                 }
                 return nullptr;
             }

            },
            {{"open", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::OpenBottle);
                 }
                 return nullptr;
             }},
            {{"drink", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::DrinkBottle);
                 }
                 return nullptr;
             }},
            {{"break", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::BreakBottle);
                 }
                 return nullptr;
             }},
            {{"throw", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::ThrowBottle);
                 }
                 return nullptr;
             }},
            {{"slash", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::SlashBottle);
                 }
                 return nullptr;
             }},
            {{"drop", "bottle"},
             [=](AIContext& ctx) -> std::shared_ptr<GameScreen> {
                 if (SetGoal != nullptr)
                 {
                     return SetGoal(ctx, GoalState::DropBottle);
                 }
                 return nullptr;
             }},
        };
	}
private:
    static std::shared_ptr<FluidHTN::Domain> DefineDomain();
	
};
