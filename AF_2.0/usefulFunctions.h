//
//  usefulFunctions.h
//  AF_2.0
//
//  Created by josh freemont on 30/08/2014.
//  Copyright (c) 2014 com.example. All rights reserved.
//

#ifndef __AF_2_0__usefulFunctions__
#define __AF_2_0__usefulFunctions__

#include <iostream>

//convert 1d coord to 2d coord pair: divide 1d array into 2d square array of length SIZE.
std::pair<int,int> index1Dto2D(int index, int SIZE);

//convert 2d coord pair to 1d coord: stretch 2d square array of length SIZE into 1d array.
int index2Dto1D(std::pair<int,int>, int SIZE);




#endif /* defined(__AF_2_0__usefulFunctions__) */
