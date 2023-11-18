//
// Created by Christian Sejersen on 17/11/2023.
//
#pragma once
#include "SDL.h"
#include <string>

//The window we'll be rendering to
extern SDL_Window* gWindow;
//The surface contained by the window
extern SDL_Surface* gScreenSurface;
//The image we will load and show on the screen
extern SDL_Surface* gHelloWorld;

//Starts up SDL and creates window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();

