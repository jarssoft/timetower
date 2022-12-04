#include <iostream>
#include "console.hpp"
#include <QFile>
#include <../../XMLTVp/xmltvread.h>
#include <../../XMLTVp/xmltvwrite.h>
#include "../../XMLTVp/vectors.h"


void Console::run()
{
    /*
    std::cout << "First message" << std::endl;
    std::cout << "> " << std::flush;
    */
    connect(m_notifier, SIGNAL(activated(int)), this, SLOT(readCommand()));
}

void Console::readCommand()
{
    std::string line;
    std::getline(std::cin, line);

    if (std::cin.eof() || line == "quit") {

        xmltvp::xmltvwrite koe2;
        koe2.addProgrammeVector(programmes);

        std::cout << koe2.toString().toStdString() << std::flush;

        //std::cout << "Good bye!" << std::endl;
        emit quit();

    } else {

        //std::cout << "Echo: " << line << std::endl;

        assert(QFile::exists(QString::fromStdString(line)));
        QFile file(QString::fromStdString(line));

        xmltvp::xmltvread koe;
        koe.load(file);

        if(programmes.size()==0){
            programmes = koe.toProgrammeVector();
            //std::cout << "Ohjelmia: " << programmes.size() << std::endl;
        }else{

            std::vector<int> found = xmltvp::matches(koe, programmes, false);
            //std::cout << "Ohjelmia: " << koe.size() << std::endl;
            //const int matches = xmltvp::matchesCount(found);
            //std::cout << "Samoja: " << matches << std::endl;

            //täydennetään lajityypit
            {
                assert(programmes.size()==found.size());

                for(unsigned int i=0; i<found.size(); i++){


                    if(found.at(i)!=-1){
                        if(koe.at(found.at(i)).url.size() > programmes.at(i).url.size()){
                            programmes.at(i).url = koe.at(found.at(i)).url;
                        }
                        //if(koe.at(found.at(i)).categories.size() > programmes.at(i).categories.size()){
                            //programmes.at(i).categories = koe.at(found.at(i)).categories;
                        //}

                        //programmes.at(i).categories += koe.at(found.at(i)).categories;
                        //if(koe.size()>0){
                            //programmes.at(i).categories.insert(programmes.at(i).categories.end(), koe.at(found.at(i)).categories.begin(), koe.at(found.at(i)).categories.end());
                            //programmes.at(i).categories.push_back("koe");
                        //}
                        int ennen=programmes.at(i).categories.size();
                        for(QString c : koe.at(found.at(i)).categories) {
                            programmes.at(i).categories.push_back(c);
                        }
                        assert(programmes.at(i).categories.size() == ennen + koe.at(found.at(i)).categories.size());

                        //std::cout << programmes.at(i).url.toStdString() << " / " << koe.at(found.at(i)).url.toStdString() << std::endl;
                        //std::cout << programmes.at(i).category.toStdString() << " / " << koe.at(found.at(i)).category.toStdString() << std::endl;
                        //vector.at(i).url = read.at(found.at(i)).url;
                    }
                }
            }
        }

        std::cout << std::flush;

    }
}
