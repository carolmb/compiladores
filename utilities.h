#ifndef _UTILITIES_
#define _UTILITIES_

#include <vector>
#include <string>
#include <iostream>


// Var ----------------------------------------------

struct TypeValue {
	std::string nameType;
	int value;
	TypeValue(std::string n, int v) : nameType(n), value(v) {}	
	TypeValue(std::string n) : nameType(n) {}	
};


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

		std::string getFieldName() const { return fieldName; }
		std::string getTypeField() const { return typeField; }
		bool isConstant() { return isConst; }
		bool operator ==(const Field &b) const {
			return fieldName == b.getFieldName() && typeField == b.getTypeField();
		}
};

// Symbol -------------------------------------------

class Symbol {
	private:
		std::string meaning;
		
	public:
		Symbol(){}
		Symbol(std::string m) : meaning(m) {}
		
		std::string getMeaning() const { return meaning; }
		virtual ~Symbol() = default;

		virtual std::string to_string() const = 0;

		friend std::ostream &operator<<(std::ostream &os, Symbol const &s) {
			os << "[Symbol] " << s.to_string();
			return os;
		}

		virtual std::string getType() const = 0; 
		virtual bool compare(Symbol *sym) = 0;
};

class AbstractionSymbol : public Symbol {
	private:
		std::string returnType;
		std::vector<Field> parameters;

	public:
		AbstractionSymbol() : Symbol("abstraction") {}

		AbstractionSymbol(std::string rt, std::vector<Field> p) 
			: Symbol("abstraction"), returnType(rt), parameters(p) {}
		
		std::string getType() const { return returnType; }
		std::vector<Field> getParameters() { return parameters; }

		std::string to_string() const {
			std::string toString = "(Abstraction) [return]: " + returnType + " [params]: ";
			for(auto it = parameters.begin(); it != parameters.end(); it++) {
				toString += it->getTypeField() + " ";
			}
			return toString;
		}

		bool compare(Symbol *s) {
			AbstractionSymbol *sym = dynamic_cast<AbstractionSymbol*>(s);
			return sym != nullptr && returnType == sym->getType() && parameters == sym->getParameters();
		}
};

class VariableSymbol : public Symbol {
	private:
		std::string varType;
		bool isConst;
		
	public:	
		VariableSymbol(std::string t, bool c) : Symbol("variable"), varType(t), isConst(c) {}
		std::string getType() const { return varType; }

		std::string to_string() const {
			return "(Variable) type: " + varType;
		}

		bool getIsConst() { return isConst; }

		bool compare(Symbol *v) {
			VariableSymbol *var = dynamic_cast<VariableSymbol*>(v);
			return var != nullptr && varType == var->getType() && isConst == var->getIsConst(); 
		}
};

// Type ---------------------------------------------

class Type : public Symbol {
	protected: 
		std::string name;
		
	public:
		Type(std::string n) : Symbol("type"), name(n) {}
	
		bool compare(Symbol *sym) {
			Type *type = dynamic_cast<Type*>(sym);
			if (type != nullptr)
				return compareType(type);
			return false;
		}
	
		bool compareType(Type *other) {
			if(this->name == other->getType()) {
				return true;
			} else {
				return false;
			}
		}

		std::string getType() const { return name; }

		std::string to_string() const {
			return "(Type declaration) nameType: " + getType();
		}
};

class PrimitiveType : public Type {
	public:
		PrimitiveType(std::string n) : Type(n) {}
		bool compareType(Type *o) {
			PrimitiveType *other = dynamic_cast<PrimitiveType*>(o);
			return other != nullptr && this->name == other->getType();
		}
};

class VectorType : public Type {
	private:
		std::string fieldType;
		int dim;
		
	public:
		VectorType() : Type("vector") {}
		VectorType(std::string t, int d) : Type("vector"), fieldType(t), dim(d) {
			std::cout << "";
		}

		bool compare(Symbol *sym) {
			VectorType *type = dynamic_cast<VectorType*>(sym);
			if (type != nullptr)
				return compareType(type);
			return false;
		}
		
		bool compareType(Type *o) {
			VectorType *other = dynamic_cast<VectorType*>(o);
			return other != nullptr && dim == other->getDim() && fieldType == other->getFieldType();
		}

		std::string getFieldType() { return fieldType; } 

		int getDim() { return dim; }
};

class UserType : public Type {
	private: 
		std::vector<Field> fields;
		bool vec;
	public:
		UserType() : Type("userType") {} 
		UserType(std::string n, std::vector<Field> f) : Type(n), fields(f), vec(false) {}
		UserType(std::string n, std::vector<Field> f, bool v) : Type(n), fields(f), vec(v) {}
		std::string getFieldType(std::string label) { 
			for(auto it = fields.begin(); it != fields.end(); it++) {
				if(it->getFieldName() == label) {
					return it->getTypeField();
				}
			}
			return ""; 
		}
		
		bool isVec() { return vec; }
		std::string getFieldType() { if(isVec()) { return fields[0].getTypeField(); } return ""; }
};

class EnumType : public Type {
	private:
		std::vector<std::string> fieldNames;
	
	public:
		EnumType(std::string n, std::vector<std::string> fs) : Type(n), fieldNames(fs) {}
		std::vector<std::string> getFieldNames() { return fieldNames; }
		std::string getFieldType(std::string label) {
			for(auto it = fieldNames.begin(); it != fieldNames.end(); it++) {
				if(*it == label) {
					return getType();
				}
			}
			return "";
		}
};

class RangeType : public Type {
	private:
		std::string father; // inteiro ou EnumType
		// std::string begin;
		// std::string end;
		
	public:
		RangeType(std::string n, std::string f) : Type(n), father(f) {}
};

#endif