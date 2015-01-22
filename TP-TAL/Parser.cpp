/*
 * Parser.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#include <numeric>
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
	m_ind_docs_test = NULL;
	m_ind_docs_test_test = NULL;
	m_nb_test = 0;
	//m_vect = NULL;
}

Parser::Parser(std::string filename) {
	m_filename = filename;
	m_ind_docs_test = NULL;
	m_ind_docs_test = NULL;
	m_nb_test = 0;
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
	return getIDF(m_lexicon[w]);
}

double Parser::getIDF(unsigned int w){
	std::vector<double> v;
	std::map<std::string,std::vector<double> >::iterator it;
	double resultat = 0;
	for(it=m_cat2vect.begin();it!=m_cat2vect.end();++it){
		v = m_cat2vect[it->first];
		if(v[w] != 0) // presence du mot dans le vecteur V
			resultat++;
	}
	resultat = (((double)m_cat2vect.size())/(resultat));
	return (std::log(resultat));
}

double Parser::compIDF(std::string w){
	return compIDF(m_lexicon[w]);
}

double Parser::compIDF(unsigned int w){
	std::vector<double> v;
	double resultat = 0;
	for(unsigned int i=0;i<m_nbArticles;i++){
		v = this->genVect(i);
		if(v[w] != 0) // presence du mot dans le vecteur V
			resultat++;
	}
	resultat = (((double)m_nbArticles)/(resultat));
	return (std::log(resultat));
}

double Parser::getTF(std::string w,unsigned int num){
	return getTF(m_lexicon[w],num);
}

double Parser::getTF(unsigned int w,unsigned int num){
	double resultat = 0;
	double somme = 0;
	resultat = m_vect[num][w]; // C(wi,D)
	if(resultat == 0)
		return 0;
	for(unsigned int i=0;i<m_vect[num].size();i++){
		somme = somme + m_vect[num][i];
	}
	return resultat/somme;
}

double Parser::getTF(unsigned int w,std::map<std::string,std::vector<double> >::iterator iter){
	double resultat = 0;
	resultat = iter->second[w]; // C(wi,D)
	if(resultat == 0)
		return 0;
	//for(unsigned int i=0;i<iter->second.size();i++){
	//somme = somme + iter->second[i];
	//}
	return resultat/m_nb_mots[iter->first];
}

double Parser::getTF_TEST(unsigned int w,unsigned int num){
	double resultat = 0;
	resultat = m_test_vect[num][w]; // C(wi,D)
	if(resultat == 0)
		return 0;
	//for(unsigned int i=0;i<iter->second.size();i++){
	//somme = somme + iter->second[i];
	//}
	return resultat/m_nb_mots_test[num];
}

double Parser::getTF_IDF(std::string w,unsigned int num){
	double TF = getTF(w,num);
	return TF == 0 ? 0 : (TF*getIDF(w));
}

double Parser::getTF_IDF(unsigned int w,unsigned int num){
	double TF = getTF(w,num);
	return TF == 0 ? 0 : (TF*getIDF(w));
}

double Parser::getTF_IDF_TEST(unsigned int w,unsigned int num){
	double TF = getTF_TEST(w,num);
	return TF == 0 ? 0 : (TF*getIDF(w));
}

double Parser::getTF_IDF(unsigned int w,std::map<std::string,std::vector<double> >::iterator iter){
	double TF = getTF(w,iter);
	return TF == 0 ? 0 : (TF*getIDF(w));
}

void Parser::genVectThemes(){
	std::stringstream ss; // String stream
	std::string line;
	std::string word;
	std::map<std::string,double>::iterator iter;
	size_t position;
	unsigned int count = 0;

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
	m_inputStream = new std::ifstream("data/trimmed_data.txt");
	if(!m_inputStream->is_open()){
		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}
		std::cerr << "Impossible d'ouvrir le fichier : " << "data/trimmed_data.txt" << std::endl;
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
			count++;
			//std::cout << "Article " << count << " / " << m_nbArticles << std::endl;
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
			if((!std::isdigit(word.at(0))) && (!std::ispunct(word.at(0))) ){
				m_cat2vect[m_ind_docs->at(count).first][m_lexicon[word]]++;
				m_nb_mots[m_ind_docs->at(count).first]++;
			}
		}
	}

}

void Parser::compDocInd(){

	//Declarations
	std::string line;
	size_t position;
	unsigned int count = 0;
	unsigned int num_ligne = 0;

	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL){
		delete m_inputStream;
		m_inputStream = NULL;
	}

	//std::cout << "Fichier : " << fn << " / Ligne : " << count << std::endl;
	m_inputStream = new std::ifstream("data/trimmed_data_eval.txt");

	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);


	while(std::getline(*m_inputStream,line)){ // tant qu'on peut lire des lignes
		position = line.find(std::string("</doc>"));
		if(position != std::string::npos){
			m_ind_docs_test->at(count).second.second = num_ligne-1;
			count++;
		}
		else{
			position = line.find(std::string("<doc"));
			if(position != std::string::npos){ // si on contient <doc
				m_ind_docs_test->at(count).second.first = num_ligne+1;
			}
		}
		num_ligne++;
	}
}

void Parser::compDocInd_TEST(){

	//Declarations
	std::string line;
	size_t position;
	unsigned int count = 0;
	unsigned int num_ligne = 0;

	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL){
		delete m_inputStream;
		m_inputStream = NULL;
	}

	//std::cout << "Fichier : " << fn << " / Ligne : " << count << std::endl;
	m_inputStream = new std::ifstream("data/corpus_depeche_eval.txt");

	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);


	while(std::getline(*m_inputStream,line)){ // tant qu'on peut lire des lignes
		position = line.find(std::string("</doc>"));
		if(position != std::string::npos){
			m_ind_docs_test_test->at(count).second.second = num_ligne;
			count++;
		}
		else{
			position = line.find(std::string("<doc"));
			if(position != std::string::npos){ // si on contient <doc
				m_ind_docs_test_test->at(count).second.first = num_ligne;
			}
		}
		num_ligne++;
	}
}

std::stringstream* Parser::chooseDoc_TEST(unsigned int num){

	// Verifs
	if( num > m_nb_test) // si on specifie un num trop grand
		return NULL;

	//Declarations
	std::stringstream* ss = new std::stringstream();
	std::string line;
	unsigned int count = 0;

	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL){
		delete m_inputStream;
		m_inputStream = NULL;
	}

	//std::cout << "Fichier : " << fn << " / Ligne : " << count << std::endl;
	m_inputStream = new std::ifstream("data/corpus_depeche_eval.txt");
	if(!m_inputStream->is_open()){
		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}
		std::cerr << "Impossible d'ouvrir le fichier : " << "data/trimmed_data_eval.txt" << std::endl;
	}

	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);

	//std::cout << " doc " << num << " : " << " 1 : " << m_ind_docs->at(num).first <<
	//		" / 2 : " << m_ind_docs->at(num).second << std::endl;

	// On se deplace a la bonne ligne
	while(count != m_ind_docs_test_test->at(num).second.first){
		std::getline(*m_inputStream,line); // on lit une ligne
		count++;
	}

	while(count != (m_ind_docs_test_test->at(num).second.second +1)){ // tant qu'on est pas au bon article
		std::getline(*m_inputStream,line); // on lit une ligne
		*ss << line << "\n";
		count++;
	}

	//std::cout << ss->str();
	return ss;
}

std::stringstream* Parser::chooseDoc(unsigned int num){

	// Verifs
	if( num > m_nb_test) // si on specifie un num trop grand
		return NULL;

	//Declarations
	std::stringstream* ss = new std::stringstream();
	std::string line;
	unsigned int count = 0;

	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL){
		delete m_inputStream;
		m_inputStream = NULL;
	}

	//std::cout << "Fichier : " << fn << " / Ligne : " << count << std::endl;
	m_inputStream = new std::ifstream("data/trimmed_data_eval.txt");
	if(!m_inputStream->is_open()){
		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}
		std::cerr << "Impossible d'ouvrir le fichier : " << "data/trimmed_data_eval.txt" << std::endl;
	}

	//On se place au debut du fichier
	m_inputStream->clear();
	m_inputStream->seekg(0,std::ios::beg);

	//std::cout << " doc " << num << " : " << " 1 : " << m_ind_docs->at(num).first <<
	//		" / 2 : " << m_ind_docs->at(num).second << std::endl;

	// On se deplace a la bonne ligne
	while(count != m_ind_docs_test->at(num).second.first){
		std::getline(*m_inputStream,line); // on lit une ligne
		count++;
	}

	while(count != (m_ind_docs_test->at(num).second.second+1)){ // tant qu'on est pas au bon article
		std::getline(*m_inputStream,line); // on lit une ligne
		*ss << line << "\n";
		count++;
	}

	//std::cout << ss->str();
	return ss;
}

void Parser::loadThemeVects(){
	unsigned int count = 0;
	unsigned int count_t = 0;
	std::stringstream ss; // String stream
	std::stringstream convert;
	std::string line;
	double val;
	std::map<std::string,double>::iterator it;

	for(it=m_categories.begin();it!=m_categories.end();++it ){
		count = 0;
		//std::vector<double> res (m_cat2vect.begin()->second.size(),0);
		std::string fn("data/theme_vectors/v_theme");
		convert << count_t;
		fn.append(convert.str());
		convert.str("");
		convert.clear();
		count_t++;

		if(m_inputStream->is_open())
			m_inputStream->close();

		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}

		//std::cout << "Fichier : " << fn << " / Ligne : " << count << std::endl;
		m_inputStream = new std::ifstream(fn.c_str());
		if(!m_inputStream->is_open()){
			if(m_inputStream != NULL){
				delete m_inputStream;
				m_inputStream = NULL;
			}
			std::cerr << "Impossible d'ouvrir le fichier : " << fn << std::endl;
		}

		while(std::getline(*m_inputStream,line)){

			//on efface le stream
			ss.clear();
			ss.str("");

			//on met la ligne dans le stream
			ss << line;

			//on recupere chaque premier mot du stream
			while(ss >> val){
				//res[count] = val;
				m_cat2vect[it->first][count] = val;
			}
			count++;
		}
		//m_cat2vect[it->first] = res;
	}
	std::cout << "Theme vectors loaded." << std::endl;
}

void Parser::computeNbArticle(){
	std::string line;
	std::size_t position;

	//On se place au debut du fichier
	m_inputStream->seekg(0,std::ios::beg);

	while(std::getline(*m_inputStream,line)){

		//TODO generer les themes ici

		position = line.find(std::string("</doc>"));
		if(position!=std::string::npos)
			m_nbArticles++;
	}

	m_ind_docs = new std::vector<std::pair<std::string,std::pair<unsigned int,unsigned int> > >(m_nbArticles);
	//m_vect = new std::vector<std::vector<double> > (m_nbArticles);
}

void Parser::computeNbTest(){
	std::string line;
	std::size_t position;

	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL){
		delete m_inputStream;
		m_inputStream = NULL;
	}

	//std::cout << "Fichier : " << fn << " / Ligne : " << count << std::endl;
	m_inputStream = new std::ifstream("data/trimmed_data_eval.txt");
	if(!m_inputStream->is_open()){
		if(m_inputStream != NULL){
			delete m_inputStream;
			m_inputStream = NULL;
		}
		std::cerr << "Impossible d'ouvrir le fichier : " << "data/trimmed_data_eval.txt" << std::endl;
	}


	//On se place au debut du fichier
	m_inputStream->seekg(0,std::ios::beg);

	while(std::getline(*m_inputStream,line)){

		//TODO generer les themes ici

		position = line.find(std::string("</doc>"));
		if(position!=std::string::npos)
			m_nb_test++;
	}

	m_ind_docs_test = new std::vector<std::pair<std::string,std::pair<unsigned int,unsigned int> > >(m_nb_test);
	m_ind_docs_test_test = new std::vector<std::pair<std::string,std::pair<unsigned int,unsigned int> > >(m_nb_test);
	//m_vect = new std::vector<std::vector<double> > (m_nbArticles);
}

void Parser::genAllVect(){
	for(unsigned int i=0;i<m_nb_test;i++){
		genVect(i);
	}
}

void Parser::genAllVectTF_IDF(unsigned int size){
	for(unsigned int i=0;i<m_nbArticles;i++){
		m_vect[i] = this->genVectTF_IDF(i,size);
	}
}

std::vector<double> Parser::genVect(unsigned int num){

	std::vector<double> res(m_lexicon.size());
	std::stringstream *ss = this->chooseDoc(num);
	std::stringstream ss2;
	std::string word;
	std::string line;
	std::map<std::string,double>::iterator iter;

	while(std::getline(*ss,line)){

		//on efface le stream
		ss2.clear();
		ss2.str("");

		//on met la ligne dans le stream
		ss2 << line;

		while(ss2 >> word){
			if((!std::isdigit(word.at(0))) && (!std::ispunct(word.at(0))) ){
				m_test_vect[num][m_lexicon[word]]++;
				m_nb_mots_test[num]++;
				//std::cout << "m_lexicon_test["<<word<<"] = " << m_lexicon_test[word] << " / " << m_test_vect[num][m_lexicon_test[word]] << std::endl;
			}
		}
	}
	delete ss;

	return res;
}

std::vector<double> Parser::genVectTF_IDF(unsigned int num,unsigned int size){
	std::vector<double>::iterator maxi;
	std::vector<double> res(m_lexicon.size(),0);
	std::vector<double> res2(size,0);
	std::stringstream *ss = this->chooseDoc(num);
	std::stringstream ss2;
	std::string word;
	std::string line;
	std::map<std::string,double>::iterator iter;

	while(std::getline(*ss,line)){

		//on efface le stream
		ss2.clear();
		ss2.str("");

		//on met la ligne dans le stream
		ss2 << line;

		while(ss2 >> word){
			if((!std::isdigit(word.at(0)))){
				res[m_lexicon[word]]++;
			}
		}

		//for(unsigned int i = 0;i<res.size();i++){
		//	res[i] = getTF_IDF(i,res);
		//}

		std::vector<double> copy = res;
		for(unsigned int j = 0;j<size;j++){
			maxi = std::max_element(copy.begin(),copy.end());
			res2[j] = *maxi;
			copy.erase(copy.begin() + std::distance(copy.begin(),maxi));
		}

	}

	delete ss;

	return res2;
}

void Parser::genLexicon(std::string norm_file){

	std::stringstream ss; // String stream
	std::string line;
	std::string word;
	std::map<std::string,double>::iterator iter;
	size_t position;
	unsigned int count = 0;
	unsigned int num_ligne = 0;

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
			m_ind_docs->at(count).second.second = num_ligne-1;
			count++;
		}

		//on ignore les <doc>
		position = line.find(std::string("<doc"));
		if(position != std::string::npos){
			std::string toFindBeg = "topic=\"";
			std::string toFindEnd = "\">";
			size_t position_beg, position_end;
			position_beg = line.find(std::string(toFindBeg));
			position_end = line.find(std::string(toFindEnd));
			std::string category =  line.substr(position_beg+toFindBeg.length(),
					position_end - (position_beg+toFindBeg.length()));

			m_categories[category]++;
			m_ind_docs->at(count).first = category;
			m_ind_docs->at(count).second.first = num_ligne+1;
			std::getline(*m_inputStream,line);
			ss.clear();
			ss.str("");
			ss << line;
		}

		//on recupere chaque premier mot du stream
		while(ss >> word){
			//On ignore tous les nombres
			if((!std::isdigit(word.at(0))) && (!std::ispunct(word.at(0))) ){
				m_lexicon[word]++;
			}
		}
		num_ligne++;
	}

	unsigned int id = 0;
	for(iter=m_lexicon.begin();iter != m_lexicon.end(); ++iter){
		*m_outputStream << iter->first << " " << iter->second << " " << id << "\n";
		m_occ[id] = iter->second;
		iter->second = (double)id;
		id++;
	}

	std::cout << "Lexicon size : " << m_lexicon.size() << std::endl;
	//m_vect = std::vector<std::vector<double> >(m_categories.size(), std::vector<double>(m_lexicon.size()));
	for(iter=m_categories.begin();iter!=m_categories.end();++iter){
		m_cat2vect[iter->first] = std::vector<double>(m_lexicon.size(),0);
	}

	m_outputStream->close();
	delete m_outputStream;
	m_outputStream = NULL;
}

void Parser::genLexiconTest(std::string norm_file){

	std::stringstream ss; // String stream
	std::string line;
	std::string word;
	std::map<std::string,double>::iterator iter;
	size_t position;
	//unsigned int num_ligne = 0;
	//unsigned int count = 0;

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
	m_outputStream = new std::ofstream("data/lexicon_eval.txt");

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
			//m_ind_docs_test->at(count).second.second = num_ligne-1;
			//count++;
		}

		//on ignore les <doc>
		position = line.find(std::string("<doc"));
		if(position != std::string::npos){
			std::getline(*m_inputStream,line);
			ss.clear();
			ss.str("");
			ss << line;
			//m_ind_docs_test->at(count).first = "Lol";
			//m_ind_docs_test->at(count).second.first = num_ligne+1;
		}

		//on recupere chaque premier mot du stream
		while(ss >> word){
			//On ignore tous les nombres
			if((!std::isdigit(word.at(0))) && (!std::ispunct(word.at(0))) ){
				m_lexicon_test[word]++;
			}
		}
		//num_ligne++;
	}

	unsigned int id = 0;
	for(iter=m_lexicon_test.begin();iter != m_lexicon_test.end(); ++iter){
		*m_outputStream << iter->first << " " << iter->second << " " << id << "\n";
		m_occ_test[id] = iter->second;
		iter->second = (double)id;
		id++;
	}

	std::cout << "Nombre d'articles Test : " << m_nb_test << std::endl;
	std::cout << "Test Lexicon size : " << m_lexicon_test.size() << std::endl;
	m_test_vect = std::vector<std::vector<double> >(m_nb_test, std::vector<double>(m_lexicon.size()));
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

	if(m_outputStream != NULL && m_outputStream->is_open())
		m_outputStream->close();

	if(m_outputStream != NULL)
		delete m_outputStream;

	if(m_ind_docs != NULL)
		delete m_ind_docs;

	if(m_ind_docs_test != NULL)
		delete m_ind_docs_test;

	if(m_ind_docs_test_test != NULL)
		delete m_ind_docs_test_test;

	//if(m_vect != NULL)
	//delete m_vect;
}

void Parser::showCat(){
	std::map<std::string,double>::iterator iter;
	for(iter=m_categories.begin();iter!=m_categories.end();++iter){
		std::cout << " - " << iter->first << " : " << iter->second << " articles" << std::endl;
	}
}

void Parser::compThemeVectTF_IDF(){
	std::stringstream ss; // String stream
	std::stringstream convert;
	std::string line;
	std::string word;
	unsigned int count_t = 0;

	std::map<std::string,std::vector<double> >::iterator iter;
	for(iter=m_cat2vect.begin();iter!=m_cat2vect.end();++iter){

		if(m_outputStream != NULL){
			delete m_outputStream;
			m_outputStream = NULL;
		}

		//Creation du fichier vecteur
		std::string fn("data/theme_vectors/v_theme");
		convert << count_t;
		fn.append(convert.str());
		convert.str("");
		convert.clear();
		count_t++;
		m_outputStream = new std::ofstream(fn.c_str());

		for(unsigned int i= 0;i<iter->second.size();i++){
			iter->second[i] = getTF_IDF(i,iter);
			*m_outputStream << iter->second[i] << "\n";
		}
		std::cout << " Vecteur theme de : " << iter->first << "  - - - OK !" << std::endl;
		m_outputStream->close();
	}
}

double Parser::dotProduct(std::vector<double> a, std::vector<double> b){
	return std::inner_product(a.begin(),a.end(),b.begin(),0.0);
}

void Parser::showTest(){
	std::map<std::string,std::vector<double> >::iterator it;
	std::vector<std::pair<std::string,double> >::iterator it2;
	std::vector<std::pair<std::string,double> > vals;
	std::stringstream *ss; // String stream
	std::string word;

	if(m_outputStream != NULL){
		delete m_outputStream;
		m_outputStream = NULL;
	}

	//Creation du fichier lexique
	m_outputStream = new std::ofstream("data/resultats.txt");

	//std::cout << " m_test_vect size =  " << m_test_vect.size() << " / " << m_nb_test << std::endl;
	for(unsigned int i=0;i<m_nb_test;i++){
		ss = chooseDoc_TEST(i);
		for(unsigned int j=0; j<m_test_vect[i].size();j++){
			m_test_vect[i][j] = getTF_IDF_TEST(j,i);
		}
		std::cout << " test_vect " << i << " / " << m_nb_test << std::endl;

		for(it=m_cat2vect.begin();it!=m_cat2vect.end();++it){
			vals.push_back(std::pair<std::string,double>(it->first,compCos(m_test_vect[i],it->second)));
		}

		double maxi = 0;
		std::string ret;
		for(it2=vals.begin();it2!=vals.end();++it2){
			if(it2->second > maxi){
				maxi = it2->second;
				ret = it2->first;
			}
		}
		std::cout << "Categorie choisie pour l'article " << i << " = " << ret << std::endl;


		std::string retour = "target=\"";
		retour.append(ret);
		retour.append("\"");

		while(*ss >> word){
			if(word.compare(">")){
				insert(ss,retour);
				break;
			}
		}
		//std::cout << ss->str();
		*m_outputStream << ss->str();
		delete ss;
		vals.clear();

	}

	//for(unsigned int i = 0; i<m_nb_test;i++){

	//}
}

std::stringstream* Parser::insert( std::stringstream* oss, const std::string& s )
{
	size_t position = 0;
	std::streamsize pos = oss->tellp();
	std::string n = oss->str();
	position = n.find(std::string(">"));
	if(position != std::string::npos){
		n.insert(position,s);
	}

	oss->str(n);
	oss->seekp( pos + s.length() );
	return oss;
}

double Parser::compCos(std::vector<double> a, std::vector<double> b){
	return dotProduct(a,b)/(l2_norm(a)*l2_norm(b));
}

double Parser::l2_norm(std::vector<double> const& u) {
	double accum = 0.;
	for (unsigned int i = 0; i < u.size(); ++i) {
		accum += u[i] * u[i];
	}
	return sqrt(accum);
}
