//
//  window.cpp
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "window.h"

//constructor
window::window()
{
    gWindow = NULL;
    gRenderer = NULL;
    targets.reserve(4);//reserve room for 4 displays by default
};


//initialize SDL, window, renderer.
bool window::init (int W_WIDTH, int W_HEIGHT, std::string title)
{
    bool IsSuccess = true;//Initialization flag
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )//Initialize SDL
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
        gWindow = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W_WIDTH, W_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            IsSuccess = false;
        }
    }
    //Create renderer for window
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        IsSuccess = false;
    }
    else
    {
        //Initialize renderer color as white by default
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    }
    return IsSuccess;
}

//refresh screen
//ClearDisplay
//Sets display colour to white and clears memory for new frame data.
void window::refresh()
{
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );//set render colour to white by default.
    SDL_RenderClear( gRenderer );//clear screen with render colour as white.
}


//add display to window
bool window::addDisplay(baseDisplay* newDisp, int x, int y, int w, int h)
{
    bool isClash = false;//clashing flag: assume no clash of displays then prove at least one.
    //iterate through and compare with current displays.
    for(auto currentDisp = targets.begin(); currentDisp != targets.end(); currentDisp++)
    {
        //check display fits in window.
        int w_wind, h_wind;
        SDL_GetWindowSize(gWindow, &w_wind, &h_wind);
        if(x < 0 || x + w > w_wind) return false;//too wide for window.
        if(y < 0 || y + h > h_wind) return false;//too tall for window.
        
        //check for x and y collisions.
        /*if(currentDisp->second.x >= x + w) continue; //currentLeft vs. newRight.
        else if(currentDisp->second.x + currentDisp->second.w <= x) continue;//currentRight vs newLeft.
        else if(currentDisp->second.y >= y + h) continue; //currentBot vs newTop.
        else if(currentDisp->second.y + currentDisp->second.h <= y) continue;//currentTop vs. new Bot.
        else isClash = true;
        
        //if clash, check for display type compatibility.
        //eg. rotor display superimposed upon excited cell display.
        if(isClash)
        {
            const std::type_info& newType = typeid(*newDisp);
            const std::type_info& currentType = typeid(currentDisp->first);
            if(newType.name() == currentType.name()) isClash = false;
        }*/ 
    }
    
    //if no clashes then add display data to window.
    if(!isClash)
    {
        SDL_Rect targetRect = {x, y, w, h};
        targets.push_back(std::make_pair(newDisp, targetRect));
    }
    return !isClash;//if isClash = true then return false, vice versa.
}

//print
void window::print(int frame)
{
    for(auto display = targets.begin(); display != targets.end(); display++)
    {
        SDL_Point origin = {display->second.x , display->second.y};
        display->first->addToRenderer(frame, origin, gRenderer);
    }
    
    SDL_RenderPresent(gRenderer);
}

//Close
//destroy renderer.
void window::close()
{
    SDL_DestroyRenderer( gRenderer );//delete renderer.
    SDL_DestroyWindow( gWindow );//delete window
	gWindow = NULL;//set window to null.
    gRenderer = NULL;
}
