//
//  baseDisplay.h
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__baseDisplay__
#define __AF_2_0__baseDisplay__

#include <iostream>
#include <vector>
#include "SDL2/SDL.h"

class baseDisplay
{
protected:
    int MEM_LIMIT;
    int naturalW;
    int naturalH;
    int liveFrame;
    int displayFrame;
    
public:
    baseDisplay();
    baseDisplay(int memoryLimit, int naturalWInit, int naturalHInit);
    virtual void addToRenderer(int displayFrame, SDL_Point origin, SDL_Renderer* gRenderer);
};



#endif /* defined(__AF_2_0__baseDisplay__) */
