#include "stringrutins.h"

void split(const std::string& s, char c, std::vector<std::string>& v) {
   std::string::size_type i = 0;
   std::string::size_type j = s.find(c);

   while (j != std::string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == std::string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

int string_to_int(std::string text) {
    char *endptr;
    const char* kausistr = text.c_str();
    int luku=strtol(text.c_str(), &endptr, 10);
    if (endptr != kausistr) {
        return luku;
    }else{
        return -1;
    }
}

int longest_word(std::string s) {
    int counter = 0;
    int max_word = -1;
    int len = s.length();
    std::string max = " ";

    for (int i = 0; i < len; i++)
    {
        if(s[i] != ' ') {
            //max.append(s[i]);
            counter++;
        }
        if(s[i] == ' ' || i == len - 1) {
            if(counter > max_word) {
                max_word = counter;
                //here,handle the end of the string
                if(i == len - 1)
                    max = s.substr(i + 1 - max_word, max_word);
                else
                    max = s.substr(i - max_word, max_word);
             }

            counter = 0;
        }
    }
    return max_word;
    return 0;
}

std::string before(std::string s, std::string delimiter) {
    std::string token = s.substr(0, s.find(delimiter));
    return token;
}

std::string first_sentence(std::string s) {
    std::string toteamus = before(s, std::string(". "));
    std::string huuto = before(s, std::string("! "));
    std::string kysymys = before(s, std::string("? "));
    if(toteamus.length()>huuto.length()){
        if(huuto.length()>kysymys.length())
            return kysymys+"?";
        else
            return huuto+"!";
    }else{
        if(toteamus.length()>kysymys.length())
            return kysymys+"?";
        else
            return toteamus+".";
    }
}
