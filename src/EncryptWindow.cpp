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
#include <QFuture>
#include <QtConcurrent/QtConcurrent>

#define DEBUT 330
#define FIN 360

using namespace std;
string encryptwindow::currentText;

//constructeur de la classe
encryptwindow::encryptwindow() : QWidget()
{
    setStyleSheet("QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

    setFixedSize(900, 500);
    //this->showMaximized();
    //texte de la fenetre
    this->setWindowTitle("Outil automatique d'aide au décryptage");

    // 1 : Créer le QTabWidget
    onglets = new QTabWidget(this);
    onglets->setGeometry(10, 10, 880, 480);

    // 2 : Créer les pages, en utilisant un widget parent pour contenir chacune des pages


    texedit = new QTextEdit("PUT YOUR TEXT HERE",this);
    texedit->setGeometry(160, 60 , 600, 250);

    liste = new QComboBox(this);
    liste->setMaximumSize(QSize(200, 30));
    liste->addItem("Vigenere");
    liste->addItem("Substitution");
    liste->setGeometry(560, 320 , 390, 30);

    btn_g = new QPushButton("load",this);
    btn_g->setGeometry(160, 320 , 130, 30);

    //clé vigenere
    linedit = new QLineEdit("DEFAULTKEY",this);
    linedit->setAlignment(Qt::AlignCenter);
    linedit->setGeometry(320, 380 , 250, 30);
    linedit->setMaxLength(10);



    //clé substitution

    for(int i = 0; i < 26 ; i++)
    {
        alphabet[i] = 65 + i;
    }

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
            lines[i]->setGeometry(DEBUT+((i-13)*30)-50, FIN+35 , 30, 30);
        lines[i]->setMaxLength(1);
        lines[i]->setAlignment(Qt::AlignCenter);
        lines[i]->hide();
        connect(lines[i], &QLineEdit::textChanged, [this,i](){ create_key(i,lines[i]->text()); });
    }

    connect(liste,SIGNAL(currentIndexChanged(int)),this,SLOT(key_type()));

    checkbox = new QCheckBox("See the decryption process", this);
    checkbox->setGeometry(360, 425 , 250, 30);

    bouton1 = new QPushButton("Encrypt",this);
    bouton1->setGeometry(400, 455 , 100, 30);
/*
    QVBoxLayout *vbox1 = new QVBoxLayout;
    vbox1->addWidget(texedit);
    vbox1->addWidget(btn_g);
    vbox1->addWidget(checkbox);
    vbox1->addWidget(linedit);
    for(int i = 0; i < 26; i++)
        vbox1->addWidget(lines[i]);
    vbox1->addWidget(bouton1);
    page1->setLayout(vbox1);
*/

    QPushButton* btn_cinq = new QPushButton(QString::fromWCharArray(L"\u25C2").toStdString().c_str(),this);
    btn_cinq->setGeometry(20, 20, 20, 24);

    btn_six = new QPushButton("🔑",this);
    btn_six->setGeometry(295,383,20,24);

    QObject::connect(bouton1, SIGNAL(clicked()), this, SLOT(result_encrypt()));
    QObject::connect(btn_g, SIGNAL(clicked()), this, SLOT(load()));
    QObject::connect(btn_cinq, SIGNAL(clicked()), this, SLOT(back_home()));
    QObject::connect(btn_six, SIGNAL(clicked()), this, SLOT(key_creator()));
    QObject::connect(linedit, SIGNAL(textChanged(QString)),this ,SLOT(treat(QString)));
    this->setAttribute(Qt::WA_DeleteOnClose);
}
//slot qui charge le fichier
void encryptwindow::load()
{
    QString filename =  QFileDialog::getOpenFileName(
              this,
              "Open Document",
              QDir::currentPath(),
              "Document files (*.doc *.txt *.pdf)");

        if( !filename.isNull() )
        {
            texedit->setText(QString::fromStdString(importPdf(filename.toStdString().c_str())));
            //system("rm ");
        }
}
//méthode qui détermine le type de la clée
void encryptwindow::create_key(int i, QString nouveau)
{
    string temp = nouveau.toStdString();
    string tmp;

    if(!isupper(temp[0]))
    {
        temp[0] = (char) ((int)temp[0] - 32);
        lines[i]->setText(QString::fromStdString(temp));
    }
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
    temp = nouveau.toStdString();
    alphabet[i] = temp[0];

    int ascii = 0;
    for(int k = 0; k < 26 ; k++)
    {
        if(lines[k]->text().toStdString()[0] == '\0')
        {
            for(int i = 0; i < 26 ; i++)
            {
                tmp = lines[i]->text().toStdString();
                ascii+= (int)tmp[0];
            }
            tmp[0] = (char) (2015-ascii);
            lines[k]->setText(QString::fromStdString(tmp));
        }
    }
}
//méthode qui crée la clée de substitution
void encryptwindow::key_type()
{
    if(liste->currentIndex() == 1)
    {
        linedit->hide();
        btn_six->setGeometry(250,383,20,24);

        for(int i = 0 ; i < 26 ; i++)
        {
            lines[i]->show();
        }
    }

    else if(liste->currentIndex() == 0)
    {
        linedit->show();
        btn_six->setGeometry(295,383,20,24);

        for(int i = 0 ; i < 26 ; i++)
        {
            lines[i]->hide();
        }
    }
}

