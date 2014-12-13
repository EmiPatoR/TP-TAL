/*
 * main.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#include <iostream>
//#include <boost/algorithm/string.hpp>

#include "Parser.h"

using namespace std;

int main(int argc,char* argv[]){

	//unsigned int num = 658;
	//stringstream *ss;
	std::string mot("australie");
	std::string mot2("allemagne");

	cout << "Hello world !" << endl;

	//Parser p("data/corpus_depeche_part1.txt");
	Parser p("data/trimmed_data.txt");
	p.computeNbArticle();

	cout << "Nombre d'articles : " << p.getNbArticles() << endl;

	//Normalisation du texte
	//p.trimData();

	//Generation du lexique
	p.genLexicon("data/trimmed_data.txt");

	//Test
	p.compDocInd();
	p.genAllVect();

	cout << "IDF(" << mot << ") = " << p.getIDF(mot) << endl;
	cout << "TF (" << mot << ") = " << p.getTF(mot,0) << endl;
	cout << "TF-IDF (" << mot << ") = " << p.getTF_IDF(mot,0) << endl;
	cout << "IDF(" << mot2 << ") = " << p.getIDF(mot2) << endl;
	cout << "TF (" << mot2 << ") = " << p.getTF(mot2,0) << endl;
	cout << "TF-IDF (" << mot2 << ") = " << p.getTF_IDF(mot2,0) << endl;

	return 0;
}


