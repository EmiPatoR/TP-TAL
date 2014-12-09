/*
 * Parser.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#include <iostream>
#include <string>
#include <fstream>

#include "Parser.h"

Parser::Parser() {
	m_filename = std::string();
	m_inputStream = NULL;
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
}

Parser::~Parser() {
	if(m_inputStream->is_open())
		m_inputStream->close();

	if(m_inputStream != NULL)
		delete m_inputStream;
}

