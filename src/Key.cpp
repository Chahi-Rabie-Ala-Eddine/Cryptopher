#include "Key.h"

string Key::getContent(){
    return this->contenu;
}

void Key::setContent(string key)
{
    this->contenu = key;
}

Key::Key(string cont)
{
  contenu=cont;
}

Key::~Key(){
}
