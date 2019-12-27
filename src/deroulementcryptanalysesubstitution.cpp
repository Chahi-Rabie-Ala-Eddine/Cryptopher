#include "deroulementcryptanalysesubstitution.h"
#include <QCheckBox>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QString>
#include <iostream>
#include <ResultWindow.h>
#include <Text.h>
#include <Key.h>
#include <KeyS.h>
#include <TextS.h>
#include <TextV.h>
#include <EncryptWindow.h>
#include <MainWindow.h>
#include <QFileDialog>
#include"deroulementcryptsub.h"
#include"deroulementvigenere.h"
#include"gestfichier.h"
#include <cstdlib>
#include <TextS.h>
#include <math.h>
#include <sstream>
#include <utility>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

DeroulementCryptanalyseSubstitution::DeroulementCryptanalyseSubstitution(string m_text) : QWidget()
{
    setStyleSheet("QLabel {color : blue} QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

    setFixedSize(900, 500);
    this->m_score_max = -99e9;
    this->init_text = m_text;
    this->textSub = m_text;
    texte = new QTextEdit();
    const char* a = m_text.c_str();
    const char* b = "TEXTES : <br>________________________________________________________<br><br>1 - Texte à décrypter : </br></br><br></br><br></br></br>";
    char result[10000];
    strcpy(result,b);
    strcat(result,a);
    const char* c = result;
    texte = new QTextEdit(c ,this);
    texte->setReadOnly(true);
    texte->setGeometry(10, 10 , 430, 480);
    txt = new QTextEdit();
    txt = new QTextEdit("<html> ETAPES : <br><br> On cherche à décrypter (🔓) le texte sans posséder de la 🗝.<br>L'indice de compatibilité de départ est fixé à : -99e9</br><br><br> Appuier sur le bouton Start pour commencer !</br></br></br></br>",this);
    txt->setReadOnly(true);
    txt->setGeometry(450, 10 , 440, 430);
    QPushButton* next = new QPushButton("Start",this);
    next->setGeometry(820, 450 , 70, 30);
    QObject::connect(next, SIGNAL(clicked()), this, SLOT(process_steps()));
    TextS* final_txt = new TextS(this->init_text);
    QFuture<void> future = QtConcurrent::run(final_txt, &TextS::decrypt);
    future.waitForFinished();
    cout << "Text clair : " << final_txt->getTextInput() << endl;
    //final_txt->decrypt();
    cout << "Text crypté " <<final_txt->getTextOutput() << "\n" << endl;
    this->init_text = final_txt->getTextOutput();
}

string DeroulementCryptanalyseSubstitution::generate_m_key()
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

    this->key_mother = m_key;

    string m_txt = txt->toPlainText().toStdString() + "__________________________________________________________\n\n On tire d'abord une clée aléatoire de 26 caractère : \n\n🔑 [" + m_key+ "]\n__________________________________________________________\n\n";
    txt->setText(QString::fromStdString(m_txt));
    this->key_mother = m_key;

    step = 1;

    return m_key;


}

string DeroulementCryptanalyseSubstitution::permute_key()
{
    this->key_daughter = this->key_mother;
    int a=rand()%25;
    int b=rand()%25;
    swap(key_daughter[a],key_mother[b]);
    string m_txt_un = txt->toPlainText().toStdString() + "\n\nOn mélange maintenant deux caractère de la clé mère 🗝 \npour avoir la clé fille 🔑 : \n\n🗝 ["+  this->key_mother +"]\n🔑 [" + this->key_daughter +"]\n________________________________________________________";
    txt->setText(QString::fromStdString(m_txt_un));

    step = 4;

    return " ";
}

string DeroulementCryptanalyseSubstitution::new_text()
{
    TextS* t = new TextS(this->textSub);
    KeyS* a_key = new KeyS(this->key_mother);
    t->decrypt(*a_key);
    string m_txt = texte->toPlainText().toStdString() + "\n________________________________________________________\n\ntexte crypé n°  "+ std::to_string(turn) + " : \n "+"\n" + t->getTextOutput()+"\n________________________________________________________\n\n";
    texte->setText(QString::fromStdString(m_txt));
    string m_txt_un = txt->toPlainText().toStdString() + "On décrypte maintenant le texte avec cette clé 🗝\n__________________________________________________________";
    txt->setText(QString::fromStdString(m_txt_un));
    this->textSub = t->getTextOutput();
    this->turn++;
    if(turn > 2)
        step = 5;
    else
        step = 2;
    getQuadrigrams();

    return " ";
}

void DeroulementCryptanalyseSubstitution::getQuadrigrams()
{
    for (auto& kv : this->P.QuadFrancais)
    {
        double val = kv.second;
        val = 100 * val;
        val = val / this->P.somme ;
        this->P.QuadFrancais[kv.first] = val;
    }
}

