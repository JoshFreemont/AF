//
//  histDisplay2d.h
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__histDisplay2d__
#define __AF_2_0__histDisplay2d__

#include <iostream>
#include "baseDisplay.h"

class histDisplay2d:public baseDisplay
{
    char colour;
    int maxFreq;//maxFrequency, for normalization
    std::vector<std::vector<int> > pixelMem;//pixel memory or 1d vectors representing 2d lattice of bins.
    
public:
    histDisplay2d(int memoryLimit, int naturalWInit, int naturalHInit, char colour);
    void addPixelData(int frame, std::vector<int> data);
    void addPixelData(int frame, std::vector<std::vector<int> > data);
    void addToRenderer(int displayFrame, SDL_Point origin, SDL_Renderer* gRenderer);
};
#endif /* defined(__AF_2_0__histDisplay2d__) */
