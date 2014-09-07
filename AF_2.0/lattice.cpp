//
//  lattice.cpp
//  AF_2.0
//
//  Created by josh freemont on 29/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "lattice.h"
#include <ctime>

//Constructor
//Initialises SinoAtrial Period, Grid Size. Reserve capacity for coordinate data.
//Set lattice1D new pointer to instances of cell class.
lattice2D::lattice2D(int SAP_INIT, const int SIZE_INIT)
{
    SAP = SAP_INIT;
    SIZE = SIZE_INIT;
    
    //vector of pairs: vector<pair<xcoord,ycoord> >
    newExCoords.reserve(SIZE*SIZE);
    oldExCoords.reserve(SIZE*SIZE);
    
    lattice1D = new cell[SIZE_INIT*SIZE_INIT];
}

//InitCells
//Initializes/Reinitializes all cells: sets parameter values to default, callibrates cell positional relationships.
//Sets defect/vertical connection locations dependent on the model type.
bool lattice2D::initCells(int RP, double defectDensity, double epsilon, std::string defectType, double vertDensity, double eta, std::string vertType, MTRand_open& rand)
{
    //Initialize/Re-Initialize.
    for (int i = 0; i<SIZE*SIZE; i++)
    {
        lattice1D[i].init(RP);//set individual values.
        lattice1D[i].setCellRelations(lattice1D, SIZE, i);//calibrate cell position and link to neighbours
    }

    //Set Defect Locations
    if(defectType == "static")//static defects.
    {
        double measuredDensity = 0.0;
        int defectCount = 0;
        int index = 0;
        
        //verify measured density never greater than required density.
        while (measuredDensity < defectDensity)
        {
            //case epsilon = 0.0 - epsilon set to 0.0 above, so break.
            if(epsilon == 0.0) break;
            
            //case epsilon != 0.0 - (rnd select && unique select).
            if(rand() < defectDensity && lattice1D[index].getEpsilon() != epsilon)
            {
                lattice1D[index].setEpsilon(epsilon);
                defectCount++;//increment defect count
                measuredDensity = defectCount/(double)(SIZE*SIZE);//recalculate measured density
            }
            index = (index+1)%(SIZE*SIZE);//increment index, periodic with total length of lattice1D
        }
    }
    
    else if(defectType == "dynamic")//dynamic defects
    {
        for(int i = 0; i<SIZE*SIZE; i++){lattice1D[i].setEpsilon(epsilon*defectDensity);}
    }

    else return false; //if unknown defect type.
    
    //Set Vert Locations
    if(vertType == "static")//static verts
    {
        double measuredDensity = 0.0;
        int vertCount = 0;
        int index = 0;
        
        //verify that measured density is never greater than required density.
        while (measuredDensity < vertDensity)
        {
            //case eta = 0.0 - eta set to 0.0 above, so break.
            if(eta == 0.0) break;//
            
            //case eta != 0.0 - (rnd select && unique select)
            if(rand() < vertDensity && lattice1D[index].getEtaN() != eta)
            {
                //determine north index with continuous BCs.
                int index_N = (index + (SIZE*SIZE) - SIZE)%(SIZE*SIZE);
                
                //set north connection on current cell == south connection on north cell.
                lattice1D[index].setEtaN(eta);
                lattice1D[index_N].setEtaS(eta);
                vertCount++;//increment vert count
                measuredDensity = vertCount/(double)(SIZE*SIZE);//recalculate measured density.
            }
            index = (index+1)%(SIZE*SIZE);
        }
    }

    else if(vertType == "dynamic")//dynamic verts
    {
        for(int i = 0; i<SIZE*SIZE; i++){lattice1D[i].setEtaN(eta); lattice1D[i].setEtaS(eta);}
    }
    
    else return false;//unknown vert type
    
    return true;//successful initiation
}

//Iterate
//Method carries out one step iteration: takes old excited cell excites neighbours probabilistically.
//Stores new excited coordinates.
void lattice2D::iterate(int frame, MTRand_open &rand)
{
    //iterate through oldExCoords to calculate newExCoords.
    for(auto coords = oldExCoords.begin(); coords != oldExCoords.end(); coords++)
    {
        //find previously excited cell and calculate subsequent excitation of neighbours.
        int index1D = coords->first + SIZE*coords->second;//find 1d index.
        std::vector<int> exctdNeighbrs = lattice1D[index1D].exciteAll(frame, rand);//return excited coords
        
        //iterate through exctdNeighbours and add to newExCoords.
        for(auto neighbrs = exctdNeighbrs.begin(); neighbrs != exctdNeighbrs.end(); neighbrs++)
        {
            newExCoords.push_back(index1Dto2D(*neighbrs, SIZE));
        }
    }
}

//Pacemaker
//Periodically excites the Westmost cells, still according to the rules of excitation.
void lattice2D::pacemaker(int frame, MTRand_open& rand)
{
    if(frame%SAP==0)
    {
        for (int j = 0; j<SIZE*SIZE; j+=SIZE)
        {
           if(lattice1D[j].excite(frame, rand))
           {
               newExCoords.push_back(std::make_pair(0, (int)j/SIZE));
               lattice1D[j].setExBy(&lattice1D[j]);
           }
        }
    }
}

//Update
//Updates excited coordinate memory.
void lattice2D::update()
{
    oldExCoords.clear();
    oldExCoords = newExCoords;
    newExCoords.clear();
}

