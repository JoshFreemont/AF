//
//  window.h
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__window__
#define __AF_2_0__window__

#include <iostream>
#include "SDL2/SDL.h"
#include "baseDisplay.h"

class window
{
    SDL_Window* gWindow;//global window
    SDL_Renderer* gRenderer;//global renderer
    std::vector<std::pair<baseDisplay* , SDL_Rect > > targets;//container with pairs of <display, window target> as elements
    
public:
    window();
    bool init(int W_WIDTH, int W_HEIGHT, std::string title);
    void refresh();
    bool addDisplay(baseDisplay* , int x, int y, int w, int h);
    void print(int frame);
    void close();
    
};


#endif /* defined(__AF_2_0__window__) */
