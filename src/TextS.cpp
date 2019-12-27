#include "TextS.h"
#include <utility> //swap
#include <algorithm> //random suffle
#include <map>
#include <math.h>  //log
#include "Polygramme.h" //log
#include "KeyS.h"
#include <DecryptWindow.h>

TextS::TextS() : Text()
{

}

TextS::TextS(string text) : Text()
{
    this->textInput=text;
    cleanInputText();
}
/*
* La methode decrypt s'occupe du déchiffrement par Substitution
* elle est de type void
* elle prend comme paramètre une clé de type Key
* dans cette méthode on va lire le texte caractère par caractère
* tout en remplaçant chaque lettre par celle qui lui convient à partir de key (La clé)
*
*/
void TextS::decrypt(Key key){

    char c; //variable intermediaire

    for (int i = 0; i < (int) textModified.length() ; ++i)
    {

            for (int j = 0 ; j < 26 ; j++){
                if ( textModified.at(i) == key.getContent()[j]){

                    c = j + 65;
                    //cout << key.getContent()[j];
                    textOutput += c;

                    j = 26; //to quit the loop
                }
            }

    }
}

void TextS::encrypt(Key key){

    string k=key.getContent();

    int i=0;

    while (i<(int)textModified.length()){
         switch(textModified[i])
    {
        case 'A' :
            textOutput+=k[0]; break;
        case 'B' :
            textOutput+=k[1]; break;
        case 'C' :
            textOutput+=k[2]; break;
        case 'D' :
            textOutput+=k[3]; break;
        case 'E' :
            textOutput+=k[4]; break;
        case 'F' :
            textOutput+=k[5]; break;
        case 'G' :
            textOutput+=k[6]; break;
        case 'H' :
            textOutput+=k[7]; break;
        case 'I' :
            textOutput+=k[8]; break;
        case 'J' :
            textOutput+=k[9]; break;
        case 'K' :
            textOutput+=k[10]; break;
        case 'L' :
            textOutput+=k[11]; break;
        case 'M' :
            textOutput+=k[12]; break;
        case 'N' :
            textOutput+=k[13]; break;
        case 'O' :
            textOutput+=k[14]; break;
        case 'P' :
            textOutput+=k[15]; break;
        case 'Q' :
           textOutput+=k[16];  break;
        case 'R' :
            textOutput+=k[17]; break;
        case 'S' :
           textOutput+=k[18];  break;
        case 'T' :
            textOutput+=k[19]; break;
        case 'U' :
            textOutput+=k[20]; break;
        case 'V' :
           textOutput+=k[21]; break;
        case 'W' :
           textOutput+=k[22]; break;
        case 'X' :
           textOutput+=k[23]; break;
        case 'Y' :
           textOutput+=k[24]; break;
        case 'Z' :
            textOutput+=k[25]; break;
        default :
            textOutput+=textModified[i]; break;
    }

    i++;

  }

}
/*
 * La méthode decrypt sans paramètres est de type void
 * cette dernière est chargé de cryptanalyser un texte (chiffré par substitution )
 * afin de trouver une grande partie du texte avant cryptage
 * au début on génère une clé aléatoire et on décrypte le chiffré avec
 * on calcule le taux de compatibilité
 * ensuite on appliquera des permutations sur la clé et on recalcule a chaque fois le taux de compatibilité
 * jusqu'a a ce qu'il n y ait plus d'amélioration du taux de compatibilité
 * on décrypte le texte une dernière fois en utilisant la clé ayant le meilleur score
 */
