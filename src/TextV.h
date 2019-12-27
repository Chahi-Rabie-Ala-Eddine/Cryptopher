#ifndef TEXT_VIGENERE_H
#define TEXT_VIGENERE_H

#include "Text.h"
#include <iostream>
#include <string>
#include <list>
#include"KeyV.h"

class TextV: public Text
{

public:
    TextV();
    TextV(std::string);
    void encrypt(Key);
    void decrypt(Key);
    void decrypt();
    void setTxt(string txt);
    int findKeySize();
    list<string> divideText(int keySize);
    Key findKeyUsed(int keySize);
    float indiceOfCoincidence(string line);

    ~TextV();
    
private:
    using Text::decrypt;

};

#endif // TEXT_VIGENERE_H
