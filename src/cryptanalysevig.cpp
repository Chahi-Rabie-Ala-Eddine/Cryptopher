#include "cryptanalysevig.h"
#include "ui_cryptanalysevig.h"
#include"Polygramme.h"
#include<QDebug>

CryptanalyseVig::CryptanalyseVig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CryptanalyseVig)
{
    ui->setupUi(this);
    setStyleSheet("QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

}

CryptanalyseVig::~CryptanalyseVig()
{
    delete ui;
}


/**
 * \(VOID) remplirText(text)
 * \rôle : méthode qui remplit le texte dans le tableau de l'interface.
 * \arg text : c'est le texte en question qui doit être affichés sur l'interface.
 * \Return ; void
 */
void CryptanalyseVig::remplirText(TextV text)
{

    int i=0;
    for(int k=0;k<(int)text.getTextModified().length();k++)
    {
        if(text.getTextModified()[k]!='\n')
        {
        if(i==0) un+=text.getTextModified()[k];
        if(i==1) deux+=text.getTextModified()[k];
        if(i==2) trois+=text.getTextModified()[k];
        if(i==3) quatre+=text.getTextModified()[k];
        }
        else {
            if(i==0) un+='\n';
            if(i==1) deux+='\n';
            if(i==2) trois+='\n';
            if(i==3) quatre+='\n';
            i++;
        }
    }
    string h;
    int l=0;
        while(trois[l]!= ':')
        {
            h+=trois[l];
            l++;
        }

        string cle;
         l=0;
            while(quatre[l]!= ':')
            {
                cle+=quatre[l];
                l++;
            }

            quatre=cle;

    keySize=std::atoi(h.c_str());

    // Méthode qui remplit le Tableau Text a partir du text entré par l'utilisateur

    this->ui->tableWidget->setColumnCount(15);
    this->ui->tableWidget->setRowCount(text.getTextInput().length()/15);
    if(text.getTextInput().length()%15!=0)
    {
        this->ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);

    }

    int col=0,ligne=0;
    for(unsigned long i=0;i<text.getTextInput().length();i++)
    {
        if(col==16)
        {
            col=0;
            ligne++;
        }
        ui->tableWidget->setItem(ligne,col,new QTableWidgetItem(QChar(text.getTextInput().at(i))));
        col++;
    }


        myText=text;
}


/**
 * \(VOID) polygramsDetails(text)
 * \rôle : méthode qui affiche les polygrammes les plus fréquents ainsi que la distance qui les sépare.
 * \Return ; void
 */
void CryptanalyseVig::polygramsDetails()
{

    std::string str1="_____________________________________________________\n- Tout d'abord on recherche les polygrammes fréquents \n et on calcule la distance entre les differentes répetitions \nde ces polygrammes ";
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);
    for(map<string,Polygramme *>::iterator i=myText.bigram.begin();i!=myText.bigram.end();i++)
    {
        std::string str1="";
        if(i->second->getPositions().size()>1) {str1+= i->first;
        str1+=" : "; }
        list<int> pos=i->second->getPositions();
        std::list<int>::iterator it = pos.begin();
        for(int k=0;k<(int)pos.size()-1;k++)
        {
            str1+=std::to_string(*std::next(it,1)-(*it));
            str1+= " , ";
            it++;
        }
        if(i->second->getPositions().size()>1)
        {QString qsteps=QString::fromStdString(str1);
        ui->listWidget->addItem(qsteps);}
    }

    for(map<string,Polygramme *>::iterator i=myText.trigram.begin();i!=myText.trigram.end();i++)
    {
        std::string str1="";
        if(i->second->getPositions().size()>1) {str1+= i->first;
        str1+=" : "; }
        list<int> pos=i->second->getPositions();
        std::list<int>::iterator it = pos.begin();
        for(int k=0;k<(int)pos.size()-1;k++)
        {
            str1+=std::to_string(*std::next(it,1)-(*it));
            str1+= " , ";
            it++;
        }
        if(i->second->getPositions().size()>1)
        {QString qsteps=QString::fromStdString(str1);
        ui->listWidget->addItem(qsteps);}
    }

    for(map<string,Polygramme *>::iterator i=myText.quadrigram.begin();i!=myText.quadrigram.end();i++)
    {
        std::string str1="";
        if(i->second->getPositions().size()>1) {str1+= i->first;
        str1+=" : "; }
        list<int> pos=i->second->getPositions();
        std::list<int>::iterator it = pos.begin();
        for(int k=0;k<(int)pos.size()-1;k++)
        {
            str1+=std::to_string(*std::next(it,1)-(*it));
            str1+= " , ";
            it++;
        }
        if(i->second->getPositions().size()>1)
        {QString qsteps=QString::fromStdString(str1);
        ui->listWidget->addItem(qsteps);}
    }


    etape++;
}



