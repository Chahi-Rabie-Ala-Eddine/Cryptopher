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
#include"gestfichier.h"
#include <MainWindow.h>
#include <QString>
#include <iostream>
#include <QLineEdit>
#include <QString>
#include <iostream>
#include <QLineEdit>
#include <QSignalMapper>
#include <chrono>
#include <thread>
#include <limits>
#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>

#define DEBUT 310
#define FIN 50

using namespace std;

resultwindow::resultwindow(QString txt,int type) : QWidget()
{
    setStyleSheet("QPushButton { background-color: yellow } QTabWidget::hover { background-color: red } QPushButton:hover { background-color: red } ");

    for(int i = 0; i < 26 ; i++)
    {
        alphabet[i] = 65 + i;
    }
    setFixedSize(900, 500);
    onglets = new QTabWidget(this);
    onglets->setGeometry(10, 10, 880, 480);

    //texte de la fenetre
    this->setWindowTitle("Outil automatique d'aide au décryptage");

    setTextCrypte(txt);

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
            lines[i]->setGeometry(DEBUT+((i-13)*30)-50, FIN+30 , 30, 30);
        lines[i]->setMaxLength(1);
        lines[i]->setAlignment(Qt::AlignCenter);
        connect(lines[i], &QLineEdit::textChanged, [this,i](){ modify_char(i,lines[i]->text()); });
    }
    edit = new QTextEdit(this); // assuming this is your text edit
    QScrollBar *sb = edit->verticalScrollBar();
    sb->setValue(sb->maximum());
    edit->setGeometry(160, 120 , 600, 300);
    edit->setText(txt);

    QPushButton* btn_un = new QPushButton(QString::fromWCharArray(L"\u27f3").toStdString().c_str(),this);
    btn_un->setGeometry(DEBUT+350, FIN+15, 35, 35);

    QPushButton* btn_cinq = new QPushButton(QString::fromWCharArray(L"\u25C2").toStdString().c_str(),this);
    btn_cinq->setGeometry(20, 20, 20, 24);

    QObject::connect(btn_un, SIGNAL(clicked()), this, SLOT(reset()));

    QPushButton* btn_deux = new QPushButton("Export",this);
    btn_deux->setGeometry(660, 425 , 100, 30);


    //QPushButton* btn_trois = new QPushButton(QString::fromWCharArray(L"\u23EA").toStdString().c_str(),this);
    QPushButton* btn_trois = new QPushButton(QString::fromWCharArray(L"\u2A2F").toStdString().c_str(),this); //25B8
    btn_trois->setGeometry(42, 20, 20, 24);

    free(letter);

    QObject::connect(btn_deux, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(btn_trois, SIGNAL(clicked()), this, SLOT(back_home()));
    QObject::connect(btn_cinq, SIGNAL(clicked()), this, SLOT(retour()));

    this->setAttribute(Qt::WA_DeleteOnClose);

    if(type == 2)
    {
        QPushButton* btn_six = new QPushButton("Traîter",this); //25B8
            btn_six->setGeometry(550, 425, 100,30);
            QObject::connect(btn_six, SIGNAL(clicked()), this, SLOT(addSpaces()));
    }
}

void resultwindow::setType(int i)
{
    type=i;
}

void resultwindow::retour()
{
    if(type==1)
    {
        this->setAttribute(Qt::WA_DeleteOnClose);
        QWidget::hide();
        encryptwindow* my_window = new encryptwindow();
        //my_window->setCurrent();
        my_window-> show();
        //resultwindow::~resultwindow();
    }
    else
    if(type==2)
    {
        this->setAttribute(Qt::WA_DeleteOnClose);

        QWidget::close();
        decryptwindow* my_window = new decryptwindow();
        //my_window->setCurrent();
        my_window-> show();
       // resultwindow::~resultwindow();
    }
}

