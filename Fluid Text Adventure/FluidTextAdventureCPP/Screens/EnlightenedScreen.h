#pragma once
#include "Screens/GameScreen.h"

class EnlightenedScreen : public GameScreen
{
public:
    EnlightenedScreen()
    {
        GameScreen::Initialize(nullptr);
    }

protected:
    virtual std::shared_ptr<GameScreen> OnRun(AIContext& context) override;
    

private:
    std::shared_ptr<GameScreen> Intro(AIContext& context)
    {
        Write("The room seems to change around you.");
        Write("The walls disappear.");
        Write("The ceiling disappears.");
        Write("The floor disappears!");
        Write("You fall into the darkness...");
        return nullptr;
    }
};

