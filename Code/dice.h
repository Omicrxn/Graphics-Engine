#pragma once
#include "platform.h"
struct App;


void initDice(App* app,u32(*loadProgram)(App*,const char*, const char*), u32(*loadTexture2D)(App*, const char*));
void renderDice(App* app);