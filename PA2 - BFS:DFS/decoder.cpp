#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
    :start(s),mapImg(tm) {

    //pathPts.push_back(start);    

    for (int col = 0; col < mapImg.width(); col++) {
        vector<int> rowArr;
        for (int row = 0; row < mapImg.height(); row++) {
            rowArr.push_back(getDistance(*mapImg.getPixel(col, row)));
        }
        distance.push_back(rowArr);
    }

    vector<vector<pair<int, int>>> visitedOrder;
    vector<vector<bool>> visited(mapImg.width(), vector<bool> (mapImg.height(), false));

    vector<pair<int, int>> positions;
    positions.push_back(start);
    positions.push_back(start);
    visitedOrder.push_back(positions);

    Queue<pair<int, int>> queue;
    visited[start.first][start.second] = true;
    queue.enqueue(start);

    while (!queue.isEmpty()) {
        pair<int, int> curr = queue.dequeue();
        vector<pair<int, int>> neighborPixels = neighbors(curr);
        
        for (pair<int, int> p : neighborPixels) {
            positions.clear();
            bool isGood = good(visited, distance, curr, p);
            if (isGood) {
                visited[p.first][p.second] = true;
                queue.enqueue(p);

                positions.push_back(p);
                positions.push_back(curr);
                visitedOrder.push_back(positions);
            }
        }
    }

    //cout<<visitedOrder[0].size()<<endl;

    // for (int i = 0; i < visitedOrder.size(); i++) {
    //     pathPts.push_back(visitedOrder[i][1]);
    // }

    Stack<pair<int, int>> stack;
    int index = visitedOrder.size()-1;
    stack.push(visitedOrder[index][0]);
    stack.push(visitedOrder[index][1]);

    for (int i = index-1; i >= 0; i--) {
        if (stack.peek() == visitedOrder[i][0]) {
            stack.push(visitedOrder[i][1]);
        }
        if (stack.peek() == start) {
            break;
        }
    }

    while (!stack.isEmpty()) {
        pathPts.push_back(stack.pop());
    }

}


PNG decoder::renderSolution() {
    PNG solution(mapImg);

    for (pair<int, int> p : pathPts) {
        RGBAPixel *pixel = solution.getPixel(p.first, p.second);
        pixel->r = 255;
        pixel->g = 0;
        pixel->b = 0;
    }

    return solution;
}

PNG decoder::renderMaze() {
    
    PNG mapCopy(mapImg); 

    //https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
    vector<vector<bool>> visited(mapImg.width(), vector<bool> (mapImg.height(), false));

    Queue<pair<int, int>> queue;
    visited[start.first][start.second] = true;
    setGrey(mapCopy, start);

    queue.enqueue(start);

    while (!queue.isEmpty()) {
        pair<int, int> curr = queue.dequeue();
        vector<pair<int, int>> neighborPixels = neighbors(curr);
        
        for (pair<int, int> p : neighborPixels) {
            bool isGood = good(visited, distance, curr, p);
            if (isGood) {
                visited[p.first][p.second] = true;
                setGrey(mapCopy, p);
                queue.enqueue(p);
            }
        }
    }

    for (int col = start.first-3; col <= start.first+3; col++) {
        if (col >= 0 && col < mapCopy.width()) {
            for (int row = start.second-3; row <= start.second+3; row++) {
                if (row >= 0 && row < mapCopy.height()) {
                    RGBAPixel *pixel = mapCopy.getPixel(col, row);
                    pixel->r = 255;
                    pixel->g = 0;
                    pixel->b = 0;
                }
            }
        }
    }

    return mapCopy;
    
}

void decoder::setGrey(PNG & im, pair<int,int> loc) {
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

pair<int,int> decoder::findSpot() {
    return pathPts[pathPts.size()-1];
}

int decoder::pathLength() {
    return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next) {
    if (!(next.first < mapImg.width() && next.second < mapImg.height()
    && next.first >= 0 && next.second >= 0)) {
        return false;
    }

    if (v[next.first][next.second]) {
        return false;
    }

    RGBAPixel *nextPixel = mapImg.getPixel(next.first, next.second);
    if (!compare(*nextPixel, d[curr.first][curr.second])) {
        return false;
    }

    return true;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> neighborPixels;

    neighborPixels.push_back(pair<int,int> (curr.first-1, curr.second));    //left
    neighborPixels.push_back(pair<int,int> (curr.first, curr.second+1));    //bottom
    neighborPixels.push_back(pair<int,int> (curr.first+1, curr.second));    //right
    neighborPixels.push_back(pair<int,int> (curr.first, curr.second-1));    //top
    
    return neighborPixels;
}


bool decoder::compare(RGBAPixel p, int d) {
    int mazeVal = getDistance(p);
    return (mazeVal == (d+1)%64); 
}

int decoder::getDistance(RGBAPixel p) {
    int r = p.r % 4;
    int g = p.g % 4;
    int b = p.b % 4;

    return r*16 + g*4 + b;
}