void encryptwindow::treat(QString input)
{
    if(linedit->text().toStdString().empty())
    {
        cout << "i'm empty 1 " << endl;
        linedit->setText(QString::fromStdString("default"));
    }
    else
    {
        char* temp = (char*) malloc(10 * sizeof(char*));
        int pos = (int)input.toStdString().length() - 1;

        string in_put = linedit->text().toStdString();

        for (int i = 0; i < pos ; i++)
        {
            temp[i] = in_put[i];
        }
        std::string Str = std::string(temp);
        if(!isalpha(in_put[pos]))
        {
            linedit->setText(QString::fromStdString(Str));
        }

        else
        {
            for(int i = 0 ; i < pos+1 ; i++)
            {
                in_put[i] = std::toupper(in_put[i],std::locale());
            }
            linedit->setText(QString::fromStdString(in_put));
        }

        if(linedit->text().toStdString().empty())
        {
            cout << "i'm empty 2" << endl;
            linedit->setText(QString::fromStdString("default"));
        }

        free(temp);
    }
}

void encryptwindow::process_encrypt()
{

}
//méthode qui garde le texte input en mémoire
void encryptwindow::setCurrent()
{
    this->texedit->setText(QString::fromStdString(currentText));
}
//slot qui envoi à la page résultat
void encryptwindow::result_encrypt()
{

    currentText=texedit->toPlainText().toStdString();
    QWidget::close();

    if(checkbox->isChecked())
    {
        if(liste->currentIndex() == 1)
        {
        TextS* m_text = new TextS(texedit->toPlainText().toStdString());
        DeroulementCryptSub* w=new DeroulementCryptSub();
        w->remplirTabText(m_text->getTextModified());
        string key;
        for(int i = 0; i < 26; i++)
        {
            key+= lines[i]->text().toStdString();
        }
        w->remplirKeyEncrypt(key);
        w->show();
        }
        else {
            TextV* m_text = new TextV(texedit->toPlainText().toStdString());
            DeroulementVigenere* w=new DeroulementVigenere();
            w->remplirText(m_text->getTextModified(),linedit->text().toStdString(),1);
            w->show();
        }
    }

    //On ne coche pas sur la checkbox
    else
    {
        string key;
        QString final_text;
        KeyS* m_key = new KeyS();

        //Chiffrement Substitution
        if(liste->currentIndex() == 1)
        {
            TextS* m_text = new TextS(texedit->toPlainText().toStdString());

            for(int i = 0; i < 26; i++)
            {
                key+= lines[i]->text().toStdString();
            }

            m_key->setContent(key);
            QFuture<void> future = QtConcurrent::run(m_text, &TextS::encrypt,*m_key);
            future.waitForFinished();

            final_text = QString::fromStdString(m_text->getTextOutput());
        }
        //Chiffrement Vigenere
        else if(liste->currentIndex() == 0)
        {
            key = linedit->text().toStdString();
            TextV* m_text = new TextV(texedit->toPlainText().toStdString());

            if(key == "IAMTHEDEFAULTKEY" || key.length() == 0)
            {
                m_key->setContent("IAMTHEDEFAULTKEY");
            }

            else
            {
                m_key->setContent(key);
            }

            QFuture<void> future = QtConcurrent::run(m_text, &TextV::encrypt,*m_key);
            future.waitForFinished();

            final_text = QString::fromStdString(m_text->getTextOutput());
        }

        resultwindow* my_window = new resultwindow(final_text,1);
        QWidget::close();
        my_window->setType(1);
        my_window-> show();
    }
    //encryptwindow::~encryptwindow();
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void encryptwindow::back_home()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    QWidget::close();
    mainwindow* my_window = new mainwindow();
    my_window->show();
}

void encryptwindow::key_creator()
{
    if(liste->currentIndex() == 0)
    {
        string m_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        size_t taille = rand() % 5 + 4;
        string m_key = "";
        for(size_t i=0; i < taille; i++)
        {
            size_t pos = rand() % 26;
            m_key+= m_chars[pos];
        }
        linedit->setText(QString::fromStdString(m_key));
    }
    else
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
            string tmp;
            tmp[0] = m_key[0];
            lines[i]->setText(QString::fromStdString("T"));
            lines[25]->setText(lines[rand() % 25]->text());
            cout << m_key[i];
        }

        free(resultat);

    }
}

encryptwindow::~encryptwindow()
{
    delete this->texedit;
}
