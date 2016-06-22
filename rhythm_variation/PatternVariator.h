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
#include <iomanip>
#include "Matrix.h"
#include <cstdio>
#include <cstdlib>

#include "Pattern.h"

#define MAX_NUM_GIBBS 500
#define MAX_STABLE_COUNT 15


// #define _DEBUG
// #define _DEBUG_TIMING

namespace gs {
    
    /*! \class PatternVariator PatternVariator.h
     *  \brief Class which provides methods to load a variation model and create variations of rhythm patterns.
     *
     * Use the default constructor to create a new instance of a PatternVariator object. Use loadRBMMatricesAsResources to load the matrices which 
     * define the model for the pattern variation network. 
     * Variations for a seed pattern are created calling variate.
     */
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
        
        /*!
         * Loads the resources for the pattern variation network. The files to be loaded are included in the API download.
         */
        void loadRBMMatricesAsResources(std::string hidden_biases_filename, std::string visible_biases_filename, std::string weights_filename);
        PatternVariator();
        
        /* !
         * Generates variations for a provided seed pattern.
         * @param pattern The seed pattern used to create variations.
         * @param originalPatternIdx Will be set to a index of the returned list which contains the seed pattern.
         * @return A vector of RhythmPattern pointers containing the variations and the original pattern.
         */
        std::vector<RhythmPattern *> * variate(RhythmPattern * pattern, int & originalPatternIdx);
    };
}

#endif
