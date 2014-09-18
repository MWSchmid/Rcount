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

#ifndef SIMPLE_DATA_COLLECTION_H
#include "simple_data_collection.h" //! do not include while compiling (as it is inline)!
#endif

#ifndef SIMPLE_DATA_H
#include "simple_data.h"
#endif

#ifndef ANNOTATION_READER_H
#include "../p502_SOURCE/annotation_reader/annotation_reader.h"
#endif

//! at the moment, the constructor fills quite a lot with default values for the Arabidopsis thaliana gff3
//! in future one should be able to do it manually in the GUI
inline simple_data_collection::simple_data_collection()
{
    filename = "";
    filetype = "";
    loci.reserve(50000);
    transcripts.reserve(100000);
    priorities.insert("splice",1);
    three_prime = 0;
    five_prime = 0;
    useZeroBased = false;
    useEnsemblLoader = false;
}



//! simple update functions
inline void simple_data_collection::set_zeroBased(bool &useZeroBasedIn) { useZeroBased = useZeroBasedIn; }
inline void simple_data_collection::set_filename(QString& filenamein) { filename = filenamein; }
inline void simple_data_collection::set_filetype(QString& filetypein) { filetype = filetypein; }
inline void simple_data_collection::set_3_prime(uint& in){ three_prime = in; }
inline void simple_data_collection::set_5_prime(uint& in){ five_prime = in; }
inline void simple_data_collection::set_ensembl_loader(bool& useEnsemblLoaderIn) { useEnsemblLoader = useEnsemblLoaderIn; }
inline void simple_data_collection::clear_all() {
    priorities.clear();
    loci.clear();
    transcripts.clear();
    lototr.clear();
    maptranscripts.clear();
    structure.clear();
}


//! simple access functions
inline QString simple_data_collection::get_structure()
{
    QString out = structure.join(QString("\n"));
    return(out);
}

inline bool simple_data_collection::load_data()
{
    bool rval = false;
    if (useEnsemblLoader) { rval = load_data_ensembl(); }
    else { rval = load_data_generic(); }
    return(rval);
}

