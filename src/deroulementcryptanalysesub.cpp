#include "deroulementcryptanalysesub.h"
#include "ui_deroulementcryptanalysesub.h"
#include <iostream>

using namespace std;

deroulementcryptanalysesub::deroulementcryptanalysesub(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deroulementcryptanalysesub)
{
    ui->setupUi(this);
}

deroulementcryptanalysesub::~deroulementcryptanalysesub()
{
    delete ui;
}

string deroulementcryptanalysesub::generate_m_key()
{
    srand(time(NULL));
    string m_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string m_key = "";
    int* resultat = (int*)malloc((26)*sizeof (int));
    int nombre_tire=0;
    int temp=0;

    for(int i = 0; i < 26; i++)
    {
        resultat[i] = i;
    }
    for(int i = 0 ; i < 26 ; i++)
    {
        nombre_tire = rand() % 26 ;
        temp = resultat[i];
        resultat[i] = resultat[nombre_tire];
        resultat[nombre_tire] = temp;
    }
    for(int i = 0 ; i < 26 ; i++)
    {
        m_key+= m_chars[resultat[i]];
    }

    free(resultat);

    this->cle_mere = m_key;

    return m_key;
}

string deroulementcryptanalysesub::new_text()
{
    TextS* txt = new TextS(this->text);
    this->cle_mere.setContent(generate_m_key());
    //text->decrypt(this->cle_mere);

   // cout << " clé mère : " << cle_mere.getContent() << endl;

/*    cout << " Texte Crypté : " << text->textOutput << endl;

    return text->textOutput;*/
    return "";
}

void deroulementcryptanalysesub::getQuadrigrams()
{
    //remplir les quadrigrams
}

double deroulementcryptanalysesub::getCompatibility()
{
    // calcule l'indice de finesse
}

bool deroulementcryptanalysesub::getScore()
{

}
//score_max score text polygrams


