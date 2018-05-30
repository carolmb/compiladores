#ifndef __TYPE_VERIFICATION__
#define __TYPE_VERIFICATION__

std::string* verifyArithmeticExprType(std::string t1, std::string t2) {

	if ((t1 != t2 || (t1 == "texto" && t2 == "texto")) && (t1 == "texto" || t2 == "texto")){
		return new std::string("texto");
	}

	if (t2 == "") {
		return new std::string(t1);
	}

	if (t1 == t2) {
		if (t1 == "int" || t1 == "real" || t1 == "bool") { //  || t1 == "texto"
			return new std::string(t1);
		}
	}

	if ((t1 == "int" && t2 == "real") || (t2 == "int" && t1 == "real")) {
		return new std::string("real");
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


void verifyTypeAssign(std::string id, std::string t) {
	VariableSymbol *var = dynamic_cast<VariableSymbol*>(searchElementInTableByLabel(id)); 
	if (var == nullptr) { 
		yyerrorUnknownLabel(id); 
	}
	if(var->getType() != t) { 
		yyerrorType(var->getType(), t);
	}
}

void verifyCaseList(std::string t, std::vector<std::string> clauses) {
	if (t != "int" && t != "texto") {
		yyerrorType("inteiro or texto", t);
	}

	for(auto it = clauses.begin(); it != clauses.end(); it++) {
		if(t != *it) {
			yyerrorType(t, *it);
		}
	}
}

void verifyType(std::string t1, std::string t2) {
	std::cout << "verifyType: " << t1 << " " << t2 << std::endl; 
	Symbol *symT1 = searchElementInTableByLabel(t1);
	Symbol *symT2 = searchElementInTableByLabel(t2);
	if (symT1 == nullptr || symT2 == nullptr) {
		std::cout << "Ops... Type not found." << std::endl;
		yyerrorInvalidType(t1 + " or " + t2);
	}

	if (symT1->compare(symT2)) {
		return;
	}

	if (t1 == "real" && t2 == "int") {
		return;	
	}

	yyerrorType(t1, t2);
}

void verifyLabel(std::string id) {
	Symbol *l = searchElementInTableByLabel(id); 
	VariableSymbol *v = dynamic_cast<VariableSymbol*>(l); 
	if (v == nullptr) {
		yyerrorUnknownLabel(id);
	}
	
	if (v->getType() != "rotulo") {
		yyerrorType("rotulo", v->getType());
	}
}

std::string* verifyRangeType(std::string t1, std::string t2) {
	if(t1 != t2) {
		yyerrorType (t1, t2);
	}
	
	if(t1 == "int" || isType<EnumType>(t1)) {
		return new std::string(t1);
	} else {
		yyerrorInvalidType(t1);
	}
}

#endif