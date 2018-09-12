#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <set>
#include <ctime>

#include "countreads.h"

countReads::countReads(const std::string &fileNameArg, const bool &distAmbArg, const int &maxDistArg, const int &minHitsArg)
{
    this->fileName = fileNameArg;
    this->distAmb = distAmbArg;
    this->maxDist = maxDistArg;
    this->minHits = minHitsArg;
}

//countReads::~countReads(){}

//! this function is used to get a map of chromosomes (int to string)
std::map<int, std::string> countReads::getChromIDtranslator(BamTools::BamReader& reader) {
        std::map<int, std::string> IDtoName; // a map with chromID as key and chromName as value
        std::string chromName;
        int chromID;

        BamTools::RefVector refDat = reader.GetReferenceData(); // a vector with the chromosomes
        BamTools::RefVector::iterator ref; // iterator for refDat

        for ( ref = refDat.begin(); ref != refDat.end(); ++ref ) {
                chromName = ref->RefName;
                chromID = reader.GetReferenceID(chromName);
                IDtoName[chromID] = chromName;
        }

        return(IDtoName);
}


//! the run function
bool countReads::run()
{
    BamTools::BamReader reader; // the filehandler
    BamTools::BamAlignment al; // an alignment
    std::map< int, std::string > IDtoName; // a map with chromID as key and chromName as value (taken from the bamfile)

    // try to open the file
    if ( !reader.Open(this->fileName) ) {
        std::cerr << "could not open the bamfile" << std::endl << std::flush;
        return(false);
    }

    // try to open an Index - otherwise create it
    if ( !reader.LocateIndex() ) {
        if ( !reader.CreateIndex() ) {
            return(false);
        }
    }

    // load the chrom ID translator
    IDtoName = this->getChromIDtranslator(reader);

    // read through the file
    while (reader.GetNextAlignment(al)) {
        //! TODO do something
    }


    return(true);
}

