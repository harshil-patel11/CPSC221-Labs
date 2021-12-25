/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"
#include <float.h>

using namespace std;

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.
  stats stat(imIn);
  pair<int, int> ul(0, 0);

  root = buildTree(stat, ul, imIn.width(), imIn.height(), tol);  
}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  // Your code here.
  
  Node *subRoot = new Node(s, ul, w, h);
  subRoot->var = s.getVar(ul, w, h);
  subRoot->avg = s.getAvg(ul, w, h);

  if (subRoot->var <= tol || (w == 1 && h == 1)) {
    return subRoot;
  }

  double minVar = DBL_MAX;

  pair<int, int> minTopLeft(ul.first, ul.second);
  pair<int, int> minTopLeftWH(0, 0);                //WH = width, height
  pair<int, int> minTopRight(ul.first, ul.second);
  pair<int, int> minTopRightWH(0, 0);
  pair<int, int> minBottomLeft(ul.first, ul.second);
  pair<int, int> minBottomLeftWH(0, 0);
  pair<int, int> minBottomRight(ul.first, ul.second);
  pair<int, int> minBottomRightWH(0, 0);

  if (w == 1) {
    for (int row = ul.second + 1; row < ul.second + h; row++) {
      pair<int, int> topLeft(ul.first, ul.second);
      pair<int, int> bottomLeft(ul.first, row);

      //partition into top bottom
      double varTopLeft = s.getVar(topLeft, 1, row - ul.second);
      double varBottomLeft = s.getVar(bottomLeft, 1, h + ul.second - row);
      double totalVar = varTopLeft + varBottomLeft;

      if (totalVar < minVar) {
        minVar = totalVar;

        minTopLeft = topLeft;
        minTopLeftWH.first = 1;
        minTopLeftWH.second = row - ul.second;
        
        minBottomLeft = bottomLeft;
        minBottomLeftWH.first = 1;
        minBottomLeftWH.second = h + ul.second - row;
      }
    }
  }

  else if (h == 1) {
    for (int col = ul.first + 1; col < ul.first + w; col++) {
      pair<int, int> topLeft(ul.first, ul.second);
      pair<int, int> topRight(col, ul.second);

      //partition into left right
      double varTopLeft = s.getVar(topLeft, col - ul.first, 1);
      double varTopRight = s.getVar(topRight, w + ul.first - col, 1);
      double totalVar = varTopLeft + varTopRight;

      if (totalVar < minVar) {
        minVar = totalVar;

        minTopLeft = topLeft;
        minTopLeftWH.first = col - ul.first;
        minTopLeftWH.second = 1;

        minTopRight = topRight;
        minTopRightWH.first = w + ul.first - col;
        minTopRightWH.second = 1;
      }
    }
  }

  else {
    for (int col = ul.first + 1; col < ul.first + w; col++) {  
      for (int row = ul.second + 1; row < ul.second + h; row++) {          
        pair<int, int> topLeft(ul.first, ul.second);
        pair<int, int> topRight(col, ul.second);
        pair<int, int> bottomLeft(ul.first, row);
        pair<int, int> bottomRight(col, row);

        //partition into 4
        double quadVarTopLeft = s.getVar(topLeft, col - ul.first, row - ul.second);
        double quadVarTopRight = s.getVar(topRight, w + ul.first - col, row - ul.second);
        double quadVarBottomLeft = s.getVar(bottomLeft, col - ul.first, h + ul.second - row);
        double quadVarBottomRight = s.getVar(bottomRight, w + ul.first - col, h + ul.second - row);
        double quadVar = quadVarTopLeft + quadVarTopRight + quadVarBottomLeft + quadVarBottomRight;

        //partition into left right
        double lrVarTopLeft = s.getVar(topLeft, col - ul.first, h);
        double lrVarTopRight = s.getVar(topRight, w + ul.first - col, h);
        double lrVar = lrVarTopLeft + lrVarTopRight;

        //partition into top bottom
        double tbVarTopLeft = s.getVar(topLeft, w, row - ul.second);
        double tbVarBottomLeft = s.getVar(bottomLeft, w, h + ul.second - row);
        double tbVar = tbVarTopLeft + tbVarBottomLeft;

        double minTotalVar = min(quadVar, min(lrVar, tbVar));
        // double minTotalVar = quadVar;
        // if (lrVar < quadVar || tbVar < quadVar) {
        //   printf("got smaller variance with less divisions\n");
        // }

        if (minTotalVar <= minVar) {
          minVar = minTotalVar;

          if (minTotalVar == quadVar) {
            minTopLeft = topLeft;
            minTopLeftWH.first = col - ul.first;
            minTopLeftWH.second = row - ul.second;

            minTopRight = topRight;
            minTopRightWH.first = w + ul.first - col;
            minTopRightWH.second = row - ul.second;
            
            minBottomLeft = bottomLeft;
            minBottomLeftWH.first = col - ul.first;
            minBottomLeftWH.second = h + ul.second - row;

            minBottomRight = bottomRight;
            minBottomRightWH.first = w + ul.first - col;
            minBottomRightWH.second = h + ul.second - row;
          }

          else if (minTotalVar == lrVar) {
            minTopLeft = topLeft;
            minTopLeftWH.first = col - ul.first;
            minTopLeftWH.second = h;

            minTopRight = topRight;
            minTopRightWH.first = w + ul.first - col;
            minTopRightWH.second = h;

            minBottomLeftWH.first = 0;
            minBottomLeftWH.second = 0;

            minBottomRightWH.first = 0;
            minBottomRightWH.second = 0;
          }

          else {
            minTopLeft = topLeft;
            minTopLeftWH.first = w;
            minTopLeftWH.second = row - ul.second;

            minBottomLeft = bottomLeft;
            minBottomLeftWH.first = w;
            minBottomLeftWH.second = h + ul.second - row;

            minTopRightWH.first = 0;
            minTopRightWH.second = 0;

            minBottomRightWH.first = 0;
            minBottomRightWH.second = 0;
          }
        }
      }
    }
  }

  if (minTopLeftWH.first != 0 && minTopLeftWH.second != 0) 
    subRoot->NW = buildTree(s, minTopLeft, minTopLeftWH.first, minTopLeftWH.second, tol);
  else
    subRoot->NW = NULL;
  
  if (minTopRightWH.first != 0 && minTopRightWH.second != 0)
    subRoot->NE = buildTree(s, minTopRight, minTopRightWH.first, minTopRightWH.second, tol);
  else 
    subRoot->NE = NULL;

  if (minBottomLeftWH.first != 0 && minBottomLeftWH.second != 0) 
    subRoot->SW = buildTree(s, minBottomLeft, minBottomLeftWH.first, minBottomLeftWH.second, tol);
  else
    subRoot->SW = NULL;

  if (minBottomRightWH.first != 0 && minBottomRightWH.second != 0)
    subRoot->SE = buildTree(s, minBottomRight, minBottomRightWH.first, minBottomRightWH.second, tol);
  else
    subRoot->SE = NULL;

  return subRoot;
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  // Your code here.
  PNG img(root->width, root->height);
  renderImg(img, root);
  return img;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
  clearHelper(root);
}

