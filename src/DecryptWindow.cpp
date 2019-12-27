#include <QCheckBox>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QGridLayout>
#include <MainWindow.h>
#include <ResultWindow.h>
#include <Text.h>
#include <Key.h>
#include <KeyS.h>
#include <KeyV.h>
#include <TextS.h>
#include <TextS.h>
#include <TextV.h>
#include <DecryptWindow.h>
#include <QFileDialog>
#include"deroulementcryptsub.h"
#include"deroulementvigenere.h"
#include<QDebug>
#include<cryptanalysevig.h>
#include <unistd.h>
#include <QMovie>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include "deroulementcryptanalysesubstitution.h"

#define DEBUT 330
#define FIN 360

using namespace std;
string decryptwindow::currentText;

//constructeur de la classe
decryptwindow::decryptwindow() : QWidget()
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
    linedit = new QLineEdit("IMTHEDEFAULTKEY",this);
    linedit->setAlignment(Qt::AlignCenter);
    linedit->setGeometry(320, 380 , 250, 30);

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

    bouton1 = new QPushButton("Decrypt",this);
    bouton1->setGeometry(400, 455 , 100, 30);

    QPushButton* btn_cinq = new QPushButton(QString::fromWCharArray(L"\u25C2").toStdString().c_str(),this);
    btn_cinq->setGeometry(20, 20, 20, 24);
    QObject::connect(btn_cinq, SIGNAL(clicked()), this, SLOT(back_home()));

    if(!checkbox->isChecked())
        QObject::connect(bouton1, SIGNAL(clicked()), this, SLOT(result_decrypt()));
    QObject::connect(btn_g, SIGNAL(clicked()), this, SLOT(load()));

    check = new QCheckBox(this);
    check->setGeometry(580, 380 , 250, 30);
    check->setChecked(true);
    connect(check, SIGNAL(stateChanged(int)), this, SLOT(verify_check()));

    movie = new QMovie("/home/aladin/Documents/decrypt-tool/img/load.gif");
    processLabel = new QLabel(this);
    processLabel->setMovie(movie);
    movie->start();
    processLabel->move(510,450);
    processLabel->hide();

    QPixmap monImage("/home/aladin/Documents/decrypt-tool/img/load_hide.png");
    m_label = new QLabel(this);
    m_label->setPixmap(monImage);
    m_label->move(510,448);
    m_label->close();
    this->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(linedit, SIGNAL(textChanged(QString)),this ,SLOT(treat(QString)));

}
void decryptwindow::verify_check()
{
    if(check->isChecked() == 0)
    {
        for(int i = 0; i < 26 ; i++)
        {
            lines[i]->setStyleSheet("background: lightgrey");
            linedit->setStyleSheet("background : lightgrey");
            linedit->setMaxLength(0);
            lines[i]->setMaxLength(0);
        }
    }
    else
    {
        string temp;
        for(int i = 0; i < 26 ; i++)
        {
            linedit->setMaxLength(20);
            linedit->setStyleSheet("background: white");
            linedit->setText(QString::fromStdString("IMTHEDEFAULTKEY"));
            lines[i]->setMaxLength(1);
            temp += (char)(65+i);
            lines[i]->setText(QString::fromStdString(temp));
            lines[i]->setStyleSheet("background: white");
            temp = "";
        }

    }
}
//méthode qui crée la clée de substitution
void decryptwindow::create_key(int i, QString nouveau)
{
    string temp = nouveau.toStdString();
    string tmp;

    if(!isupper(temp[0]))
    {
        temp[0] = (char) ((int)temp[0] - 32);
        lines[i]->setText(QString::fromStdString(temp));
    }
    if(!isalpha(temp[0]))
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
//méthode qui détermine le type de la clée
void decryptwindow::key_type()
{
    if(liste->currentIndex() == 1)
    {
        linedit->hide();
        check->setGeometry(680, 380 , 250, 30);
        for(int i = 0 ; i < 26 ; i++)
        {
            lines[i]->show();
        }
    }

    else if(liste->currentIndex() == 0)
    {
        linedit->show();
        check->setGeometry(580, 380 , 250, 30);

        for(int i = 0 ; i < 26 ; i++)
        {
            lines[i]->hide();
        }
    }
}
void decryptwindow::treat(QString input)
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
//slot qui charge le fichier
void decryptwindow::load()
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
//slot qui garde en mémoire le texte entére
void decryptwindow::setCurrent()
{
    texedit->setText(QString::fromStdString(currentText));
}

void decryptwindow::process_decrypt()
{

}

//Aller a la page résultat
void decryptwindow::result_decrypt()
{
    //m_label->setVisible(!m_label->isVisible());
    currentText=texedit->toPlainText().toStdString();

    if(checkbox->isChecked())
    {
        if(liste->currentIndex() == 1)
        {
            if(check->isChecked())
            {
                QWidget::close();
                TextS* m_text = new TextS(texedit->toPlainText().toStdString());
                DeroulementCryptSub* w=new DeroulementCryptSub();
                w->remplirTabText(m_text->getTextModified());
                string key;
                for(int i = 0; i < 26; i++)
                {
                    key+= lines[i]->text().toStdString();
                }
                w->remplirKeyDecrypt(key);
                w->show();
            }
            else
            {
                m_label->hide();

                DeroulementCryptanalyseSubstitution* d = new DeroulementCryptanalyseSubstitution(texedit->toPlainText().toStdString());
                d->show();
                QWidget::close();
            }
        }
        else
        {
            if(linedit->text().toStdString() == "")
            {
                QWidget::close();
                TextV* m_text = new TextV(texedit->toPlainText().toStdString());
                CryptanalyseVig* w = new CryptanalyseVig();
                m_text->decrypt();
                w->remplirText(*m_text);
                w->show();
            }
            else {
                QWidget::close();
                TextV *m_text = new TextV(texedit->toPlainText().toStdString());
                DeroulementVigenere* w=new DeroulementVigenere();
                w->remplirText(m_text->getTextModified(),linedit->text().toStdString(),2);
                w->show();
            }

        }
    }

    else
    {
        string key;
        QString final_text;

        //Déchiffrement Substitution
        if(liste->currentIndex() == 1)
        {
            TextS* m_text = new TextS(texedit->toPlainText().toStdString());

            KeyS* m_key = new KeyS();
            for(int i = 0; i < 26; i++)
            {
                key+= lines[i]->text().toStdString();
            }

            if(key == "")
            {

                QFuture<void> future = QtConcurrent::run(m_text, &TextS::decrypt);
                future.waitForFinished();
            }

            else
            {
                m_key->setContent(key);
                QFuture<void> future = QtConcurrent::run(m_text, &TextS::decrypt,*m_key);
                future.waitForFinished();
            }

            final_text = QString::fromStdString(m_text->getTextOutput());
        }
        //Déchiffrement Vigenere
        else if(liste->currentIndex() == 0)
        {
            TextV* m_text = new TextV(texedit->toPlainText().toStdString());
            key = linedit->text().toStdString();
            KeyV* v_key = new KeyV();

            if(key == "")
            {
               QFuture<void> future = QtConcurrent::run(m_text, &TextV::decrypt);
               future.waitForFinished();
            }

            else
            {
                //Déchiffrement
                v_key->setContent(key);
                QFuture<void> future = QtConcurrent::run(m_text, &TextV::decrypt,*v_key);
                future.waitForFinished();
            }

            final_text = QString::fromStdString(m_text->getTextOutput());
        }

        resultwindow* my_window = new resultwindow(final_text,2);

        this->setAttribute(Qt::WA_DeleteOnClose);
        QWidget::close();
        my_window->setType(2);
        my_window-> show();
    }
    this->setAttribute(Qt::WA_DeleteOnClose);
   // decryptwindow::~decryptwindow();
}

void decryptwindow::back_home()
{
    this->setAttribute(Qt::WA_DeleteOnClose);
   //decryptwindow::~decryptwindow();
    QWidget::close();
    mainwindow* my_window = new mainwindow();
    my_window->show();
}

decryptwindow::~decryptwindow()
{
    delete this->texedit;
}
