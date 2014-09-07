//
//  display.cpp
//  AF_2.0
//
//  Created by josh freemont on 31/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "display.h"

//Default Constructor
display::display()
{
    MEM_LIMIT = 200;
    widthScale = 1;
    heightScale = 1;
    naturalWidth = 1;
    naturalHeight = 1;
    renderer = NULL;
    liveFrame = 0;
}

//Constructor with memory and capacity information.
//(naturalWidth)*(naturalHeight)= capacity = max pixel data (information) displayed.
display::display(int memoryLimit, int naturalWidthInit, int naturalHeightInit)
{
    MEM_LIMIT = memoryLimit;
    naturalWidth = naturalWidthInit;
    naturalHeight = naturalHeightInit;
    widthScale = 1;
    heightScale = 1;
    renderer = NULL;
    liveFrame = 0;

    //pixelMem capacity adjustment.
    std::vector<int> emptyVector;
    emptyVector.reserve(naturalWidth*naturalHeight);
    for(int i=0; i<memoryLimit; i++){pixelMem.push_back(emptyVector);}
}

//Init
//Determine scale factors from window dimensions.
//Create renderer and set renderer window (eventually renderer texture.)
bool display::init(SDL_Window** window)
{
    //Initialization flag
    bool IsSuccess = true;
    
    //assign x and y scale factors
    int W_WIDTH, W_HEIGHT;
    SDL_GetWindowSize((*window), &W_WIDTH, &W_HEIGHT);
    widthScale = W_WIDTH/naturalWidth;
    heightScale = W_HEIGHT/naturalHeight;
    if(widthScale==0) IsSuccess=false;
    if(heightScale==0) IsSuccess=false;
    
    //Create renderer for window
    renderer = SDL_CreateRenderer( *window, -1, SDL_RENDERER_ACCELERATED );
    if( renderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        IsSuccess = false;
    }
    else
    {
        //Initialize renderer color as white by default
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    }
    return IsSuccess;
}

//ClearDisplay
//Sets display colour to white.
void display::clear()
{
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );//set render colour to white by default.
    SDL_RenderClear( renderer );//clear screen with render colour as white.
}

//AddPixelData
//Clears memory for storage of CURRENT frame data. ie. clears [i] at frame=i.
//Adds pixel data in pixelMem and updates live frame.
void display::addPixelData(int frame, std::vector<int> data)
{
    pixelMem[frame%MEM_LIMIT].clear();
    pixelMem[frame%MEM_LIMIT] = data;
    liveFrame = frame;
}

//Print
//Read pixel data from pixel memory and print.
void display::print(int frame)
{
    //iterate through pixel memory for single frame
    for(auto it = pixelMem[frame%MEM_LIMIT].begin(); it != pixelMem[frame%MEM_LIMIT].end(); it+=2)
    {
        SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_Rect fillRect = { (*it)*widthScale, *(it+1)*heightScale, widthScale, heightScale};
        SDL_RenderFillRect( renderer, &fillRect );
    }
    SDL_RenderPresent( renderer );
}

//Close
//destroy renderer.
void display::close()
{
    SDL_DestroyRenderer( renderer );
    renderer = NULL;
}