//! load the data and build the correct order
inline bool simple_data_collection::load_data_ensembl()
{
    bool rval = true;
    annotationReader anno(filename, filetype); // open a reader
    QList<QString> geneTypes;
    QList<QString> transcriptTypes;
    QList<QString> exonTypes;
    geneTypes << "protein_coding_locus"
              << "pseudogene_locus"
              << "long_noncoding_locus"
              << "short_noncoding_locus"
              << "other_locus";
    transcriptTypes << "IG_C_gene"
                    << "IG_D_gene"
                    << "IG_gene"
                    << "IG_J_gene"
                    << "IG_LV_gene"
                    << "IG_M_gene"
                    << "IG_V_gene"
                    << "IG_Z_gene"
                    << "nonsense_mediated_decay"
                    << "nontranslating_CDS"
                    << "non_stop_decay"
                    << "polymorphic"
                    << "polymorphic_pseudogene"
                    << "protein_coding"
                    << "TR_C_gene"
                    << "TR_D_gene"
                    << "TR_gene"
                    << "TR_J_gene"
                    << "TR_V_gene"
                    << "disrupted_domain"
                    << "IG_C_pseudogene"
                    << "IG_J_pseudogene"
                    << "IG_pseudogene"
                    << "IG_V_pseudogene"
                    << "processed_pseudogene"
                    << "pseudogene"
                    << "transcribed_processed_pseudogene"
                    << "transcribed_unitary_pseudogene"
                    << "transcribed_unprocessed_pseudogene"
                    << "translated_processed_pseudogene"
                    << "TR_J_pseudogene"
                    << "TR_pseudogene"
                    << "TR_V_pseudogene"
                    << "unitary_pseudogene"
                    << "unprocessed_pseudogene"
                    << "3prime_overlapping_ncrna"
                    << "ambiguous_orf"
                    << "antisense"
                    << "antisense_RNA"
                    << "lincRNA"
                    << "ncrna_host"
                    << "non_coding"
                    << "processed_transcript"
                    << "retained_intron"
                    << "sense_intronic"
                    << "sense_overlapping"
                    << "miRNA"
                    << "miRNA_pseudogene"
                    << "misc_RNA"
                    << "misc_RNA_pseudogene"
                    << "Mt_rRNA"
                    << "Mt_tRNA"
                    << "Mt_tRNA_pseudogene"
                    << "ncRNA"
                    << "ncRNA_pseudogene"
                    << "rRNA"
                    << "rRNA_pseudogene"
                    << "scRNA"
                    << "scRNA_pseudogene"
                    << "snlRNA"
                    << "snoRNA"
                    << "snoRNA_pseudogene"
                    << "snRNA"
                    << "snRNA_pseudogene"
                    << "tRNA"
                    << "tRNA_pseudogene"
                    << "transposable_element_gene"
                    << "retrotransposed"
                    << "TEC"
                    << "CDSPROTEIN";
    exonTypes << "exon" << "CDS";
    simple_data feature; // the feature -> will be updated and pushed onto a vector (in STL, this is copying it)

    // the start and end value to add:
    uint start;
    uint end;
    uint correctedStart;
    uint correctedEnd;
    QString leftFlank;
    QString rightFlank;
    uint offsetCorrection = 0;
    if (useZeroBased) { offsetCorrection = 1; }

    //! loop through the file
    while (anno.readLine()) {
        // check what kind of feature
        // a locustype ?
        correctedStart = (anno.start-offsetCorrection);
        correctedEnd = (anno.end-offsetCorrection);
        if (geneTypes.contains(anno.feature)) {
            // get the new start and end with the flanking regions
            if (anno.strand == "+") {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + three_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
            }
            else if (anno.strand == "-") {
                if (three_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - three_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
            }
            else {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
            }
            // update the feature and add it
            feature.insert_data(anno.chrom, anno.strand, start, end, anno.source, anno.feature, anno.name, anno.parent, anno.score, anno.phase);
            feature.assembled_feature = feature.feature; // otherwise the loci always have 0 priority
            loci.push_back(feature);
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }
        }
        // a transcripttype?
        else if (transcriptTypes.contains(anno.feature)) {
            if (maptranscripts.contains(anno.name)) {
                QMessageBox::warning(0, QObject::tr("Load Data"), QObject::tr("this transcript exists at least twice:\n%1\nline number %2").arg(anno.name, QString::number(anno.linenumber)));
                rval = false;
                break;
            }
            // get the new start and end with the flanking regions
            if (anno.strand == "+") {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + three_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
                leftFlank = "five_flank";
                rightFlank = "three_flank";
            }
            else if (anno.strand == "-") {
                if (three_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - three_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
                leftFlank = "three_flank";
                rightFlank = "five_flank";
            }
            else {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
                leftFlank = "any_flank";
                rightFlank = "any_flank";
            }
            // update the feature and add it
            feature.insert_data(anno.chrom, anno.strand, start, end, anno.source, anno.feature, anno.name, anno.parent, anno.score, anno.phase);
            lototr.insertMulti(anno.parent, anno.name);
            transcripts.push_back(feature);
            maptranscripts.insert(anno.name, (transcripts.size()-1));
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }
            // add the flanking regions as utr features to the feature. NOTE that this is for the moment messing up the order, but we could sort it if necessary (note that this does also require the resorting of the features)
            if ((start-correctedStart) != 0) {
                transcripts.last().add_utr(start, correctedStart, leftFlank);
                if (!priorities.contains(leftFlank)) { priorities.insert(leftFlank, 1); }
            }
            if ((end-correctedEnd) != 0) {
                transcripts.last().add_utr(correctedEnd, end, rightFlank);
                if (!priorities.contains(rightFlank)) { priorities.insert(rightFlank, 1); }
            }
        }
        // an exontype?
        else if (exonTypes.contains(anno.feature)) {
            if (!maptranscripts.contains(anno.parent)) {
                QMessageBox::warning(0, QObject::tr("Load Data"), QObject::tr("could not find transcript:\n%1").arg(anno.parent));
                rval = false;
                break;
            }
            transcripts[maptranscripts.value(anno.parent)].add_exon(correctedStart, correctedEnd, anno.feature);
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }//! important: at() returns a constant!
        }
        // skip all the other types
        else { continue; }
    }

    //! get the splicejunctions
    QVector<simple_data>::iterator iter;
    for (iter = transcripts.begin(); iter != transcripts.end(); ++iter) { iter->get_splice_junctions(); }

    //! sort the locusvector - use first qSort on the start, then qStableSort on the chromosome
    qSort(loci.begin(), loci.end(), SortSimpleDataByStart); // in the reference they use the function call without any brackets
    qStableSort(loci.begin(), loci.end(), SortSimpleDataByChrom);

    //! load the structure
    rval = load_structure();

    return(rval);
}

