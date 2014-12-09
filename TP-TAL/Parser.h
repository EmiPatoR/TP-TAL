/*
 * Parser.h
 *
 *  Created on: Dec 9, 2014
 *      Author: emip
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <iostream>
#include <fstream>

class Parser {
protected:
	std::string m_filename;
	std::ifstream* m_inputStream;
public:
	Parser();
	Parser(std::string filename);
	virtual ~Parser();
};

#endif /* PARSER_H_ */
