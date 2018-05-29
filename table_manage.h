#ifndef __TABLE_MANAGE__
#define __TABLE_MANAGE__

#include "utilities.h"
#include "errors.h"
#include<map>
#include<vector>
#include<string> 

struct Scope {
	//Scope* parent
	std::string returnType; 
	std::map<std::string, Symbol*> symbolsTable;
	Scope(std::map<std::string, Symbol*> st) : symbolsTable(st) {}
	Scope(){}
};

std::vector<Scope> scopesTable; 
int vIndex;

std::string getTypeByPath(std::vector<std::string> path);
std::string getTypeByPath(Symbol *currentType, int index, std::vector<std::string> path);

void printTable();

template <class T> std::vector<T*> getByType() {
	std::vector<T*> enums;
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		for(auto itSym = symbolsTable.begin(); itSym != symbolsTable.end(); itSym++) {
			T *enumType = dynamic_cast<T*>(itSym->second);
			if(enumType != nullptr)
				enums.push_back(enumType);
		}
	}

	return enums;
}

Symbol* searchElementInCurrentTableByLabel(std::string label) {
    int currentScope = scopesTable.size()-1;
	std::map<std::string, Symbol*> symbolsTable = scopesTable[currentScope].symbolsTable;
	Symbol *sym = symbolsTable[label];
	if(sym != nullptr) {
		return sym;
	}
	return nullptr;
}