//! load the data and build the correct order
inline bool simple_data_collection::load_data_generic()
{
    bool rval = true;
    annotationReader anno(filename, filetype); // open a reader
    QRegExp rgen("(gene|transposable_element)", Qt::CaseInsensitive);
    QRegExp rtra("(RNA|transcript|protein)", Qt::CaseInsensitive);
    QRegExp rexo("(exon|CDS)", Qt::CaseInsensitive);
    QRegExp rutr("(UTR)", Qt::CaseInsensitive);
    //QRegExp rpro("(protein)", Qt::CaseInsensitive);
    //QRegExp rcds("(CDS|UTR)", Qt::CaseInsensitive);
    //QRegExp rtel("(transpos)", Qt::CaseInsensitive);
    simple_data feature; // the feature -> will be updated and pushed onto a vector (in STL, this is copying it)

    // the start and end value to add:
    uint start;
    uint end;
    uint correctedStart;
    uint correctedEnd;
    QString leftFlank;
    QString rightFlank;
    uint offsetCorrection = 0;
    if (useZeroBased) { offsetCorrection = 1; }

    //! loop through the file
    while (anno.readLine()) {
        // check what kind of feature
        // a locustype ?
        correctedStart = (anno.start-offsetCorrection);
        correctedEnd = (anno.end-offsetCorrection);

        if (anno.feature.contains(rgen)) {
            // get the new start and end with the flanking regions
            if (anno.strand == "+") {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + three_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
            }
            else if (anno.strand == "-") {
                if (three_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - three_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
            }
            else {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
            }
            // update the feature and add it
            feature.insert_data(anno.chrom, anno.strand, start, end, anno.source, anno.feature, anno.name, anno.parent, anno.score, anno.phase);
            feature.assembled_feature = feature.feature; // otherwise the loci always have 0 priority
            loci.push_back(feature);
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }
        }
        // a transcripttype?
        else if (anno.feature.contains(rtra)) {
            if (maptranscripts.contains(anno.name)) {
                QMessageBox::warning(0, QObject::tr("Load Data"), QObject::tr("this transcript exists at least twice:\n%1\nline number %2").arg(anno.name, QString::number(anno.linenumber)));
                rval = false;
                break;
            }
            // get the new start and end with the flanking regions
            if (anno.strand == "+") {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + three_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
                leftFlank = "five_flank";
                rightFlank = "three_flank";
            }
            else if (anno.strand == "-") {
                if (three_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - three_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
                leftFlank = "three_flank";
                rightFlank = "five_flank";
            }
            else {
                if (five_prime >= correctedStart) { start = 0; }
                else { start = correctedStart - five_prime; }
                end = correctedEnd + five_prime; //! NOTE: IF BIGGER THAN CHROM, IT DOES NOT NOTICE IT
                leftFlank = "any_flank";
                rightFlank = "any_flank";
            }
            // update the feature and add it
            // a routine for protein checking (in TAIR they are kids of the transcripts - not really what I want)
            if (loci.last().name != anno.parent) {
                feature.insert_data(anno.chrom, anno.strand, start, end, anno.source, anno.feature, anno.name, loci.last().name, anno.score, anno.phase);
                lototr.insertMulti(loci.last().name, anno.name);
            } else {
                feature.insert_data(anno.chrom, anno.strand, start, end, anno.source, anno.feature, anno.name, anno.parent, anno.score, anno.phase);
                lototr.insertMulti(anno.parent, anno.name);
            }
            transcripts.push_back(feature);
            maptranscripts.insert(anno.name, (transcripts.size()-1));
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }
            // add the flanking regions as utr features to the feature. NOTE that this is for the moment messing up the order, but we could sort it if necessary (note that this does also require the resorting of the features)
            if ((start-correctedStart) != 0) {
                transcripts.last().add_utr(start, correctedStart, leftFlank);
                if (!priorities.contains(leftFlank)) { priorities.insert(leftFlank, 1); }
            }
            if ((end-correctedEnd) != 0) {
                transcripts.last().add_utr(correctedEnd, end, rightFlank);
                if (!priorities.contains(rightFlank)) { priorities.insert(rightFlank, 1); }
            }
        }
        // an exontype?
        else if (anno.feature.contains(rexo)) {
            if (!maptranscripts.contains(anno.parent)) {
                QMessageBox::warning(0, QObject::tr("Load Data"), QObject::tr("could not find transcript:\n%1").arg(anno.parent));
                rval = false;
                break;
            }
            transcripts[maptranscripts.value(anno.parent)].add_exon(correctedStart, correctedEnd, anno.feature);
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }//! important: at() returns a constant!
        }
        // a UTR?
        else if (anno.feature.contains(rutr)) {
            transcripts[maptranscripts.value(anno.parent)].add_utr(correctedStart, correctedEnd, anno.feature);
            if (!priorities.contains(anno.feature)) { priorities.insert(anno.feature, 1); }//! important: at() returns a constant!
        }
        // skip all the other types
        else { continue; }
    }

    //! get the splicejunctions
    QVector<simple_data>::iterator iter;
    for (iter = transcripts.begin(); iter != transcripts.end(); ++iter) { iter->get_splice_junctions(); }

    //! sort the locusvector - use first qSort on the start, then qStableSort on the chromosome
    qSort(loci.begin(), loci.end(), SortSimpleDataByStart); // in the reference they use the function call without any brackets
    qStableSort(loci.begin(), loci.end(), SortSimpleDataByChrom);

    //! load the structure
    rval = load_structure();

    return(rval);
}

