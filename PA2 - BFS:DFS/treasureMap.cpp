#include "treasureMap.h"
#include "queue.h"
#include <cmath>
#include "cs221util/RGBAPixel.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s) {
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc) {
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2*(pixel->r/4);
    pixel->g = 2*(pixel->g/4);
    pixel->b = 2*(pixel->b/4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d) {
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    int modVal = d%64;
    int r = modVal >> 4;
    int g = modVal >> 2 & 0x3;
    int b = modVal & 0x3;

    pixel->r = (pixel->r & 0xfc) | r;
    pixel->g = (pixel->g & 0xfc) | g;
    pixel->b = (pixel->b & 0xfc) | b;
}

PNG treasureMap::renderMap() {
    PNG baseCopy(base); 

    //https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
    vector<vector<bool>> visited(base.width(), vector<bool> (base.height(), false));
    vector<vector<int>> distance(base.width(), vector<int> (base.height(), 0));
    // cout<<visited.size()<<endl;
    // cout<<visited[0].size()<<endl;

    Queue<pair<int, int>> queue;
    visited[start.first][start.second] = true;
    distance[start.first][start.second] = 0;
    setLOB(baseCopy, start, 0);

    queue.enqueue(start);

    while (!queue.isEmpty()) {
        pair<int, int> curr = queue.dequeue();
        vector<pair<int, int>> neighborPixels = neighbors(curr);
        
        for (pair<int, int> p : neighborPixels) {
            bool isGood = good(visited, curr, p);
            if (isGood) {
                visited[p.first][p.second] = true;
                distance[p.first][p.second] = distance[curr.first][curr.second] + 1;
                setLOB(baseCopy, p, distance[p.first][p.second]);
                queue.enqueue(p);
            }
        }

    }

    return baseCopy;
}


PNG treasureMap::renderMaze() {
    PNG baseCopy(base); 

    //https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
    vector<vector<bool>> visited(base.width(), vector<bool> (base.height(), false));

    Queue<pair<int, int>> queue;
    visited[start.first][start.second] = true;
    setGrey(baseCopy, start);

    queue.enqueue(start);

    while (!queue.isEmpty()) {
        pair<int, int> curr = queue.dequeue();
        vector<pair<int, int>> neighborPixels = neighbors(curr);
        
        for (pair<int, int> p : neighborPixels) {
            bool isGood = good(visited, curr, p);
            if (isGood) {
                visited[p.first][p.second] = true;
                setGrey(baseCopy, p);
                queue.enqueue(p);
            }
        }
    }

    for (int col = start.first-3; col <= start.first+3; col++) {
        if (col >= 0 && col < maze.width()) {
            for (int row = start.second-3; row <= start.second+3; row++) {
                if (row >= 0 && row < maze.height()) {
                    RGBAPixel *pixel = baseCopy.getPixel(col, row);
                    pixel->r = 255;
                    pixel->g = 0;
                    pixel->b = 0;
                }
            }
        }
    }

    return baseCopy;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {
    if (!(next.first < maze.width() && next.second < maze.height()
    && next.first >= 0 && next.second >= 0)) {
        return false;
    }

    if (v[next.first][next.second]) {
        return false;
    }

    RGBAPixel *currPixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel *nextPixel = maze.getPixel(next.first, next.second);

    if (*currPixel != *nextPixel) {
        return false;
    }

    return true;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int,int>> neighborPixels;

    neighborPixels.push_back(pair<int,int> (curr.first-1, curr.second));    //left
    neighborPixels.push_back(pair<int,int> (curr.first, curr.second+1));    //bottom
    neighborPixels.push_back(pair<int,int> (curr.first+1, curr.second));    //right
    neighborPixels.push_back(pair<int,int> (curr.first, curr.second-1));    //top
    
    return neighborPixels;
}
