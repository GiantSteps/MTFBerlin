#include "Pattern.h"


void convertToArray(unsigned int input, int * out) {
	for (int i = 0; i < 16; i++) {
		out[15 - i] = input & 0x01;
		input = input >> 1;
	}
}

patternProbabilities loadPatternProbabilityFile(const char* filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
	patternProbabilities pattern;
	if (file.is_open())	{
        file.seekg(0, std::ios::beg);

		char data[8];
		for (int i = 0; i < 16; i++) {
			file.read(data, 8);
			pattern.kick[i] = *((double*)data);
		}
		for (int i = 0; i < 16; i++) {
			file.read(data, 8);
			pattern.snare[i] = *((double*)data);
		}
		for (int i = 0; i < 16; i++) {
			file.read(data, 8);
			pattern.hihat[i] = *((double*)data);
		}
		for (int i = 0; i < 16; i++) {
			file.read(data, 8);
			pattern.openhihat[i] = *((double*)data);
		}
		file.close();
	}

	return pattern;
}

std::vector<drumPattern> loadPatternFile(const char* filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    std::vector<drumPattern>  patterns;
	if (file.is_open())	{
        file.seekg(0, std::ios::beg);
		char data[10];
		int readTracks = 0;

		drumPattern pattern;

		while (!file.eof())	{
			file.read(data, 10);
            pattern.kick = (uint16_t)(data[0] + data[1] * pow(2, 8));
			pattern.snare = (uint16_t)(data[2] + data[3] * pow(2, 8));
			pattern.hihat = (uint16_t)(data[4] + data[5] * pow(2, 8));
			pattern.openhihat = (uint16_t)(data[6] + data[7] * pow(2, 8));
			patterns.push_back(pattern);
			readTracks++;
		}
		file.close();
		int size = patterns.size();
	}

	return patterns;
}