
#pragma once

#include <vector>
#include <cmath>
#include <fstream>

struct drumPattern {
	uint16_t kick;
	uint16_t snare;
	uint16_t hihat;
	uint16_t openhihat;
};

struct patternProbabilities {
	double kick[16];
	double snare[16];
	double hihat[16];
	double openhihat[16];
};



void convertToArray(unsigned int input, int * out);

patternProbabilities loadPatternProbabilityFile(const char* filename);

std::vector<drumPattern> loadPatternFile(const char* filename);