/**
 * \(VOID) pgcdfind()
 * \rôle : méthode qui affiche les diviseurs les plus fréquents des distances.
 * \Return ; void
 */
void CryptanalyseVig::pgcdFind()
{
    std::string str1="_____________________________________________________\n- Ensuite nous allons nous intéreser au diviseurs les\n plus fréquents ";
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);

    //char c = myText.getTextModified()[0];
    int i =1;
    while(un[i] != '\n'){
            str1="";
            string premier_param = "";
            while(un[i] != ':' && un[i] != '\n'){
                 if(un[i]!= '+')
                     premier_param += un[i] ;

                 i++;
            }
            str1+=premier_param+" : ";
            string deuxieme_param = "";
            while(un[i] != '+' && un[i] != '\n'){
                if(un[i] != ':')
                    deuxieme_param += un[i] ;
                    i++;
            }
            str1+= std::to_string(std::atoi(deuxieme_param.c_str())-1) +" Fois";
            if(deuxieme_param>"2")
            {
            QString qsteps=QString::fromStdString(str1);
            ui->listWidget->addItem(qsteps);
            }

        i++;

    }
    etape++;
}


/**
 * \(VOID) inciceCalc()
 * \rôle : méthode qui affiche les indices de coincidences les plus fréquents des longeurs de clés les plus probables.
 * \Return ; void
 */
void CryptanalyseVig::indiceCalc()
{
    std::string str1="__________________________________________________________\n#- On calcule maintenant l'indice de coincidence du texte \n pour chaque  longeur possible de clé";
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);
    int i =1;
    while(deux[i] != '\n'){
            str1="";
            string premier_param = "";
            while(deux[i] != ':' && deux[i] != '\n'){
                 if(deux[i]!= '+')
                     premier_param += deux[i] ;

                 i++;
            }
            str1+=premier_param+" : ";
            string deuxieme_param = "";
            while(deux[i] != '+' && deux[i] != '\n'){
                if(deux[i] != ':')
                    deuxieme_param += deux[i] ;
                    i++;
            }
            str1+= deuxieme_param;


            QString qsteps=QString::fromStdString(str1);
            ui->listWidget->addItem(qsteps);

        i++;


    }



     str1="# On Choisit l'indice le plus grand \n # ";
     str1+=std::to_string(keySize);
     str1+=" est la longueur de la clé";

     qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);


    etape++;
}

/**
 * \(VOID) colorText()
 * \rôle : méthode qui colorie les caractéres qui ont été cryptés par le même decalage avec la même couleur.
 * \arg size: la taille de la clé qui permet de déterminer le nombre de blocs.
*/
void CryptanalyseVig::colorText(int size)
{
    std::string str1="______________________________________________________\n# On attribue une couleur a chaque lettre du \n texte de façon a ce que tout les caractéres \n cryptés en utilisant la même partie de la clé \n aient la meme couleur\n";
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);
    int k=0;
    QColor tabCouleurs[20]={QColor("yellow"),QColor("red"),QColor("cyan"),QColor("blue"),QColor("grey")};
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            if(ui->tableWidget->item(i,j))
                ui->tableWidget->item(i,j)->setBackgroundColor(tabCouleurs[k]);
            k++;
            if(k==size) k=0;
        }
    }
    etape++;
}
void CryptanalyseVig::sortPgcd()
{

    std::string str1="______________________________________________________\n- Ensuite on calcule les diviseurs de chaque groupe de\n distances ";
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);
    for(map<string,Polygramme *>::iterator i=myText.bigram.begin();i!=myText.bigram.end();i++)
    {
        std::string str1="";
        if(i->second->getPositions().size()>1) {str1+= i->first;
        str1+=" : "; }
        list<int> pos=i->second->getPositions();
        std::list<int>::iterator it = pos.begin();
        for(int k=0;k<(int)pos.size()-1;k++)
        {
            for(int sf=1;sf<15;sf++)
            {
                if((*std::next(it,1)-(*it))%sf==0)
                {
                    str1+=std::to_string(sf);
                    str1+=" , ";
                }
            }
            //*std::next(it,1)-(*it);

            it++;
        }


        if(i->second->getPositions().size()>1)
        {QString qsteps=QString::fromStdString(str1);
        ui->listWidget->addItem(qsteps);}
    }

    for(map<string,Polygramme *>::iterator i=myText.trigram.begin();i!=myText.trigram.end();i++)
    {
        std::string str1="";
        if(i->second->getPositions().size()>1) {str1+= i->first;
        str1+=" : "; }
        list<int> pos=i->second->getPositions();
        std::list<int>::iterator it = pos.begin();
        for(int k=0;k<(int)pos.size()-1;k++)
        {
            for(int sf=1;sf<15;sf++)
            {
                if((*std::next(it,1)-(*it))%sf==0)
                {
                    str1+=std::to_string(sf);
                    str1+=" , ";
                }
            }
            //*std::next(it,1)-(*it);

            it++;


        }
        if(i->second->getPositions().size()>1)
        {QString qsteps=QString::fromStdString(str1);
        ui->listWidget->addItem(qsteps);}
    }

    for(map<string,Polygramme *>::iterator i=myText.quadrigram.begin();i!=myText.quadrigram.end();i++)
    {
        std::string str1="";
        if(i->second->getPositions().size()>1) {str1+= i->first;
        str1+=" : "; }
        list<int> pos=i->second->getPositions();
        std::list<int>::iterator it = pos.begin();
        for(int k=0;k<(int)pos.size()-1;k++)
        {
            for(int sf=1;sf<15;sf++)
            {
                if((*std::next(it,1)-(*it))%sf==0)
                {
                    str1+=std::to_string(sf);
                    str1+=" , ";
                }
            }
            //*std::next(it,1)-(*it);

            it++;


        }
        if(i->second->getPositions().size()>1)
        {QString qsteps=QString::fromStdString(str1);
        ui->listWidget->addItem(qsteps);}
    }


    etape++;
}

