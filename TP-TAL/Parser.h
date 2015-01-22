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
	unsigned int m_nb_test;

	std::map<unsigned int,double> m_occ;
	std::map<unsigned int,double> m_occ_test;
	std::map<std::string,double> m_categories;
	std::map<std::string,std::vector<unsigned int> > m_cat2numlist;
	std::map<std::string,std::vector<double> > m_cat2vect;
	std::map<std::string,double> m_lexicon;
	std::map<std::string,double> m_lexicon_test;
	std::map<std::string,std::vector<double> > m_cat2TF_IDFvect;
	std::map<std::string,unsigned int> m_nb_mots;
	std::map<unsigned int,unsigned int> m_nb_mots_test;
	std::vector<std::pair<std::string,std::pair<unsigned int,unsigned int> > > *m_ind_docs;
	std::vector<std::pair<std::string,std::pair<unsigned int,unsigned int> > > *m_ind_docs_test;
	std::vector<std::pair<std::string,std::pair<unsigned int,unsigned int> > > *m_ind_docs_test_test;
	std::vector<std::vector<double> > m_vect;
	std::vector<std::vector<double> > m_test_vect;

public:
	//Constructors
	Parser();
	Parser(std::string filename);

	//Methods
	double l2_norm(std::vector<double> const& u);
	double compCos(std::vector<double> a, std::vector<double> b);
	void showTest();
	void compCorespondances();
	void computeNbTest();
	void compDocInd();
	void preTraitement();
	void computeNbArticle();
	void genLexicon(std::string norm_file);
	void genLexiconTest(std::string norm_file);
	void genCategories();
	std::stringstream* chooseDoc_TEST(unsigned int num);
	void compDocInd_TEST();
	std::stringstream* insert( std::stringstream* oss, const std::string& s );


	double compIDF(std::string w); // OK
	double compIDF(unsigned int w); // OK
	double getIDF(std::string w); // OK
	double getIDF(unsigned int w); // OK

	double compTF(std::string w,std::vector<double> D); // TODO faire ça
	double compTF(unsigned int w,std::vector<double> D); // TODO faire ça

	double getTF(std::string w,unsigned int num); // OK
	double getTF(unsigned int w,unsigned int num); // OK
	double getTF(unsigned int w,std::map<std::string,std::vector<double> >::iterator iter);
	double getTF_TEST(unsigned int w,unsigned int num);
	double getTF_IDF_TEST(unsigned int w,unsigned int num);

	double compTF_IDF(std::string w,std::vector<double> D); //TODO faire ça
	double compTF_IDF(unsigned int w,std::vector<double> D); //TODO faire ça
	double getTF_IDF(std::string w,unsigned int num);
	double getTF_IDF(unsigned int w,unsigned int num);
	double getTF_IDF(unsigned int w,std::map<std::string,std::vector<double> >::iterator iter);

	void compThemeVectTF_IDF();

	std::stringstream* chooseDoc(unsigned int num);
	std::vector<double> genVect(unsigned int num);
	std::vector<double> genVectTF_IDF(unsigned int num,unsigned int size);

	void loadThemeVects();
	void trimData();
	void genAllVect();
	void genAllVectTF_IDF(unsigned int size);
	void genVectThemes();

	double dotProduct(std::vector<double> a, std::vector<double> b);

	//Show categories
	void showCat();

	//Getters
	int getNbArticles(){return m_nbArticles;}

	//Destructor
	virtual ~Parser();
};

#endif /* PARSER_H_ */
