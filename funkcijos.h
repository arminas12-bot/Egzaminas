#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <regex>
#include <iostream>
#include <locale>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::sort;
using std::setw;
using std::left;
using std::right;
using std::regex;
using std::sregex_iterator;
using std::getline;
using std::tolower;
using std::ispunct;
using std::isalpha;
using std::endl;

bool nuskaityti_teksta_is_failo(const string& kel, vector<string>& eilutes);
bool nuskaityti_galunes_is_failo(const string& kel, vector<string>& galunes);

string isvalyti_zodi(const string& teis);

void zodziu_skaiciavimas(const vector<string>& eilutes, unordered_map<string, int>& kiek, unordered_map <string, vector<int>> & zodzio_eilutes);
void zodziu_surikiavimas(const unordered_map<string, int>& kiek,vector<pair<string, int>>& surikiuoti);
void zodziu_isvedimas_i_faila(const string& kel,const vector<pair<string, int>>& surikiuoti,const unordered_map<string, vector<int>>& zodzio_eilutes);

void nuorodu_istraukimas(const vector<string>& eilutes,const vector<string>& galunes,vector<string> & nuorodos);
void nuorodu_isvedimas_i_faila(const string& kel, const vector<string> & nuorodos);
