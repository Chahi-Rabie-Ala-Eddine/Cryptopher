#ifndef TEXT_SUBSTITUTION_H
#define TEXT_SUBSTITUTION_H

#include <string.h>
#include "Text.h"

using namespace std;

class TextS: public Text
{

public:
    TextS();
    TextS(string text);
    ~TextS();
    Polygramme P;
    void encrypt(Key);
    void decrypt(Key);
    void decrypt();

    double getCompatibilityRate();
    void setTextInput();
    //~TextS();


};

#endif // TEXT_SUBSTITUTION_H
