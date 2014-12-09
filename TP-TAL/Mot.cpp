/*
 * Mot.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#include "Mot.h"



Mot::Mot() {
	// TODO Auto-generated constructor stub
	m_caracteres = std::string();
	m_id = 0;
}

Mot::Mot(std::string w, int id){
	m_caracteres = w;
	m_id = id;
}

Mot::~Mot() {
	// TODO Auto-generated destructor stub
}