//! load the structure using the preformatted annotation
inline bool simple_data_collection::load_structure()
{
    bool rval = true;

    // create the string for the tree
    QMap<QString, uint> occurences;
    QString curkey;

    QVector<simple_data>::iterator iter;
    for (iter = loci.begin(); iter != loci.end(); ++iter) {
        curkey = QObject::tr("%1").arg(iter->feature);
        if (!occurences.contains(curkey)) { occurences.insert(curkey, 0); }
        ++occurences[curkey];
        foreach (const QString &transcript, lototr.values(iter->name)) {
            curkey = QObject::tr("%1|%2").arg(iter->feature, transcripts.at(maptranscripts.value(transcript)).feature);
            if (!occurences.contains(curkey)) { occurences.insert(curkey, 0); }
            ++occurences[curkey];
            transcripts[maptranscripts.value(transcript)].insert_assembled_feature(curkey); // UPDATE THE ASSEMBLED FEATURES
            for (int i = 0; i < transcripts.at(maptranscripts.value(transcript)).exon_starts.size(); ++i) {
                curkey = QObject::tr("%1|%2|%3").arg(iter->feature, transcripts.at(maptranscripts.value(transcript)).feature, transcripts.at(maptranscripts.value(transcript)).exon_features.at(i));
                if (!occurences.contains(curkey)) { occurences.insert(curkey, 0); }
                ++occurences[curkey];
            }
            for (int i = 0; i < transcripts.at(maptranscripts.value(transcript)).utr_starts.size(); ++i) {
                curkey = QObject::tr("%1|%2|%3").arg(iter->feature, transcripts.at(maptranscripts.value(transcript)).feature, transcripts.at(maptranscripts.value(transcript)).utr_features.at(i));
                if (!occurences.contains(curkey)) { occurences.insert(curkey, 0); }
                ++occurences[curkey];
            }
            for (int i = 0; i < transcripts.at(maptranscripts.value(transcript)).splice_starts.size(); ++i) {
                curkey = QObject::tr("%1|%2|%3").arg(iter->feature, transcripts.at(maptranscripts.value(transcript)).feature, transcripts.at(maptranscripts.value(transcript)).splice_features.at(i));
                if (!occurences.contains(curkey)) { occurences.insert(curkey, 0); }
                ++occurences[curkey];
            }
        }
    }

    // write the structure string -> the map should already be sorted by the keys (ascending order) - use the "|" as level counters
    QString strkey;
    QStringList splitkey;
    QString curname;
    uint curlevel;
    QString curline;
    QMap<QString, uint>::const_iterator i = occurences.constBegin();
    while (i != occurences.constEnd()) {
         strkey = i.key();
         splitkey = strkey.split("|");
         curname = splitkey.takeLast();
         curlevel = splitkey.length(); // pop/takeLast first!
         curline = QObject::tr("%1 (%2)\t%3\t%4").arg(curname, QString::number(i.value()), QString::number(priorities.value(curname)), QString::number(curlevel));
         structure << curline;
         ++i;
    }
    return(rval);
}

