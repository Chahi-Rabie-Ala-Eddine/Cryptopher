#ifndef GESTFICHIER_H
#define GESTFICHIER_H

#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <vector>
#include<string.h>

using namespace std;


void exportTxt ( string text, string chemin, string nomFichier);

void ReplaceStringInPlace(std::string& s, const std::string& from, const std::string& to);

void exportWord(string text, string chemin, string nomFichier);

void exportPdf(string text);

string importTxt( string path);

string importWord(string path);

string importPdf(const char* path);

#endif // GESTFICHIER_H
