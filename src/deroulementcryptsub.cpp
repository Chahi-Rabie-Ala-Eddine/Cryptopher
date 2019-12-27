#include "deroulementcryptsub.h"
#include "ui_deroulementcryptsub.h"
#include<QString>
#include<iostream>
#include <string.h>
#include <cstring>
#include <QLabel>
#include "DecryptWindow.h"
#include "EncryptWindow.h"
#include"ResultWindow.h"
DeroulementCryptSub::DeroulementCryptSub(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeroulementCryptSub)
{
    ui->setupUi(this);
    setStyleSheet("QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

}

DeroulementCryptSub::~DeroulementCryptSub()
{
    delete ui;
}

/*
 *
 * Méthode charger de remplir le tableau contenant le texte dans la fenetre du deroulement
 * */
void DeroulementCryptSub::remplirTabText(std::string text)
{
    // Méthode qui remplit le Tableau Text a partir du text entré par l'utilisateur

   ui->progressBar->setValue(0);
    QPixmap pixmap("/home/aladin/Documents/Projet Final Outil automatique de décryptage/img/arrow.png");
    ui->label->setPixmap(pixmap);
    ui->label->hide();

    this->ui->tableWidget->setColumnCount(16);
    this->ui->tableWidget->setRowCount(text.length()/16);
    if(text.length()%16!=0)
    {
        this->ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

    }

    int col=0,ligne=0;
    for(unsigned long i=0;i<text.length();i++)
    {
        if(col==16)
        {
            col=0;
            ligne++;
        }
        ui->tableWidget->setItem(ligne,col,new QTableWidgetItem(QChar(text.at(i))));
        col++;
    }
}


/*
 *
 * Méthode chargée de remplir le tableau Contenant la clé dans la fenetre du deroulement
 *
 * */
void DeroulementCryptSub::remplirKeyEncrypt(std::string key)
{
    ui->tableWidget_2->setRowCount(2);
    ui->tableWidget_2->setColumnCount(26);

    for(int i=0;i<26;i++)
    {
        ui->tableWidget_2->setItem(0,i,new QTableWidgetItem(QChar((char) i+65)));
        ui->tableWidget_2->setItem(1,i,new QTableWidgetItem(QChar(key.at(i))));
    }
}

void DeroulementCryptSub::remplirKeyDecrypt(std::string key)
{
    ui->tableWidget_2->setRowCount(2);
    ui->tableWidget_2->setColumnCount(26);

    for(int i=0;i<26;i++)
    {
        ui->tableWidget_2->setItem(1,i,new QTableWidgetItem(QChar((char) i+65)));
        ui->tableWidget_2->setItem(0,i,new QTableWidgetItem(QChar(key.at(i))));
        ui->tableWidget_2->setVerticalHeaderItem(0,new QTableWidgetItem("Chiffré"));
        ui->tableWidget_2->setVerticalHeaderItem(1,new QTableWidgetItem("Clair"));
    }

}


/*
 *
 * Méthode chargée de selectionner tout les Char correspondant
 * au codeascii passée en parametre
 *
 * */
void DeroulementCryptSub::EtapeSelectChar(int asciiCode)
{
    bool found=0;
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {

            if(this->ui->tableWidget->item(i,j) && this->ui->tableWidget->item(i,j)->backgroundColor()!=Qt::green)
            {
            QString carInText= this->ui->tableWidget->item(i,j)->text();
            int carInTextAscii=carInText.at(0).toLatin1();
             if(asciiCode==carInTextAscii)
            {
                 found=1;
                ui->tableWidget->item(i,j)->setBackgroundColor(Qt::red);
            }
            }
        }
    }
    if(!found)
    {
        CaractereEnCours++;
        EtapeActuelle=0;
        std::string selection="# Aucun ";
        selection+=(char) (asciiCode);
        selection+=" dans le texte #";
        QString qselection=QString::fromStdString(selection);
        ui->listWidget->addItem(qselection);
        if(CaractereEnCours==26) ui->pushButton->setEnabled(false);

    }
}


/*
 *
 * Méthode chargée de changer tout les caracteres correspendants
 * a originalchar en caractere correspondant au ascii de replacementchar
 * */
