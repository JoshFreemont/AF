//
//  cell.h
//  AF_2.0
//
//  Created by josh freemont on 29/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__cell__
#define __AF_2_0__cell__

#include "mtrand.h"
#include "usefulFunctions.h"
#include <cmath>
#include <iostream>
#include <vector>

//Cell class which is neighbour-aware, excitation-aware and lattice-position-aware.
//Cell stores pointers to neighbours and can carry out neighbour interactions from within the class.
//Cell can send positional information out of class to lattice.

class cell
{
    int RP;//refractory period
    double Eta_N;//dysfunctionality of north connection parameter
    double Eta_S;//dysfunctionality of south connection parameter
    double Epsilon;//dysfunctionality of a cell parameter
    int exFrame;//frame of excitation
    cell* exBy;//cell from which current cell was last excited.
    std::vector<cell*> neighbours;//vector of pointers to neighbours stored as [0]=N, [1]=E, [2]=S, [3]=W. MAKE THIS A POINTER OF POINTERS
    int coord1D;//1d lattice coordinate
    std::pair<int,int> coord2D;
    
public:
    cell();
    void init(int RP);//initialize dynamic variables
    void setCellRelations(cell* lattice1D, int SIZE, int maxY);//set positional relations, "self" and "neighbours".
    void setEpsilon(double Epsilon);
    double getEpsilon();
    void setEtaN(double Epsilon);
    void setEtaS(double Epsilon);
    double getEtaN();
    double getEtaS();
    int getCoord1D();
    std::pair<int,int> getCoord2d();
    cell* getExBy();
    void setExBy(cell*);
    bool excite(int frame, MTRand_open& rand);
    std::vector<int> getExChainCoords(int length, int &minRepeatLength, int maxX, int maxY);
    std::vector<int> exciteAll(int frame, MTRand_open& rand);
};


#endif /* defined(__AF_2_0__cell__) */
