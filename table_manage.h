#ifndef __TABLE_MANAGE__
#define __TABLE_MANAGE__

#include "utilities.h"
#include "errors.h"
#include<map>
#include<vector>
#include<string> 

struct Scope {
	//Scope* parent
	std::map<std::string, Symbol*> symbolsTable;
	Scope(std::map<std::string, Symbol*> st) : symbolsTable(st) {}
	Scope(){}
};

std::vector<Scope> scopesTable; 
int vIndex = 0;

std::string getTypeByPath(std::vector<std::string> path);
std::string getTypeByPath(Symbol *currentType, int index, std::vector<std::string> path);

std::vector<EnumType*> getEnums() {
	std::vector<EnumType*> enums;

	//int currentScope = scopesTable.size()-1;
	//std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
			EnumType *enumType = dynamic_cast<EnumType*>(itSym->second);
			if(enumType != nullptr)
				enums.push_back(enumType);
		}
	}

	return enums;
}

Symbol* searchElementInCurrentTableByLabel(std::string label) {
    int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
		if(itSym->first == label){
			return itSym->second;
		}
	}
	return nullptr;
}

/*Veirificar na pilha de tabela de símbolos se o nome da variável foi declarado*/
Symbol* searchElementInTableByLabel(std::string label) {
	/*printTable();*/
	/*TODO: recursive in scopes and verify labels in enums*/
	
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
			if(itSym->first == label){
				return itSym->second;
			}
		}
	}

	std::vector<EnumType*> enums = getEnums();
	for(auto it = enums.begin(); it != enums.end(); it++) {
		std::vector<std::string> fields = (*it)->getFieldNames();
		for(auto field = fields.begin(); field != fields.end(); field++) {
			if(*field == label) {
			Symbol *sym = *it;
				return sym;
			}
		}
	}

	return nullptr;
}

/*Veirificar na pilha de tabela de símbolos se o símbolo foi utiutilizizadda*/
std::string searchElementInTableBySymbol(Symbol *symbol) {
	//int currentScope = scopesTable.size()-1;
	//std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
			if(itSym->second->compare(symbol)){
				return itSym->first;
			}
		}
	}
	return "";
}

void addSymbol(std::string label, Symbol *sym) {
	Symbol* element = searchElementInCurrentTableByLabel(label);

	if(element != nullptr) {
		yyerrorDuplicateIdentifier(label);
	}

	int currentScope = scopesTable.size()-1;
	std::cout << "Scope: " << currentScope << std::endl;

	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	symbolsTable[label] = sym;
	scopesTable[currentScope].symbolsTable = symbolsTable;
	
	std::cout << "Symbol added called '" << label << "' (" << *sym << ")" << std::endl;
}

void addVarDecList(std::pair<std::vector<std::string>, std::string> vardeclist, bool isConst) { 
	std::vector<std::string> idlist = vardeclist.first;
	
	std::string typeName = vardeclist.second;
	Symbol *var = new VariableSymbol(typeName, isConst);
	for(auto id = idlist.begin(); id != idlist.end(); id++) {
		addSymbol(*id, var); /* if there is some repetition the addSymbol will emit error */
	}
}

std::string addVec(int size, std::string expectedType, std::vector<std::string> *initTypes) {
	if (initTypes != nullptr) {
		if (size != initTypes->size()) {
			yyerrorSize(size, initTypes->size());
		}
		for(auto it = initTypes->begin(); it != initTypes->end(); it++) {
			if(*it != expectedType) {
				yyerrorType (expectedType, *it);
			}
		}
	}

	VectorType *vec = new VectorType(expectedType, size); 
	std::string label = searchElementInTableBySymbol(vec);
	if (label != "") {
		return label;
	} else {
		std::string vecName = "-v" + std::to_string(vIndex);
		vIndex++;
		addSymbol(vecName, vec);
		return vecName;
	}
}

int rangeSize(TypeValue *first, TypeValue *second) {
	int value = 0;
	if(first->nameType == "int" && second->nameType == "int") {
		if (second->value > first->value) {
			return second->value - first->value;
		}
	} else {
		/* incompatible type */
		std::string currentType = first->nameType;
		if(currentType == "int")
			currentType = second->nameType;
		yyerrorType ("int", currentType);
	}
	return 0;
}

void addUserType(std::string label, std::string name) {
	Field field(name, name);
	std::vector<Field> fields;
	fields.push_back(field);
	UserType *userType = new UserType(label, fields);
	addSymbol(label, userType);
}

std::string verifyUsertype(std::string label) {
	Symbol* sym = searchElementInTableByLabel(label);

	if (sym == nullptr) 
		yyerrorUnknownType(label);

	UserType *userType = dynamic_cast<UserType*>(sym);

	if (userType == nullptr) 
		yyerrorUnknownType(label);

	return label;
}

