/*
 * main.cpp
 *
 *  Created on: Fall 2016
 *      Author: wcs
 *
 *  Copyright (c) 2016 School of Engineering Science, Simon Fraser University
 */

#include <iostream>
#include <sstream>

#include "tokenClassifier.hpp"

using namespace std;

#ifndef DONT_RUN_MAIN
int main() {
	int rv = -1;
	//istringstream input("float n ; char * b ; n ++ ; n = 3.20e-2 / ( 2 + ~ 8f ) ; b = \"String\"; n >> 5 ; n = exp ? exp ? 5.001 : 2 : .3 ;");
	istringstream input("errno=5;");
	ensc251::tokenObjectPs = tokenClassifier(input);

	ensc251::Token *ast_rootP = ensc251::recursive_parser();
	if(ast_rootP) {
		ast_rootP->print_ast();
		rv = 0;
	}
	else
	{
		OUT << "Syntax error in input. Please try again..." << endl;
		rv = 1;
	}

#ifdef USE_OSS
	std::string s = oss.str();
	cout << s << endl;
#endif

	return rv;
}
#endif
