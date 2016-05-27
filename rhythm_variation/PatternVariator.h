//
//  PatternVariator.h
//  rhythmpattern
//
//  Created by Richard Vogl on 24/05/16.
//
//

#ifndef rhythmpattern_PatternVariator_h
#define rhythmpattern_PatternVariator_h

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#include "Matrix.h"
#include "Pattern.h"
#include <cstdio>
#include <cstdlib>

#include "Sequencer.h"

#define MAX_NUM_GIBBS 500
#define MAX_STABLE_COUNT 15


// #define _DEBUG
// #define _DEBUG_TIMING

namespace pvr {
    class PatternVariator {
        
    private:
        const int nrVisible = 64;
        const int nrHidden = 500;
        const int nrNewPatterns = 32;
        
        double ** weights;
        double ** biases_v;
        double ** biases_h;
        
        int createRBMVariations(double ** input, double ** newPatterns, const int nrPatterns, double ** distances);
        void gibbsStep(double ** input, double ** hidden);
        
        int activationsForPattern(double * pattern);
        
        int activationsForPattern(double ** pattern);
        
        std::vector<RhythmPattern *> * createPatternVecFromMatrix(double ** newPatterns, int nrNewPatterns, double ** originalPatter, int & originalPatternIdx, double ** distances);
        
        double patternDistance(double ** input, double * patternB, int instr, int &distance, int &extra, int &missing, int &activationCount);
        bool hasMinMaxPattern(double ** pattern, int startIdx, int endIdx, double threshold, const int min, const int max);
        bool isSaneSnarePattern(double ** pattern, int startIdx, int endIdx, double threshold);
        bool isSaneBasePattern(double ** pattern, int startIdx, int endIdx, double threshold);
        std::string getStringForPattern(double ** input, double thres = 0.5);
        std::string getStringForPattern(double * input, double thres = 0.5);
        void sortInstruments(double ** newPatterns, double ** distances, int nrPatterns);
        
        void sigmoidFunction(double ** matrix, int rows, int cols);
        void randBinarization(double ** matrix, int rows, int cols);
        RhythmPattern * createEventList(double ** mat, int patternNr, bool transpose = false);
    public:
        void loadRBMMatricesAsResources(std::string hidden_biases_filename, std::string visible_biases_filename, std::string weights_filename);
        PatternVariator();
        
        std::vector<RhythmPattern *> * variate(RhythmPattern * pattern, int & originalPatternIdx);
    };
}

#endif
