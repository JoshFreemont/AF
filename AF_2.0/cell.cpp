//
//  cell.cpp
//  AF_2.0
//
//  Created by josh freemont on 29/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "cell.h"

//Default Constructor
//left with no argument so cell class may be used as a dereferrenced pointer.
cell::cell(){ RP = exFrame = coord1D = -1; Eta_N = Eta_S = Epsilon = 0.0;}

//Init
//Initialize experimental properties of cell:refractory period, excitation frame, Eta and Epsilon (also reset coord1D)
void cell::init(int RPInit){RP = RPInit; exFrame = -RPInit-1; coord1D = -1; Eta_N = Eta_S = Epsilon = 0.0;}

//Set Cell Relations
//Inform cell of its position in lattice and assign pointers to neighbours.
//Obey N/S continuous b.c's and E/W solid b.c's.
//At solid E/W boundaries neighbour=self where (self)-excitation may never occur.
void cell::setCellRelations(cell* lattice1D, int GRIDSIZE, int index)
{
    //capture errors
    if(index>(GRIDSIZE*GRIDSIZE))return;
    
    //set "self" coordinates, 1D and 2D
    coord1D = index;
    coord2D = index1Dto2D(coord1D, GRIDSIZE);
    
    //set excitedBy ptr to "self" by default.
    exBy = &lattice1D[index];
    
    //continuous N/S b.c's.
    int index_N = (index + GRIDSIZE*GRIDSIZE - GRIDSIZE)%(GRIDSIZE*GRIDSIZE);
    int index_S = (index + GRIDSIZE)%(GRIDSIZE*GRIDSIZE);
    
    //solid E/W b.c's.
    int index_W, index_E;
    if(index%GRIDSIZE == 0)index_W = index;//at boundary set W neighbour = self.
    else index_W = index - 1;//set W neighbour behind index.
    if(index%GRIDSIZE == GRIDSIZE-1) index_E = index;//at boundary set E neighbour = self.
    else index_E = index + 1;//set E neighbour behind index.
    
    //fill neighbours pointers with addresses of neighbours.
    neighbours = {&(lattice1D[index_N]), &(lattice1D[index_E]), &(lattice1D[index_S]), &(lattice1D[index_W])};
    
}

//Standard Sets and Gets for all params: Epsilon, EtaN, EtaN, Coord1D.
void cell::setEpsilon(double EpsilonInit){Epsilon=EpsilonInit;}
double cell::getEpsilon(){return Epsilon;}
double cell::getEtaN(){return Eta_N;}
double cell::getEtaS(){return Eta_S;}
void cell::setEtaN(double etaInit){Eta_N = etaInit;}
void cell::setEtaS(double etaInit){Eta_S = etaInit;}
int cell::getCoord1D(){return coord1D;}
std::pair<int,int> cell::getCoord2d(){return coord2D;}
cell* cell::getExBy(){return exBy;}
void cell::setExBy(cell* excitorCell){exBy = excitorCell;}

//Excite
//excite cell probabilistically- depending on individual cellular dysfunctionality.
bool cell::excite(int frame, MTRand_open& rand)
{
    //check cell is de-excited ie. it was excited at a timeframe before timeframe-RP
    if(frame - exFrame > RP)
    {
        //excite with p = 1-Epsilon, MTRand_open rand = rnd (0,1).
        if(rand()<= (1.0-Epsilon)){exFrame = frame; return true;}
    }
    return false;
}

//Excite All
//probabilistically excite neighbouring cells.
//return a vector of coordinates for successful excitations.
std::vector<int> cell::exciteAll(int frame, MTRand_open& rand)
{
    std::vector<int> excitedCoords;//container for excited coords
    
    //North Cell
    if(rand() < Eta_N)//check connection exists
    {
        //add coords to excited coords if excited
        if(neighbours[0]->excite(frame, rand))
        {
            excitedCoords.push_back(neighbours[0]->getCoord1D());
            neighbours[0]->setExBy(this);
        }
    }
    
    //East Cell
    //add coords to excited coords if excited
    if(neighbours[1]->excite(frame, rand))
    {
        excitedCoords.push_back(neighbours[1]->getCoord1D());
        neighbours[1]->setExBy(this);
    }
    
    //South Cell
    if(rand() < Eta_S)//check connection exists
    {
        if(neighbours[2]->excite(frame, rand))
        {
            excitedCoords.push_back(neighbours[2]->getCoord1D());
            neighbours[2]->setExBy(this);
        }
    }
    
    //West Cell
    //add coords to excited coords if excited
    if(neighbours[3]->excite(frame, rand))
    {
        excitedCoords.push_back(neighbours[3]->getCoord1D());
        neighbours[3]->setExBy(this);
    }
    
    return excitedCoords;//return vector of excited coords.
}

//getExChainCoords
//return chain of excitation: default length = RP.
//note: even for heterogeneous RP, chain length is limited by largest RP.
//return format: vector<int> coords = {coord1D_1, coord_1D_2 ... coord1D_N} for chainLength = N.
std::vector<int> cell::getExChainCoords(int length, int &minRepeatLength, int maxX, int maxY)
{
    //create chain vector and reserve
    std::vector<int> chain;
    chain.reserve(length);
    
    //insert current cell into chain and cache data
    cell* currentCell = this;
    chain.push_back(currentCell->getCoord1D());
    const int xInit = currentCell->getCoord2d().first;
    const int yInit = currentCell->getCoord2d().second;
    
    //fill chain with linked cells.
    for(int i = 1; i<length; i++)
    {
        //Exception Checks and Chain filling.
        //if chain extends too far away in x or y direction it is impossible for a rotor to form
        //LimitY
        if(abs(currentCell->getExBy()->getCoord2d().second - yInit) > maxY)
        {
            std::vector<int> emptyChain;
            return emptyChain;
        }
        //LimitX
        else if(abs(currentCell->getExBy()->getCoord2d().first - xInit) > maxX)
        {
            std::vector<int> emptyChain;
            return emptyChain;
        }
        //if "self-excited" return "empty chain". Chain is truncated at this cell (ie. at solid E/W boundary)
        else if(currentCell->getExBy()->getCoord1D() == currentCell->getCoord1D())
        {
            std::vector<int> emptyChain;
            return emptyChain;
        }
        //if not "self-excited" or out of bounds:
        //re-assign current cell to next link in chain and fill chain vector.
        else
        {
            currentCell = currentCell->getExBy();//jump along chain by one link
            chain.push_back(currentCell->getCoord1D());//push back coordinates of next cell.
        }
    }
    
    minRepeatLength = RP;//set minimum repeat length.
    return chain;
}