//! reclaim the priorityvalues one updated in the treeview
inline bool simple_data_collection::update_treestructure(QString treestructure)
{
    bool rval = true;
    structure = treestructure.split(QString("\n"));
    return(rval);
}

inline bool simple_data_collection::update_priorities()
{
    bool rval = true;
    QRegExp rx("(\\w+)");
    int pos = 0;
    QStringList curline;
    QString feature;
    uint priority;
    uint level;
    QStringList locusfeatures;
    QStringList transcriptfeatures;
    QString assembledfeature;
    foreach (const QString &linestr, structure) {
        curline.clear();
        pos = 0;
        while ((pos = rx.indexIn(linestr, pos)) != -1) {
            curline << rx.cap(1);
            pos += rx.matchedLength();
        }
        feature = curline.at(0);
        priority = curline.at(2).toUInt();
        level = curline.at(3).toUInt();
        if (priorities.value(feature) != priority) {
            priorities.insert(feature, priority);
            //QMessageBox::warning(0, QObject::tr("changed priorities"), QObject::tr("%1\t%2").arg(feature, QString::number(priority))); THIS MESSAGE SHOULD BE DISPLAYED LATER ON THE LAST PAGE
        }
        // also the assembled features
        if (level > 0) {
            if (level == 1) {
                assembledfeature = QObject::tr("%1|%2").arg(locusfeatures.last(), feature);
                transcriptfeatures << feature;
            }
            else {
                assembledfeature = QObject::tr("%1|%2|%3").arg(locusfeatures.last(), transcriptfeatures.last(), feature);
            }
            if (priorities.value(assembledfeature) != priority) {
                priorities.insert(assembledfeature, priority);
                //QMessageBox::warning(0, QObject::tr("changed priorities"), QObject::tr("%1\t%2").arg(feature, QString::number(priority))); THIS MESSAGE SHOULD BE DISPLAYED LATER ON THE LAST PAGE
            }
        } else {
            locusfeatures << feature;
        }
    }
    return(rval);
}

