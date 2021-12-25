#include "stats.h"
#include <utility>
#include <cmath>
#include <vector>

stats::stats(PNG & im){

  sumRed.resize(im.width(), vector<long>(im.height()));
  sumGreen.resize(im.width(), vector<long>(im.height()));
  sumBlue.resize(im.width(), vector<long>(im.height()));
  sumsqRed.resize(im.width(), vector<long>(im.height()));
  sumsqGreen.resize(im.width(), vector<long>(im.height()));
  sumsqBlue.resize(im.width(), vector<long>(im.height()));

  for (int col = 0; col < im.width(); col++) {
    for (int row = 0; row < im.height(); row++) {
      RGBAPixel *pixel = im.getPixel(col, row);
      
      if (col == 0 && row == 0) {
        sumRed[col][row] = pixel->r;
        sumGreen[col][row] = pixel->g;
        sumBlue[col][row] = pixel->b;
        sumsqRed[col][row] = pixel->r * pixel->r;
        sumsqGreen[col][row] = pixel->g * pixel->g;
        sumsqBlue[col][row] = pixel->b * pixel->b;
      }

      else if (col == 0) {
        sumRed[col][row] = sumRed[col][row-1] + pixel->r;
        sumGreen[col][row] = sumGreen[col][row-1] + pixel->g;
        sumBlue[col][row] = sumBlue[col][row-1] + pixel->b;
        sumsqRed[col][row] = sumsqRed[col][row-1] + (pixel->r * pixel->r);
        sumsqGreen[col][row] = sumsqGreen[col][row-1] + (pixel->g * pixel->g);
        sumsqBlue[col][row] = sumsqBlue[col][row-1] + (pixel->b * pixel->b);
      }

      else if (row == 0) {
        sumRed[col][row] = sumRed[col-1][row] + pixel->r;
        sumGreen[col][row] = sumGreen[col-1][row] + pixel->g;
        sumBlue[col][row] = sumBlue[col-1][row] + pixel->b;
        sumsqRed[col][row] = sumsqRed[col-1][row] + pixel->r * pixel->r;
        sumsqGreen[col][row] = sumsqGreen[col-1][row] + pixel->g * pixel->g;
        sumsqBlue[col][row] = sumsqBlue[col-1][row] + pixel->b * pixel->b;
      }

      else {
        sumRed[col][row] = sumRed[col-1][row] + sumRed[col][row-1] - sumRed[col-1][row-1] + pixel->r;
        sumGreen[col][row] = sumGreen[col-1][row] + sumGreen[col][row-1] - sumGreen[col-1][row-1] + pixel->g;
        sumBlue[col][row] = sumBlue[col-1][row] + sumBlue[col][row-1] - sumBlue[col-1][row-1] + pixel->b;
        sumsqRed[col][row] = sumsqRed[col-1][row] + sumsqRed[col][row-1] - sumsqRed[col-1][row-1] + pixel->r * pixel->r;
        sumsqGreen[col][row] = sumsqGreen[col-1][row] + sumsqGreen[col][row-1] - sumsqGreen[col-1][row-1] + pixel->g * pixel->g;
        sumsqBlue[col][row] = sumsqBlue[col-1][row] + sumsqBlue[col][row-1] - sumsqBlue[col-1][row-1] + pixel->b * pixel->b;
      }
    }
  }
}

long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
  long sum = 0;

  switch (channel) {
    case 'r':
      sum += getSumHelper(sumRed, ul, w, h);
      break;
    case 'g':
      sum += getSumHelper(sumGreen, ul, w, h);
      break;
    case 'b':
      sum += getSumHelper(sumBlue, ul, w, h);
      break;    
  }
  
  return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
  long sum = 0;

  switch (channel) {
    case 'r':
      sum += getSumHelper(sumsqRed, ul, w, h);
      break;
    case 'g':
      sum += getSumHelper(sumsqGreen, ul, w, h);
      break;
    case 'b':
      sum += getSumHelper(sumsqBlue, ul, w, h);
      break;    
  }
  
  return sum;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */
  
  long red = getSumSq('r', ul, w, h) - (getSum('r', ul, w, h) * getSum('r', ul, w, h))/(w*h);
  long green = getSumSq('g', ul, w, h) - (getSum('g', ul, w, h) * getSum('g', ul, w, h))/(w*h);
  long blue = getSumSq('b', ul, w, h) - (getSum('b', ul, w, h) * getSum('b', ul, w, h))/(w*h);
  
  return (double)(red) + (double)(green) + (double)(blue);
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */
  long numPixels = w * h;
  long r = getSum('r', ul, w, h) / numPixels;
  long g = getSum('g', ul, w, h) / numPixels;
  long b = getSum('b', ul, w, h) / numPixels;

  return RGBAPixel(r, g, b);
}


long stats::getSumHelper(vector< vector< long >> & vect, pair<int,int> ul, int w, int h) {
  if (ul.first == 0 && ul.second == 0) {
    return vect[ul.first + w-1][ul.second + h-1];
  }  
  else if (ul.first == 0) {
    return vect[ul.first + w-1][ul.second + h-1] - vect[w-1][ul.second - 1];
  }
  else if (ul.second == 0) {
    return vect[ul.first + w-1][ul.second + h-1] - vect[ul.first-1][h-1];
  }
  else {
    return vect[ul.first + w-1][ul.second + h-1] - vect[ul.first-1][ul.second + h-1] 
      - vect[ul.first + w-1][ul.second-1] + vect[ul.first-1][ul.second-1];
  }
}
