#include "deroulementvigenere.h"
#include "ui_deroulementvigenere.h"
#include"ResultWindow.h"
#include "EncryptWindow.h"
#include "DecryptWindow.h"

DeroulementVigenere::DeroulementVigenere(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeroulementVigenere)
{
    ui->setupUi(this);
    setStyleSheet("QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

}


/*
 *
 * Méthode charger de remplir le tableau contenant la clé dans la fenetre du deroulement
 * */
void DeroulementVigenere::remplirCle(std::string key)
{
    QColor couleur1(249,94,4);
    QColor couleur2(220,220,95);
    ui->listWidget->addItem("________________________________________________________\n- On commence par répeter la clé  tous le long  \n du texte.");
    ui->listWidget->scrollToBottom();
    int col=0,actuallInKey=0,i=0;
    while(i<ui->tableWidget->rowCount())
    {
        if(col==14)
        {
            col=0;
            i+=3;
        }

       if(ui->tableWidget->item(i,col)) { ui->tableWidget->setItem(i+1,col,new QTableWidgetItem(QChar(key.at(actuallInKey))));
        ui->tableWidget->item(i+1,col)->setBackgroundColor(couleur1);}
        actuallInKey++;
        if(actuallInKey==(int)key.length()) { actuallInKey=0;
        QColor temp(couleur1.red(),couleur1.green(),couleur1.blue());
        couleur1.setRgb(couleur2.red(),couleur2.green(),couleur2.blue());
        couleur2.setRgb(temp.red(),temp.green(),temp.blue());
        }
        col++;
    }
    etape++;
}


/*
 *
 * Méthode chargé de faire les calculs Message[i]+Cle[i] mod 26
 * afin de trouver le crypté d'un caractére
 * */
void DeroulementVigenere::calculAdditionMod()
{
    std::string str1="________________________________________________________\n- Calcul des caractéres cryptés en utilisant la lettre ";
    str1+= keyCipher.at(caractereEnCours);
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);
    ui->listWidget->scrollToBottom();

    int tab[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int ligne=0;ligne<ui->tableWidget->rowCount();ligne+=3)
    {
        for(int col=0;col<ui->tableWidget->columnCount();col++)
        {
            if(this->ui->tableWidget->item(ligne,col) && this->ui->tableWidget->item(ligne+1,col)->text().at(0).toLatin1()==(int) keyCipher.at(caractereEnCours))
            {

                QString originalCar= this->ui->tableWidget->item(ligne,col)->text();
                int originalCarAscii=originalCar.at(0).toLatin1();
                if(tab[originalCarAscii-65]==0) { tab[originalCarAscii-65]=1;
                std::string ecr="(";
                ecr+=(char) originalCarAscii;

                if(typeOfTreatement==1)
                    ecr+=" + "; else
                    ecr+=" - ";

                int keyCarAscii=(int) keyCipher.at(caractereEnCours);
                ecr+=(char) keyCarAscii;
                ecr+=") Modulo 26 = ";
                char result;
                if(typeOfTreatement==1)
                result= (char) ((((keyCarAscii-65)+(originalCarAscii-65))%26) +65);
                else  result= (char) ((((originalCarAscii-65)-(keyCarAscii-65)+26)%26) +65);
                ecr+=result;
                ecr+="       [";
                ecr+=originalCarAscii;
                ecr+="------->";
                ecr+= result;
                ecr+="\]";
                QString qsteps=QString::fromStdString(ecr);
                ui->listWidget->addItem(qsteps);
                ui->listWidget->scrollToBottom();}

            }
        }
    }
    etape++;
     str1="________________________________________________________\n";

     qsteps=QString::fromStdString(str1);
     ui->listWidget->addItem(qsteps);
}

