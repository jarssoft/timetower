#ifndef VIIKKOLISTA_H
#define VIIKKOLISTA_H

#include <vector>
#include <string.h>
#include <iostream>
#include <assert.h>
#include "xml_writer.hpp"
#include "sending.h"
#include "html_table_writer.h"

//add unique object to vector and return index where object is saved. counts number of objects.
template <class T> static int addUnique(T ohj, std::vector<T> &vektori);

//weekly table for one tv-channel
class WeekTable
{

public:

    WeekTable();

    void setWeek(int week);

    void setChannelName(std::string name_of_channel);

    //add program to table
    void addProgram(const tm start, const tm end, std::string name, std::string categories,
                    std::string director, int year, int season, int episode, int episodetotal, std::string subtitle, std::string desc, std::string url);

    //generate table to HTML
    void getHTML(std::ostream& out);

    void getSVG(std::ostream& out);

    void getTable(Table_writer &writer);

private:

    //try to level programs on slight different time
    void level();

    void level2();

    void addProgram(const int startday, const int starttick, const int endday, const int endtick, int prog_id, episode_t epis);

    bool canJoint(sending_t &sending1, sending_t &sending2);

    int sendingtable[DAYSINWEEK][TICKSINDAY];
    int sendingcount[DAYSINWEEK][TICKSINDAY];

    sending_t sendingByCoords(int x, int y);

    std::vector<sending_t> sendings;
    std::vector<program_t> programs;
    std::vector<episode_t> episodes;

    std::string name_of_channel;
    int weeknro;
    tm monday;
    bool monday_has_set;

};

#endif // VIIKKOLISTA_H
