#ifndef KeyVigenere_h
#define KeyVigenere_h


#include "Key.h"
#include <stdio.h>

class KeyV : public Key{

private:
    int size;
    
public:
    
    KeyV();
    KeyV(string contenu);
    int getSize();
    
};
#endif /* KeyVigenere_hpp */
