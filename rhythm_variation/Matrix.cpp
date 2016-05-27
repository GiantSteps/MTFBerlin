
#include "Matrix.h"
#include <iostream>
#include <iomanip>
using namespace std;

double ** createMatrix(int rows, int cols) {
	double ** mat = new double*[rows];
	for (int r = 0; r < rows; r++){
		mat[r] = new double[cols];
	}
	return mat;
}

void freeMatrix(double ** mat, int rows, int cols) {
	for (int r = 0; r < rows; r++){
		delete[](mat[r]);
	}
	delete[](mat);
}

double ** copyMatrix(double ** in, int rows, int cols) {
	double ** out = createMatrix(rows, cols);

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			out[r][c] = in[r][c];
		}
	}
	return out;
}

double ** clearMatrix(double ** in, int rows, int cols) {
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			in[r][c] = 0;
		}
	}
	return in;
}

void multiplyMatrix(double ** m1, double ** m2, double ** res, int rows1, int cols1, int cols2, bool transpose1){
	for (int rrow = 0; rrow < rows1; rrow++) {
		for (int rcol = 0; rcol < cols2; rcol++) {
			res[rrow][rcol] = 0;
			for (int scol = 0; scol < cols1; scol++) {
				if (transpose1) {
					res[rrow][rcol] = res[rrow][rcol] + m1[scol][rrow] * m2[scol][rcol];
				}
				else {
					res[rrow][rcol] = res[rrow][rcol] + m1[rrow][scol] * m2[scol][rcol];
				}
			}
		}
	}
}

void addMatrix(double ** m1, double ** m2, int rows, int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			m1[row][col] = m1[row][col] + m2[row][col];
		}
	}
}

void readTextMat(std::string filename, int rows, int cols, double ** vector) {
	using namespace std;
    std::string      line;
    std::string      strnum;

	const std::string delim = " \t";

    std::ifstream fs(filename, ios::in);

	int lineNr = 0;

	// parse line by line
	while (getline(fs, line)) {

		//matrix->push_back(vector<double>());
		int colNr = 0;
		for (string::const_iterator i = line.begin(); i != line.end(); ++i) {
			// If i is not a delim, then append it to strnum
			if (delim.find(*i) == string::npos)	{
				strnum += *i;
				if (i + 1 != line.end()) // If it's the last char, do not continue
					continue;
			}

			// if strnum is still empty, it means the previous char is also a
			// delim (several delims appear together). Ignore this char.
			if (strnum.empty())
				continue;

			// If we reach here, we got a number. Convert it to double.
			double       number;

			istringstream(strnum) >> number;

			vector[lineNr][colNr] = number;
			// matrix->back().push_back(number);

			strnum.clear();
			colNr++;
		}
		lineNr++;
		if (lineNr >= rows) {
			break;

		}
	}

	fs.close();
}

void writeBinaryMatrixFile(std::string filename, double ** matrix, int rows, int cols) {
    std::ofstream ofile(filename, ios::out | ios::binary);
	int doubleSize = sizeof(matrix[0][0]);
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			ofile.write(reinterpret_cast<char*>(&matrix[row][col]), doubleSize);
		}
	}
	ofile.close();
}

void loadBinaryMatrixFile(std::string filename, double ** mat, int rows, int cols) {
    
    std::ifstream is (filename, ios::binary);
    if (is.is_open()) {
        // get length of file:
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);
        
        // allocate memory:
        char * buffer = new char [length];
        
        // read data as a block:
        is.read (buffer,length);
        is.close();
        
        double* double_values = reinterpret_cast<double*>(buffer);
        
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                mat[row][col] = double_values[row*cols + col];
            }
        }
        
        delete[] buffer;
    }
}


void loadResourceMatrix(int rows, int cols, double ** mat, std::string filename) {
    loadBinaryMatrixFile(filename, mat, rows, cols);
}
