//
//  Pattern.h
//  tenoten
//
//  Created by Richard Vogl on 31/05/16.
//
//

#ifndef __tenoten__Pattern__
#define __tenoten__Pattern__

#include <vector>

/*!
 * /struct InstNote Pattern.h
 *
 * Holds the informations for a note onset.
 */
struct InstNote {
    bool active;
    float velocity;
    float timing;
    
};

/*! InstTrack
 * Vector of InstNotes. Represents a Track for a single instrument.
 */
typedef std::vector<InstNote *> InstTrack;
/*! RhythmPattern
 * Vector of InstTracks. The single tracks represent different instruments.
 */
typedef std::vector<InstTrack *> RhythmPattern;


/*!
 * Makes a deep copy of a RhythmPattern instance.
 * @param original The pointer to the original RhythmPattern.
 * @return Returns a deep copy of the RhythmPattern pointed to.
 */
RhythmPattern * copyPattern(RhythmPattern * original);

/*!
 * Counts active notes of a RhythmPattern
 * @param pattern Pointer to the RhythmPattern for which active notes should be counted.
 * @return The number of active notes of the pattern provided.
 */
int countActiveNotes(RhythmPattern * pattern);

/*!
 * Compares two patterns, and returns true if the right pattern is greater than the left.
 * @param left The left pattern for comparision.
 * @param right The right pattern for comparision.
 * @return True if, and only if, the right pattern is greater (not equal) than the left.
 */
bool isPatternGreater(RhythmPattern * left, RhythmPattern * right );

#endif /* defined(__tenoten__Pattern__) */
