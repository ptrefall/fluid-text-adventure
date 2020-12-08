// FluidTextAdventureCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "StdIncludes.h"
#include "Player.h"
#include "Screens/GameScreen.h"
#include "Screens/IntroScreen.h"

WORD Console::_wStartConsoleAttributes;

int main()
{
    std::cout << "Hello World!\n";
    Console::Init();

    auto player = std::make_shared<Player>();
    player->Init();

    std::shared_ptr<GameScreen> nextScreen = std::make_shared<IntroScreen>();
    while (nextScreen)
    {
        nextScreen = nextScreen->Run(*(player->Context()));
    }
    Console::WriteLine("The End!");
    Console::ReadKey();
}
