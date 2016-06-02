Rcount: simple and flexible RNA-Seq read counting
======

[About Rcount](http://dx.doi.org/10.1093/bioinformatics/btu680)

[Getting started with Rcount](https://github.com/MWSchmid/Rcount/blob/master/other/Rcount_user_guide.pdf?raw=true)

Note on the hierarchy and priority assignments: Rcount takes the feature information from the third column in the GFF/GTF. This is true for some DBs (e.g. TAIR), but others add this info as a separate field in the last column (entries in column 3 are then very simple, e.g. just gene, transcript, exon, UTR). GTFs from ensembl are an example for the latter case. To specify the priorities for the detailed feature types, the GTF needs to be reformatted. I may have a look into this once.

64 bit binaries are available here:

[Linux (ubuntu-like)](https://github.com/MWSchmid/Rcount/blob/master/other/linux_64bit.zip?raw=true); IMPORTANT: install the Qt5 libraries: `sudo apt-get install qt5-default`

[Windows7](https://github.com/MWSchmid/Rcount/blob/master/other/windows_64bit.zip?raw=true)

[MacOSX](https://github.com/MWSchmid/Rcount/blob/master/other/mac_64bit.zip?raw=true)

Additional data for the tutorial and reference annotations are available here:

[Data for the rice tutorial](http://www.botinst.uzh.ch/static/rcount/rice_tutorial.zip)

[Test data annotations](http://www.botinst.uzh.ch/static/rcount/test_data_annotations.zip)

[Test data results](http://www.botinst.uzh.ch/static/rcount/test_data_results.zip)

NOTE: On linux, GLIBC needs to be at least version 2.14. Biolinux6 has a lower version.

RECENT UPDATE CONCERNING MACOSX BUILD INSTRUCTIONS (advanced users only):

A.T. managed to simplify the build procedure on OSX to quite some extent. He will host code specifically adapted for this on his GitHub account:

[MacOSX build from source, by A.T.](https://github.com/axtu/Rcount)

We are working on the build instructions. Thanks a lot for your work A.

# Command-line version - Update 2016-03-25:

I merged the command line version and the GUI version (on all platforms, it was a prerequisite for a [workflow](https://github.com/MWSchmid/RNAseq_protocol) I am working on at the moment).

Usage notes:

1) For Rcount-multireads, type:

./Rcount-multireads -c infile,outfile,doReweight,allocationDistance
- infile must be a sorted bam file, outfile is bam.
- doReweight should be either y or n
- allocationDistance should be a number (e.g. 100)

Example:

./Rcount-multireads -c mySample.bam,mySampleReweighted.bam,y,100

2) For Rcount-distribute, type:

./Rcount-distribute -c [list of comma-separated project files]

Examples:

./Rcount-distribute -c myFirstProject.xml

./Rcount-distribute -c myFirstProject.xml,mySecondProject.xml

Note:
the project files are the ones normally created by the GUI-version of Rcount ("create project"). I added a Python script which can be used to generate project files via the command-line.

# API

Here just a brief example if you would like to use the data base with the annotation where you can query read mappings or positions:

Include the following files:

```c++
#include "../p502_SOURCE/dataStructure/databaseitem.h"
#include "../p502_SOURCE/dataStructure/database.h"
#include "../p502_SOURCE/dataStructure/mappingtreeitem.h"
#include "../bamHandler/bamhelpers.h"
```

then later on in the code - initialize and load the data base:

```c++
QString annofile = "/path/to/annotation.xml";
QVector<QVariant> headers;
headers << "Sname" << "Schrom" << "Sstrand" << "Ustart" << "Uend" << "Sfeature" << "SassembledFeature" << "Upriority";
database anno(headers);
anno.print_time("START");
if ( anno.readData(annofile) ) { anno.print_time("annotation loaded"); }
```

finally, you can query intervals or positions - there are multiple functions - check them the database header file:

"../p502_SOURCE/dataStructure/database.h"

for an example how to use it, check the function readMapper::run() in:

"../p502_SOURCE/dataAnalysis/readmapper.cpp"

here an example to map a simple position (chrom + pos):

```c++
QString chrom = "Chr1";
uint pos = 11351183;
QVector<databaseItem*> mapping = anno.bestRmapPosition(chrom, pos); // note that there are also functions which fill in pre-allocated vectors - if you like to avoid the return-by-value
foreach (databaseItem* element, mapping) {
  std::cerr << element->data(0).toString().toStdString() << std::endl << std::flush;
}
```
(if you have specific questions, contact me)






