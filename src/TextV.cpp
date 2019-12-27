#include "TextV.h"
#include "KeyV.h"
#include "Text.h"
#include"QString"
using namespace std;
/**
 * \TextV::TextV()
 * \role : constructeur par defaut
 */
TextV::TextV()
{

}

/**
 * \TextV::TextV(string content)
 * \role : constructeur avec argument
 * \arg content : le texte a chiffrer ou dechiffrer
 */
TextV::TextV(string content){
    this->textInput = content;
    this->cleanInputText();
    QString val = QString::fromStdString(textInput);
    val.replace(" ","");
    textInput = val.toStdString();
}

/**
 * \rôle Méthode qui s'occupe de chiffrer un text selon le chiffre de Vigenère
 * \arg key la clé qu'on utilise pour le chiffrement
 *  \description
    à chaque itération de la boucle  on effectue une addition mod26 des codes ASCII de chaque lettre du texte avec
    le code ASCII de la lettre qui lui correspond -65 afin d'avoir leirs indices dans l'alphabet, on a une condition à l'interieur de la boucle
   qui remet le keyIndex à 0 afin de répeter la clé tout au long du texte.
   \return cetye méthode ne renvoie rien par contre elle modifie textOutput afin d'y stocker le chiffré calculé
*/
void TextV::encrypt(Key key){

    int i;
    unsigned long int KeyIndex  = 0;
    textOutput="";
    for(i = 0; i < (int)textModified.length(); i++)
    {
        textOutput += (((textModified[i] - 65) + (key.getContent()[KeyIndex] - 65)) % 26) + 65;
        KeyIndex ++;
        if (KeyIndex == key.getContent().length()){
            KeyIndex = 0;
        }

    }

}

/**
 * \void TextV::decrypt( Key key)
 * \role : méthode qui dechiffre un texte avec la possession de la clé
 * \arg key : la clé de dechiffrement.
 */
void TextV::decrypt( Key key){

    for (int i = 0, j = 0; i <(int) this->getTextInput().length() ; i++ , j = (j+1) % key.getContent().length()  ) {
        textOutput += char ((((textInput[i]-65) - (key.getContent()[j] - 65) )+26)% 26) + 65 ;
    }
}

/**
 * \void TextV::decrypt()
 * \role : méthode qui fait Appel a tous les méthodes a fin de dechifrer un texte sans clé
 */
void TextV::decrypt(){

    this->textModified="";
    generatePolygrams();

    int keySize = findKeySize() ;


    Key key=findKeyUsed(keySize);

    textModified += to_string(keySize) + ":0+\n"; // ":0," juste pour conserver la syntaxe (une seule boucle)
    textModified += key.getContent() + ":0+\n";
    textModified += '\0';//fin fichier



    decrypt(key);

   // qInfo() << QString::fromStdString(this->textOutput) ;

}

/**
 * \int TextV::findKeySize()
 * \role : méthode qui calcule la taille de la clé, tout en calculant les diviseurs des distances qui separent les polygrammes
 * \return int : elle retourne la taille de la clé la plus probable
 */
