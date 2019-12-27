#include "KeyV.h"

KeyV::KeyV():Key("IMTHEDEFAULTKEY") {
    this->size=15;
    
}

KeyV::KeyV(string content):Key (contenu)
{
    this->size=content.length();
}

int KeyV::getSize(){
    return this->size;
}

