//
//  display.h
//  AF_2.0
//
//  Created by josh freemont on 31/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__display__
#define __AF_2_0__display__

#include <iostream>
#include <vector>
#include "SDL2/SDL.h"


//add texture to display, render this texture to screenRects "coordinator class."
/*
    new code:
            construct textures within display class.
            change pixelMem vector to texture vector.
            initialize plain textures and manipulate pixels.
            only use natural height/width to define natural texture size.
            display on/off flag.
            link to rectangle in coordinator class.
 */

class display
{
    int MEM_LIMIT;
    int naturalWidth;
    int naturalHeight;
    int widthScale;
    int heightScale;
    int liveFrame;
    std::vector<std::vector<int> > pixelMem;
    SDL_Renderer* renderer;
    
public:
    display();
    display(int memoryLimit, int naturalWidthInit, int naturalHeightInit);
    bool init(SDL_Window** window);
    void clear();
    void addPixelData(int frame, std::vector<int> data);
    void print(int frame);
    void close();
 
};

#endif /* defined(__AF_2_0__display__) */
