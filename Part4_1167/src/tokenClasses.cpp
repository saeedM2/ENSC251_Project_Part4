//============================================================================
//
//% Student Name 1: student1
//% Student 1 #: 123456781
//% Student 1 userid (email): stu1 (stu1@sfu.ca)
//
//% Student Name 2: student2
//% Student 2 #: 123456782
//% Student 2 userid (email): stu2 (stu2@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  ___________
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in tokenClasses.hpp  too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Your group name should be "P4_<userid1>_<userid2>" (eg. P4_stu1_stu2)
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Submit files to courses.cs.sfu.ca
//
// Name        : tokenClasses.cpp
// Description : Course Project Part 4 (Parser)
// Original portions Copyright (c) 2016 School of Engineering Science, Simon Fraser University
//============================================================================

#include "tokenClasses.hpp"

// I'm not sure if next line works with other than GNU compiler
#include <cxxabi.h>

namespace ensc251 {

unsigned int tracker = 0; // tracker index
std::vector<Token*> tokenObjectPs; // objects for abstract syntax tree

char *demangle(const char *typeName) {
	int status;
	// I'm not sure if the below line works with compilers other than GNU
	return abi::__cxa_demangle(typeName, 0, 0, &status);
}

// functions for parsing are below
Token* postfix_exp()
{
	int old_tracker = tracker;

	Token* tObjP;

	if(auto subTreeP = tokenObjectPs[tracker]->process_primary_exp())
	{
		tObjP = tokenObjectPs[tracker]->is_postfix_operator();
		subTreeP->add_childP(tObjP);
		return subTreeP;
	}

}

Token* unary_exp()
{
	int old_tracker = tracker;

	Token* tObjP;

	if(!(tObjP = tokenObjectPs[tracker]->process_unary()))
	{
		tracker = old_tracker;
		tObjP = postfix_exp();
	}
	return tObjP;
}
// ***** Add more functions around here somewhere *****
Token* primary_exp()
{

}
Token* div_exp()
{

}
Token* additive_exp()
{

}

Token* shift_exp()
{
	Token* subTreeP = nullptr;
	Token* lowerNodeP = nullptr;
	Token* upperNodeP = nullptr;
	if ((lowerNodeP = unary_exp())) {
		while ((upperNodeP = tokenObjectPs[tracker]->advance_past_shift_operator()))
		{
			if(!(subTreeP = unary_exp()))
				return nullptr;
			upperNodeP->add_childP(lowerNodeP);
			upperNodeP->add_childP(subTreeP);
			lowerNodeP = upperNodeP;
		}
	}
	return lowerNodeP;
}

Token* ternary_exp()
{
	int old_tracker = tracker;
	Token* subTreeP;

	// ***** Complete this function

	tracker = old_tracker;
	subTreeP = shift_exp();
	if(subTreeP)
		return subTreeP;
	else
		return nullptr;
}

Token* assignment_exp()
{
	int old_tracker = tracker;

	// production: id assignment_operator assignment_exp
	Token* subTreeP = tokenObjectPs[tracker]->process_id();
	if(subTreeP)
	{
		if(Token* tObjP = tokenObjectPs[tracker]->advance_past_assignment_operator())
		{
			tObjP->add_childP(subTreeP);
			subTreeP = assignment_exp();
			if(subTreeP)
			{
				tObjP->add_childP(subTreeP);
				return tObjP;
			}
			else
				// we have assignment operator, but not valid assignment expression,
				//	so there must be an error and it should be reported.
				return nullptr;
		}
		// no assignment operator, so this is not the first production

		// start production:  ternary_exp
		if(tokenObjectPs[tracker]->has_string_value(";"))
			// we have already built a unary expression subtree, which satisfies the ternary_exp production.
			return subTreeP;
	}

	// production:  ternary_exp
	tracker = old_tracker;
	subTreeP = ternary_exp();
	if(subTreeP)
		return subTreeP;
	else
		return nullptr;
}

Token* stat()
{
	int old_tracker = tracker;
	if(auto subTreeP = tokenObjectPs[tracker]->process_declaration())
		return subTreeP;
	else
	{
		tracker = old_tracker;
		return assignment_exp();
	}
}

// because the below function deals with ";", it is a bit different than the pattern seen elsewhere.
Token* stat_list()
{
	Token* statTreeP = nullptr;
	Token* lowerNodeP = nullptr;
	Token* upperNodeP = nullptr;
	if ((lowerNodeP = stat())) {

		while (tokenObjectPs[tracker]->has_string_value(";")) {
			tracker++;
			if (tracker == tokenObjectPs.size())
				return lowerNodeP;
			upperNodeP = tokenObjectPs[tracker - 1];

			if(!(statTreeP = stat()))
				return nullptr;

			upperNodeP->add_childP(lowerNodeP);
			upperNodeP->add_childP(statTreeP);
			lowerNodeP = upperNodeP;
		}
		return nullptr;

	}
	return nullptr;
}

Token* recursive_parser()
{
	if (tokenObjectPs.size())
		return stat_list();
	else
		return nullptr;
}

} // namespace ensc251

