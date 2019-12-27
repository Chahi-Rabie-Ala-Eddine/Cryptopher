#include "Polygramme.h"
#include <vector>
#include <fstream>
#include <iostream>


/*
const char Polygramme::LETTER_FR[] = {'a','e','b'}; //esq il faut savoir la frequence ou juste l'ordre
const string Polygramme::BIGRAM_FR[] = {"es","de","du"};
const string Polygramme::TRIGRAM_FR[] = {"des","ces"};
*/
float Polygramme::LetterFrancais[26] = {8.4, 1.06, 3.03, 4.18, 17.26, 1.12, 1.27, 0.92, 7.34, 0.31, 0.05, 6.01, 2.96, 7.13, 5.26, 3.01, 0.99, 6.55, 8.08, 7.07, 5.74, 1.32, 0.04, 0.45, 0.30, 0.12 };

map<string, float> Polygramme::BigFrancais = {{"ES",3318},{"DE",2409},{"LE",2366},{"EN",2121},{"RE",1885},{"NT",1694},{"ON",1646},{"ER",1514},{"TE",1484},{"EL",1382},{"AN",1378},{"SE",1377},{"ET",1307},{"LA",1270},{"AI",1255},{"IT",1243},{"ME",1099},{"OU",1086},{"EM",1056},{"IE",1030}};

map<string, float> Polygramme::TrigFrancais =  {{"ES",3318},{"DE",2409},{"LE",2366},{"EN",2121},{"RE",1885},{"NT",1694},{"ON",1646},{"ER",1514},{"TE",1484},{"EL",1382},{"AN",1378},{"SE",1377},{"ET",1307},{"LA",1270},{"AI",1255},{"IT",1243},{"ME",1099},{"OU",1086},{"EM",1056},{"IE",1030}};

map<string, float> Polygramme::QuadFrancais ;

const vector<string> split_(const string& s, const char& c)
{
	string temp="";
	vector<string> v;

	for(auto n:s)
	{
		if(n != c) temp+=n; else
		if(n == c && temp != "") { v.push_back(temp); temp = ""; }
	}
	if(temp != "") v.push_back(temp);

	return v;
}

Polygramme::Polygramme(){

    /* Calculer les frequences des quad :
    *on va télécharger le fichier contenant la plupart des quad de la langue fr
    *le lire ligne par ligne
    *puis on stockera les quad avec leurs fréquences dans une map QuadFrancais
    */
    double somme = 0;
    ifstream fichier("../src/french_quadgrams.txt", ios::in);  // on ouvre le fichier en lecture
        if(fichier)  // si l'ouverture a réussi
        {   string line;
            while( getline(fichier,line)) //lire une ligne
            {
                vector<string> result = {split_(line, ' ')};
                string temp =result[0];
                float val = stod(result[1]);
                somme = somme + val;             //calculer la somme des val a partir des valeurs de QuadFrancais
                QuadFrancais.insert(pair<string, float>(temp,val));


            }


        fichier.close();  // on ferme le fichier
        }
        else
        {// sinon
            cerr << "Impossible d'ouvrir le fichier !" << endl;
            exit(0);
        }
        this->somme=somme;

    
}

Polygramme::Polygramme(string content){
    this->content = content;
    frequence = 0;
}



list<int> Polygramme::getPositions(){
    return positions;
}
int Polygramme::getFrequency(){
    return frequence;
}
string Polygramme::getContent(){
    return content;
}

void Polygramme::addPosition(int pos){
    positions.push_back(pos);
    
    frequence++;
}

Polygramme::~Polygramme()
{}
