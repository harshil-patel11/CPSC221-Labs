#include "block.h"
#include <cmath>
#include <iostream>

using namespace std;

int Block::height() const {
  if (!data.empty()) {
    return data[0].size();
  }
  return 0;
}

int Block::width() const {
  if (!data.empty()) {
    return data.size();
  }

  return 0;
}

void Block::render(PNG &im, int x) const {
  for (int col = x; col <= x+width()-1; col++) {
    for (int row = 0; row <= height()-1; row++) {
      *im.getPixel(col, row) = data[col-x][row];
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  
  //https://stackoverflow.com/questions/42249303/how-can-i-push-back-data-in-a-2d-vector-of-type-int
  
  int imgHeight = im.height();

  for (int col = x; col <= x+width-1; col++) {
    vector<HSLAPixel> rowArr;
    for (int row = 0; row <= imgHeight-1; row++) {
      rowArr.push_back(*im.getPixel(col, row));
    }
    data.push_back(rowArr);
  }
}
