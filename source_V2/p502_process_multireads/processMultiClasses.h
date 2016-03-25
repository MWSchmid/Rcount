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

#ifndef PROCESSMULTICLASSES_H
#define PROCESSMULTICLASSES_H

#include <QtCore>
#define SEQAN_HAS_ZLIB 1
#include <zlib.h>

#include "../seqan/stream.h"
#include "../seqan/bam_io.h"

#include <numeric> // std::accumulate

class chrom_coverage {

private:
    int chrom_id;
    int chrom_len;
    std::vector<int> chrom_cov; // chromosome coverage init to 0
    std::map<std::pair<int,int>,int> splice_map; // splicemap -> first int == prespliceend, second int == postsplicestart

public:

    inline explicit chrom_coverage(const int& id, const int& len) : chrom_id(id), chrom_len(len) {chrom_cov.assign(len+1e4, 0);}
    //! March 2016: Some aligners cause reads to extend over the reference - to solve this, I initiated slightly larger chromosomes (added 10 kb)

    // inline ~chrom_coverage() {}

    inline const int get_id() {return(chrom_id);}

    inline const int get_len() {return(chrom_len);}

    inline const quint64 load_cov(seqan::BamStream& reader, seqan::BamIndex<seqan::Bai>& baiIndex, bool only_unique = true) {
        quint64 numProcessed = 0;
        uint numal = 0; // number of alignments for a given read
        // ungapped OR gapped read
        int start; // start OR the start of the current cigar operation
        int end; // end OR the end of the current cigar operation
        bool isgapped = false; //dummyvar telling if it is a spliceread
        unsigned tagID;

        // set the region

        bool region_has_alignments = false;
        if (!seqan::jumpToRegion(reader, region_has_alignments, chrom_id, 0, chrom_len, baiIndex)) {
            std::cerr << "could not jump to a certain region" << std::endl << std::flush;
        }

        seqan::BamAlignmentRecord record;
        // load all the multireads
        if (region_has_alignments) {
            while (!atEnd(reader)) {
                seqan::readRecord(record, reader);
                if (seqan::hasFlagUnmapped(record)) { continue; }
                ++numProcessed;
                if (record.rID == -1 || record.rID > chrom_id || record.beginPos > chrom_len) { break; }
                seqan::BamTagsDict tags(record.tags);
                // get numberof alignments
                if (seqan::findTagKey(tagID, tags, "NH")) {seqan::extractTagValue(numal, tags, tagID);}
                else {numal = 1;}
                // skip multireads if requested
                if ((numal > 1) && (only_unique)) { continue; }
                // check if alignment is gapped
                isgapped = false;
                if (seqan::length(record.cigar) > 1) {
                    for (int i = 0; i < seqan::length(record.cigar); ++i) {
                         if (record.cigar[i].operation == 'N') { isgapped = true; break; }
                    }
                }
                if (!isgapped) {
                    start = record.beginPos;
                    end = start + seqan::getAlignmentLengthInRef(record);
                    for (int i = start; i != end; ++i) { ++chrom_cov.at(i); }
                }
                else {
                    start = record.beginPos;
                    for (int i = 0; i < seqan::length(record.cigar); ++i){
                        if (record.cigar[i].operation == 'I') { continue; } // a short insert does not change the alignment
                        end = start + record.cigar[i].count - 1; // minus one because be need closed interval (as in the annotation)
                        if (record.cigar[i].operation == 'N') {
                            if (splice_map.count(std::make_pair(start,end)) == 0) {
                                splice_map[std::make_pair(start,end)] = 0;
                            }
                            splice_map[std::make_pair(start,end)] += 1;
                        }
                        // get the start of the next operation
                        start = end + 1; // we need the plus one again, as the start needs to be included
                        //! NOTE THAT THIS ONLY DEALS WITH THE TYPES N, M, D, I (S, H as well)
                    }
                }
            }
        }
        return(numProcessed);
    }

    inline const int get_cov(seqan::BamAlignmentRecord &record, const int& allodist) {
        int start; // start OR the start of the current cigar operation
        int end; // end OR the end of the current cigar operation
        int cov = 0;
        // check if alignment is gapped
        bool isgapped = false; //dummyvar telling if it is a spliceread
        if (seqan::length(record.cigar) > 1) {
            for (int i = 0; i < seqan::length(record.cigar); ++i) {
                 if (record.cigar[i].operation == 'N') { isgapped = true; break; }
            }
        }
        if (!isgapped) {
            start = record.beginPos;
            end = start + seqan::getAlignmentLengthInRef(record); //! NOTE - i compared this with the previously used bamtools - al.GetEndPosition() - gives exactly the same.
            int checkstart = (start-allodist);
            int checkend = (end + allodist);//! now correct with the half-opened interval in bamtools 2.0 - also correct with seqan
            if (checkstart < 0) {checkstart = 0;}
            if (checkend >= chrom_len) {checkend = (chrom_len-1);}
            for (int i = checkstart; i != checkend; ++i) { cov += chrom_cov.at(i); }
        }
        else {
            start = record.beginPos;
            for (int i = 0; i < seqan::length(record.cigar); ++i) {
                if (record.cigar[i].operation == 'I') { continue; } // a short insert does not change the alignment
                end = start + record.cigar[i].count - 1; // minus one because be need closed interval (as in the annotation)
                if (record.cigar[i].operation == 'N') {
                    if (splice_map.count(std::make_pair(start,end)) > 0) {
                        if (splice_map[std::make_pair(start,end)] > cov) { cov = splice_map[std::make_pair(start,end)]; } // take the splice junction with the highest value
                    }
                }
                // get the start of the next operation
                start = end + 1; // we need the plus one again, as the start needs to be included
                //! NOTE THAT THIS ONLY DEALS WITH THE TYPES N, M, D, I (S, H as well)
            }
        }
        return(cov);
    }
};

