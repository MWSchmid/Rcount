/*
This file is part of Rcount.

Rcount is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Rcount is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See <http://www.gnu.org/licenses/> for a a copy of the GNU General Public License.
*/

#ifndef TEMPORARYRESULTS_H
#define TEMPORARYRESULTS_H

//! this class is used in the readcounter for temporary result storage

class temporaryResults
{
public:
    temporaryResults();
    //~temporaryResults();

    //! sum of hits and sum of hits with a minimal distance of X to the 3prime end
    // set in first round counting
    float _sumUnAmb;
    float _sumAmb;
    float _minDistUnAmb;
    float _minDistAmb;

    // set in second round counting
    float _sumAllo;
    float _minDistAllo;

    void firstRound(const float &weight, const bool &addToMinDist, const bool &isAmb);
    void secondRound(const float &weight, const bool &addToMinDist);
    bool isValidFirstRound(const int &minReads, const int &minBelowMaxDist);
    bool isValidSecondRound(const int &minReads, const int &minBelowMaxDist);
};

#endif // TEMPORARYRESULTS_H