void DeroulementCryptSub::EtapeChangeChar(int originalChar,int replacementChar )
{
    char repChar=(char) replacementChar;


    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {

            if(this->ui->tableWidget->item(i,j) && this->ui->tableWidget->item(i,j)->backgroundColor()!=Qt::green)
            {
            QString carInText= this->ui->tableWidget->item(i,j)->text();
            int carInTextAscii=carInText.at(0).toLatin1();
             if(originalChar==carInTextAscii)
            {
                ui->tableWidget->setItem(i,j,new QTableWidgetItem(QChar(repChar)));
                ui->tableWidget->item(i,j)->setBackgroundColor(Qt::green);
            }
            }
        }
    }
    bool fin=true;
    for(int r=0;r<ui->tableWidget->rowCount();r++)
    {
        for(int s=0;s<ui->tableWidget->columnCount();s++)
        {
            if(ui->tableWidget->item(r,s) && ui->tableWidget->item(r,s)->backgroundColor()!=Qt::green)
                fin=false;
        }
    }
    if(fin) { ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);}
}


void DeroulementCryptSub::on_pushButton_clicked()
{
    ui->label->show();
    //char carAct=CaractereEnCours+65;
    char carKey=this->ui->tableWidget_2->item(1,CaractereEnCours)->text().at(0).toLatin1();
    EtapeActuelle++;
    switch (EtapeActuelle) {
    case 1 :
    {

        std::string step="________________________________________________________\nLettre  ";
        step+=ui->tableWidget_2->item(0,CaractereEnCours)->text().toStdString();
        QString qsteps=QString::fromStdString(step);
        ui->listWidget->addItem(qsteps);
        ui->label->setGeometry(ui->label->geometry().x()+30,ui->label->geometry().y(),ui->label->geometry().width(),ui->label->geometry().height());
        ui->tableWidget_2->selectColumn(CaractereEnCours);
        ui->listWidget->scrollToBottom();
        break;
    }
    case 2 :
    {
        std::string selection="- On Commence par sélectionner tout les ";
        selection+=ui->tableWidget_2->item(0,CaractereEnCours)->text().toStdString();
        selection+=" du texte";
        QString qselection=QString::fromStdString(selection);
        ui->listWidget->addItem(qselection);
        QString originalCar= this->ui->tableWidget_2->item(0,CaractereEnCours)->text();
        int originalCarAscii=originalCar.at(0).toLatin1();
        EtapeSelectChar(originalCarAscii);
        ui->listWidget->scrollToBottom();
         break;
    };
    case 3 :
    {
        std::string selection="- Ensuite on remplace tout les ";
        selection+=ui->tableWidget_2->item(0,CaractereEnCours)->text().toStdString();
        selection+=" du texte par la lettre ";
        selection+=carKey;
        QString qselection=QString::fromStdString(selection);
        ui->listWidget->addItem(qselection);
        QString originalCar= this->ui->tableWidget_2->item(0,CaractereEnCours)->text();
        int originalCarAscii=originalCar.at(0).toLatin1();
        QString carInKey= this->ui->tableWidget_2->item(1,CaractereEnCours)->text();
        int carInKeyAScii=carInKey.at(0).toLatin1();
        EtapeChangeChar(originalCarAscii,carInKeyAScii);
        CaractereEnCours++;
        EtapeActuelle=0;
        ui->listWidget->scrollToBottom();
        if(CaractereEnCours==26) ui->pushButton->setEnabled(false);
         break;
    };

    }

    changeProgress();

}

void DeroulementCryptSub::on_pushButton_2_clicked()
{
    std::string final="";
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            if(ui->tableWidget->item(i,j))
            final+=ui->tableWidget->item(i,j)->text().toStdString();
        }
    }
    this->hide();
    resultwindow* my_window = new resultwindow(QString::fromStdString(final),this->type);
    my_window-> show();
}

void DeroulementCryptSub::changeProgress()
{
    int total=0,fait=0;
    for(int l=0;l<ui->tableWidget->rowCount();l++)
    {
       for(int k=0;k<ui->tableWidget->columnCount();k++)
        {
            if(ui->tableWidget->item(l,k))
            {
                total++;
                if(ui->tableWidget->item(l,k)->backgroundColor()!="Qt::green")
                    fait++;
            }
        }
    }
    ui->progressBar->setRange(0,total);
    ui->progressBar->setValue(fait);



}

/*
 *
 * Setter qui met a jour le type du traitement 1 pour cryptage 2 pour decryptage
 * */
void DeroulementCryptSub::setType(int a)
{
    type=a;
}

void DeroulementCryptSub::on_pushButton_3_clicked()
{
    if(type==1)
    {
        QWidget::hide();
        encryptwindow* my_window = new encryptwindow;
        my_window->setCurrent();
        my_window-> show();
    }
    else
    if(type==2)
    {
        QWidget::hide();
        decryptwindow* my_window = new decryptwindow;
        my_window->setCurrent();
        my_window-> show();
    }
}
