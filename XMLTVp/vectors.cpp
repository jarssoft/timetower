#include "vectors.h"

namespace xmltvp {

int findaikatitle(const std::vector<xmltvprogramme> &vector, const QString &startime, const QString &title, const bool exact){

    for(unsigned int iv=0; iv<vector.size(); iv++){
        if(aikatitle(vector.at(iv), startime, title, exact)){
            return iv;
        }
    }
    return -1;
}

std::vector<int> matches(const xmltvp::xmltvread& read,
         const std::vector<xmltvp::xmltvprogramme>& vector,
         const bool exact)
{
    std::vector<int> found(vector.size(), -1);
    assert(found.at(0)==-1);

    for(unsigned int ir=0; ir<read.size(); ir++){
        xmltvp::xmltvprogramme programme1 = read.at(ir);
        for(unsigned int iv=0; iv<vector.size(); iv++){
            xmltvp::xmltvprogramme programme2 = vector.at(iv);
            //if(found.at(iv)==-1 && xmltvp::equals(programme1, programme2)){
            //    found.at(iv)=ir;
            //    break;
            //}
            if(xmltvp::equals(programme1, programme2, exact)){
                //assert(found.at(iv) == -1);
                found.at(iv)=ir;
            }
        }
    }

    return found;
}

unsigned int matchesCount(const std::vector<int> &found){

    int count=0;

    for(unsigned int i2=0; i2<found.size(); i2++){
        if(found.at(i2)!=-1){
            count++;
        }
    }

    return count;
}

bool allEquals(const xmltvp::xmltvread& read, const std::vector<xmltvp::xmltvprogramme>& vector)
{
    if(vector.size() != read.size()){
        return false;
    }

    std::vector<int> found = matches(read, vector);

    return matchesCount(found)==vector.size();
}

void replaceURL(const xmltvp::xmltvread &read, std::vector<xmltvp::xmltvprogramme> &vector, const std::vector<int> &found)
{
    assert(vector.size()==found.size());

    for(unsigned int i=0; i<found.size(); i++){
        if(found.at(i)!=-1){
            vector.at(i).url = read.at(found.at(i)).url;
        }
    }
}

std::vector<int> nonMatches(const xmltvp::xmltvread &read, const std::vector<int> &found)
{
    std::vector<bool> hasFound(read.size(), false);
    assert(hasFound.size()>0);
    assert(read.size()>0);

    for(unsigned int i=0; i<found.size(); i++){
        if(found.at(i) != -1){
            assert(found.at(i)<hasFound.size());
            hasFound.at(found.at(i))=true;
        }
    }

    std::vector<int> notfound;

    for(unsigned int i=0; i<hasFound.size(); i++){
        if(!hasFound.at(i)){
            notfound.push_back(i);
        }
    }
    return notfound;

}

std::vector<int> jatkumo(const std::vector<xmltvp::xmltvprogramme> &vector, const bool exact)
{
    std::vector<int> found(vector.size(), -1);

    for(unsigned int ir=0; ir<vector.size(); ir++){
        xmltvp::xmltvprogramme programme1 = vector.at(ir);
        for(unsigned int iv=0; iv<vector.size(); iv++){
            if(ir!=iv){
                xmltvp::xmltvprogramme programme2 = vector.at(iv);
                if(xmltvp::perakkain(programme1, programme2, exact)){
                    assert(found.at(iv) == -1);
                    found.at(ir)=iv;
                }
            }
        }
    }

    return found;
}

}