//! function to write the new annotation file
inline bool simple_data_collection::write_xml_V1(QString& outfilein)
{
    bool rval = true;

    //! open and check the file
    QFile file(outfilein);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("IO-Problem"), QObject::tr("could not open the file:\n%1").arg(outfilein));
        rval = false;
    }

    //! print the annotation
    QXmlStreamWriter out(&file);
    out.setAutoFormatting(true);
    out.writeStartDocument();
    out.writeStartElement("p502annotation");
    out.writeAttribute("version", "1.0");
    simple_data *tpoi;
    for (int i = 0; i < loci.size(); ++i){
        // we call the first feature locus - it contains transcripts which contain features of the type exon, utr, splice
        out.writeStartElement("locus");
        // write the locus entry
        write_simple_data_V1(&out, &loci[i]);
        foreach (const QString &transcript, lototr.values(loci[i].name)) {
            // now the transcriptentry
            tpoi = &transcripts[maptranscripts.value(transcript)];
            out.writeStartElement("transcript");
            write_simple_data_V1(&out, tpoi);
            // now the individual exons, splices and utrs
            for (int i = 0; i < tpoi->exon_starts.size(); ++i){
                out.writeStartElement("exon");
                write_vector_data_V1(
                            &out,
                            tpoi,
                            &(tpoi->exon_features[i]),
                            &(tpoi->assembled_exon_features[i]),
                            &(tpoi->exon_starts[i]),
                            &(tpoi->exon_ends[i]),
                            &i);
                out.writeEndElement();
            }
            for (int i = 0; i < tpoi->splice_starts.size(); ++i){
                out.writeStartElement("intron");
                write_vector_data_V1(
                            &out,
                            tpoi,
                            &(tpoi->splice_features[i]),
                            &(tpoi->assembled_splice_features[i]),
                            &(tpoi->splice_starts[i]),
                            &(tpoi->splice_ends[i]),
                            &i);
                out.writeEndElement();
            }
            for (int i = 0; i < tpoi->utr_starts.size(); ++i){
                out.writeStartElement("utr");
                write_vector_data_V1(
                            &out,
                            tpoi,
                            &(tpoi->utr_features[i]),
                            &(tpoi->assembled_utr_features[i]),
                            &(tpoi->utr_starts[i]),
                            &(tpoi->utr_ends[i]),
                            &i);
                out.writeEndElement();
            }
            // write the end of the transcript
            out.writeEndElement();
        }
        // write the end of the locus
        out.writeEndElement();
    }
    out.writeEndElement();
    out.writeEndDocument();

    //! close the file and check for errors
    file.close();
    if (file.error()) {
        QMessageBox::warning(0, QObject::tr("IO-Problem"), QObject::tr("could not write the file:\n%1").arg(outfilein));
        rval = false;
    }



    return(rval);
}

inline void simple_data_collection::write_simple_data_V1(QXmlStreamWriter *out, simple_data *dat) {
    out->writeTextElement("Sname", dat->name);
    out->writeTextElement("Schrom", dat->chrom);
    out->writeTextElement("Sstrand", dat->strand);
    out->writeTextElement("Ustart", QString::number(dat->start));
    out->writeTextElement("Uend", QString::number(dat->end));
    out->writeTextElement("Ssource", dat->source);
    out->writeTextElement("Sfeature", dat->feature);
    out->writeTextElement("SassembledFeature", dat->assembled_feature);
    out->writeTextElement("Sphase", dat->phase);
    out->writeTextElement("Fscore", QString::number(dat->score));
    out->writeTextElement("Upriority", QString::number(priorities[dat->assembled_feature]));
    //out->writeTextElement("UnumberReadsTotal", QString::number(0));
    //out->writeTextElement("UnumberReadsUnambiguous", QString::number(0));
    //out->writeTextElement("UnumberReadsAmbiguous", QString::number(0));
    //out->writeTextElement("FnumberHitsTotal", QString::number(0));
    //out->writeTextElement("FnumberHitsUnambiguous", QString::number(0));
    //out->writeTextElement("FnumberHitsAmbiguous", QString::number(0));
    //out->writeTextElement("Scoverage", QString(""));
    //out->writeTextElement("", );
    //out->writeTextElement("", QString::number());
}



