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

/* Get first sentence of phrase. */
std::string first_sentence(std::string s);

std::string finnishHyphenation(std::string s, std::string hyphenseparator);

#endif // STRINGRUTINS_H
