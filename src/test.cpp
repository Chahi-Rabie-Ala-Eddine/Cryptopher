#include "test.h"
#include "ResultWindow.h"
#include "EncryptWindow.h"
#include "QCheckBox"
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include <DecryptWindow.h>
#include <MainWindow.h>
#include <QString>
#include <iostream>
#include <QLineEdit>
#include <QSignalMapper>
#include <chrono>
#include <thread>
#include <limits>
#include <QScrollBar>

#define DEBUT 180
#define FIN 20

using namespace std;

test::test() : QWidget()
{
    for(int i = 0; i < 26 ; i++)
    {
        alphabet[i] = 65 + i;
    }
    setFixedSize(760, 520);
    //texte de la fenetre
    this->setWindowTitle("Outil automatique d'aide au décryptage");
    setStyleSheet("QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

    char* letter = (char*) malloc(1 * sizeof(char*));
    letter[0] = 64;
    letter[1] = '\0';
    for(int i = 0; i < 26 ; i++)
    {
        letter[0]+= 1;
        lines[i] = new QLineEdit(letter,this);
        if(i<13)
            lines[i]->setGeometry(DEBUT+(i*30)-50, FIN, 30, 30);
        else
            lines[i]->setGeometry(DEBUT+((i-13)*30)-50, FIN*2.5 , 30, 30);
        lines[i]->setMaxLength(1);
        lines[i]->setAlignment(Qt::AlignCenter);
        connect(lines[i], &QLineEdit::textChanged, [this,i](){ f(i,lines[i]->text()); });
    }
    edit = new QTextEdit(this); // assuming this is your text edit
    QScrollBar *sb = edit->verticalScrollBar();
    sb->setValue(sb->maximum());
    edit->setGeometry(DEBUT-50, FIN+80, 500, 340);
    edit->setText(QString::fromStdString("DESHZJETSARCOEHLHGIQUELECHDEDOAMMURAZILAVENUSDEMILHLAJHCHNDEDELEHNARDDEVINCIHUENCHRELALIZERTEGUIDANTLEPEUPLEDEUGENEDELACRHIXSHNTLESHEUVRESLESPLUSAPPRECIEESDUMUSEELELHUVREESTLEMUSEELEPLUSVISITEAUMHNDEAVECSESMILLIHNSDEVISITEURSENCESTLEMUSEELEPLUSPHPULAIRESURINSTAGRAMENETLESIXIEMELIEULEPLUSGEHLHCALISEAUMHNDELEPUZLICESTJEUNEENEFFETDESVISITEURSHNTMHINSDEANSNHTHNSENFINQUEDESVISITEURSDUMUSEEDULHUVRESHNTETRANGERSLESAMERICAINSLESCOINHISLESITALIENSLESANGLAISETLESZRESILIENSSHNTLESPLUSNHMZREUX"));

    QPushButton* btn_un = new QPushButton(QString::fromWCharArray(L"\u27f3").toStdString().c_str(),this);
    btn_un->setGeometry(DEBUT+375, FIN*1.7, 35, 35);
    QObject::connect(btn_un, SIGNAL(clicked()), this, SLOT(reset()));

    QPushButton* btn_deux = new QPushButton("Save",this);
    btn_deux->setGeometry(DEBUT*2-5, FIN+445, 70, 40);

    /*QPushButton* btn_trois = new QPushButton("Menu",this);
    btn_trois->setGeometry(DEBUT*3+23, FIN+445, 70, 40);*/

    free(letter);
}

void test::modify_text(string texte, char ancien, char nouveau)
{
    for(int i = 0; i < (int)texte.length(); i++)
    {
        if(texte[i] == ancien)
            texte[i] = nouveau;

        else if(texte[i] == nouveau)
            texte[i] = ancien;
    }
    edit->setText(QString::fromStdString(texte));
}

void test::f(int i, QString nouveau)
{
    string temp = nouveau.toStdString();

    /*if(temp[0] == '\0')
        return;
        l'idée est que quand on efface le caractère le texte ne change pas et qu'on avance ensuite si l'utilisateur
        ne met aucun caractère a la place a la prochaine étape si on efface une autre lettre et qu'on l'échange
        l'ancien caractère effacé reviens
    */
    if(!isalpha(temp[0]) || !isupper(temp[0]))
    {
        temp[0] = alphabet[i];
        lines[i]->setText(QString::fromStdString(temp));

        return;
    }

    for(int j = 0; j < 26; j++)
    {
        if(j == i)
            continue;
        temp = lines[j]->text().toStdString();
        if(temp == nouveau.toStdString())
        {
            temp[0] = alphabet[i];
            lines[j]->setText(QString::fromStdString(temp));
            temp = nouveau.toStdString();
            break;
        }
    }
    cout << "\n" << i << endl;
    cout << "=> Ancien Carctère : " << (char)alphabet[i] << endl;
    cout << "=> Nouveau Caractère : " << qPrintable(nouveau) << endl;
    test::modify_text(edit->toPlainText().toStdString(),alphabet[i],temp[0]);
    temp = nouveau.toStdString();
    alphabet[i] = temp[0];
}

void test::reset()
{
    char* letter = (char*) malloc(1 * sizeof(char*));
    letter[0] = 64;
    letter[1] = '\0';
    for(int i = 0; i < 26 ; i++)
    {
        letter[0]+= 1;
        QString qstr(letter);
        lines[i]->setText(qstr);
    }
    edit->setText(QString::fromStdString("DESHZJETSARCOEHLHGIQUELECHDEDOAMMURAZILAVENUSDEMILHLAJHCHNDEDELEHNARDDEVINCIHUENCHRELALIZERTEGUIDANTLEPEUPLEDEUGENEDELACRHIXSHNTLESHEUVRESLESPLUSAPPRECIEESDUMUSEELELHUVREESTLEMUSEELEPLUSVISITEAUMHNDEAVECSESMILLIHNSDEVISITEURSENCESTLEMUSEELEPLUSPHPULAIRESURINSTAGRAMENETLESIXIEMELIEULEPLUSGEHLHCALISEAUMHNDELEPUZLICESTJEUNEENEFFETDESVISITEURSHNTMHINSDEANSNHTHNSENFINQUEDESVISITEURSDUMUSEEDULHUVRESHNTETRANGERSLESAMERICAINSLESCOINHISLESITALIENSLESANGLAISETLESZRESILIENSSHNTLESPLUSNHMZREUX"));
}

/*
test::test() : QWidget()
{
    for(int i = 0; i < 26 ; i++)
    {
        alphabet[i] = 65 + i;
    }
    setFixedSize(760, 520);
    //texte de la fenetre
    this->setWindowTitle("Outil automatique d'aide au décryptage");
    char* letter = (char*) malloc(1 * sizeof(char*));
    letter[0] = 64;
    letter[1] = '\0';
    for(int i = 0; i < 26 ; i++)
    {
        letter[0]+= 1;
        cout << letter[0];
        lines[i] = new QLineEdit(letter,this);
        if(i<13)
            lines[i]->setGeometry(DEBUT+(i*30), FIN, 30, 30);
        else
            lines[i]->setGeometry(DEBUT+((i-13)*30), FIN*2.5 , 30, 30);
        lines[i]->setMaxLength(1);
        lines[i]->setAlignment(Qt::AlignCenter);
        connect(lines[i], &QLineEdit::textChanged, [this,i](){ f(i,lines[i]->text()); });
    }
    free(letter);
}

void test::f(int i, QString nouveau)
{
    if(nouveau.toStdString() == "")
        return;

    cout << "\n" << i << endl;
    cout << "=> Ancien Carctère : " << (char)alphabet[i] << endl;
    cout << "=> Nouveau Caractère : " << qPrintable(nouveau) << endl;

    string temp = nouveau.toStdString();
    alphabet[i] = temp[0];
}*/
