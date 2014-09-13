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
//          Rendering to a texture- use multiple displays +  pixel manipulation.
//          Make display class frame-aware: for rewinding or fastforwarding + adding pixel data to a frame + colour data.
//          Look into parallelizing engine of atrium vs. display vs. output files.
//          Make initialization screen
//          Screen display, file writing, experiment params.
//
//  small:
//          Make neighbour vector a pointer of pointers in cell class.
//          Cap frame rate.
//
//
//  questions: what are the important metrics in this project- number of rotors, spatial correlation of rotors, initiation probabilities and stabilities.

//  Simulate ECG "in equilibrium" whereby rotor location and number isn't changing. Perhaps synthesise rotors in random spatial locations and compare ECG readouts.

#include <iostream>
#include <ctime>
#include "window.h"
#include "lattice.h"
#include "LatticeDisplay.h"
#include "histDisplay2d.h"



int main( int argc, char* args[] )
{
    //Setup SDL 2.0
    bool IsRunning = true;//global running flag

    const int W_WIDTH = 1000;//global window width
    const int W_HEIGHT = 500;//global window height
    window gWindow;//declare and initialize window
    if(!gWindow.init(W_WIDTH, W_HEIGHT, "Experiment Window"))IsRunning = false;
    
    SDL_Event event;
    int frame = 0;
    
    //Experiment Params
    const int SIZE = 500;
    const int SAP = 210;
    const int RP = 50;
    const double DELTA = 0.01;
    const double EPSILON = 0.05;
    const double NU = 0.1;
    const double ETA = 1.0;
    
    //Master random number generator.
    MTRand_open mstrRnd(time(NULL));
    
    //Initialize atrium
    lattice2D atrium(SAP, SIZE);
    atrium.initCells(RP, DELTA, EPSILON, "dynamic", NU, ETA, "static", mstrRnd);
    
    std::vector<int> rotors;
    
    //Initialize Displays
    latticeDisplay latDisp(SAP, SIZE, SIZE);
    histDisplay2d rtrHist(SAP, SIZE, SIZE, 'c');
    histDisplay2d reentryHist(1, SIZE, SIZE, 'b');
    gWindow.addDisplay(&latDisp, 0, 0, SIZE, SIZE);
    gWindow.addDisplay(&reentryHist, 500, 0, SIZE, SIZE);
    gWindow.addDisplay(&rtrHist, 500, 0, SIZE, SIZE);
    
    //Pre-analyse atrium + add reentry data.
    reentryHist.addPixelData(frame, atrium.detectSimpleReEntry((int)RP/2));
    
    //Experiment loop
    while(IsRunning)
    {
        //Event handling
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )IsRunning = false;
        }

        //Clear Window
        gWindow.refresh();
        
        //CA Live calculation
        atrium.pacemaker(frame, mstrRnd);//pacemaker fire every SAP frames.
        atrium.iterate(frame, mstrRnd);//iterate atrium
        
        //Add pixel data
        latDisp.addPixelData(frame, atrium.getExcitedCoords());//excited cells pixel data
        rotors = atrium.detectRtrs(3*RP, 2*RP, (int)RP*NU);
        latDisp.addPixelData(frame, rotors);
        rtrHist.addPixelData(frame, rotors);
        
        //Print Display block.
        gWindow.print(frame);
        
        //Frametime updates
        atrium.update();//update atrium
        latDisp.freeMem(frame);//free lattice display memory.
        frame++;//update frame
    }
    
    //Close SDL
    gWindow.close();
	SDL_Quit();//Quit SDL subsystems

    return 0;
}

