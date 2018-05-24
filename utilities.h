#ifndef _UTILITIES_
#define _UTILITIES_

#include <vector>
#include <string>
#include <iostream>



// Field --------------------------------------------
	
class Field {
	private:
		std::string fieldName;
		std::string typeField;
		bool isConst;
	
	public:
		Field() {}
		Field(std::string n, std::string t) : fieldName(n), typeField(t), isConst(false) {}
		Field(std::string n, std::string t, bool c) : fieldName(n), typeField(t), isConst(c) {}
};

// Symbol -------------------------------------------

class Symbol {
	private:
		std::string meaning;
		
	public:
		Symbol(){}
		Symbol(std::string m) : meaning(m) {}
		
		bool compare(Symbol other) {
			if(this->meaning == other.get_meaning()){
				return true;
			}	
			return false;
		}
		
		std::string get_meaning() { return meaning; }
		virtual ~Symbol() = default;
};

class AbstractionSymbol : public Symbol {
	private:
		std::string returnType;
		std::vector<Field> parameters;

	public:
		AbstractionSymbol() : Symbol("abstraction") {}

		AbstractionSymbol(std::string rt, std::vector<Field> p) 
			: Symbol("abstraction"), returnType(rt), parameters(p) {}
		
		std::string get_returnType() { return returnType; }
		std::vector<Field> get_parameters() { return parameters; }
};

class VariableSymbol : public Symbol {
	private:
		std::string varType;
		bool isConst;
		
	public:	
		VariableSymbol(std::string t, bool c) : Symbol("variable"), varType(t), isConst(c) {}
		std::string get_varType() { return varType; }
};

// Type ---------------------------------------------

class Type : public Symbol {
	protected: 
		std::string name;
		
	public:
		Type(std::string n) : Symbol("type"), name(n) {}
	
		bool compareType(Type other) {
			if(this->name == other.get_name()) {
				return true;
			} else {
				return false;
			}
		}
		std::string get_name() { return name; }
};

class PrimitiveType : public Type {
	public:
		PrimitiveType(std::string n) : Type(n) {}
		
		bool compareType(Type other) {
			if(this->name == other.get_name()) {
				return true; // ver cast 
			}
			return false;
		}
};

class VectorType : public Type {
	private:
		std::string fieldType;
		int size;
		
	public:
		VectorType() : Type("vector") {}
		VectorType(std::string t, int s) : Type("vector"), fieldType(t), size() {}
};

class UserType : public Type {
	private: 
		std::vector<Field> fields;

	public:
		UserType() : Type("temp") {} 
		UserType(std::string n, std::vector<Field> f) : Type(n), fields(f) {}
};

class EnumType : public Type {
	private:
		std::vector<std::string> fieldNames;
	
	public:
		EnumType(std::string n, std::vector<std::string> fs) : Type(n), fieldNames(fs) {}
};

class RangeType : public Type {
	private:
		std::string father; // inteiro ou EnumType
		std::string begin;
		std::string end;
		
	public:
		RangeType() : Type("rangeTemp") {}
		RangeType(std::string n, std::string f, std::string b, std::string e) : 
			Type(n), father(f), begin(b), end(e) {}
};

#endif