class multiread {

private:
    std::vector<int> chrom_vec; // chromosome IDs (RefID)
    std::vector<int> start_vec; // start positions
    std::vector<int> end_vec; //! end positions (passed on as HALF-OPENED interval)
    std::vector<int> cov_vec; // number unique reads overlapping the multiread / at the splice.
    std::vector<float> weight_vec; // weights

public:
    inline multiread(const int& num_al = 10) {
        chrom_vec.reserve(num_al);
        start_vec.reserve(num_al);
        end_vec.reserve(num_al);
        cov_vec.reserve(num_al);
        weight_vec.reserve(num_al);
    } // absichtlich nicht explicit

    // inline ~multiread() {}

    inline void add_hit(const int& chromid, const int& start, const int& end, const int& cov) {
        chrom_vec.push_back(chromid);
        start_vec.push_back(start);
        end_vec.push_back(end);
        cov_vec.push_back(cov);
    }

    inline void fill_in_weights() {
        float tot_score = std::accumulate(cov_vec.begin(), cov_vec.end(), 0); // needs to be a float to get the weight_vec proper
        if (tot_score > 0) {
            for (int i = 0; i != cov_vec.size(); ++i) {
                weight_vec.push_back((cov_vec.at(i)/tot_score));
            }
        }
        else {
            for (int i = 0; i != cov_vec.size(); ++i) {
                weight_vec.push_back(0);
            }
        }
    }

    inline const float get_weight(const int& chromid, const int& start, const int& end) { // start and end just serve here as unique identifiers
        for (int i = 0; i != chrom_vec.size(); ++i) {
            if ((chrom_vec.at(i) == chromid) && (start_vec.at(i) == start) && (end_vec.at(i) == end)) {
                return(weight_vec.at(i));
            }
        }
    }

};

typedef std::map<std::string,multiread> mr_map;

class multireadcollection {
private:
    int ad; //allodist
    mr_map reads; //multireads

public:
    inline explicit multireadcollection(const int& allodist) : ad(allodist) {}
    // inline ~multireadcollection(){}

    inline void add_from_one_chromosome(seqan::BamStream& reader, seqan::BamIndex<seqan::Bai>& baiIndex, chrom_coverage& chrom_cov) {
        int chrom_id = chrom_cov.get_id();
        int chrom_len = chrom_cov.get_len();
        std::string name; // name of the read
        uint numal; // number of alignments for a given read
        int cov; // coverage close to the read
        unsigned tagID;

        // set the region
        bool region_has_alignments = false;
        if (!seqan::jumpToRegion(reader, region_has_alignments, chrom_id, 0, chrom_len, baiIndex)) {
            std::cerr << "could not jump to a certain region" << std::endl << std::flush;
        }

        seqan::BamAlignmentRecord record;
        // load all the multireads
        if (region_has_alignments) {
            while (!atEnd(reader)) {
                seqan::readRecord(record, reader);
                if (record.rID == -1 || record.rID > chrom_id || record.beginPos > chrom_len) { break; }
                seqan::BamTagsDict tags(record.tags);
                // get numberof alignments
                if (seqan::findTagKey(tagID, tags, "NH")) {seqan::extractTagValue(numal, tags, tagID);}
                else {numal = 1;}
                // skip unique reads
                if (numal == 1) {continue;}
                name = seqan::toCString(record.qName);
                cov = chrom_cov.get_cov(record, ad);
                multiread mr = reads[name]; // in case read was already processed, it will init a new multiread - otherwise it returns a reference to it. reason for the default constructor
                mr.add_hit(chrom_id, record.beginPos, record.beginPos+seqan::getAlignmentLengthInRef(record), cov); //! does not really matter if end is here now opened - it just serves as an identifier
                reads[name] = mr;
            }
        }
    }

    inline void calculate_weights() {
        for(mr_map::iterator ii=reads.begin(); ii!=reads.end(); ++ii) {(*ii).second.fill_in_weights();}
    }

    inline const float get_weight(std::string& name, const int& chromid, const int& start, const int& end) {
        float weight;
        weight = reads[name].get_weight(chromid,start,end);
        return(weight);
    }
};

#endif // PROCESSMULTICLASSES_H
