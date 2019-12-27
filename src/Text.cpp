#include "Text.h"
#include <QString>
int Text::getSize(){
    return this->size;
}

string Text::getTextInput(){
    return this->textInput;
}

string Text::getTextOutput(){
    return this->textOutput;
}

string Text::getTextModified(){
    return this->textModified;
}

/**
 * \void Text::cleanInputText()
 * \role : méthode utilisé lors de chiffrement a fin de chiffrer un texte sans espaces et caracteres speciaux
 */
void Text::cleanInputText(){

    QString str= QString::fromStdString(textInput);
    str.toLower();
    str.replace("á","A");
    str.replace("à","A");
    str.replace("â","A");
    str.replace("ä","A");
    str.replace("ã","A");
    str.replace("å","A");

    str.replace("é","E");
    str.replace("è","E");
    str.replace("ê","E");
    str.replace("ë","E");

    str.replace("í","I");
    str.replace("ì","I");
    str.replace("î","I");
    str.replace("ï","I");

    str.replace("ó","O");
    str.replace("ò","O");
    str.replace("ô","O");
    str.replace("ö","O");
    str.replace("õ","O");

    str.replace("ú","U");
    str.replace("ù","U");
    str.replace("û","U");
    str.replace("ü","U");

    str.replace("ç","C");


    textInput=str.toUpper().toStdString();

    string tab[10]={"ZERO","UN","DEUX","TROIS","QUATRE","CINQ","SIX","SEPT","HUIT","NEUF"};

    for(int i=0;i<(int)textInput.length();i++)
       {
            if((int) textInput[i] <=90 && (int) textInput[i] >=65 )
                   textModified+=textInput[i];

             if((int) textInput[i] <=57 && (int) textInput[i] >=48 )
                 textModified+=tab[(int) textInput[i]-48];
       }


    size = (int)textModified.length();

}

/**
 * \void Text::generatePolygrams()
 * \role : méthode qui cherche tous les polygrammes de textInput a fin de les utiliser dans la cryptanalyse
 */
void Text::generatePolygrams(){

    //Contruction des polygrammes

    //Initialiser le tableau Text::letter en instanciant des Polygramme
    for (int i = 0; i< 26; i++) {
        string s = string(1, i + 65);
        letter[i] = new Polygramme(s);
    }

    //chaque caractère du polygramme , exp : "ABCD"
    int first_char = textInput[0]; //first_char = 'A'
    int second_char = textInput[1]; //second_char = "B"
    int third_char = textInput[2]; //third_char = "C"
    int fourth_char = textInput[3]; //fourth_char = "D"

    Polygramme *STLetter; // second and third letter of polygram
    Polygramme *TFLetter; // third and fourth letter of polygram

    Polygramme *FSTLetter; // first and second and third letter of polygram
    Polygramme *STFLetter; // second and third and fourth letter of polygram

    Polygramme *FSTFLetter;// Quadrigramme

    //Ajouter la postion des Trois premieres lettres
    letter[first_char - 65]->addPosition(0);
    letter[second_char - 65]->addPosition(1);
    letter[third_char - 65]->addPosition(2);

    // Les premiers bigrammes
    string firstSecond = string(1,first_char) + string(1,second_char);
    string secondThird = string(1,second_char) + string(1,third_char);
    string thirdFourth = string(1,third_char) + string(1,fourth_char);

    //les Premiers Trigrammes
    string FirstSecondThird = string(1,first_char) +  string(1,second_char) + string(1,third_char);
    string SecondThirdFourth = string(1,second_char) + string(1,third_char) + string(1,fourth_char);

    string FirstSecondThirdFourth = string(1,first_char) +  string(1,second_char) + string(1,third_char) + string(1,fourth_char);


    //First Bigramme
    STLetter = new Polygramme(firstSecond);
    bigram.insert( make_pair(firstSecond, STLetter));
    STLetter->addPosition(0);

    //Second Bigramme
    if (bigram.find( secondThird ) != bigram.end()) {
        TFLetter = bigram.find(secondThird)->second;
    } else {
        TFLetter = new Polygramme(secondThird);
        bigram.insert( make_pair(secondThird, TFLetter));
    }
    TFLetter->addPosition(1);

    //First Trigramme
    FSTLetter = new Polygramme(FirstSecondThird);
    trigram.insert( make_pair(FirstSecondThird, FSTLetter) );
    FSTLetter->addPosition(1);

    //Boucle pour Construire le reste des Polygrammes
    for (int i = 3; i <(int) textInput.length(); i++) {

        //Ajouter la lettre et sa position
        fourth_char = textInput[i];
        letter[fourth_char - 65]->addPosition(i);

        thirdFourth =  string(1,third_char) + string(1,fourth_char);
        SecondThirdFourth = string(1,second_char) +  string(1,third_char) + string(1,fourth_char);
        FirstSecondThirdFourth = string(1,first_char) + string(1,second_char) +  string(1,third_char) + string(1,fourth_char);
        if (bigram.find(thirdFourth) != bigram.end()) {
            TFLetter = bigram.find(thirdFourth)->second;
        } else {
            TFLetter = new Polygramme(thirdFourth);
            bigram.insert( make_pair(thirdFourth, TFLetter));
        }
        TFLetter->addPosition(i-1);

        if (trigram.find(SecondThirdFourth) != trigram.end()) {
            STFLetter = trigram.find(SecondThirdFourth)->second;
        } else {
            STFLetter = new Polygramme(SecondThirdFourth);
            trigram.insert( make_pair(SecondThirdFourth, STFLetter));
        }
        STFLetter->addPosition(i-2);

        if (quadrigram.find(FirstSecondThirdFourth) != quadrigram.end()) {
            FSTFLetter = quadrigram.find(FirstSecondThirdFourth)->second;
        } else {
            FSTFLetter = new Polygramme(FirstSecondThirdFourth);
            quadrigram.insert( make_pair(FirstSecondThirdFourth, FSTFLetter));
        }
        FSTFLetter->addPosition(i-3);

        first_char = second_char;
        second_char = third_char;
        third_char = fourth_char;

    }



    /*
     //Pour afficher les Polygrammes
     cout << "\n";
     for (int i = 0; i < 26; ++i)
     cout << letter[i]->getContent() << letter[i]->getFrequency() <<" \\ ";
     cout << "\n";
     for (map<string,Polygramme *>::iterator it = bigram.begin(); it != bigram.end(); ++it)
     cout << it->first << it->second->getFrequency() << " \\ ";
     cout << "\n";
     for (map<string,Polygramme *>::iterator it = trigram.begin(); it != trigram.end(); ++it)
     cout << it->first << it->second->getFrequency() << " \\ ";
    cout << "\n";
     for (map<string,Polygramme *>::iterator it = quadrigram.begin(); it != quadrigram.end(); ++it)
        cout << it->first << it->second->getFrequency() << " \\ ";
    */
}

Text::~Text(){}

/*
la Saint-Valentin, la fête des amoureux. Une fête très ancienne qui a pris une importance particulière ces dernières années. Les Français offrent un petit cadeau à leur amoureux. Ce petit cadeau peut être un bouquet de fleurs, un bijou ou une sortie… ou bien, les trois à la fois. Ça dépend vraiment des gens, des goûts, de l’imagination et des moyens de chacun.
*/
