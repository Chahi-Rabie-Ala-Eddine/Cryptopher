#ifndef POLYGRAMME_H
#define POLYGRAMME_H


#include <string>
#include <list>
#include "map"

using namespace std;

class Polygramme
{
    
private:
    string content;
    list<int> positions;
    int frequence;

public:
    static float LetterFrancais[26] ;
    static map<string, float> BigFrancais;
    static map<string, float> TrigFrancais;
    static map<string, float> QuadFrancais;
    float somme=0;

public:
    Polygramme();
    Polygramme(string content);

    void incfreq();
    void addPosition(int pos);

    list<int> getPositions();
    int getFrequency();
    string getContent();


    ~Polygramme();

};

#endif // POLYGRAMME_H
