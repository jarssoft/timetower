#include "sendinggroup.h"
#include <assert.h>

sendingGroup::sendingGroup(std::vector<sending_t> jointSendings,
                           program_t program,
                           std::vector<episode_t> jointEpisodes):

                            jointSendings(jointSendings), program(program), jointEpisodes(jointEpisodes)
{
    assert(jointSendings.size() == jointEpisodes.size());
}

program_t sendingGroup::getProgram()
{
    return this->program;
}

std::vector<sending_t> sendingGroup::getJointSendings()
{
    return jointSendings;
}

std::vector<episode_t> sendingGroup::getJointEpisodes()
{
    return jointEpisodes;
}

unsigned int sendingGroup::size()
{
    return jointSendings.size();
}
