#include <iostream>
#include <fstream>
#include "functions.h"
using namespace std;

//   Normally you should not use global variables. However, the stack is
//   limited in size and will not allow arrays of this size. We'll learn
//   later how to put these arrays on the heap instead. Regardless, for now
//   we'll use these and treat them as if they were declared in main. So
//   when used in functions, we'll pass them as we would any other array.
static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
	int rows = 0;
	int cols = 0;
	int max;
	int min;
	int startRow = 0;
	string fileName;
	string outputName;
	cin >> rows;
	cin >> cols;
	if(!cin.good()){ // ensures that the rows and columns were input correctly
		cout << "Error: Problem reading in rows and columns" << endl;
		return 1;
	}
	else if(rows <=0 || cols <=0){
		cout << "Error: Problem reading in rows and columns" << endl;
		return 1;
	}
	cin >> fileName;
	ifstream infs;
	infs.open(fileName);
	if(!infs.is_open()){ // ensures the file was able to be opened
		cout << "Error: Unable to open file " << fileName << endl;
		return 1;
	}
	outputName = fileName;
	outputName += ".ppm"; // adds .ppm for the output file
	int totNumPoints = rows*cols;
	int currentPoints = 0;
	ifstream check(fileName); // input file stream to check how many data points are in the file
	while(check >> fileName){
		currentPoints++;
	}
	if(currentPoints > totNumPoints){ // ensures correct amount of data
		cout << "Error: Too many data points." << endl;
		return 1;
	}
	else if(currentPoints < totNumPoints && check.eof()){ // ensures correct amount of data
		cout << "Error: End of file reached prior to getting all the required data." << endl;
		return 1;
	}
	loadData(elevations, rows, cols, infs); // loads the data into the elevation array
	findMaxMin(elevations, rows, cols, max, min); // checks for the max and min value within elevations array
	loadGreyscale(image, elevations, rows, cols, max, min); // loads the Pixel array with rgb values (in greyscale for now)
	infs.close();
	Pixel red; // creates the color red
	red.red = 252;
	red.green = 25;
	red.blue = 63;
	Pixel green; // creates the color green
	green.red = 31;
	green.green = 253;
	green.blue = 13;
	int shortestPath = colorPath(elevations, image, rows, cols, red, startRow); // initializes the shortest path
	int shortestPathIndex = startRow; // initializes the starting row
	int currentPath;
	for(int i = startRow; i < rows; ++i){ // loops through the height (rows) of the image
		currentPath = colorPath(elevations, image, rows, cols, red, i); // paints the path on row i
		if(currentPath < shortestPath){ // if the returned distance is the shortest distance
			shortestPath = currentPath; // reassign shortest path 
			shortestPathIndex = i; // reassign the index of the path
		}
	}
	shortestPath = colorPath(elevations, image, rows, cols, green, shortestPathIndex); // paints over the shortest path with green
	ofstream ofs;
	ofs.open(outputName); // opens the output file stream
	outputImage(image, rows, cols, ofs); // loads the data in ppm format into a file
	ofs.close();
	return 0;
}