inline void simple_data_collection::write_vector_data_V1(QXmlStreamWriter *out, simple_data *dat, QString *feature, QString *assembledFeature, uint *start, uint *end, int *number) {
    QString name;
    name = QObject::tr("%1-%2-%3").arg(dat->name, *feature, QString::number(*number));
    out->writeTextElement("Sname", name);
    out->writeTextElement("Schrom", dat->chrom);
    out->writeTextElement("Sstrand", dat->strand);
    out->writeTextElement("Ustart", QString::number(*start));
    out->writeTextElement("Uend", QString::number(*end));
    out->writeTextElement("Ssource", dat->source);
    out->writeTextElement("Sfeature", *feature);
    out->writeTextElement("SassembledFeature", *assembledFeature);
    out->writeTextElement("Sphase", dat->phase);
    out->writeTextElement("Fscore", QString::number(dat->score));
    out->writeTextElement("Upriority", QString::number(priorities[*assembledFeature]));
    //out->writeTextElement("UnumberReadsTotal", QString::number(0));
    //out->writeTextElement("UnumberReadsUnambiguous", QString::number(0));
    //out->writeTextElement("UnumberReadsAmbiguous", QString::number(0));
    //out->writeTextElement("FnumberHitsTotal", QString::number(0));
    //out->writeTextElement("FnumberHitsUnambiguous", QString::number(0));
    //out->writeTextElement("FnumberHitsAmbiguous", QString::number(0));
    //out->writeTextElement("Scoverage", QString(""));
}

inline bool simple_data_collection::write_xml_V2(QString& outfilein)
{
    bool rval = true;

    //! open and check the file
    QFile file(outfilein);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("IO-Problem"), QObject::tr("could not open the file:\n%1").arg(outfilein));
        rval = false;
    }

    //! print the annotation
    QXmlStreamWriter out(&file);
    out.setAutoFormatting(true);
    out.writeStartDocument();
    out.writeStartElement("p502annotation");
    out.writeAttribute("version", "2.0");
    simple_data *tpoi;
    for (int i = 0; i < loci.size(); ++i){
        // we call the first feature locus - it contains transcripts which contain features of the type exon, utr, splice
        if (priorities[loci[i].assembled_feature] == 0) { continue; }
        out.writeStartElement("locus");
        // write the locus entry
        write_simple_data_V2(&out, &loci[i]);
        foreach (const QString &transcript, lototr.values(loci[i].name)) {
            // now the transcriptentry
            tpoi = &transcripts[maptranscripts.value(transcript)];
            if (priorities[tpoi->assembled_feature] == 0) { continue; }
            out.writeStartElement("transcript");
            write_simple_data_V2(&out, tpoi);
            // now the individual exons, splices and utrs
            for (int i = 0; i < tpoi->exon_starts.size(); ++i){
                if (priorities[tpoi->assembled_exon_features[i]] == 0) { continue; }
                out.writeStartElement("exon");
                write_vector_data_V2(
                            &out,
                            tpoi,
                            &(tpoi->exon_features[i]),
                            &(tpoi->assembled_exon_features[i]),
                            &(tpoi->exon_starts[i]),
                            &(tpoi->exon_ends[i]),
                            &i);
                out.writeEndElement();
            }
            for (int i = 0; i < tpoi->splice_starts.size(); ++i){
                if (priorities[tpoi->assembled_splice_features[i]] == 0) { continue; }
                out.writeStartElement("intron");
                write_vector_data_V2(
                            &out,
                            tpoi,
                            &(tpoi->splice_features[i]),
                            &(tpoi->assembled_splice_features[i]),
                            &(tpoi->splice_starts[i]),
                            &(tpoi->splice_ends[i]),
                            &i);
                out.writeEndElement();
            }
            for (int i = 0; i < tpoi->utr_starts.size(); ++i){
                if (priorities[tpoi->assembled_utr_features[i]] == 0) { continue; }
                out.writeStartElement("utr");
                write_vector_data_V2(
                            &out,
                            tpoi,
                            &(tpoi->utr_features[i]),
                            &(tpoi->assembled_utr_features[i]),
                            &(tpoi->utr_starts[i]),
                            &(tpoi->utr_ends[i]),
                            &i);
                out.writeEndElement();
            }
            // write the end of the transcript
            out.writeEndElement();
        }
        // write the end of the locus
        out.writeEndElement();
    }
    out.writeEndElement();
    out.writeEndDocument();

    //! close the file and check for errors
    file.close();
    if (file.error()) {
        QMessageBox::warning(0, QObject::tr("IO-Problem"), QObject::tr("could not write the file:\n%1").arg(outfilein));
        rval = false;
    }



    return(rval);
}

