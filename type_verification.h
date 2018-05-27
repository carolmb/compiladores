#ifndef __TYPE_VERIFICATION__
#define __TYPE_VERIFICATION__


std::string* verifyArithmeticExprType(std::string t1, std::string t2) {
	if (t2 == "") {
		return new std::string(t1);
	}

	if (t1 == t2) {
		if (t1 == "int" || t1 == "real" || t1 == "bool") {
			return new std::string(t1);
		}
	}

	if (t1 == "int" && t2 == "real") {
		return new std::string(t2);
	}

	yyerrorInvalidType(t1); /* TODO: verify if this msg makes sense */
}

std::string* verifyRelationalExprType(std::string t1, std::string t2) {
	if (t2 == "") {
		return new std::string(t1);
	}

	if (t1 == t2) {
		return new std::string("bool");
	}

	if ((t1 == "int" && t2 == "real") || (t2 == "int" && t1 == "real")) {
		return new std::string("bool");
	}

	yyerrorInvalidType(t1); /* TODO: verify if this msg makes sense */
}

std::string* verifyLogicExprType(std::string t1, std::string t2) {
	if (t2 == "") {
		return new std::string(t1);
	}

	if (t1 == t2 && t1 == "bool") {
		return new std::string("bool");
	}

	yyerrorInvalidType(t1); /* TODO: verify if this msg makes sense */	
}

#endif