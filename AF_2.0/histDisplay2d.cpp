//
//  histDisplay2d.cpp
//  AF_2.0
//
//  Created by josh freemont on 11/09/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "histDisplay2d.h"

histDisplay2d::histDisplay2d(int memoryLimit, int naturalWInit, int naturalHInit, char clr)
{
    MEM_LIMIT = memoryLimit;
    naturalW = naturalWInit;
    naturalH = naturalHInit;
    liveFrame = 0;
    maxFreq = 0;
    colour = clr;
    
    std::vector<int> defaultMem (naturalWInit*naturalHInit, 0);
    for(int i = 0; i < memoryLimit; i++)
    {
        pixelMem.push_back(defaultMem);
    }
}

void histDisplay2d::addPixelData(int frame, std::vector<int> data)
{
    liveFrame = frame;
    pixelMem[frame%MEM_LIMIT] = pixelMem[(frame-1+MEM_LIMIT)%MEM_LIMIT];
    for(auto it = data.begin(); it != data.end(); it+=2)
    {
        //add to bin data
        pixelMem[frame%MEM_LIMIT][*it + *(it+1)*naturalW]++;
        if(pixelMem[frame%MEM_LIMIT][*it + *(it+1)*naturalW] > maxFreq)//update max freq.
        {
            maxFreq = pixelMem[frame%MEM_LIMIT][*it + *(it+1)*naturalW];
        }
    }
}

void histDisplay2d::addPixelData(int frame, std::vector<std::vector<int> > data)
{
    liveFrame = frame;
    pixelMem[frame%MEM_LIMIT] = pixelMem[(frame-1+MEM_LIMIT)%MEM_LIMIT];
    int i = 0, j = 0;
    for(auto row = data.begin(); row != data.end(); row++)
    {
        j = 0;
        for(auto col = row->begin(); col != row->end(); col++)
        {

            pixelMem[frame%MEM_LIMIT][i + j*naturalW] += *col;
            if(pixelMem[frame%MEM_LIMIT][i + j*naturalW] > maxFreq)maxFreq = pixelMem[frame%MEM_LIMIT][i + j*naturalW];
            j++;
        }
        i++;
    }
}

void histDisplay2d::addToRenderer(int frame, SDL_Point origin, SDL_Renderer* gRenderer)
{
    switch (colour) {
        case 'c':
            for(int i = 0; i < naturalW*naturalH; i++)
            {
                double p = pixelMem[frame%MEM_LIMIT][i]/(double)maxFreq;//calculate percentage
                if(pixelMem[frame%MEM_LIMIT][i])
                {
                    SDL_SetRenderDrawColor( gRenderer, 100 + (int)155*p, 100, 100, 0xFF);
                    SDL_Rect fillRect = { i%naturalW + origin.x , i/naturalW + origin.y , 1, 1};
                    SDL_RenderFillRect( gRenderer, &fillRect );
                }
            }
            break;
            
        case 'b':
            for(int i = 0; i < naturalW*naturalH; i++)
            {
                double p = pixelMem[frame%MEM_LIMIT][i]/(double)maxFreq;//calculate percentage
                if(pixelMem[frame%MEM_LIMIT][i])
                {
                    SDL_SetRenderDrawColor( gRenderer, (int)255*(1.0-p), (int)255*(1.0-p), (int)255*(1.0-p), 0xFF );
                    SDL_Rect fillRect = { i%naturalW + origin.x , i/naturalW + origin.y , 1, 1};
                    SDL_RenderFillRect( gRenderer, &fillRect );
                }
            }
            
        default:
            break;
    }
}
