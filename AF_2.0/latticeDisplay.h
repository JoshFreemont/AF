//
//  latticeDisplay.h
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__latticeDisplay__
#define __AF_2_0__latticeDisplay__

#include <iostream>
#include "baseDisplay.h"

class latticeDisplay:public baseDisplay
{
    std::vector<std::vector<int> > pixelMem;
    
public:
    latticeDisplay(int memoryLimit, int naturalWInit, int naturalHInit);
    void addPixelData(int frame, std::vector<int> data);
    void addToRenderer(int displayFrame, SDL_Point origin, SDL_Renderer* gRenderer);
    void freeMem(int frame);
    
};
#endif /* defined(__AF_2_0__latticeDisplay__) */
