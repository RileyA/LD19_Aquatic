#include "stdafx.h"

#include <iostream>
#include "LD19.h"

#include "Game.h"

#include "TestState.h"

//int main()
//{
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    Game* game = new Game();
    game->init();

	game->addState(new TestState());

    game->go();

    delete game;

    std::cin.get();
    return 0;
}
//}