void TextS::decrypt(){

    char* key_p = (char*) malloc(26 * sizeof(char*));
    char* key_c = (char*) malloc(26 * sizeof(char*));
    double Scoremax=-99e9;
    double Score =0;
    int k;

        /*choix 1*/

        KeyS key_tempo("OPQRSTUVWXYZABCDEFGHIJKLMN");
        encrypt(key_tempo);
        textInput = textOutput; //modifiedtext =textinput

        /*init key_p*/
        for(int i=0; i<26; i++)
        {
           key_p[i] = i + 65;
        }
        /*shuffle*/
        /*choix 2*/
        random_shuffle(&key_p[0],&key_p[25]);



            /* convertir les occurences des quadrigrammes en pourcentage*/
           P.somme = 0;
            for (auto& kv : this->P.QuadFrancais)
            {
                double val = kv.second;

                P.somme = P.somme +val;

            }

            for (auto& kv : this->P.QuadFrancais)
            {
                double val = kv.second;
                val = 100 * val;
                val = val / this->P.somme ;
                //cout<<P.somme<<endl;
                this->P.QuadFrancais[kv.first] = val;
            }

            /*Start*/
            k = 0;

            while (k < 2000)
            {
            //affectation key_c=key_p
            for(int i=0; i<26; i++){

                key_c[i] = key_p[i];
            }


            /*random index*/
            int a=rand()%25;
            int b=rand()%25;
            swap(key_c[a],key_c[b]);



            KeyS key_dec(key_c);
            textOutput = "";
            decrypt(key_dec);

            //calculer l'indice de finesse score=ifinesse
            Score = getCompatibilityRate();
            //Tester si le score s'est amélioré

            if (Scoremax < Score)
            {    //affectation key_c=key_p
                for(int i=0; i<26; i++)
                {
                    key_p[i] = key_c[i];
                }
                Scoremax = Score;
                k=0;
            }
            k++;


            }
            KeyS key_dec(key_p);
            textOutput = "";
            decrypt(key_dec);

        free(key_c);
        free(key_p);

}

/*
 * getCompatibilityRate ne prend pas de paramètres
 * de type double
 * cette méthode calcule le taux de compatibilité d'un text en utilisant la map des quadrigrammes de la langue fr
 * elle retourne le score (taux de compatibilité du texte)
 */
double TextS::getCompatibilityRate(){
    double Score =0;
    string temp;

        /* Calcul de la somme des frequences */
        float somme =2.81601e+08;

        /* Calcul du score */
        for(int i=0 ; i<(long)textOutput.length()-3 ; i++) //parcourir la map
        {
            temp = textOutput.substr(i,4);
            if (this->P.QuadFrancais.count(temp))
            {
                Score = Score + log10(this->P.QuadFrancais[temp]);
            }else{
                Score = Score +log10(0.01/somme);
            }

        }

        Score = Score / textOutput.length()-3 ;

    return Score;
}
TextS::~TextS()
{

}
void TextS::setTextInput()
{

}
//IGNGPYTBGIUYTPYIGQTUDUNGKESXUSASYUQTUTXNGYCPUYYUJSPGRXPNSYUPKREXTGYCURGXTPCSIPXUCUNDUXYPXUNGYYUNIUNQXGYGPNEQQXUYTSYRUTPTCGDUGSIUSXGKESXUSACURUTPTCGDUGSRUSTTXUSYWESJSUTDUQIUSXNSYWPHESESSYUNEXTPUESWPUYIUNTXEPNIGQEPNGDRUYDBXGPKUYTDUNOUYNDUNOETNDUIPKGOPYGTPEYUTDUNKEFUYNDUCZGCSYCUIGRUSTTXUSYUNEPXUXEKGYTPJSUDGYNSYXUNTGSXGYTGBUCBSUNSXIGNUPYURESXIUNSYNSYNPKRIUWGPNUXCZGYONESNSYREXCZURESXIUNGSTXUNCUNTCUJSPQGPTRUSTTXUIUNSCCNDUCUTTUQTUXPUYYUNTDCPDCZGCSYQGPTCEKKUPIBUSTUTCEYTXGPXUKUYTIGRISRGXTDUNQTUNCEKKURJSUNYEIESIUYESBUIGYCUNTSYUQTUJSPYUNUQTURGNUYQGKPIIURGNJSUNTPEYDUKKUYUXIUNUYQGYTNCUHESXICUNTIGYESYESJSPNUYECCSRUUTCUSAJSPYEYTRGNDURUTPTGKPUTWPUYPINEYTIUCZEPAUYTXUNEXTPXUYTXUCIPWGTGPXUNESTUYPXIGCZGYDUIIUUTCZUVBESNCUNTCEKKUYTCEKKUYTQTUTEYIGNGPYTBGIUYTPYEQQXUVBESNDUNQIUSXNESGSTXUCZENUCUTTUQTUUAPNTUTUIIUGSNNPDGYNBETXURGFN
