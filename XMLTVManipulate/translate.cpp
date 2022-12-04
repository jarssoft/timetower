#include "translate.h"
#include "translate.h"
#include <string>
#include <fstream>
#include <iostream>
#include "../Rutiinit/stringrutins.h"
#include "descparser.h"
#include "csv.h"
#include <parsewikipedia.h>

void addTitle (std::string title, xmlNode *node) {
    std::cout << title << std::endl;
}

Translate::Translate()
{
    //loadFromCVS(filename);
    //loadFromMediawiki("/home/jari/works/lataukset/wikipedia/wikipedia.xml");
}


std::string toCompare(std::string title){
    std::transform(title.begin(), title.end(), title.begin(), ::tolower);
    removeSpecialCharacter(title);
    return title;
}

void Translate::loadFromCVS(char * filename)
{
    io::CSVReader<3> in(filename);
    in.read_header(io::ignore_extra_column, "title", "url", "category");
    std::string title;
    std::string url;
    std::string category;
    while(in.read_row(title, url, category)){    
        // do stuff with the data
        //std::cout << title << " " << category << std::endl;
        titles.push_back(toCompare(title));
        urls.push_back(url);
        categories.push_back(category);
    }
}

void Translate::loadFromRuutuCVS(char * filename)
{
    //io::CSVReader<2> in(filename);
    io::CSVReader<2, io::trim_chars<' '>,io::double_quote_escape<',','\"'>> in(filename);
    in.read_header(io::ignore_no_column, "title", "url");
    std::string title;
    std::string url;
    while(in.read_row(title, url)){        
        // do stuff with the data
        //std::cout << title << std::endl;
        titles.push_back(toCompare(title));
        urls.push_back(url);
        categories.push_back("");
    }
}

void Translate::loadFromDplayCVS(char * filename)
{
    //io::CSVReader<2> in(filename);
    io::CSVReader<3, io::trim_chars<' '>,io::double_quote_escape<',','\"'>> in(filename);
    in.read_header(io::ignore_no_column, "title", "url", "category");
    std::string title;
    std::string url;
    std::string category;
    while(in.read_row(title, url, category)){
      // do stuff with the data
        //std::cout << title << std::endl;
        //titles.push_back(title);
        //urls.push_back(url);
        //categories.push_back("");

        if(url.length()>0){
            titles.push_back(toCompare(title));
            urls.push_back(url);
            categories.push_back("");
            url="";
        }

        if(category.length()>0){
            std::istringstream iss(category);
            std::vector<std::string> results(
                        std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>());

            for(std::string sana: results){
                titles.push_back(title);
                urls.push_back("");
                categories.push_back(sana);
            }
        }
    }
}



void Translate::loadFromPetscanCVS(char * filename, char *lang)
{
    io::CSVReader<6, io::trim_chars<' '>,io::double_quote_escape<',','\"'>> in(filename);
    in.read_header(io::ignore_no_column, "number", "title", "pageid", "namespace", "length", "touched");
    std::string number;
    std::string title;
    std::string pageid;
    std::string namespace_s;
    std::string length;
    std::string touched;

    while(in.read_row(number, title, pageid, namespace_s, length, touched)){
        // do stuff with the data
        //std::cout << title << std::endl;

        if(title.find("kausi)") == std::string::npos){
            urls.push_back("http://"+std::string(lang)+".wikipedia.org/wiki/"+title);
            std::replace( title.begin(), title.end(), '_', ' ');


            titles.push_back(removeSign(toCompare(title)));

            //std::cout << removeSign(title) << std::endl;
            categories.push_back("");
        }
        /*
        std::cout << title << std::endl;
        for(int i=0; i<title.length();i++){
            std::cout << title.at(i) << (int)(title.at(i)) << " ";
        }
        std::cout << std::endl;
        */
    }

}

void Translate::loadFromMediawiki(char * filename)
{
    wikip::initMediawiki(filename);
    while (wikip::nextPage()) {
        std::string title = wikip::currentTitle();


        if(title.find("tuotantokausi)") == std::string::npos){
            titles.push_back(toCompare(removeSign(title)));
            urls.push_back("http://fi.wikipedia.org/wiki/"+title);
            categories.push_back("");
        }
    }
    wikip::closeMediawiki();
}

void Translate::newSearch()
{
    pos=0;
}

bool Translate::getProgram(std::string title, std::string& url, std::string& category)
{    
    std::vector<std::string>::iterator it;
    it = std::find(titles.begin()+pos+1, titles.end(), toCompare(title));
    pos = distance(titles.begin(), it);
    //std::cout << pos << std::endl;
    if(pos<titles.size()){
        //std::cout << "Tunnistettiin "<< title << std::endl;
        url=urls.at(pos);
        category=categories.at(pos);
        return true;
    }else{
        return false;
    }
}
