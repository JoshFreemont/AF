//
//  main.cpp
//  AF_2.0
//
//  Created by josh freemont on 29/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//
//  BIG QUESTION: can signal from current detection methods be used to generate spatial electrical activity plots?
//
//
//  TO DO:
//  big:
//          Change coordinate usage- need interchangeable 1d/2d coords.
//          ReDesign addPrintData procedure.
//          Rendering to a texture- use multiple displays +  pixel manipulation.
//          Use inheritence with different display classes and have an abstract base display class.
//          Make display class frame-aware: for rewinding or fastforwarding + adding pixel data to a frame + colour data.
//          Look  into multiple textures on top of one another.
//          Make Pixel Struct.
//          Look into parallelizing engine of atrium vs. display vs. output files.
//          colour keying for overlaying texutres - ie. colour key white for the
//          Make initialization screen
//          Screen display, file writing, experiment params.
//
//
//  small:
//          Make neighbour vector a pointer of pointers in cell class.
//          Cap frame rate.
//          Add position to display class.
//
//
#include <iostream>
#include <ctime>
#include "SDL2/SDL.h"
#include "SDLFunctions.h"
#include "lattice.h"
#include "display.h"


int main( int argc, char* args[] )
{
    //Setup SDL 2.0
    bool IsRunning = true;//global running flag
    SDL_Window* gWindow = NULL;//global window
    const int W_WIDTH = 700;//global window width
    const int W_HEIGHT = 700;//global window height
    
    //Initialize window
    if(!init(&gWindow, W_WIDTH, W_HEIGHT, "Experiment Window"))IsRunning = false;
    
    SDL_Event event;
    int frame = 0;
    
    //Experiment Params
    const int SIZE = 700;
    const int SAP = 210;
    const int RP = 50;
    const double DELTA = 0.03;
    const double EPSILON = 0.05;
    const double NU = 0.15;
    const double ETA = 1.0;
    
    //Master random number generator.
    MTRand_open mstrRnd(time(NULL));
    
    //Initialize atrium and lattice display
    lattice2D atrium(SAP, SIZE);
    atrium.initCells(RP, DELTA, EPSILON, "static", NU, ETA, "static", mstrRnd);
    display latticeDisplay(SAP, SIZE, SIZE);//display memory, naturalwidth, natural height.
    latticeDisplay.init(&gWindow);
    
    //Rotor Detector.
    std::vector<int> rotors;
    std::vector<int> exCells;
    std::vector<int> reEntry;
    std::vector<int> pixels;

    //Pre-analyse atrium
    std::vector<std::vector<int> > reEntryStructure = atrium.detectSimpleReEntry((int)RP/2);
    for(int i = 0; i<SIZE; i++)
    {
        for(int j = 0; j<SIZE; j++)
        {
            //add Re-entry structure pixel data.
            if(reEntryStructure[i][j]) {reEntry.push_back(i); reEntry.push_back(j);}
        }
    }
    
    while(IsRunning)
    {
        //Event handling
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )IsRunning = false;
        }
        
        //Clear Lattice Display
        latticeDisplay.clear();
        
        //CA Live calculation
        atrium.pacemaker(frame, mstrRnd);//pacemaker
        atrium.iterate(frame, mstrRnd);//iterate atrium
        exCells.erase(exCells.begin(), exCells.end());//erase exCells
        exCells = atrium.getExcitedCoords();//add excited cell data
        
        //Rotor detection
        rotors.erase(rotors.begin(), rotors.end());
        rotors = atrium.detectRtrs(2*RP, RP, (int)2*RP*NU);
        
        //Generate pixel vector: concatenate rotors and exCells.
        pixels.erase(pixels.begin(), pixels.end());
        pixels.insert(pixels.end(), rotors.begin(), rotors.end());
        pixels.insert(pixels.end(), exCells.begin(), exCells.end());
        //pixels.insert(pixels.end(), reEntry.begin(), reEntry.end());
        
        //Add pixel data
        latticeDisplay.addPixelData(frame, pixels);
        
        //Print Display block.
        latticeDisplay.print(frame);
        
        //Frametime updates
        atrium.update();//update atrium
        frame++;//update frame
    }
    
    //Close SDL
    latticeDisplay.close();//delete display
	SDL_DestroyWindow( gWindow );//delete window
	gWindow = NULL;//set window to null.
	SDL_Quit();//Quit SDL subsystems

    return 0;
}

