#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdlib>
#include <vector>
#include<string.h>
#include"gestfichier.h"
#include <QtWidgets>
#include <QPrinter>
#include <cstring>

using namespace std;

/**
 * void ReplaceStringInPlace(std::string& s, const std::string& from, const std::string& to)
 * role : cette fonction se chargera de remplacer un morceau d'une chaine de caractère par un autre
 * s : la chaine de caractère qui contient une sous chaine à remplacer
 * from : la sous chaine remplacer
 * to : par quoi la remplacer
*/
void ReplaceStringInPlace(std::string& s, const std::string& from, const std::string& to)
{
    size_t pos = 0;
    while ((pos = s.find(from, pos)) != std::string::npos)
    {
         s.replace(pos,from.length(),to);

         pos += to.length();
    }
}

void exportTxt ( string text, string chemin, string nomFichier){
    string path;
    path = chemin ;
    nomFichier = "";
    ofstream monFlux(path);
    if(monFlux)
    {
        monFlux << text << endl;

    }
    else
    {
        cout << "Error: The file couldn't be opened" << endl;
    }
}


void exportWord(string text, string chemin, string nomFichier){

    ofstream resWord;
    nomFichier = "";
    string path;
    path = chemin ;


    resWord.open(path);
    if(resWord.is_open()){
        cout << "file was oppened";
        resWord <<  text << endl;
        resWord.close();
    }else{
        cout << "Error: file could not be opened";
    }

}

void exportPdf(string text)
{
    QString m_text = QString::fromStdString(text);
    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Cryptopher certification", QString(), "*.pdf");
    QTextDocument doc;
    QPrinter printer(QPrinter::PrinterResolution);

    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    doc.setHtml(" <FONT color= red> <br> <U> <B> <p>Votre Texte : <p/> </B> </U> <br /> </FONT>"
                + m_text
                + "<FONT color= blue><br> <I><p>Written by Cryptopher </p></I><br /><FONT color= red>");
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);
}

string importTxt( string path){
    string text;
    ifstream myfile;
    string line;

    myfile.open(path);  //opening the file

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            text+= line;
        }
        myfile.close();
    }

    else cout << "Unable to open file";


    myfile.close();  //close the file

    ReplaceStringInPlace(text,"Votre Texte :","");
    ReplaceStringInPlace(text,"Written by Cryptopher","");

    return text;
}

string importWord(string path){
    string text;

    string cmd;
    cmd+="unzip -p ";
    cmd+= path;
    cmd+=" word/document.xml | sed -e 's/<[^>]\\{1,\\}>//g; s/[^[:print:]]\\{1,\\}//g'";


    char pathChar[cmd.size()+1];
    strcpy(pathChar, cmd.c_str());

    FILE *fp = popen( pathChar, "r");

    if (fp == nullptr) {
        clog << "popen failed:" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    char* linbuf = nullptr;
    size_t linsiz = 0;
    do {
        ssize_t linlen = getline(&linbuf, &linsiz, fp);
        if (linlen<=0) break;
        text+= string(linbuf, linlen);
    } while (!feof(fp));
    free (linbuf);

    int excod = pclose(fp);
    if (excod != 0)
        clog << "pclose failed " << excod << std::endl;

    return text;
}

/**
 * \string importPdf(char* path)
 * \role :  cette fonction se chargera de l'importation d'un texte contenu dans un fichier pdf
            elle se chargera d'executer une ligne de commande qui créera un fichier texte contenant le texte à charger
            elle appellera par la suite la fonction importTxt
 * \path : chemin du fichier que l'on souhaite charger
 * \retourne un string contenant le texte contenu dans le fichier chargé
 */

string importPdf(const char* path)
{
    cout << "path = " << path << endl;
    const char* p = path;
    cout << "p = " << p << endl;
    char cmd[300]="pdftotext -layout "; //ceci est la première partie de la ligne de commande à exécuter


    strcat(cmd,path); //la suite de la ligne de commande qui est le chemin absolu du fichier à charger
    cout << "cmd = " << cmd << endl;
    system (cmd); //exécution de la ligne de commande qui se chargera de créer un fichier texte contenant le texte à charger
    std::string str(p);
    cout << "2 cmd = " << cmd << endl;

    // string path2(path); //créer un string contenant le chemin du fichier à charger pour le passer en parametre à la fonction importTxt
   // cout << "path2 : " << path2 << endl;

    ReplaceStringInPlace(str,".pdf",".txt"); //remplacer le .pdf par .txt
    cout << "je fonctionne" << endl;
    cout << "str == " + str << endl;
    cout << importTxt("str = " + str);
    return importTxt(str);

    //return "fufyuyff";
}