//getExcitedCoords
//Returns coordinates of currently excited cells.
std::vector<int> lattice2D::getExcitedCoords()
{
    std::vector<int> coords;
    coords.reserve(SIZE*SIZE);
    for(auto it = newExCoords.begin(); it !=newExCoords.end(); it++)
    {
        coords.push_back(it->first);
        coords.push_back(it->second);
    }
    return coords;
}

//DetectRtrs
//do not care about rotor identification.
//for each excited cell site get chain of excited cells which may form a rotor
std::vector<int> lattice2D::detectRtrs(int maxLength, int maxX, int maxY)
{
    //declare rotors pixel data.
    std::vector<int> rotors;
    for(auto coord = newExCoords.begin(); coord != newExCoords.end(); coord++)
    {
        //declare local variables.
        int exCoord1D = index2Dto1D(*coord, SIZE);
        int minRepeatLength = 1;
        
        //declare temp chain vector
        std::vector<int> chain;
        chain.reserve(maxLength);//reserve max rotor length
        chain = lattice1D[exCoord1D].getExChainCoords(maxLength, minRepeatLength, maxX, maxY);//fill chain with coords.
        
        //check excited chain is physical
        if(!chain.size()) continue;//if chain length==0: skip to next coords.
        //look for repeats.
        for(int i = minRepeatLength; i < maxLength; i++)
        {
            if(chain[i-1] == exCoord1D)
            {
                chain.erase(chain.begin()+i, chain.end());//erase elements beyond the repeat.
                for(auto it = chain.begin(); it != chain.end(); it++)
                {
                    //return vector in format {x1, y1, ... xN, yN}.
                    rotors.push_back(index1Dto2D(*it, SIZE).first);
                    rotors.push_back(index1Dto2D(*it, SIZE).second);
                }
            }
        }
    }
    return rotors;
}

//DetectSimpleReEntry
//Assume "simple" re-entry- ie. only one lane behaves defectively st. all vertical connections are excited from flanking lanes.
//Check for respective intervals between North/South vertical connections, then check for re-entry condition to a defect.
//function returns a 2d vector, indexed [i][j], with a count of how many re-entry circuits each cell is a member.
std::vector<std::vector<int> > lattice2D::detectSimpleReEntry(int minLength)
{
    std::vector<int> reEntry1D (SIZE*SIZE, 0);//1d vector for re-entry structure count (indexed by 1d cell coord)
    
    int vertN1 = 0,vertN2 = 0,vertS1 = 0,vertS2 = 0;
    for(int i = 0; i < SIZE*SIZE; i++)//iterate over 1d lattice
    {
        //if north vertical or if at end of row.
        if(lattice1D[i].getEtaN() != 0.0 || i%SIZE == 0)
        {
            vertN2 = i;//set vertN2 to current location.
            if(vertN2 - vertN1 > minLength)//if interval is large enough for re-entry check for defects.
            {
                int reEntryCount = 0;//reEntryCount counts how many re-entry circuits are detected
                for(int j = vertN1; j <= vertN2; j++)//iterate from vertN1 forwards
                {
                    //if defect exists obeying condition for re-entry: add to reEntry count
                    if((j < vertN2 - minLength) && lattice1D[j].getEpsilon() != 0.0){reEntryCount ++;}
                    reEntry1D[j] += reEntryCount;//add count to 1d re-entry vector.
                }
                reEntryCount = 0;
                for(int j = vertN2; j >= vertN1; j--)//iterate from vertN2 backwards checking defect.
                {
                    //if defect exists obeying condition for re-entry: add to reEntry count
                    if((j > vertN1 + minLength) && lattice1D[j].getEpsilon() != 0.0){reEntryCount ++;}
                    reEntry1D[j] += reEntryCount;//add count to 1d re-entry vector.
                }
            }
            vertN1 = vertN2;//reassign vertN1
            if(!i%SIZE) vertN1++;//+1 to position if at end of row: reset vertN1 at start of next row.
        }
        
        //check south vertical
        if(lattice1D[i].getEtaS() != 0.0 || i%SIZE == 0)
        {
            vertS2 = i;
            if(vertS2 - vertS1 > minLength)//if interval is large enough for re-entry.
            {
                int reEntryCount = 0;
                for(int j = vertS1; j <= vertS2; j++)//iterate from vertS1 forwards checking defects
                {
                    //if defect exists obeying condition for re-entry: add to reEntry count
                    if((j < vertS2 - minLength) && lattice1D[j].getEpsilon() != 0.0){reEntryCount ++;}
                    reEntry1D[j] += reEntryCount;//add count to 1d re-entry vector.
                }
                
                reEntryCount = 0;
                for(int j = vertS2; j >= vertS1; j--)//iterate from vertS2 backwards checking defect.
                {
                    //if elligible defect exists add to reEntry count
                    if((j > vertS1 + minLength) && lattice1D[j].getEpsilon() != 0.0){reEntryCount ++;}
                    reEntry1D[j] += reEntryCount;//add count to 1d re-entry vector.
                }
            }
            vertS1 = vertS2;//reassign vertS1
            if(!i%SIZE) vertS1++;//+1 to position if at end of row: reset vertN1 at start of next row.
        }
    }
    
    //Return reEntry count vector in 2D.
    std::vector<std::vector<int> > reEntry2D (SIZE, std::vector<int>(SIZE,0));
    for(int i=0; i<SIZE; i++)//iterate through columns
    {
        for(int j=0; j<SIZE; j++)//iterate through rows
        {
            reEntry2D[i][j]= reEntry1D[i+SIZE*j];//add data from 1D to 2D vector.
        }
    }

    return reEntry2D;//return 2D vector.
}