void resultwindow::save()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Sous quel format voudriez vous enregistrer votre fichier ?"));
    QAbstractButton* pdf = msgBox.addButton(tr("PDF"), QMessageBox::YesRole);
    QAbstractButton* word = msgBox.addButton(tr("WORD"), QMessageBox::YesRole);
    QAbstractButton* txt = msgBox.addButton(tr("TXT"), QMessageBox::YesRole);
    msgBox.addButton(tr("Annulé"), QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton()== pdf)
    {
        exportPdf(edit->toPlainText().toStdString());
    }
    else if (msgBox.clickedButton()== word)
        {
            QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Cryptopher certification", QString(), "*.doc");
            QTextDocument doc;
            if (QFileInfo(fileName).suffix().isEmpty())
            {
                fileName.append(".doc");
            }
            exportWord(edit->toPlainText().toStdString(),fileName.toStdString(),"");

        }
        else if (msgBox.clickedButton()== txt)
        {
            QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Cryptopher certification", QString(), "*.txt");
            QTextDocument doc;
            if (QFileInfo(fileName).suffix().isEmpty())
            {
                fileName.append(".txt");
            }

            exportTxt(edit->toPlainText().toStdString(),fileName.toStdString(),"");
        }


}

void resultwindow::setTextCrypte(QString txt)
{
    this->texte_crypte = txt;
}

void resultwindow::modify_text(string texte, char ancien, char nouveau)
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

void resultwindow::modify_char(int i, QString nouveau)
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
    cout << "\n" << i << endl;
    cout << "=> Ancien Carctère : " << (char)alphabet[i] << endl;
    cout << "=> Nouveau Caractère : " << qPrintable(nouveau) << endl;
    resultwindow::modify_text(edit->toPlainText().toStdString(),alphabet[i],temp[0]);
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

void resultwindow::reset()
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
    edit->setText(this->texte_crypte);
}

void resultwindow::back_home()
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    QWidget::hide();
    mainwindow* my_window = new mainwindow();
    my_window->show();
    //resultwindow::~resultwindow();
}
resultwindow::~resultwindow()
{
    delete this->edit;
}

