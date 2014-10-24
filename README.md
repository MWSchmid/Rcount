Rcount
======

Rcount: simple and flexible RNA-Seq read counting

<a class="reference external" href="https://github.com/MWSchmid/Rcount/blob/master/Rcount_user_guide.pdf?raw=true">Getting started with Rcount</a>

64 bit binaries are available here:

<a class="reference external" href="https://github.com/MWSchmid/Rcount/blob/master/linux_64bit.zip?raw=true">Linux (ubuntu-like)</a>

<a class="reference external" href="https://github.com/MWSchmid/Rcount/blob/master/windows_64bit.zip?raw=true">Windows7</a>

<a class="reference external" href="https://github.com/MWSchmid/Rcount/blob/master/mac_64bit.zip?raw=true">MacOSX</a>

Additional data for the tutorial and reference annotations are available here:

<a class="reference external" href="http://www.botinst.uzh.ch/static/rcount/rice_tutorial.zip">Data for the rice tutorial</a>

<a class="reference external" href="http://www.botinst.uzh.ch/static/rcount/test_data_annotations.zip">Test data annotations</a>

<a class="reference external" href="http://www.botinst.uzh.ch/static/rcount/test_data_results.zip">Test data results</a>

API - I'm planning to add more detailed description for using the source code directly within your code. Here just a very quick example if you would like to use the data base with the annotation where you can query read mappings or positions:

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

(if you have specific questions, contact me)






