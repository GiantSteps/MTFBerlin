// GiantStepsAgentLib.cpp : Defines the exported functions for the DLL application.
//

#include "PatternVariator.h"

#include <string>
#include <limits.h>
#include <unistd.h>

std::string getexepath() {
    char buffer[1024];
    char *answer = getcwd(buffer, sizeof(buffer));
    std::string s_cwd;
    if (answer) {
        s_cwd = answer;
    }
    return s_cwd;
}

using namespace std;

namespace gs {
    
    void PatternVariator::loadRBMMatricesAsResources(std::string hidden_biases_filename,
                                                     std::string visible_biases_filename,
                                                     std::string weights_filename){
        loadResourceMatrix(nrHidden, nrVisible, weights, weights_filename);
        loadResourceMatrix(nrVisible, 1, biases_v, visible_biases_filename);
        loadResourceMatrix(nrHidden, 1, biases_h, hidden_biases_filename);
    }
    
    void loadRBMmatrices(int num_visible, int num_hidden, double ** weights, double ** biases_v, double ** biases_h){
        const string NAME_WEIGHTS = "E:\\weights.txt";
        const string NAME_BIAS_V = "E:\\bias_v.txt";
        const string NAME_BIAS_H = "E:\\bias_h.txt";
        
        readTextMat(NAME_WEIGHTS, num_hidden, num_visible, weights);
        readTextMat(NAME_BIAS_V, num_visible, 1, biases_v);
        readTextMat(NAME_BIAS_H, num_hidden, 1, biases_h);
        
        writeBinaryMatrixFile("weights.bin", weights, num_hidden, num_visible);
        writeBinaryMatrixFile("bias_v.bin", biases_v, num_visible, 1);
        writeBinaryMatrixFile("bias_h.bin", biases_h, num_hidden, 1);
    }
    
    void writePatterns(double ** patternMat, int patternLength, int nrPatterns) {
        ofstream file("patternOut.mat");
        
        for (int patternNr = 0; patternNr < nrPatterns; patternNr++) {
            for (int col = 0; col < patternLength; col++) {
                file << patternMat[patternNr][col];
                file << " ";
            }
            file << "\n";
        }
        
        file.close();
    }
    
    double PatternVariator::patternDistance(double ** input, double * patternB, int instr, int &distance, int &extra, int &missing, int &activationCount) {
        distance = 0;
        extra = 0;
        missing = 0;
        
        for (int idx = 0; idx < 16; idx++) {
            if (input[idx + instr * 16][0] != patternB[idx + instr * 16]) {
                distance += 1;
            }
            if (input[idx + instr * 16][0] >= 0.5 && patternB[idx + instr * 16] <= 0.5) {
                missing += 1;
            }
            if (input[idx + instr * 16][0] <= 0.5 && patternB[idx + instr * 16] >= 0.5) {
                extra += 1;
            }
            if (input[idx + instr * 16][0] >= 0.5) {
                activationCount += 1;
            }
        }
        return distance;
    }
    
    bool PatternVariator::hasMinMaxPattern(double ** pattern, int startIdx, int endIdx, double threshold, const int min, const int max){
        int activeCount = 0;
        for (int row = startIdx; row < endIdx; row++) {
            if (pattern[row][0] > threshold) {
                activeCount++;
            }
        }
        return activeCount >= min && activeCount <= max;
    }
    
    bool PatternVariator::isSaneSnarePattern(double ** pattern, int startIdx, int endIdx, double threshold){
        return hasMinMaxPattern(pattern, startIdx, endIdx, threshold, 1, 8);
    }
    
    bool PatternVariator::isSaneBasePattern(double ** pattern, int startIdx, int endIdx, double threshold){
        return hasMinMaxPattern(pattern, startIdx, endIdx, threshold, 1, 8);
    }
    
    std::string PatternVariator::getStringForPattern(double ** input, double thres) {
        std::string output = "";
        for (int instrument = 0; instrument < 4; instrument++){
            for (int note = 0; note < 16; note++) {
                output = output + (input[instrument*16+note][0] > thres ? "X " : "o ");
            }
            output = output + '\n';
        }
        return output;
    }
    
    std::string PatternVariator::getStringForPattern(double * input, double thres) {
        std::string output = "";
        for (int instrument = 0; instrument < 4; instrument++){
            for (int note = 0; note < 16; note++) {
                output = output + (input[instrument * 16 + note] > thres ? "X " : "o ");
            }
            output = output + '\n';
        }
        return output;
    }
    
