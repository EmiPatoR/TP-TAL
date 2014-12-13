/*
 * Parser.h
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

class Parser {
protected:
	std::string m_filename;
	std::ifstream* m_inputStream;
	std::ofstream* m_outputStream;
	unsigned int m_nbArticles;
	std::map<std::string,double> m_lexicon;
	std::vector<std::pair<unsigned int,unsigned int> > *m_ind_docs;
	std::vector<std::vector<double> > m_vect;

public:
	//Constructors
	Parser();
	Parser(std::string filename);

	//Methods
	void compDocInd();
	void preTraitement();
	void computeNbArticle();
	void genLexicon(std::string norm_file);
	void genCategories();

	double compIDF(std::string w); // OK
	double getIDF(std::string w); // OK

	double compTF(std::string w,std::vector<double> D); // TODO faire ça
	double getTF(std::string w,unsigned int num); // OK

	double compTF_IDF(std::string w,std::vector<double> D); //TODO faire ça
	double getTF_IDF(std::string w,unsigned int num);

	std::stringstream* chooseDoc(unsigned int num);
	std::vector<double> genVect(unsigned int num);
	void trimData();
	void genAllVect();

	//Getters
	int getNbArticles(){return m_nbArticles;}

	//Destructor
	virtual ~Parser();
};

#endif /* PARSER_H_ */
