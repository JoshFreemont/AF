//
//  usefulFunctions.cpp
//  AF_2.0
//
//  Created by josh freemont on 30/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#include "usefulFunctions.h"

//convert 1d coord to 2d coord pair: divide 1d array into 2d square array of length SIZE.
//coord pair is of form pair<x,y>
std::pair<int,int> index1Dto2D(int index, int SIZE)
{
    return std::make_pair(index%SIZE, (int)index/SIZE);
}

//convert 2d coord pair to 1d coord: stretch 2d square array of length SIZE into 1d array.
//coord pair is of form pair<x,y>
int index2Dto1D(std::pair<int,int> coord, int SIZE)
{
    return coord.first + SIZE*coord.second;
}