    void PatternVariator::sortInstruments(double ** newPatterns, double ** distances, int nrPatterns) {
        
        // sort
        for (int instrNr = 0; instrNr < 4; instrNr++) {
            int startIdx = instrNr * 16;
            int endIdx = startIdx + 15;
            
            // bubble sort ... inefficient but simple and just right for this length
            for (int patternAIdx = 0; patternAIdx < nrPatterns; patternAIdx++) {
                for (int patternBIdx = patternAIdx + 1; patternBIdx < nrPatterns; patternBIdx++) {
                    int patternAdist = distances[patternAIdx][instrNr * 4];
                    int patternAextr = distances[patternAIdx][instrNr * 4 + 1];
                    int patternAmiss = distances[patternAIdx][instrNr * 4 + 2];
                    
                    int patternBdist = distances[patternBIdx][instrNr * 4];
                    int patternBextr = distances[patternBIdx][instrNr * 4 + 1];
                    int patternBmiss = distances[patternBIdx][instrNr * 4 + 2];
                    
                    int resultDistA = patternAdist + (patternAextr>0 ? 16 : 0) + (patternAextr>0 && patternAmiss>0 ? 32 : 0);
                    int resultDistB = patternBdist + (patternBextr>0 ? 16 : 0) + (patternBextr>0 && patternBmiss>0 ? 32 : 0);
                    
                    if (resultDistA < 2) resultDistA = 1;
                    if (resultDistB < 2) resultDistB = 1;
                    
                    if (resultDistA > resultDistB) {
                        //swap
                        double temp[16];
                        for (int note = 0; note < 16; note++) {
                            int noteIdx = note + startIdx;
                            temp[note] = newPatterns[patternAIdx][noteIdx];
                            newPatterns[patternAIdx][noteIdx] = newPatterns[patternBIdx][noteIdx];
                            newPatterns[patternBIdx][noteIdx] = temp[note];
                        }
                        
                        distances[patternAIdx][instrNr * 4] = patternBdist;
                        distances[patternAIdx][instrNr * 4 + 1] = patternBextr;
                        distances[patternAIdx][instrNr * 4 + 2] = patternBmiss;
                        
                        distances[patternBIdx][instrNr * 4] = patternAdist;
                        distances[patternBIdx][instrNr * 4 + 1] = patternAextr;
                        distances[patternBIdx][instrNr * 4 + 2] = patternAmiss;
                    }
                }
            }
        }
    }
    
    
    void PatternVariator::sigmoidFunction(double ** matrix, int rows, int cols){
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                matrix[row][col] = (1+tanh( matrix[row][col] / 2)) / 2;
            }
        }
    }
    
    void PatternVariator::randBinarization(double ** matrix, int rows, int cols){
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                double randValue = ((double)rand() / (RAND_MAX));
                if (matrix[row][col] >= randValue){
                    matrix[row][col] = 1;
                } else {
                    matrix[row][col] = 0;
                }
            }
        }
    }
    
    void PatternVariator::gibbsStep(double ** input, double ** hidden) {
        multiplyMatrix(weights, input, hidden, nrHidden, nrVisible, 1);
        addMatrix(hidden, biases_h, nrHidden, 1);
        sigmoidFunction(hidden, nrHidden, 1);
        randBinarization(hidden, nrHidden, 1);
        multiplyMatrix(weights, hidden, input, nrVisible, nrHidden, 1, true);
        addMatrix(input, biases_v, nrVisible, 1);
        sigmoidFunction(input, nrVisible, 1);
    }
    
    int PatternVariator::createRBMVariations(double ** input, double ** newPatterns, const int nrPatterns, double ** distances) {
        double ** visible = copyMatrix(input, nrVisible, 1);
        double ** hidden = createMatrix(nrHidden, 1);
        int nrCreatedPatterns = nrPatterns;
        
        const double threshold[] = { 0.6, 0.5, 0.9, 0.9 };
        
        clearMatrix(newPatterns, nrPatterns, nrVisible);
        
        for (int instrNr = 0; instrNr < 4; instrNr++) {
            int startIdx = instrNr * 16;
            int endIdx = startIdx + 16;
            
            int newPatternIdx = 0;
            int stableCount = 0;
            
#if defined(_DEBUG)
        std:string filename = "out" + std::to_string(instrNr) + ".mat";
            ofstream file(filename);
#endif
            
            for (int step = 0; step < MAX_NUM_GIBBS && newPatternIdx < nrPatterns; step++) {
                gibbsStep(visible, hidden);
                
                
                // clamping:
                for (int row = 0; row < nrVisible; row++) {
                    if (row < startIdx || row >= endIdx) {
                        visible[row][0] = input[row][0];
                    }
                }
                
                //std::cout << "\n"+getStringForPattern(visible);
                
                bool different = false;
                bool sane = true;
                bool stable = false;
                for (int row = startIdx; row < endIdx; row++) {
                    double lastVisible = 0;
                    if (newPatternIdx <= 0) {
                        lastVisible = input[row][0];
                    } else {
                        lastVisible = newPatterns[newPatternIdx - 1][row];
                    }
                    if ((visible[row][0] < threshold[instrNr] && lastVisible >= threshold[instrNr]) ||
                        (visible[row][0] >= threshold[instrNr] && lastVisible < threshold[instrNr])) {
                        different = true;
                    }
                }
                if (different) {
                    stableCount = 0;
                } else {
                    stableCount++;
                }
                
                switch (instrNr) {
                    case 0: // base
                        sane = isSaneBasePattern(visible, startIdx, endIdx, threshold[instrNr]);
                        break;
                    case 1: // snare
                        sane = isSaneSnarePattern(visible, startIdx, endIdx, threshold[instrNr]);
                        break;
                    default:
                        sane = true;
                        break;
                }
                if (stableCount > MAX_STABLE_COUNT) {
                    stable = true;
                    stableCount = 0;
                }
                bool added = false;
                if ((different && sane) || (stable && sane)) {
                    for (int row = startIdx; row < endIdx; row++) {
                        newPatterns[newPatternIdx][row] = visible[row][0] >= threshold[instrNr] ? 1 : 0;
                    }
                    int distance = 0;
                    int extra = 0;
                    int missing = 0;
                    int activationCount = 0;
                    patternDistance(input, newPatterns[newPatternIdx], instrNr, distance, extra, missing, activationCount);
                    distances[newPatternIdx][instrNr * 4] = distance;
                    distances[newPatternIdx][instrNr * 4 + 1] = extra;
                    distances[newPatternIdx][instrNr * 4 + 2] = missing;
                    distances[newPatternIdx][instrNr * 4 + 3] = activationCount;
                    
                    //std::string curPattern = getStringForPattern(newPatterns[newPatternIdx]);
                    //std::string inputPattern = getStringForPattern(input);
                    
                    newPatternIdx++;
                    added = true;
                }
#if defined(_DEBUG)
                for (int row = 0; row < nrVisible; row++) {
                    file << visible[row][0];
                    file << " ";
                }
                file << (added?"1.0":"0.0") << "\n";
#endif
            }
            nrCreatedPatterns = min(newPatternIdx + 1, nrCreatedPatterns);
#if defined(_DEBUG)
            file.close();
#endif
        }
        
        sortInstruments(newPatterns, distances, nrPatterns);
        
        freeMatrix(visible, nrVisible, 1);
        freeMatrix(hidden, nrHidden, 1);
        
        //int countMiss = 0;
        //int countExtra = 0;
        //int countBoth = 0;
        //std::string out = "";
        //for (int patternIdx = 0; patternIdx < nrPatterns; patternIdx++) {
        //	int dist = 0;
        //	int extr = 0;
        //	int miss = 0;
        //	for (int instrNr = 0; instrNr < 4; instrNr++) {
        //		dist += distances[patternIdx][instrNr * 4];
        //		extr += distances[patternIdx][instrNr * 4 + 1];
        //		miss += distances[patternIdx][instrNr * 4 + 2];
        //	}
        //	countMiss += (miss > 0 && extr <= 0 ? 1 : 0);
        //	countExtra += (miss <= 0 && extr > 0 ? 1 : 0);
        //	countBoth += (miss > 0 && extr > 0 ? 1 : 0);
        
        //	int total = extr - miss;
        //	out = out + " " + std::to_string(total) + ",";
        //}
        
        return nrCreatedPatterns;
    }
    
    
    RhythmPattern * PatternVariator::createEventList(double ** mat, int patternNr, bool transpose) {
        RhythmPattern * drumEvents = new RhythmPattern();
        
        int eventId = 0;
        for (int instNr = 0; instNr < 4; instNr++) {
            InstTrack * track = new InstTrack();
            
            for (int sixteenth = 0; sixteenth < 16; sixteenth++){
                bool active = false;
                if (transpose) {
                    active = mat[instNr * 16 + sixteenth][patternNr] > 0;
                } else {
                    active = mat[patternNr][instNr * 16 + sixteenth] > 0;
                }
                InstNote * note = new InstNote();
                note->active = active;
                note->timing = 0.0f;
                note->velocity = 1.0f;
                
                track->push_back(note);
            }
            drumEvents->push_back(track);
        }
        return drumEvents;
    }
    
    /*
     * count activations (notes played) of a single pattern
     */
    int PatternVariator::activationsForPattern(double * pattern) {
        int activations = 0;
        for (int i = 0; i < nrVisible; i++) {
            if (pattern[i] >= 0.5) {
                activations++;
            }
        }
        return activations;
    }
    
    int PatternVariator::activationsForPattern(double ** pattern) {
        int activations = 0;
        for (int i = 0; i < nrVisible; i++) {
            if (pattern[i][0] >= 0.5) {
                activations++;
            }
        }
        return activations;
    }
    
    vector<RhythmPattern *> * PatternVariator::createPatternVecFromMatrix(double ** newPatterns, int nrNewPatterns, double ** originalPatter, int & originalPatternIdx, double ** distances) {
        vector<RhythmPattern *> * patternVec = new vector<RhythmPattern *>();
        
        int nrLess;
        int nrMore;
        int activationsPattern = activationsForPattern(originalPatter);
        
        nrLess = 0;
        nrMore = 0;
        
        RhythmPattern * drumEvents;
        int patternNr = nrNewPatterns - 1;
        for (; patternNr >= 0; patternNr--) {
            if (activationsForPattern(newPatterns[patternNr]) <= activationsPattern) {
                drumEvents = createEventList(newPatterns, patternNr);
                patternVec->push_back(drumEvents);
                nrLess++;
            }
        }
        
        drumEvents = createEventList(originalPatter, 0, true);
        patternVec->push_back(drumEvents);
        originalPatternIdx = nrLess;
        
        for (patternNr += 1; patternNr < nrNewPatterns; patternNr++) {
            if (activationsForPattern(newPatterns[patternNr]) > activationsPattern) {
                drumEvents = createEventList(newPatterns, patternNr);
                patternVec->push_back(drumEvents);
                nrMore++;
            }
        }
        
        std::cout << "patterns less activations: " << nrLess << "; patterns with more activations: " << nrMore << std::endl;
        return patternVec;
    }
    
    
    PatternVariator::PatternVariator() {
        
        weights = createMatrix(nrHidden, nrVisible);
        biases_v = createMatrix(nrVisible, 1);
        biases_h = createMatrix(nrHidden, 1);
    }
    
    //eats json string, returns json string. 
    //Caller allocates memory for *out, if not enough return memory needed
    vector<RhythmPattern *> * PatternVariator::variate(RhythmPattern * pattern, int & origPatternIdx) {
        
        double ** input = createMatrix(nrVisible, 1);
        clearMatrix(input, nrVisible, 1);
        
        
        for (int instNr = 0; instNr < 4; instNr++){
            for ( int noteNr = 0; noteNr < 16; noteNr++) {
                if (pattern->at(instNr)->at(noteNr)->active) {
                    input[noteNr + instNr * 16][0] = 1;
                }
            }
        }
        double ** newPatterns = createMatrix(nrNewPatterns, nrVisible);
        double ** distances = createMatrix(nrNewPatterns, 16);
        //int patternsCreated =
        createRBMVariations(input, newPatterns, nrNewPatterns, distances);
        
#if defined(_DEBUG)
        std::cout << getStringForPattern(input);
        std::cout << getexepath();
        // write debug files
        writePatterns(newPatterns, nrVisible, nrNewPatterns);
#endif
        
        origPatternIdx = nrNewPatterns / 2;
        vector<RhythmPattern * > * patternVec = createPatternVecFromMatrix(newPatterns, nrNewPatterns, input, origPatternIdx, distances);
        
        freeMatrix(newPatterns, nrNewPatterns, nrVisible);
        freeMatrix(distances, nrNewPatterns, 16);
        
//        for (vector<RhythmPattern * >::iterator it = patternVec->begin() ; it != patternVec->end(); ++it) {
//            RhythmPattern * pat = (*it);
//            cout << pat->size()<<endl;
//        }
        
        // std::sort(patternVec->begin(), patternVec->end(), isPatternGreater);
        
        return patternVec;
    }
}