inline void simple_data_collection::write_simple_data_V2(QXmlStreamWriter *out, simple_data *dat) {
    out->writeAttribute("Sname", dat->name);
    out->writeAttribute("Schrom", dat->chrom);
    out->writeAttribute("Sstrand", dat->strand);
    out->writeAttribute("Ustart", QString::number(dat->start));
    out->writeAttribute("Uend", QString::number(dat->end));
    out->writeAttribute("Ssource", dat->source);
    out->writeAttribute("Sfeature", dat->feature);
    out->writeAttribute("SassembledFeature", dat->assembled_feature);
    out->writeAttribute("Sphase", dat->phase);
    out->writeAttribute("Fscore", QString::number(dat->score));
    out->writeAttribute("Upriority", QString::number(priorities[dat->assembled_feature]));
    //out->writeAttribute("UnumberReadsTotal", QString::number(0));
    //out->writeAttribute("UnumberReadsUnambiguous", QString::number(0));
    //out->writeAttribute("UnumberReadsAmbiguous", QString::number(0));
    //out->writeAttribute("FnumberHitsTotal", QString::number(0));
    //out->writeAttribute("FnumberHitsUnambiguous", QString::number(0));
    //out->writeAttribute("FnumberHitsAmbiguous", QString::number(0));
    //out->writeAttribute("Scoverage", QString(""));
    //out->writeAttribute("", );
    //out->writeAttribute("", QString::number());
}



inline void simple_data_collection::write_vector_data_V2(QXmlStreamWriter *out, simple_data *dat, QString *feature, QString *assembledFeature, uint *start, uint *end, int *number) {
    QString name;
    name = QObject::tr("%1-%2-%3").arg(dat->name, *feature, QString::number(*number));
    out->writeAttribute("Sname", name);
    out->writeAttribute("Schrom", dat->chrom);
    out->writeAttribute("Sstrand", dat->strand);
    out->writeAttribute("Ustart", QString::number(*start));
    out->writeAttribute("Uend", QString::number(*end));
    out->writeAttribute("Ssource", dat->source);
    out->writeAttribute("Sfeature", *feature);
    out->writeAttribute("SassembledFeature", *assembledFeature);
    out->writeAttribute("Sphase", dat->phase);
    out->writeAttribute("Fscore", QString::number(dat->score));
    out->writeAttribute("Upriority", QString::number(priorities[*assembledFeature]));
    //out->writeAttribute("UnumberReadsTotal", QString::number(0));
    //out->writeAttribute("UnumberReadsUnambiguous", QString::number(0));
    //out->writeAttribute("UnumberReadsAmbiguous", QString::number(0));
    //out->writeAttribute("FnumberHitsTotal", QString::number(0));
    //out->writeAttribute("FnumberHitsUnambiguous", QString::number(0));
    //out->writeAttribute("FnumberHitsAmbiguous", QString::number(0));
    //out->writeAttribute("Scoverage", QString(""));
}

