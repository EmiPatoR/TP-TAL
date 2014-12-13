/*
 * Parser.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <boost/algorithm/string.hpp>

#include "Parser.h"

Parser::Parser() {
	m_filename = std::string();
	m_inputStream = NULL;
	m_outputStream = NULL;
	m_nbArticles = 0;
	m_ind_docs = NULL;
	//m_vect = NULL;
}

Parser::Parser(std::string filename) {
	m_filename = filename;
	m_inputStream = new std::ifstream(filename.c_str());
	if(!m_inputStream->is_open()){
		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}
		std::cerr << "Impossible d'ouvrir le fichier : " << filename << std::endl;
	}
	m_nbArticles = 0;
	m_outputStream = NULL;
	m_ind_docs = NULL;
	//m_vect = NULL;
}

double Parser::getIDF(std::string w){
	std::vector<double> v;
	double resultat = 0;
	for(unsigned int i=0;i<m_nbArticles;i++){
		v = this->m_vect.at(i);
		if(v[m_lexicon[w]] != 0) // presence du mot dans le vecteur V
			resultat++;
	}
	resultat = (((double)m_nbArticles)/(resultat));
	return (std::log(resultat));
}

double Parser::compIDF(std::string w){
	std::vector<double> v;
	double resultat = 0;
	for(unsigned int i=0;i<m_nbArticles;i++){
		v = this->genVect(i);
		if(v[m_lexicon[w]] != 0) // presence du mot dans le vecteur V
			resultat++;
	}
	resultat = (((double)m_nbArticles)/(resultat));
	return (std::log(resultat));
}

double Parser::getTF(std::string w,unsigned int num){
	double resultat = 0;
	double somme = 0;
	resultat = m_vect[num][m_lexicon[w]]; // C(wi,D)
	for(unsigned int i=0;i<m_vect[num].size();i++){
		somme = somme + m_vect[num][i];
	}
	return resultat/somme;
}

double Parser::getTF_IDF(std::string w,unsigned int num){
	return (getTF(w,num)*getIDF(w));
}

void Parser::compDocInd(){

	//Declarations
	std::string line;
	size_t position;
	unsigned int count = 0;
	unsigned int num_ligne = 0;

	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);


	while(std::getline(*m_inputStream,line)){ // tant qu'on peut lire des lignes
		position = line.find(std::string("</doc>"));
		if(position != std::string::npos){
			m_ind_docs->at(count).second = num_ligne-1;
			count++;
		}
		else{
			position = line.find(std::string("<doc"));
			if(position != std::string::npos){ // si on contient <doc
				m_ind_docs->at(count).first = num_ligne+1;
			}
		}
		num_ligne++;
	}
}

std::stringstream* Parser::chooseDoc(unsigned int num){

	// Verifs
	if( num > m_nbArticles) // si on specifie un num trop grand
		return NULL;

	//Declarations
	std::stringstream* ss = new std::stringstream();
	std::string line;
	unsigned int count = 0;

	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);

	//std::cout << " doc " << num << " : " << " 1 : " << m_ind_docs->at(num).first <<
	//		" / 2 : " << m_ind_docs->at(num).second << std::endl;

	// On se deplace a la bonne ligne
	while(count != m_ind_docs->at(num).first){
		std::getline(*m_inputStream,line); // on lit une ligne
		count++;
	}

	while(count != (m_ind_docs->at(num).second+1)){ // tant qu'on est pas au bon article
		std::getline(*m_inputStream,line); // on lit une ligne
		*ss << line << "\n";
		count++;
	}

	return ss;
}

void Parser::computeNbArticle(){

	std::string line;
	std::size_t position;

	//On se place au debut du fichier
	m_inputStream->seekg(0,std::ios::beg);

	while(std::getline(*m_inputStream,line)){
		position = line.find(std::string("</doc>"));
		if(position!=std::string::npos)
			m_nbArticles++;
	}

	m_ind_docs = new std::vector<std::pair<unsigned int,unsigned int> >(m_nbArticles);
	//m_vect = new std::vector<std::vector<double> > (m_nbArticles);
}

void Parser::genAllVect(){
	for(unsigned int i=0;i<m_nbArticles;i++){
		m_vect[i] = this->genVect(i);
	}
}

std::vector<double> Parser::genVect(unsigned int num){

	//	if(m_outputStream != NULL){
	//		delete m_outputStream;
	//		m_outputStream = NULL;
	//	}

	//Creation du fichier vecteur
	//	m_outputStream = new std::ofstream("data/vector.txt");

	std::vector<double> res(m_lexicon.size(),0);
	std::stringstream *ss = this->chooseDoc(num);
	std::stringstream ss2;
	std::string word;
	std::string line;
	std::map<std::string,double>::iterator iter;

	//std::cout << ss->str() << std::endl;

	while(std::getline(*ss,line)){

		//on efface le stream
		ss2.clear();
		ss2.str("");

		//on met la ligne dans le stream
		ss2 << line;
		//std::cout << " SS2 : " << ss2.str() << std::endl;
		while(ss2 >> word){
			if((!std::isdigit(word.at(0)))){
				res[m_lexicon[word]]++;
			}
		}
	}

	//	for(iter=m_lexicon.begin();iter!=m_lexicon.end();++iter){
	//		*m_outputStream << res[iter->second] << "\n";
	//	}

	delete ss;
	//	m_outputStream->close();
	//	delete m_outputStream;
	//	m_outputStream = NULL;
	return res;
}

void Parser::genLexicon(std::string norm_file){

	std::stringstream ss; // String stream
	std::string line;
	std::string word;
	std::map<std::string,double>::iterator iter;
	size_t position;

	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL){
		delete m_inputStream;
		m_inputStream = NULL;
	}

	if(m_outputStream != NULL){
		delete m_outputStream;
		m_outputStream = NULL;
	}

	//Creation du fichier lexique
	m_outputStream = new std::ofstream("data/lexicon.txt");

	//Ouverture du fichier normalisé
	m_inputStream = new std::ifstream(norm_file.c_str());
	if(!m_inputStream->is_open()){
		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}
		std::cerr << "Impossible d'ouvrir le fichier : " << norm_file << std::endl;
	}

	while(std::getline(*m_inputStream,line)){

		//on efface le stream
		ss.clear();
		ss.str("");

		//on met la ligne dans le stream
		ss << line;

		//on ignore les </doc>
		position = line.find(std::string("</doc>"));
		if(position != std::string::npos){
			std::getline(*m_inputStream,line);
			ss.clear();
			ss.str("");
			ss << line;
		}

		//on ignore les <doc>
		position = line.find(std::string("<doc"));
		if(position != std::string::npos){
			std::getline(*m_inputStream,line);
			ss.clear();
			ss.str("");
			ss << line;
		}


		//on recupere chaque premier mot du stream
		while(ss >> word){
			//On ignore tous les nombres
			if((!std::isdigit(word.at(0)))){
				m_lexicon[word]++;
			}
		}

	}

	unsigned int id = 0;
	for(iter=m_lexicon.begin();iter != m_lexicon.end(); ++iter){
		*m_outputStream << iter->first << " " << iter->second << " " << id << "\n";
		iter->second = (double)id;
		id++;
	}

	m_vect = std::vector<std::vector<double> >(m_nbArticles, std::vector<double>(m_lexicon.size()));

	m_outputStream->close();
	delete m_outputStream;
	m_outputStream = NULL;
}

void Parser::trimData(){

	if(m_outputStream != NULL){
		delete m_outputStream;
		m_outputStream = NULL;
	}

	m_outputStream = new std::ofstream("data/trimmed_data.txt");
	std::stringstream ss;
	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);

	std::string line;
	std::string word;

	size_t position;
	size_t position2;
	//size_t occ = 0;

	unsigned int it = 0;
	int i = 0;
	int j = 0;
	//double val;


	while(std::getline(*m_inputStream,line)){

		//on efface le stream
		ss.clear();
		ss.str("");

		// On supprime les espaces UNICODES presents dans la ligne
		position = line.find(std::string(" "));
		while(position!=std::string::npos){
			line.erase(position,std::string(" ").length());
			position = line.find(std::string(" "));
		}

		//on met la ligne dans le stream
		ss << line;

		//std::cout << "Ligne " << i << " = " << line << std::endl;

		//on recupere chaque premier mot du stream
		while(ss >> word){
			if(j!=0)
				*m_outputStream << ' ';

			// Ponctuation _________________________________

			//Points(.)
			position = word.find(std::string("."));
			if(position!=std::string::npos){
				if(position == word.length()-1)
					word.insert(position,1,' ');
			}

			//points de suspension(...)
			position = word.find(std::string("..."));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
			}

			//Points d'eclamations(!)
			position = word.find(std::string("!"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//dieses(#)
			position = word.find(std::string("#"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//Arobases(@)
			position = word.find(std::string("@"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//Signes plus(+)
			position = word.find(std::string("+"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//Virgules(,)
			position = word.find(std::string(","));
			if(position!=std::string::npos){
				if(position == word.length()-1){
					word.insert(position,1,' ');
				}
				else{
					if(position == 0){
						word.insert(position+1,1,' ');
					}
					else{
						//blabla
					}
				}
			}

			//Apostrophes(')
			position = word.find(std::string("'"));
			if(position!=std::string::npos){
				word.insert(position+1,1,' ');
			}

			//Apostrophes2(’)
			position = word.find(std::string("’"));
			if(position!=std::string::npos){
				word.replace(position,std::string("’").length(),std::string("'"));
			}

			//www
			position = word.find(std::string("www"));
			if(position!=std::string::npos){
				position2 = word.find(std::string("http"));
				if((position != 0) && position2==std::string::npos)
					word.insert(position,1,' ');
			}

			//etoiles(*)
			position = word.find(std::string("*"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//&amp;(&)
			position = word.find(std::string("&amp;"));
			if(position!=std::string::npos){
				word = word.substr(position,1);
			}

			//point virgule(;)
			position = word.find(std::string(";"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//Pourcentages(%)
			position = word.find(std::string("%"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//Deux points(:)
			position = word.find(std::string(":"));
			if(position!=std::string::npos){
				//gestion des adresses web
				position2 = word.find(std::string("http"));
				if(position2==std::string::npos){
					word.insert(position,1,' ');
				}
				else{
					if(position2 > position){
						word.insert(position+1,1,' ');
					}
					else{
						if(position2 != 0){
							word.insert(position2,1,' ');
						}
					}
				}
			}

			//Parentheses ouvrantes(()
			position = word.find(std::string("("));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			//Parentheses fermantes())
			position = word.find(std::string(")"));
			if(position!=std::string::npos){
				word.insert(position,1,' ');
				word.insert(position+2,1,' ');
			}

			// Elements graphiques_________________________

			//Tirets(-)
			position = word.find(std::string("-"));
			if(position!=std::string::npos){

				//if(position == 0)
				//word.erase(position,1);

				while(it < word.length()){
					if(it < word.length()-1 && word.at(it) == '-'){
						if(word.at(it+1)=='-'){
							word.erase(it,2);

						}
						else{
							it++;
						}
					}
					else if ((word.at(it) == '-') && (it == word.length()-1)){
						word.erase(it,1);
						it--;
					}
					else{
						it++;
					}
				}
				it = 0;
			}

			//Egals(=)
			position = word.find(std::string("="));
			if(position!=std::string::npos){

				if(position == 0)
					word.erase(position,1);

				while(it < word.length()){
					if(it < word.length()-1 && word.at(it) == '='){
						if(word.at(it+1)=='='){
							word.erase(it,2);
						}
						else{
							it++;
						}
					}
					else if ((word.at(it) == '=') && (it == word.length()-1)){
						word.erase(it,1);
						it--;
					}
					else{
						it++;
					}
				}
				it = 0;
			}

			std::transform(word.begin(),word.end(),word.begin(), ::tolower);

			//std::cout << "Mot " << j << " = " << word << std::endl;

			*m_outputStream << word;
			j++;
		}
		*m_outputStream << '\n';
		i++;
		j = 0;
	}

	m_outputStream->close();
	delete m_outputStream;
	m_outputStream = NULL;
}

Parser::~Parser() {
	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL)
		delete m_inputStream;

	if(m_outputStream->is_open())
		m_outputStream->close();

	if(m_outputStream != NULL)
		delete m_outputStream;

	if(m_ind_docs != NULL)
		delete m_ind_docs;

	//if(m_vect != NULL)
		//delete m_vect;
}

