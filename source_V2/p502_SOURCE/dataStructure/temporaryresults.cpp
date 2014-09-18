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

#include <QtGui>
#include "temporaryresults.h"

temporaryResults::temporaryResults()
{
    // first round counting
    this->_sumUnAmb = 0;
    this->_sumAmb = 0;
    this->_minDistUnAmb = 0;
    this->_minDistAmb = 0;

    // second round counting
    this->_sumAllo = 0;
    this->_minDistAllo = 0;
}

void temporaryResults::firstRound(const float &weight, const bool &addToMinDist, const bool &isAmb)
{
    if ( !isAmb ) {
        this->_sumUnAmb += weight;
        if (addToMinDist) { this->_minDistUnAmb += weight; }
    } else {
        this->_sumAmb += weight;
        if (addToMinDist) { this->_minDistAmb += weight; }
    }
}

void temporaryResults::secondRound(const float &weight, const bool &addToMinDist)
{
    this->_sumAllo += weight;
    if (addToMinDist) { this->_minDistAllo += weight; }
}

bool temporaryResults::isValidFirstRound(const int &minReads, const int &minBelowMaxDist)
{
    // check minreads and maxdist, both must be valid
    bool rval = ((this->_sumUnAmb+this->_sumAmb) >= minReads) && ((this->_minDistUnAmb + this->_minDistAmb) >= minBelowMaxDist);

    // return
    return(rval);
}

bool temporaryResults::isValidSecondRound(const int &minReads, const int &minBelowMaxDist)
{
    // check minreads and maxdist, both must be valid
    bool rval = ((this->_sumUnAmb+this->_sumAllo) >= minReads) && ((this->_minDistUnAmb + this->_minDistAllo) >= minBelowMaxDist);

    // return
    return(rval);
}
