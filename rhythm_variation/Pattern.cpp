//
//  Pattern.cpp
//  tenoten
//
//  Created by Richard Vogl on 31/05/16.
//
//

#include "Pattern.h"
#include <iostream>
#include <string>


RhythmPattern * copyPattern(RhythmPattern * original) {
    RhythmPattern * newPattern = new RhythmPattern();
    
    for (int instNr=0; instNr<original->size(); instNr++) {
        InstTrack * track = original->at(instNr);
        InstTrack * newTrack = new InstTrack();
        for (int noteNr=0; noteNr<track->size(); noteNr++) {
            InstNote * note = track->at(noteNr);
            InstNote * newNote = new InstNote();
            newNote->active = note->active;
            newNote->timing = note->timing;
            newNote->velocity = note->velocity;
            
            newTrack->push_back(newNote);
        }
        newPattern->push_back(newTrack);
    }
    return newPattern;
}

int countActiveNotes(RhythmPattern * pattern) {
    int noteCount = 0;
    
    if (pattern != nullptr) {
        int numInstruments = pattern->size();
        for (int instNr = 0; instNr < numInstruments; instNr++){
            InstTrack * track = pattern->at(instNr);
            int numNotes = track->size();
            for (int noteNr = 0; noteNr < numNotes; noteNr++) {
                if (track->at(noteNr)->active) {
                    noteCount++;
                }
            }
        }
    }
    return noteCount;
}

bool isPatternGreater(RhythmPattern * left, RhythmPattern * right ) {
    // std::cout << std::to_string((long)left) << " and " << std::to_string((long)right) << std::endl;
    return countActiveNotes(left) < countActiveNotes(right);
}