void insertVarDec(std::vector<Field> *fields, std::pair<std::vector<std::string>, std::string> *varDec) {
	/* get usertype fields to create UserType */
	std::vector<std::string> idlist = varDec->first;
	std::string type = varDec->second;
	
	/* identify if user used the same label more than one time */
	for(auto it1 = idlist.begin(); it1 != idlist.end(); it1++) {
		for(auto it2 = std::next(it1); it2 != idlist.end(); it2++) {
			if (*it1 == *it2) 
				yyerrorDuplicateIdentifier(*it1);
		}
	}

	/* identify if label is already used in previously usertype fields defined */
	for(auto it1 = idlist.begin(); it1 != idlist.end(); it1++) {
		for(auto it2 = fields->begin(); it2 != fields->end(); it2++) {
			if(*it1 == it2->getFieldName())
				yyerrorDuplicateIdentifier(*it1);
		}
	}

	for(auto it = idlist.begin(); it != idlist.end(); it++) {
		Field f(*it, type);
		fields->insert(fields->begin(), f);
	}
}

void addUserType(std::string label, std::vector<Field> fields) {
	UserType *userType = new UserType(label, fields);
	addSymbol(label, userType);
}

bool isEnumType(std::string t) {
	std::vector<EnumType*> enums = getEnums();
	for(auto e = enums.begin(); e != enums.end(); e++) {
		std::vector<std::string> fields = (*e)->getFieldNames();
		for(auto f = fields.begin(); f != fields.end(); f++) {
			if(*f == t)
				return true;
		}
	}
	return false;
}

void addUserType(std::string label, std::string t1, std::string t2) {
	if(t2 == "") {
		NamedType *namedType = new NamedType(label, t1);
	}

	if(t1 != t2) {
		/* incompatible type */
		yyerrorType (t1, t2);
	}

	if(t1 == "int" || isEnumType(t1)) {
		RangeType *range = new RangeType(label, t1);
		addSymbol(label, range);
	} else {
		yyerrorInvalidType(t1);
	}
}

/*Adiciona tipo de usuário na tabela de símbolos*/
void addUserType(std::string label, std::vector<std::string> idlist) {
	/* Enum */
	for(auto it = idlist.begin(); it != idlist.end(); it++) {
		Symbol* sym = searchElementInCurrentTableByLabel(*it);
		if (sym != nullptr) 
			yyerrorDuplicateIdentifier(*it);
	}
	EnumType *enumType = new EnumType(label, idlist);
	addSymbol(label, enumType);
}

std::string getTypeByPath(std::vector<std::string> path) {
	// for(auto it = path.begin(); it != path.end(); it++) {
	// 	std::cout << *it << " ";
	// } 
	// std::cout << std::endl;

	Symbol *sym = searchElementInTableByLabel(path[0]);
	if(sym == nullptr) {
		yyerrorUnknownLabel(path[0]);
	}
	
	return getTypeByPath(sym, 1, path);
}

std::string getTypeByPath(Symbol *currentSymbol, int index, std::vector<std::string> path) {
	/* TODO: when it is a vector acess or abstraction call */
	if(index >= path.size()) {
		return currentSymbol->getType();
	}

	/*First try if the current step is a field in user type */
	Type *type = dynamic_cast<Type*>(currentSymbol);
	if(type != nullptr) {
		std::string fieldType = type->getFieldType(path[index]);
		if(fieldType != "") {
			Symbol *type = searchElementInTableByLabel(fieldType);
			return getTypeByPath(type, index + 1, path);
		} else {
			yyerrorUnknownLabel(path[index]);
		}
	}

	/* Try abstraction call, in this case, the next type is the returned type */
	AbstractionSymbol *abs = dynamic_cast<AbstractionSymbol*>(currentSymbol);
	if(abs != nullptr) {
		int step = std::stoi(path[index]);
		std::vector<std::string> args;
		for(int i = index+1; i < index + 1 + step; i++) {
			args.push_back(path[i]);
		}
		std::vector<Field> params = abs->getParameters();
		if(args.size() == params.size()) {
			for(int i = 0; i < args.size(); i++) {
		    	if(args[i] != params[i].getTypeField()) {
		        	yyerrorType(params[i].getTypeField(), args[i]);
		    	} 
			}
			std::string returnType = abs->getType();
			if(returnType == "") {
				yyerrorUnknownLabel("\"\"[void]");
			}
			Symbol *type = searchElementInTableByLabel(returnType);
			return getTypeByPath(type, index + 1 + step, path);
		} else {
		   yyerrorInvalidArgs(path[index-1]);
		}
	}
}

/*Adiciona parametros de uma função como novos símbolos na tabela de símbolos*/
void addParams(std::string absLabel, std::vector<Field> params) {
	for(auto f = params.begin(); f != params.end(); f++) {
		if(absLabel == f->getFieldName()) {
			yyerrorDuplicateIdentifier(absLabel);
		}
		Symbol *var = new VariableSymbol(f->getTypeField(), f->isConstant());
		addSymbol(f->getFieldName(), var);
	}
}

#endif