/**
 * \(VOID) divideText()
 * \rôle : méthode qui divise le texte en plusieurs parties.
*/
void CryptanalyseVig::divideText()
{
    std::string str1="_______________________________________________________\n# On regroupe tout les caractéres cryptés par  \n le même caractére dans une seule colonne";
    QString qsteps=QString::fromStdString(str1);
    ui->listWidget->addItem(qsteps);
    int size=keySize; int k=0;
    vector<string> tabTexts;
    for(int l=0;l<size;l++)
    {
        tabTexts.push_back("");
    }
    for(int i=0;i<ui->tableWidget->rowCount();i++)
    {
        for(int j=0;j<ui->tableWidget->columnCount();j++)
        {
            if(ui->tableWidget->item(i,j))
            {
            tabTexts.at(k)+=ui->tableWidget->item(i,j)->text().toStdString();
            k++;
            if(k==size) k=0;
            }
        }
    }

    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(size);

    for(int s=0;s<size;s++)
    {
        if(ui->tableWidget->rowCount()<(int)tabTexts.at(s).length()) ui->tableWidget->setRowCount(tabTexts.at(s).length());
        for(int p=0;p<(int)tabTexts.at(s).length();p++)
        {

            ui->tableWidget->setItem(p,s,new QTableWidgetItem(QString(tabTexts.at(s).at(p))));
        }
    }
    etape++;
}

/**
 * \(VOID) mostFrequent()
 * \rôle : méthode qui permet de trouver le caractére le plus répété dans une partie du texte.
*/
void CryptanalyseVig::mostFrequent(int carAct)
{

   ui->tableWidget->selectColumn(carAct);
   std::string str1="_______________________________________________________________\n";
   str1+="# Le caractére le plus repeté dans cette partie  est \nle caractere ";
   int ab=((((int) quatre[carAct]) -65) + 4)%26;
   str1+= (char) (65+ab);
   str1+=" qui représente forcémment la lettre E \n- On calcule alors le décalage pour trouver la lettre  \nqui a crypté cette partie du texte \n";
   str1+=(char) ab+65 ;
   str1+=" - E modulo 26 =";
   str1+=(char) quatre[carAct];
   ui->label->setText(QString::fromStdString(ui->label->text().toStdString()+quatre[carAct]));
   QString qsteps=QString::fromStdString(str1);
   ui->listWidget->addItem(qsteps);

   if(carAct==keySize-1) { carActuel=0; etape++; }
   else carActuel++;
}

void CryptanalyseVig::initSteps()
{

}





void CryptanalyseVig::on_pushButton_clicked()
{
    switch (etape) {
    case 1: polygramsDetails(); break;
    case 2: sortPgcd(); break;
    case 3: pgcdFind(); break;
    case 4: indiceCalc(); break;
    case 5: colorText(keySize); break;
    case 6: divideText(); break;
    case 7: mostFrequent(carActuel); break;
    default: ui->pushButton->setEnabled(false);
    }
}
