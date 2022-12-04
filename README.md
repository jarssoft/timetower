# timetower
Generates broadcast schedule tables from XMLTV and process XMLTV data.

Command line application, programmed by C++.

## weektable (./XMLTVtoHTML)
Generates broadcast schedule tables from XMLTV

<pre>
Usage: weektable [OPTION...] [tvxml-file1 tvxml-file2...]

  -@, --stdin                Read XML filenames from standard input
  -c, --channel=CHANNEL      Select channel. Use if several
  -o, --output=FILE          Output to FILE instead of standard output
  -s, --svg                  Output in svg-format instead of html.
  -w, --week=WEEK            Number of week
  -z, --timezone=TIMEZONE    Timezone correction in hours
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
</pre>

## manipuloi (./XMLTVManipulate)
Process XMLTV data

<pre>
Usage: manipuloi [OPTION...] [tvxml-file manipulate-file...]

  -a, --autodst              Is Automatic Daylight Saving used?
  -c, --channel=CHANNEL      Channel
  -d, --startdate=STARTDATE  Date of start
  -i, --manipulate=FILE      Read manipulation rules from file
  -o, --output=FILE          Output to FILE instead of standard output
  -z, --timeoffset=TIMEOFFSET   Timezone correction in hours
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
</pre>

## yhdista (./XMLTVYhdista)
Join XMLTV-files.

## Exaples
find -name ".xml" -type f | yhdista > joined.xml

Showcase:
http://www.ohjelmakartta.fi/

