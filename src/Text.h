#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "map"
#include "Key.h"
#include "Polygramme.h"


using namespace std;

//comment on peut savoir le taux de satisfaction . peut etre on rajoute un attribut et une fonction !

class Text
{

protected:
    string textInput;
    string textModified; // texte sans espaces
    string textOutput;
    int size;
public:
    Polygramme * letter[26];
    map<string,Polygramme *> bigram;
    map<string,Polygramme *> trigram;
    map<string,Polygramme *> quadrigram;


public:

    int getSize();
    string getTextInput();
    string getTextModified();
    string getTextOutput();

    virtual void encrypt(Key) = 0;
    virtual void decrypt(Key) = 0;
    virtual void decrypt() = 0;

    void cleanInputText();
    void generatePolygrams();

    virtual ~Text();

};

#endif // TEXT_H
