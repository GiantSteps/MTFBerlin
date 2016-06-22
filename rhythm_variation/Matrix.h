#pragma once

#include <string>
#include <fstream>
#include <sstream>


/*!
 * Allocates memory for a matrix with double precision with given size.
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 */
double ** createMatrix(int rows, int cols);

/*!
 * Frees memory for a matrix with double precision with given size.
 *
 * @param mat Pointer to matrix.
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 */
void freeMatrix(double ** mat, int rows, int cols);

/*!
 * Creates a deep copy of a matrix with given size.
 *
 * @param mat Pointer to matrix.
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 * @return A deep copy of the provided matrix.
 */
double ** copyMatrix(double ** mat, int rows, int cols);

/*!
 * Clears a matrix with zeros.
 *
 * @param mat Pointer to matrix.
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 * @return A deep copy of the provided matrix.
 */
double ** clearMatrix(double ** mat, int rows, int cols);

/*!
 * Multiplies two matrices. Note that the number of columns of matrix one and number of rows of matrix two must be equal.
 *
 * @param m1 Pointer to first matrix.
 * @param m2 Pointer to second matrix.
 * @param res Pointer to result matrix.
 * @param rows1 Number of rows of the first matrix.
 * @param cols1 Number of columns of the first matrix.
 * @param cols2 Number of columns of the second matrix.
 * @param transpose1 If set to true, the first matrix will be transposed for multiplication.
 */
void multiplyMatrix(double ** m1, double ** m2, double ** res, int rows1, int cols1, int cols2, bool transpose1 = false);

/*!
 * Adds two matrices.
 *
 * @param m1 Pointer to first matrix.
 * @param m2 Pointer to second matrix.
 * @param rows Number of rows of the matrices.
 * @param cols Number of columns of the matrices.
 */
void addMatrix(double ** m1, double ** m2, int rows, int cols);

/*!
 * Reads matrix from text file.
 *
 * @param filename Name of file to load matrix from.
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 * @param mat Pointer to matrix.
 */
void readTextMat(std::string filename, int rows, int cols, double ** mat);

/*!
 * Write matrix to binary file.
 *
 * @param filename Name of file to load matrix from.
 * @param mat Pointer to matrix.
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 */
void writeBinaryMatrixFile(std::string filename, double ** matrix, int rows, int cols);

/*!
 * Load a resource matrix.
 *
 * @param rows Number of rows of the matrix.
 * @param cols Number of columns of the matrix.
 * @param mat Pointer to matrix.
 * @param name Name of file to load matrix from.
 */
void loadResourceMatrix(int rows, int cols, double ** mat, std::string name);

