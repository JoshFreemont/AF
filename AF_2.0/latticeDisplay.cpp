//
//  latticeDisplay.cpp
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "latticeDisplay.h"

//Overloaded Base Constructor with memory and capacity information.
//(naturalW)*(naturalH)= capacity = max pixel data (information) displayed.
latticeDisplay::latticeDisplay(int memoryLimit, int naturalWInit, int naturalHInit)
{
    MEM_LIMIT = memoryLimit;
    naturalW = naturalWInit;
    naturalH = naturalHInit;
    liveFrame = 0;
    
    //pixelMem capacity adjustment.
    std::vector<int> emptyVector;
    emptyVector.reserve(naturalW*naturalH);
    for(int i=0; i<memoryLimit; i++){pixelMem.push_back(emptyVector);}
}


//AddPixelData
//Clears memory for storage of CURRENT frame data. ie. clears [i] at frame=i.
//Adds pixel data in pixelMem and updates live frame.
void latticeDisplay::addPixelData(int frame, std::vector<int> data)
{
    //insert data into pixel mem
    liveFrame = frame;//update live frame.
    pixelMem[frame%MEM_LIMIT].insert(pixelMem[frame%MEM_LIMIT].end(), data.begin(), data.end());
}

//addToRenderer
//Read pixel data from pixel memory and print.
//add origin data as Upper Left coord.
void latticeDisplay::addToRenderer(int frame, SDL_Point origin, SDL_Renderer* gRenderer)
{
    //iterate through pixel memory for single frame
    for(auto it = pixelMem[frame%MEM_LIMIT].begin(); it != pixelMem[frame%MEM_LIMIT].end(); it+=2)
    {
        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
        SDL_Rect fillRect = { *it + origin.x , *(it+1) + origin.y , 1, 1};
        SDL_RenderFillRect( gRenderer, &fillRect );
    }
}

//FreeMem
//free pixel memory for next frame's data
void latticeDisplay::freeMem(int frame)
{
    pixelMem[(frame+1)%MEM_LIMIT].erase(pixelMem[(frame+1)%MEM_LIMIT].begin(), pixelMem[(frame+1)%MEM_LIMIT].end());
}

