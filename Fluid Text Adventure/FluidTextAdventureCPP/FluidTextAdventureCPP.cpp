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

    auto player = MakeSharedPtr<Player>();
    player->Init();

    SharedPtr<GameScreen> nextScreen = MakeSharedPtr<IntroScreen>();
    while (nextScreen)
    {
        nextScreen = nextScreen->Run(*(player->Context()));
    }
    Console::WriteLine("The End!");
    Console::ReadKey();
}
