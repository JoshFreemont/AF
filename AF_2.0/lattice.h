//
//  lattice.h
//  AF_2.0
//
//  Created by josh freemont on 29/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__lattice__
#define __AF_2_0__lattice__

#include <iostream>
#include <vector>
#include "cell.h"
#include "mtrand.h"
#include "usefulFunctions.h"

//2d wrapper class for 1d lattice of cells.
class lattice2D
{
    int SAP;//Sino-Atrial Period
    int SIZE;//side length of lattice2D
    cell* lattice1D;//1d cell pointer.
    std::vector<std::pair<int,int> > newExCoords;//store excited coordinates in array of pairs<x,y>.
    std::vector<std::pair<int,int> > oldExCoords;//store previously excited coordinates in array of pairs<x,y>.
    
public:
    lattice2D(const int SAP, const int SIZE);//lattice constructor
    bool initCells(int RP, double defectDensity, double epsilon, std::string defectType, double vertDensity, double eta, std::string vertType, MTRand_open& rand);//cell initializer
    void iterate(int frame, MTRand_open &rand);//iterate function
    void pacemaker(int frame, MTRand_open& rand);//pacemaker function
    void update();
    
    //detection mechanisms.
    std::vector<int> detectRtrs(int maxLength, int maxX, int maxY);
    std::vector<int>  getExcitedCoords();
    std::vector<std::vector<int> > detectSimpleReEntry(int minLength);
    
    
};

#endif /* defined(__AF_2_0__lattice__) */
