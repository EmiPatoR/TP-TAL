/*
 * ModelLanguage.h
 *
 *  Created on: Jan 22, 2015
 *      Author: emip
 */

#ifndef MODELLANGUAGE_H_
#define MODELLANGUAGE_H_


#include <iostream>
#include <vector>
#include <map>
/*
 *
 */
class ModelLanguage {

private:
	std::map<unsigned int,std::vector<std::string> > m_phrases; //phrases
	std::vector<std::string> m_mots; //mots
	std::vector<std::vector <double> > m_P; //Probas

public:
	ModelLanguage();
	virtual ~ModelLanguage();
};

#endif /* MODELLANGUAGE_H_ */
