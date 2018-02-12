#ifndef XMLTV_H
#define XMLTV_H

void setWeek(int weeknum);

void setChannel(char* akanava);

void importXML(const char* xmlfile);

void exportHTML(std::ostream& out);

void exportSVG(std::ostream& out);

#endif // XMLTV_H
