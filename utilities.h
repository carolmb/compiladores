#ifndef _UTILITIES_
#define _UTILITIES_

#include <vector>
#include <string>
#include <iostream>



// Field --------------------------------------------
	
class Field {
	std::string fieldName;
	std::string typeField;
	bool isConst;
	Field() {}
	Field(std::string n, std::string t) : fieldName(n), typeField(t), isConst(false) {}
	Field(std::string n, std::string t, bool c) : fieldName(n), typeField(t), isConst(c) {}
};

// Symbol -------------------------------------------

class Symbol {
	public:
		std::string meaning;
		
		Symbol(){}
		Symbol(std::string m) : meaning(m) {}
		
		bool compare(Symbol other) {
			if(this->meaning == other.get_meaning()){
				return true;
			}	
			return false;
		}
		
		std::string get_meaning() { return meaning; }
};

class AbstractionSymbol : public Symbol {
	public:
		AbstractionSymbol() { 
			this->meaning = "abstraction";
		}
		AbstractionSymbol(std::string rt, std::vector<Field> p) { 
			this->meaning = "abstraction";
			this->returnType = rt;
			this->parameters = p;
		}
		std::string returnType;
		std::vector<Field> parameters;
		
		std::string get_returnType() { return returnType; }
		std::vector<Field> get_parameters() { return parameters; }
};

class VariableSymbol : public Symbol {
	public:	
		VariableSymbol(std::string t, bool c) : Symbol("variable"), varType(t), isConst(c) {}
		std::string varType;
		bool isConst;
		std::string get_varType() { return varType; }
};

// Type ---------------------------------------------

class Type : public Symbol {
	public:
		std::string name;
		Type(std::string n) : Symbol("type"), name(n) {}
		
		bool compareType(Type other) {
			if(name == other.name) {
				return true;
			} else {
				return false;
			}
		}
};

class PrimitiveType : public Type {
	public:
		PrimitiveType(std::string n) : Type(n) {}
		
		bool compareType(Type other) {
			if(name == other.name) {
				return true; // ver cast 
			}
			return false;
		}
};

class VectorType : public Type {
	public:
		std::string fieldType;
		int size;
		
		VectorType() : Type("vector") {}
		VectorType(Type fieldType) : Type("vector"), fieldType(fieldType) {}
		
		bool compareType(Type other) {
			if(fieldType == other.fieldType && size == other.size) {
				return true;
			}
			return false;
		}
};

class UserType : public Type {
	public: 
		std::vector<Field> fields;
		
		UserType() {}
		UserType(std::string n, std::vector<Field> f) : Type(n), fields(f) {}
};

class EnumType : public Type {
	public:
		std::vector<std::string> fieldNames;
		EnumType(std::vector fs) : fieldNames(fs) {}
};

class RangeType : public Type {
	public:
		std::string father; // inteiro ou EnumType
		std::string begin;
		std::string end;
		
		RangeType() : {}
		RangeType(std::string n, std::string f, std::string b, std::string e) : 
			Type(n), father(f), begin(b), end(e) {}
};

#endif