#define CATCH_CONFIG_MAIN

#include <iostream>
#include <limits.h>
#include <sys/stat.h>
#include <vector>

#include "cs221util/RGBAPixel.h"
#include "cs221util/PNG.h"
#include "cs221util/catch.hpp"

#include "decoder.h"
#include "treasureMap.h"

using namespace std;
using namespace cs221util;


#define IMAGEDIR "images/"
#define SOLNIMGDIR "soln_images/"

/**
 * Test Cases
 * these are just examples of a logical flow similar to 
 * that of main.cpp. We suggest decoupling the embedding
 * tests from the decoding tests, but we leave it to you
 * to do so.
 */

/*
TEST_CASE("treasureMap::setGrey", "[weight=1][part=setGrey]") 
{
    PNG maze;
	maze.readFromFile("images/snake.png");
    pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    for (int col = 0; col < base.width(); col++) {
        for (int row = 0; row < base.height(); row++) {
            M.setGrey(base, pair<int, int> (col, row));
        }
    }

    base.writeToFile("images/testgrey.png");
}
*/

TEST_CASE("treasureMap::renderMaze", "[weight=1][part=treasureMap]") 
{
    PNG maze;
	maze.readFromFile("images/snake.png");
	pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    PNG treasuremaze = M.renderMaze();
	treasuremaze.writeToFile("images/greyedsnake2.png");
    PNG treasuremazeans;
	treasuremazeans.readFromFile("images/greyedsnake.png");
    REQUIRE( treasuremaze == treasuremazeans );
}

TEST_CASE("treasureMap::render map and maze", "[weight=1][part=treasureMap]")
{
    PNG maze;
	maze.readFromFile("images/snake.png");
	pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    PNG treasure = M.renderMap();
	treasure.writeToFile("images/embeddedsnake2.png");
    PNG treasureans;
    treasureans.readFromFile("images/embeddedsnake.png");
    REQUIRE( treasure == treasureans );

    PNG treasuremaze = M.renderMaze();
	treasuremaze.writeToFile("images/greyedsnake2.png");
    PNG treasuremazeans;
	treasuremazeans.readFromFile("images/greyedsnake.png");
    REQUIRE( treasuremaze == treasuremazeans );
}

TEST_CASE("decoder::renderMaze", "[weight=1][part=decoder]")
{
    PNG maze;
	maze.readFromFile("images/snake.png");
	pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    PNG treasure = M.renderMap();
    PNG treasureans;
    treasureans.readFromFile("images/embeddedsnake.png");

    PNG treasuremaze = M.renderMaze();
    PNG treasuremazeans;
	treasuremazeans.readFromFile("images/greyedsnake.png");

    decoder dec(treasure,start);

    PNG solnmaze = dec.renderMaze();
    solnmaze.writeToFile("images/solnsnakemaze2.png");
    PNG solnmazeans;
    solnmazeans.readFromFile("images/solnsnakemaze.png");
    REQUIRE( solnmaze == solnmazeans );
}


TEST_CASE("treasureMap::basic no cycles", "[weight=1][part=treasureMap]")
{

	PNG maze;
	maze.readFromFile("images/snake.png");
	pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    PNG treasure = M.renderMap();
	treasure.writeToFile("images/embeddedsnake2.png");
    PNG treasureans;
    treasureans.readFromFile("images/embeddedsnake.png");
    REQUIRE( treasure == treasureans );

    PNG treasuremaze = M.renderMaze();
	treasuremaze.writeToFile("images/greyedsnake2.png");
    PNG treasuremazeans;
	treasuremazeans.readFromFile("images/greyedsnake.png");
    REQUIRE( treasuremaze == treasuremazeans );

    decoder dec(treasure,start);

    PNG soln = dec.renderSolution();
    soln.writeToFile("images/solnsnake2.png");
    PNG solnans;
    solnans.readFromFile("images/solnsnake.png");
    REQUIRE( soln == solnans );

    PNG solnmaze = dec.renderMaze();
    solnmaze.writeToFile("images/solnsnakemaze2.png");
    PNG solnmazeans;
    solnmazeans.readFromFile("images/solnsnakemaze.png");
    REQUIRE( solnmaze == solnmazeans );

}


TEST_CASE("decoder::basic cycles", "[weight=1][part=decoder]")
{
	PNG maze;
	maze.readFromFile("images/maze.png");
	pair<int,int> start(1,1);

    PNG base;
    base.readFromFile("images/sunshine.png");

    treasureMap M(base, maze, start);

    PNG treasure = M.renderMap();
	treasure.writeToFile("images/embeddedmaze2.png");
    PNG treasureans;
    treasureans.readFromFile("images/embeddedmaze.png");
    REQUIRE( treasure == treasureans );

    PNG treasuremaze = M.renderMaze();
	treasuremaze.writeToFile("images/greyedmaze2.png");
    PNG treasuremazeans;
	treasuremazeans.readFromFile("images/greyedmaze.png");
    REQUIRE( treasuremaze == treasuremazeans );

    decoder dec(treasure,start);

    PNG soln = dec.renderSolution();
    soln.writeToFile("images/solnmaze2.png");
    PNG solnans;
    solnans.readFromFile("images/solnmaze.png");
    REQUIRE( soln == solnans );

    PNG solnmaze = dec.renderMaze();
    solnmaze.writeToFile("images/solnmazemaze2.png");
    PNG solnmazeans;
    solnmazeans.readFromFile("images/solnmazemaze.png");
    REQUIRE( solnmaze == solnmazeans );

}

