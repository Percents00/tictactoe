#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <stdexcept>

#include "../Board.h"
#include "../Bot.h"
#include "../Game.h"

void check(bool condition, const std::string& message);
void runBoardTests();
void runBotTests();