int TextV::findKeySize(){

    /*
     dans cette fonction, on recupere les posistions qui separent les polygrammes, on clacule leurs diviseurs
     et on stock les plus cumuns dans un tableau 'most_propable_divisors' ;
     Ensuite on fait appelle
     */
    textModified = "";
    map<int,int> divisors; // correspendance entre diviseur et nombre d'occurence
    // most_propable_divisors : les 10 diviseurs les plus communs
    int most_propable_divisors[10][2] = {{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1},{12,1}};
    for ( const auto &myBigram : bigram ) {
        if (myBigram.second->getFrequency() != 1) {
            int size = (int)myBigram.second->getPositions().size();
            int positions[size];
            list<int> distances;
            int i = 0;
            for (int pos : myBigram.second->getPositions() ){
                positions[i] = pos;
                i++;
            }
            for(int i=0 ; i<=size-1 ; i++){

                for (int j=i+1; j < size  ; j++) {

                    int distance = positions[j] - positions[i];
                    distances.push_back(distance);
                    for (int divisor = 3; divisor < distance; divisor++) {

                        if(distance%divisor==0){
                            if (divisors.find(divisor) != divisors.end()) {

                                divisors.find(divisor)->second++;

                                int min = most_propable_divisors[0][1];
                                int min_index = 0;
                                for (int z = 1; z < 10; z++) {
                                    if (most_propable_divisors[z][0] == divisors.find(divisor)->first) {
                                        most_propable_divisors[z][1]++;
                                        min_index = -1;
                                        break;
                                    }
                                    if (most_propable_divisors[z][1] < min) {
                                        min = most_propable_divisors[z][1];
                                        min_index = z;
                                    }
                                }
                                if (min_index != -1 && most_propable_divisors[min_index][1] < divisors.find(divisor)->second) {
                                    most_propable_divisors[min_index][1] = divisors.find(divisor)->second;
                                    most_propable_divisors[min_index][0] = divisors.find(divisor)->first;
                                }

                            }else{
                                divisors.insert({divisor, 1});
                            }
                        }
                    }
                }
            }
        }
    }

    for ( const auto &myTrigram : trigram ) {
        if (myTrigram.second->getFrequency() != 1) {
            int size = (int)myTrigram.second->getPositions().size();
            int positions[size];
            list<int> distances;
            int i = 0;
            for (int pos : myTrigram.second->getPositions() ){
                positions[i] = pos;
                i++;
            }

            for(int i=0 ; i<=size-1 ; i++){

                for (int j=i+1; j < size  ; j++) {

                    int distance = positions[j] - positions[i];
                    distances.push_back(distance);
                    for (int divisor = 3; divisor <= distance; divisor++) {

                        if(distance%divisor==0){
                            if (divisors.find(divisor) != divisors.end()) {
                                divisors.find(divisor)->second++; int min = most_propable_divisors[0][1];

                                int min_index = 0;
                                for (int z = 1; z < 10; z++) {
                                    if (most_propable_divisors[z][0] == divisors.find(divisor)->first) {
                                        most_propable_divisors[z][1]++;
                                        min_index = -1;
                                        break;
                                    }
                                    if (most_propable_divisors[z][1] < min) {
                                        min = most_propable_divisors[z][1];
                                        min_index = z;
                                    }
                                }
                                if (min_index != -1 && most_propable_divisors[min_index][1] < divisors.find(divisor)->second) {
                                    most_propable_divisors[min_index][1] = divisors.find(divisor)->second;
                                    most_propable_divisors[min_index][0] = divisors.find(divisor)->first;
                                }

                            }else{
                                divisors.insert({divisor, 1});
                            }
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < 10 ; i++ ){
        textModified += to_string(most_propable_divisors[i][0]) + ":" + to_string(most_propable_divisors[i][1]) + "+";
    }
    textModified += "\n";



    float keySize[10][2]; // correspondance entre l'indice de coicidence et la tailles de la clé

    //cette boucle rempli le tableau 'keySize'
    for (int k = 0; k< 10; k++ ) {
        list<string> lines = divideText(most_propable_divisors[k][0]);
        float indice_of_given_size = 0;
        for(auto line : lines){
            float indice_of_line = indiceOfCoincidence(line);
            indice_of_given_size += indice_of_line;
        }
        indice_of_given_size = indice_of_given_size / lines.size();
        keySize[k][1] =  most_propable_divisors[k][0];
        keySize[k][0] = indice_of_given_size;
    }

    //Cette boucle trie le tableau 'keySize' par rapport à l'indice de coicidence , du max au min
    for (int i = 0; i < 9 ; i++) {
        for (int j = i+1; j < 10; j++) {
            if (keySize[j][0] > keySize[i][0]) {
                float ic = keySize[i][0];
                int size = keySize[i][1];
                keySize[i][0] = keySize[j][0];
                keySize[i][1] = keySize[j][1];
                keySize[j][0] = ic;
                keySize[j][1] = size;
            }
        }
    }

    //indice de coicidence et la tailles de la clé
    for(int i = 0; i < 10 ; i++ ){
        textModified += to_string(keySize[i][0]*100) + ':' + to_string((int)keySize[i][1]) + '+';

    }
    textModified += '\n';//fin ligne

    float size = keySize[0][1];
    float ICKey = keySize[0][0];
    //Prendre la clé la plus prbable et la plus PETITE,
    //exp : on prend une clé de tailles 12 à la place de 24 si la diffrence entre
    //  ces deux derniers ne depasse pas 0.01
    for (int i = 1; i < 10; i++) {
        if ( ( ICKey - keySize[i][0] ) < 0.01 ){
            if( ( size > keySize[i][1] ) && ( ( ((int)size) % ((int)keySize[i][1]) ) == 0) ){
                size = (int)keySize[i][1];
                ICKey = keySize[i][0];
            }
        }
    }

    return size;

}

/**
 * \list<string> TextV::divideText(int keySize)
 * \role : méthode qui regroupe textInput en plusieurs partie par rapport a la taille de la clé
 * \arg keySize : la taille de la clé (nombre de repartition).
 * \return list<string> : une listes des chaines de caractere groupées
 */
list<string> TextV::divideText(int keySize){
    //cette fonction divise le texte en liste de chaine de caractere dont le nombre est egal à keySize
    list<string> lines;
    for (int j = 0; j < keySize; j++) {
        string line = "";
        for (int i = j; i < (int)textInput.length() ; i += keySize) {
            line += textInput[i];
        }
        lines.push_back(line);
    }

    return lines;
}

/**
 * \Key TextV::findKeyUsed(int keySize
 * \role : méthode qui cherche la clé utilsé a partir de sa taille
 * \arg keySize : la taille de la clé.
 * \return Key : une instance de la clé trouvée
 */
Key TextV::findKeyUsed(int keySize){

    //cette fonction retourne la clé trouvée

    list<string> lines = divideText(keySize);
    string keyFound;
    int decalage = 0;
    //cette boucle utilise la formule décrite dans le livre 'Cryptographie' page 33
    for (auto line : lines) {
        // letter_in_line : chaque lettre et sa frequence dans la chaine de caractere 'line'
        map<int,int> letter_in_line = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{19,0},{20,0},{21,0},{22,0},{23,0},{24,0},{25,0}};

        for (int i = 0; i < (int) line.length() ; i++) {
            letter_in_line[line[i] - 65]++;
        }
        float max = 0;

        for (int g = 0; g < 26; g++) {
            int somme = 0;
            for (int i = 0; i < 26; i++) {
                float produit = Polygramme::LetterFrancais[i] * letter_in_line[(i + g)%26];
                float division = produit * (float)( textInput.length() / keySize );
                somme += division;
            }
            if (max < somme) {
                max = somme;
                decalage = g;
            }
        }
        string temp = string(1, decalage+65);
        keyFound += temp;
    }

    //Instanciation de la clé trouvée
    KeyV* key=new KeyV();
    key->setContent(keyFound);
    return *key;
}

/**
 * \float TextV::indiceOfCoincidence(string line)
 * \role : calcule l'indice de coincidence d'une chaine de charactere
 * \arg line : la chaine de caractere auquelle on cherche l'indice de coincidence.
 * \return float : la valeur trouvé apres léapplication de la formule de l'indice de coincidence
 */
float TextV::indiceOfCoincidence(string line){
    //cette fonction revoie l'indice de coicidence par rapport à la langue francaise

    //letter_in_line : correspondance lettre et sa frequence dans la chaine line
    map<int,int> letter_in_line = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{19,0},{20,0},{21,0},{22,0},{23,0},{24,0},{25,0}};
    for (int i = 0; i < (int)line.length(); i++) {
        letter_in_line[line[i] - 65]++;
    }

    //Utilisation de la formule de l'indice de coincidence
    float somme = 0;
    float produit_2 = line.length() * ( line.length() - 1);
    for (int i = 0; i < 26; i++) {
        float produit_1 = letter_in_line.at(i) * ( letter_in_line.at(i) - 1 );
        float division = produit_1 / produit_2;
        somme += division;
    }

    return somme;
}

void TextV::setTxt(string txt)
{
    this->textInput = txt;
}

TextV::~TextV(){}