void DeroulementVigenere::replaceClearWithCipher()
{
    int tab[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    for(int ligne=0;ligne<ui->tableWidget->rowCount();ligne+=3)
    {
        for(int col=0;col<ui->tableWidget->columnCount();col++)
        {
            if(this->ui->tableWidget->item(ligne,col) && this->ui->tableWidget->item(ligne+1,col)->text().at(0).toLatin1()==(int) keyCipher.at(caractereEnCours))
            {


                 QString originalCar= this->ui->tableWidget->item(ligne,col)->text();
                int originalCarAscii=originalCar.at(0).toLatin1();
                int keyCarAscii=(int) keyCipher.at(caractereEnCours);
                char result;
                if(typeOfTreatement==1)
                result= (char) ((((keyCarAscii-65)+(originalCarAscii-65))%26) +65);
                else  result= (char) ((((originalCarAscii-65)-(keyCarAscii-65)+26)%26) +65);
                 if(tab[originalCarAscii-65]==0) { tab[originalCarAscii-65]=1;
                std::string ecr="- On Remplace tout les ";
                ecr+=(char) originalCarAscii;

                ecr+=" cryptés en utilissant le  ";
                ecr+=(char) keyCarAscii;
                ecr+="\n par des ";
                ecr+=result;
                ecr+=".";
                QString qsteps=QString::fromStdString(ecr);
                ui->listWidget->addItem(qsteps);
                 ui->listWidget->scrollToBottom();
                 }
                ui->tableWidget->setItem(ligne+2,col,new QTableWidgetItem(QChar( result)));
                ui->tableWidget->item(ligne+2,col)->setBackgroundColor(Qt::green);

            }

            }
        }

    etape=1;
    caractereEnCours++;
    if(caractereEnCours==(int)keyCipher.length()) { ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    }
}


/*
 *
 * Méthode chargé de remplir le tableau de texte contenu dans la fenetre
 * */
void DeroulementVigenere::remplirText(std::string text,std::string key,int treatement)
{

    ui->tableWidget->verticalHeader()->setVisible(true);
    ui->progressBar->setValue(0);
    this->typeOfTreatement=treatement;
    this->keyCipher=key;
    //int cle=0;
    this->ui->tableWidget->setColumnCount(14);
    this->ui->tableWidget->setRowCount(text.length()/14*3);
    if(text.length()%14!=0)
    {
        this->ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+3);

    }

    int col=0,ligne=0;//actuallInKey=0;
    for(unsigned long i=0;i<text.length();i++)
    {
        if(col==14)
        {
            col=0;
            ligne+=3;
        }

        if(typeOfTreatement==1)
        {
        ui->tableWidget->setVerticalHeaderItem(ligne,new QTableWidgetItem("Clair"));
        ui->tableWidget->setVerticalHeaderItem(ligne+1,new QTableWidgetItem("Clé"));
        ui->tableWidget->setVerticalHeaderItem(ligne+2,new QTableWidgetItem("Chiffré"));
        }
        else {
            ui->tableWidget->setVerticalHeaderItem(ligne,new QTableWidgetItem("Chiffré"));
            ui->tableWidget->setVerticalHeaderItem(ligne+1,new QTableWidgetItem("Clé"));
            ui->tableWidget->setVerticalHeaderItem(ligne+2,new QTableWidgetItem("Clair"));
        }

        ui->tableWidget->setItem(ligne,col,new QTableWidgetItem(QChar(text.at(i))));
        ui->tableWidget->item(ligne,col)->setBackgroundColor(QColor(45,214,233));
        col++;
    }

}



DeroulementVigenere::~DeroulementVigenere()
{
    delete ui;
}

void DeroulementVigenere::on_pushButton_clicked()
{
    switch (etape) {
    case 0: this->remplirCle(keyCipher); break;
    case 1: this->calculAdditionMod(); break;
    case 2: this->replaceClearWithCipher(); break;
    };
    changeProgress();
}


void DeroulementVigenere::on_pushButton_2_clicked()
{
    std::string final="";
    for(int i=2;i<ui->tableWidget->rowCount();i+=3)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            if(ui->tableWidget->item(i,j))
            final+=ui->tableWidget->item(i,j)->text().toStdString();
        }
    }
    this->hide();
    resultwindow* my_window = new resultwindow(QString::fromStdString(final),this->typeOfTreatement);
    my_window-> show();
}


/*
 *
 * Méthode chargé de gerer la barre de progression et son avancement
 * */
void DeroulementVigenere::changeProgress()
{
    int total=0,fait=0;
    for(int l=2;l<ui->tableWidget->rowCount();l+=3)
    {
       for(int k=0;k<ui->tableWidget->columnCount();k++)
        {
            if(ui->tableWidget->item(l-1,k))
            {
                total++;
                if(ui->tableWidget->item(l,k) && ui->tableWidget->item(l,k)->backgroundColor()!="Qt::green")
                    fait++;
            }
        }
    }
    ui->progressBar->setRange(0,total);
    ui->progressBar->setValue(fait);
}



void DeroulementVigenere::on_pushButton_3_clicked()
{
    if(typeOfTreatement==1)
    {
        QWidget::hide();
        encryptwindow* my_window = new encryptwindow;
        my_window->setCurrent();
        my_window-> show();
    }
    else
    if(typeOfTreatement==2)
    {
        QWidget::hide();
        decryptwindow* my_window = new decryptwindow;
        my_window->setCurrent();
        my_window-> show();
    }
}

