//
//  baseDisplay.cpp
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "baseDisplay.h"

//Default Constructor
baseDisplay::baseDisplay()
{
    MEM_LIMIT = 0;
    naturalW = 1;
    naturalH = 1;
    liveFrame = 0;
}

//Constructor with memory and capacity information.
//(naturalW)*(naturalH)= capacity = max pixel data (information) displayed.
baseDisplay::baseDisplay(int memoryLimit, int naturalWInit, int naturalHInit)
{
    MEM_LIMIT = memoryLimit;
    naturalW = naturalWInit;
    naturalH = naturalHInit;
    liveFrame = 0;
}

//virtual function addToRenderer.
void baseDisplay::addToRenderer(int displayFrame, SDL_Point origin, SDL_Renderer* gRenderer) {return;}