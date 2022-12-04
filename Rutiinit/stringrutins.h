#ifndef STRINGRUTINS_H
#define STRINGRUTINS_H

#include <vector>
#include <string>

/* Split strings by char */
void split(const std::string& s, char c, std::vector<std::string>& v);

/* Converts string to int, and return -1 if it fails. */
int string_to_int(std::string text);

/* Find the longest word in a string. */
int longest_word(std::string s);

/* String of characters before delimiter */
std::string before(std::string s, std::string delimiter);

std::string after(std::string s, std::string delimiter);

/* Get first sentence of phrase. */
std::string first_sentence(std::string s);

/* Lyhentää tekstin järkevään kokonaisuuteen, jonka maksimi-/tavoitepituus on l.*/
std::string shorten_text(std::string s, unsigned int l);

std::string bestPhrase(std::string s, unsigned int (*ptr)(std::string));

/* Etsii yhteiset lauseet kahdesta merkkijonosta. */
std::string commonPhrases(std::string s1, const std::string s2);

std::string last_sentence(std::string s);

std::string finnishHyphenation(const std::string s, std::string hyphenseparator);

void eraseSubStr(std::string & mainStr, const std::string & toErase);


// function to remove characters and
// print new string
void removeSpecialCharacter(std::string & s);

#endif // STRINGRUTINS_H