double DeroulementCryptanalyseSubstitution::getCompatibility()
{
    double Score =0;
    string temp;

        /* Calcul de la somme des frequences */
        float somme =2.81601e+08;

        /* Calcul du score */
        for(int i=0 ; i<(long)this->textSub.length()-3 ; i++) //parcourir la map
        {
            temp = this->textSub.substr(i,4);
            if (this->P.QuadFrancais.count(temp))
            {
                Score = Score + log10(this->P.QuadFrancais[temp]);
            }else{
                Score = Score +log10(0.01/somme);
            }

        }

        Score = Score / this->textSub.length()-3 ;
    this->m_score = Score;

    std::ostringstream strs;
    strs << Score;
    std::string str = strs.str();

    string m_txt_un = txt->toPlainText().toStdString() + "\n\nOn calcule l'indice de compatibilité du texte crypté : \n\nIndice de compatibilité = [" + str + "].\n__________________________________________________________";
    txt->setText(QString::fromStdString(m_txt_un));
    this->m_score = Score;

    step = 3;

    return Score;
}

bool DeroulementCryptanalyseSubstitution::getScore()
{
    std::ostringstream strs;
    strs << this->m_score;
    std::string str = strs.str();
    std::ostringstream strs_u;
    strs_u << this->m_score_max;
    std::string str_u = strs_u.str();

    string m_txt_un = txt->toPlainText().toStdString() + "\n\nLe nouvel indice de compatibilité [" + str + "] est supérieur à l'ancien [" + str_u + "].\n\nOn fixe le nouvel indice avec cette nouvelle valeur\n\nOn crypte le nouveau texte avec la nouvelle clé.\n__________________________________________________________";
    txt->setText(QString::fromStdString(m_txt_un));

    step = 1;

    return true;
}

void DeroulementCryptanalyseSubstitution::process()
{
    if(step == 5)
    {
        string m_txt_un = txt->toPlainText().toStdString() + "\n\n On refait ceci à peu près 2 pow(26) fois pour avoir finalement le texte clair\n__________________________________________________________";
        txt->setText(QString::fromStdString(m_txt_un));
        string m_txt = texte->toPlainText().toStdString() + "\n________________________________________________________\n\nLe texte clair : \n "+"\n" + init_text +"\n________________________________________________________\n\n";
        texte->setText(QString::fromStdString(m_txt));
        step = 6;
    }

}

void DeroulementCryptanalyseSubstitution::end()
{
    QWidget::hide();
    resultwindow* n = new resultwindow(QString::fromStdString(this->init_text),2);

    n->show();
}
void DeroulementCryptanalyseSubstitution::process_steps()
{
    switch(step)
    {
    case 0: generate_m_key();
        break;
    case 1: new_text();
        break;
    case 2: getCompatibility();
        break;
    case 3: permute_key();
        break;
    case 4: getScore();
        break;
    case 5: process();
        break;
    case 6: end();
        break;
    }
}
//LPXPFSYMPLHSYFSLPAYHGHXPTOIWHIUISHAYHYWXPSCFHSSHQIFPBWFXISHFTBOWYPSCHBPWYFCILFWHCHXGHWSFWHXPSSHXLHXAWPSPFXOAAWHSYISBHYFYCPGHPILHIWPTOIWHIUCHBHYFYCPGHPIBHIYYWHISDOIQIHYGHALHIWXISDFJOIOIISHXOWYFHOIDFHSLHXYWOFXLPAOFXPGBHSGMWPFTHSYGHXZHSXGHXZOYXGHLFTPZFSPYFOSHYGHXTOVHSXGHCNPCISCHLPBHIYYWHISHXOFWHWOTPSYFQIHGPSXISWHXYPIWPSYPMHCMIHXIWLPXHFSHBOIWLHXISXISXFTBLHDPFXHWCNPSZXOIXISBOWCNHBOIWLHXPIYWHXCHXYCHQIFAPFYBHIYYWHLHXICCXGHCHYYHAYHWFHSSHXYGCFGCNPCISAPFYCOTTHFLMHIYHYCOSYWPFWHTHSYLPBLIBPWYGHXAYHXCOTTHBQIHXSOLOILHSOIMHLPSCHXYISHAYHQIFSHXHAYHBPXHSAPTFLLHBPXQIHXYFOSGHTTHSHWLHXHSAPSYXCHJOIWLCHXYLPSOISOIQIFXHSOCCIBHHYCHIUQIFSOSYBPXGHBHYFYPTFHYDFHSFLXOSYLHCNOFUHSYWHXOWYFWHSYWHCLFDPYPFWHXOIYHSFWLPCNPSGHLLHHYCNHRMOIXCHXYCOTTHSYCOTTHSYAYHYOSLPXPFSYMPLHSYFSOAAWHRMOIXGHXALHIWXOIPIYWHCNOXHCHYYHAYHHUFXYHYHLLHPIXXFGPSXMOYWHBPVX
