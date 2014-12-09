/*
 * Mot.h
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#ifndef MOT_H_
#define MOT_H_

#include <string>

class Mot {
public:
	Mot();
	Mot(std::string w, int id);
	virtual ~Mot();

	//Getters
	std::string getCaracteres(){return m_caracteres;}
	int getId(){return m_id;}

	//Setters
	void setCaracteres(std::string val){m_caracteres = val;}
	void setId(int val){m_id = val;}

protected:
	std::string m_caracteres;
	int m_id;

};

#endif /* MOT_H_ */
