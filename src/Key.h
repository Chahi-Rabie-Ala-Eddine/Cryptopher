#ifndef KEY_H
#define KEY_H

#include <iostream>
#include <string.h>

using namespace std;

class Key
{
    protected:
        string contenu;

    public:
        Key(string cont);
        string getContent();
        void setContent(string key);
        virtual ~Key();

};

#endif // KEY_H
