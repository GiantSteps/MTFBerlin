#pragma once

#include <string>
#include <fstream>
#include <sstream>


/*
*/
double ** createMatrix(int rows, int cols);

void freeMatrix(double ** mat, int rows, int cols);

double ** copyMatrix(double ** in, int rows, int cols);

double ** clearMatrix(double ** in, int rows, int cols);

void multiplyMatrix(double ** m1, double ** m2, double ** res, int rows1, int cols1, int cols2, bool transpose1 = false);

void addMatrix(double ** m1, double ** m2, int rows, int cols);

void readTextMat(std::string filename, int rows, int cols, double ** vector);

void writeBinaryMatrixFile(std::string filename, double ** matrix, int rows, int cols);

void loadResourceMatrix(int rows, int cols, double ** mat, std::string filename);