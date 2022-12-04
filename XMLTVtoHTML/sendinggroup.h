#ifndef SENDINGGROUP_H
#define SENDINGGROUP_H

#include <vector>
#include <sending.h>

class sendingGroup
{

public:
    sendingGroup(std::vector<sending_t> jointSendings, program_t program, std::vector<episode_t> jointEpisodes);

    program_t getProgram();

    std::vector<sending_t> getJointSendings();
    std::vector<episode_t> getJointEpisodes();

    unsigned int size();

private:
    std::vector<sending_t> jointSendings;
    program_t program;
    std::vector<episode_t> jointEpisodes;


};

#endif // SENDINGGROUP_H