void SQtree::copy(const SQtree & other) {
  // Your code here.
  root = copyHelper(other.root);  
}

SQtree::Node * SQtree::copyHelper(const Node * rootOther) {

  if (rootOther == NULL) {
    return NULL;
  }

  Node * rootNew = new Node(rootOther->upLeft, rootOther->width, rootOther->height, rootOther->avg, rootOther->var);
  rootNew->NW = copyHelper(rootOther->NW);
  rootNew->NE = copyHelper(rootOther->NE);
  rootNew->SW = copyHelper(rootOther->SW);
  rootNew->SE = copyHelper(rootOther->SE);

  return rootNew;

}

int SQtree::size() {
  // Your code here.
  return sizeHelper(root);
}

int SQtree::sizeHelper(Node *& root) {
  if (root == NULL) return 0;
  return 1 + sizeHelper(root->NW) + sizeHelper(root->NE) + sizeHelper(root->SW) + sizeHelper(root->SE);
}

void SQtree::clearHelper(Node *& root) {
  if (root == NULL) {
    return;
  }

  clearHelper(root->NW);
  clearHelper(root->NE);
  clearHelper(root->SW);
  clearHelper(root->SE);

  delete root;
  root = NULL;
}

void SQtree::renderImg(PNG &img, Node *& root) { 
  if (root == NULL) {
    return;
  } 

  if (root->NW == NULL && root->NE == NULL && root->SW == NULL && root->SE == NULL) {
    pair<int,int> & ul = root->upLeft;
    int w = root->width;
    int h = root->height;

    for (int col = ul.first; col < ul.first + w; col++) {
      for (int row = ul.second; row < ul.second + h; row++) {
        RGBAPixel *pixel = img.getPixel(col, row);
        *pixel = root->avg;
      }
    }

    return;
  }

  renderImg(img, root->NW);
  renderImg(img, root->NE);
  renderImg(img, root->SW);
  renderImg(img, root->SE);
}

