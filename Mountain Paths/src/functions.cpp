#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"
using namespace std;

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row) {
	int totalDistance = 0;
	int curRow = start_row;
	for(int i = 0; i < cols-1; ++i){ // loops through the length of the image
		int curElevation = elevations[curRow][i]; // gets the elevation after each loop
		image[curRow][i].red = color.red; // sets color for current pixel
		image[curRow][i].green = color.green;
		image[curRow][i].blue = color.blue;
		if(curRow == 0){ // if the row is at the top
			int forward = elevations[curRow][i+1];
			int southEast = elevations[curRow+1][i+1];
			int forwardDist = abs(forward-curElevation);
			int seDist = abs(southEast-curElevation);
			if(seDist < forwardDist){ // if going south east is shorter
				image[curRow+1][i+1].red = color.red;
				image[curRow+1][i+1].green = color.green;
				image[curRow+1][i+1].blue = color.blue;
				totalDistance += seDist;
				curRow += 1;
			}
			else{ // if going forward is shorter (or equal to SE)
				image[curRow][i+1].red = color.red;
				image[curRow][i+1].green = color.green;
				image[curRow][i+1].blue = color.blue;
				totalDistance += forwardDist;
			}
		}
		else if(curRow == rows-1){ // if the row is at the bottom
			int northEast = elevations[curRow-1][i+1];
			int forward = elevations[curRow][i+1];
			int forwardDist = abs(forward-curElevation);
			int neDist = abs(northEast-curElevation);
			if(neDist < forwardDist){ // if going north east is shorter
				image[curRow-1][i+1].red = color.red;
				image[curRow-1][i+1].green = color.green;
				image[curRow-1][i+1].blue = color.blue;
				totalDistance += neDist;
				curRow -= 1;
			}
			else{ // if going forward is shorter (or equal)
				image[curRow][i+1].red = color.red;
				image[curRow][i+1].green = color.green;
				image[curRow][i+1].blue = color.blue;
				totalDistance += forwardDist;
			}
		}
		else{ // if the row is anywhere in between
			int forward = elevations[curRow][i+1];
			int southEast = elevations[curRow+1][i+1];
			int northEast = elevations[curRow-1][i+1];
			int forwardDist = abs(forward-curElevation);
			int seDist = abs(southEast-curElevation);
			int neDist = abs(northEast-curElevation);
			if(forwardDist <= neDist && forwardDist <= seDist){ // if going forward is shorter and/or equal to both
				image[curRow][i+1].red = color.red;
				image[curRow][i+1].green = color.green;
				image[curRow][i+1].blue = color.blue;
				totalDistance += forwardDist;
			}
			else if(neDist < forwardDist && neDist < seDist){ // if going north east is the shortest path
				image[curRow-1][i+1].red = color.red;
				image[curRow-1][i+1].green = color.green;
				image[curRow-1][i+1].blue = color.blue;
				totalDistance += neDist;
				curRow -= 1;
			}
			else{ // if going south east is shorter than forward and shorter/equal to north east
				image[curRow+1][i+1].red = color.red;
				image[curRow+1][i+1].green = color.green;
				image[curRow+1][i+1].blue = color.blue;
				totalDistance += seDist;
				curRow += 1;
			}
		}
	}
	return totalDistance; // summed up through each loop then returned
}

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min) {
	max = elevations[0][0];
	min = elevations[0][0];
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){ // nested loop checks every value within the array
			if(elevations[i][j] >= max){
				max = elevations[i][j]; // max is set to the current index if the current index is greater than the previous max
			}
			if(elevations[i][j] <= min){
				min = elevations[i][j]; // min is set to the current index if the current index is less than the previous min
			}
		}
	}
}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData) {
	int requiredPoints = rows*cols;
	int pointsCollected = 0;
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){
			inData >> elevations[i][j]; // puts the next value of the file into the index [i][j] of elevation array
			pointsCollected++;
			if(inData.bad()){ // if there is an error in the stream
				cout << "Error: Problem reading the file." << endl;
				exit(1);
			}
			if(pointsCollected < requiredPoints && inData.eof()){ // if there is not enough data points
				cout << "Error: End of file reached prior to getting all the required data." << endl;
				exit(1);
			}
			if(inData.fail() && !inData.eof()){ // if the data in the file is not an integer
				cout << "Error: Read a non-integer value." << endl;
				exit(1);
			}
		}
	}
}

int scaleValue(int value, int max, int min) {
	if(max == min){
		return 255;
	}
	float scale = 255.0*(value-min)/(max-min);
	scale += 0.5; // setting up the value so it rounds correctly
	static_cast<int>(scale); // casting the set-up value to an integer
    return scale; // sets a value to a greyscale value
}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS], 
                    int rows, int cols, int max, int min) {
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){
			image[i][j].red = scaleValue(elevations[i][j], max, min); // sets the red value of the pixel to the greyscaled value
			image[i][j].green = scaleValue(elevations[i][j], max, min); // sets the green value of the pixel to the greyscaled value
			image[i][j].blue = scaleValue(elevations[i][j], max, min); // sets the blue value of the pixel to the greyscaled value
		}
	}
}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
	outData << "P3" << endl; // formatting the output file to match that of a ppm file
	outData << cols << " " << rows << endl;
	outData << 255 << endl;
	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < cols; ++j){
			outData << image[i][j].red << " ";
			outData << image[i][j].green << " ";
			outData << image[i][j].blue << " ";
		}
		outData << endl; // after each row of data, there is an end line
	}
}