/* Veirificar na pilha de tabela de símbolos se o nome da variável foi declarado */
Symbol* searchElementInTableByLabel(std::string label) {
	
	std::vector<Scope>::reverse_iterator scope = scopesTable.rbegin();
	
	for (; scope != scopesTable.rend(); ++scope) {
		std::map<std::string, Symbol*> symbolsTable = scope->symbolsTable;
		Symbol *sym = symbolsTable[label];
		if(sym != nullptr) {
			return sym;
		}
	}

	std::vector<EnumType*> enums = getByType<EnumType>();
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

std::string addVec(int dim, std::string expectedType, std::vector<std::string> *initTypes) {
	if (initTypes != nullptr) {
		for(auto it = initTypes->begin(); it != initTypes->end(); it++) {
			if(*it != expectedType) {
				yyerrorType (expectedType, *it);
			}
		}
	}

	std::string vecName = "-v" + std::to_string(vIndex);
	vIndex++;
	
	VectorType *vec = new VectorType(vecName, expectedType, dim); 
	std::string label = searchElementInTableBySymbol(vec);
	if (label != "") {
		vIndex--;
		return label;
	} else {
		addSymbol(vecName, vec);
		return vecName;
	}
}

int rangeSize(TypeValue *first, TypeValue *second) {
	int value = 0;
	if(first->nameType == "int" && second->nameType == "int") {
		if (second->value > first->value) {
			return 1;
		}
	}
	/* incompatible type */
	std::string currentType = first->nameType;
	if(currentType == "int")
		currentType = second->nameType;
	yyerrorType ("int", currentType);
}

void addUserType(std::string label, std::string name) {
	Field field(name, name);
	std::vector<Field> fields;
	fields.push_back(field);
	UserType *userType = new UserType(new std::string(label), fields, true);
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
	UserType *userType = new UserType(new std::string(label), fields);
	addSymbol(label, userType);
}

template<class T> bool isType(std::string t) {
	std::vector<T*> enums = getByType<T>();
	for(auto e = enums.begin(); e != enums.end(); e++) {
		if((*e)->getType() == t) {
			return true;
		}
	}
	return false;
}

void addUserType(std::string label, std::string t1, std::string t2) {
	if(t2 == "") {
		RangeType *range = new RangeType(new std::string(label), t1);
	}

	if(t1 != t2) {
		/* incompatible type */
		yyerrorType (t1, t2);
	}
	
	if(t1 == "int" || isType<EnumType>(t1)) {
		RangeType *range = new RangeType(new std::string(label), t1);
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
	EnumType *enumType = new EnumType(new std::string(label), idlist);
	addSymbol(label, enumType);
}

std::string getTypeByPath(std::vector<std::string> path) {
	// std::cout << "path: ";
	// for(auto it = path.begin(); it != path.end(); it++) {
	// 	std::cout << *it << " ";
	// } 
	// std::cout << std::endl;
	
	Symbol *sym = searchElementInTableByLabel(path[0]);
	if(sym == nullptr) {
		yyerrorUnknownLabel(path[0]);
	}
	
	VariableSymbol *varDec = dynamic_cast<VariableSymbol*>(sym);
	if(varDec != nullptr) {
		Symbol *type = searchElementInTableByLabel(varDec->getType());
		return getTypeByPath(type, 1, path);
	}
	
	AbstractionSymbol *abs = dynamic_cast<AbstractionSymbol*>(sym);
	if(abs != nullptr) {
		int index = 1;
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
			
			Symbol *type = searchElementInTableByLabel(returnType);
			return getTypeByPath(type, index + 1 + step, path);
		} else {
		   yyerrorInvalidArgs(path[index-1]);
		}
	}
	
	if(isType<EnumType>(sym->getType())) {
		Symbol *type = searchElementInTableByLabel(sym->getType());
		return getTypeByPath(type, 1, path);
	}
	
	std::cout << "Ops... Esse caso não foi tratado (ver table_manage.h linha 304)" << std::endl;	
	yyerrorUnknownLabel(path[0]);
}

std::string getTypeByPath(Symbol *currentSymbol, int index, std::vector<std::string> path) {
	/* TODO: when it is a vector acess or abstraction call */
	// std::cout << "index: " << index << " " << *currentSymbol << " size: " << path.size() << std::endl;
	
	if(index >= path.size()) {
		if (currentSymbol == nullptr) {
			return "";
		}
		return currentSymbol->getType();
	} 
	
	/*First try if the current step is a field in user type */
	UserType *userType = dynamic_cast<UserType*>(searchElementInTableByLabel(currentSymbol->getType()));
	if(userType != nullptr) {
		if(userType->isVec()) {
			Symbol *vec = searchElementInTableByLabel(userType->getFieldType());
			return getTypeByPath(vec, index, path);
		}
		std::string fieldType = userType->getFieldType(path[index]);
		if(fieldType != "") {
			Symbol *userType = searchElementInTableByLabel(fieldType);
			return getTypeByPath(userType, index + 1, path);
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
			
			Symbol *type = searchElementInTableByLabel(returnType);
			return getTypeByPath(type, index + 1 + step, path);
		} else {
		   yyerrorInvalidArgs(path[index-1]);
		}
	}

	/* vector access */
	VectorType *vec = dynamic_cast<VectorType*>(currentSymbol);
	if(vec != nullptr) {
		int step = std::stoi(path[index]);
		// std::cout << "step: " << step << " args: ";
		std::vector<std::string> args;
		for(int i = index+1; i < index + 1 + step; i++) {
			// std::cout << path[i] << " ";
			args.push_back(path[i]);
		}
		int dim = vec->getDim();
		if(dim == step) {
			// std::cout << "dim == step" << std::endl;
			for(int i = 0; i < args.size(); i++) {
		    	if(args[i] != "int") {
		        	yyerrorType("int", args[i]);
		    	} 
			}
			std::string returnType = vec->getFieldType();
			Symbol *type = searchElementInTableByLabel(returnType);
			// std::cout << *type << std::endl;
			return getTypeByPath(type, index + step + 1, path);
		} else {
		   yyerrorInvalidArgs(path[index-1]);
		}
	}
	
	std::cout << "Ops... Esse caso não foi tratado (ver table_manage.h linha 388)" << std::endl;	
	yyerrorUnknownLabel(path[0]);
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