void resultwindow::addSpaces(){

   int A_end = 183867 ;
   int B_end = 280415 ;
   int C_end = 508334 ;
   int D_end = 746449 ;
   int E_end = 904674 ;
   int F_end = 987205 ;
   int G_end = 1048426 ;
   int H_end = 1084482 ;
   int I_end = 1180318 ;
   int J_end = 1195400 ;
   int K_end = 1197540 ;
   int L_end = 1236685 ;
   int M_end = 1330081 ;
   int N_end = 1353727 ;
   int O_end = 1390924 ;
   int P_end = 1571678 ;
   int Q_end = 1578578 ;
   int R_end = 1809510 ;
   int S_end = 1946814 ;
   int T_end = 2047355 ;
   int U_end = 2053302 ;
   int V_end = 2097662 ;
   int W_end = 2097891 ;
   int X_end = 2098070 ;
   int Y_end = 2098329 ;

   ifstream iFile;

    string cleared_text = "";

    string line ;
    int size = 16;
    int begin = 0;
    bool found;

    while (size != 0) {

        iFile.open("/home/aladin/Documents/Projet Final Outil automatique de décryptage/src/french_word.txt");
        found = false;

        if (iFile.is_open()){

            char probable_word[size];
            this->edit->toPlainText().toStdString().copy(probable_word, size, begin);
            probable_word[size-1] = '\0';

            switch (probable_word[0]) {
                case 'A': break;
                case 'B': iFile.seekg(A_end); break;
                case 'C': iFile.seekg(B_end); break;
                case 'D': iFile.seekg(C_end); break;
                case 'E': iFile.seekg(D_end); break;
                case 'F': iFile.seekg(E_end); break;
                case 'G': iFile.seekg(F_end); break;
                case 'H': iFile.seekg(G_end); break;
                case 'I': iFile.seekg(H_end); break;
                case 'J': iFile.seekg(I_end); break;
                case 'K': iFile.seekg(J_end); break;
                case 'L': iFile.seekg(K_end); break;
                case 'M': iFile.seekg(L_end); break;
                case 'N': iFile.seekg(M_end); break;
                case 'O': iFile.seekg(N_end); break;
                case 'P': iFile.seekg(O_end); break;
                case 'Q': iFile.seekg(P_end); break;
                case 'R': iFile.seekg(Q_end); break;
                case 'S': iFile.seekg(R_end); break;
                case 'T': iFile.seekg(S_end); break;
                case 'U': iFile.seekg(T_end); break;
                case 'V': iFile.seekg(U_end); break;
                case 'W': iFile.seekg(V_end); break;
                case 'X': iFile.seekg(W_end); break;
                case 'Y': iFile.seekg(X_end); break;
                case 'Z': iFile.seekg(Y_end); break;
            }
            while ( getline(iFile, line)){

                if (line[0] != probable_word[0] || line.length()+1 > size) {
                    break;
                }

                if ( line.compare(probable_word) == 0) {
                    found = true;
                    transform(line.begin(),line.end(), line.begin(), ::tolower);
                    line[0] = toupper(line[0]);
                    cleared_text += line + ' ';
                    break;
                }
            }

            if (found) {
                begin += size -1;
                size = 17;
                if(begin >= this->edit->toPlainText().toStdString().length()){
                    break;
                }
            }
            size-- ;
        }
        iFile.close();
    }
    this->edit->setText(QString::fromStdString( cleared_text));

}
//LASAINTVALENTINLACTEDESAMOUREUXUNECTETRSANZIENNEQUIAPRISUNEIMPORTANZEPARTIZULIREZESDERNIRESANNESLESCRANAISOCCRENTUNPETITZADEAULEURAMOUREUXZEPETITZADEAUPEUTTREUNBOUQUETDECLEURSUNBIJOUOUUNESORTIEOUBIENLESTROISLACOISADPENDVRAIMENTDESGENSDESGOTSDELIMAGINATIONETDESMOYENSDEZHAZUNZELAPEUTTREUNESOIREROMANTIQUEDANSUNRESTAURANTAVEZVUESURLASEINEPOURLESUNSUNSIMPLEBAISERZHANGSOUSUNPORZHEPOURLESAUTRESZESTZEQUICAITPEUTTRELESUZZSDEZETTECTERIENNESTDZIDZHAZUNCAITZOMMEILVEUTETZONTRAIREMENTLAPLUPARTDESCTESZOMMEPQUESNOLOULENOUVELANZESTUNECTEQUINESECTEPASENCAMILLEPASQUESTIONDEMMENERLESENCANTSZEJOURLZESTLANOUNOUQUISENOZZUPEETZEUXQUINONTPASDEPETITAMIETBIENILSONTLEZHOIXENTRESORTIRENTREZLIBATAIRESOUTENIRLAZHANDELLEETZHEFVOUSZESTZOMMENTZOMMENTCTETONLASAINTVALENTINOCCREFVOUSDESCLEURSOUAUTREZHOSEZETTECTEEXISTETELLEAUSSIDANSVOTREPAYS
//ZMWMRANDMZKANRAZMSNKXKWMIJLPKLFLAKSNKNPWMAHRKAAKOLRMVPRWLAKRIVJPNMAHKVMPNRHLZRPKHKWXKPARPKWMAAKWZKWSPMAMRWJSSPKANLAVKNRNHMXKMLZKLPMIJLPKLFHKVKNRNHMXKMLVKLNNPKLATJLOLKNXKSZKLPWLATRUJLJLLAKWJPNRKJLTRKAZKWNPJRWZMSJRWMXVKAXDPMRIKANXKWYKAWXKWYJNWXKZRIMYRAMNRJAKNXKWIJGKAWXKHBMHLAHKZMVKLNNPKLAKWJRPKPJIMANROLKXMAWLAPKWNMLPMANMDKHDLKWLPZMWKRAKVJLPZKWLAWLAWRIVZKTMRWKPHBMAYWJLWLAVJPHBKVJLPZKWMLNPKWHKWNHKOLRSMRNVKLNNPKZKWLHHWXKHKNNKSNKPRKAAKWNXHRXHBMHLASMRNHJIIKRZDKLNKNHJANPMRPKIKANZMVZLVMPNXKWSNKWHJIIKVOLKWAJZJLZKAJLDKZMAHKWNLAKSNKOLRAKWKSNKVMWKASMIRZZKVMWOLKWNRJAXKIIKAKPZKWKASMANWHKUJLPZHKWNZMAJLAJLOLRWKAJHHLVKKNHKLFOLRAJANVMWXKVKNRNMIRKNTRKARZWJANZKHBJRFKANPKWJPNRPKANPKHZRTMNMRPKWJLNKARPZMHBMAXKZZKKNHBKCDJLWHKWNHJIIKANHJIIKANSNKNJAZMWMRANDMZKANRAJSSPKCDJLWXKWSZKLPWJLMLNPKHBJWKHKNNKSNKKFRWNKNKZZKMLWWRXMAWDJNPKVMGW
