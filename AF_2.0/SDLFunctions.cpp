//
//  SDLFunctions.cpp
//  AF_2.0
//
//  Created by josh freemont on 31/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "SDLFunctions.h"

bool init(SDL_Window** window, int W_WIDTH, int W_HEIGHT, std::string title)
{
    //Initialization flag
    bool IsSuccess = true;
        
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        IsSuccess = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
            
        //Create window
        *window = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            IsSuccess = false;
        }
    }
    return IsSuccess;
}