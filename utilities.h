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
			os << "Meaning: " << s.getMeaning() << " " << s.to_string() << std::endl;
			return os;
		}

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
		
		std::string getReturnType() { return returnType; }
		std::vector<Field> getParameters() { return parameters; }

		std::string to_string() {
			return "Abstraction: return type: " + returnType + " Params number: " + std::to_string(parameters.size());
		}

		bool compare(Symbol *s) {
			AbstractionSymbol *sym = dynamic_cast<AbstractionSymbol*>(s);
			return sym != nullptr && returnType == sym->getReturnType() && parameters == sym->getParameters();
		}
};

class VariableSymbol : public Symbol {
	private:
		std::string varType;
		bool isConst;
		
	public:	
		VariableSymbol(std::string t, bool c) : Symbol("variable"), varType(t), isConst(c) {}
		std::string getVarType() { return varType; }

		std::string to_string() const {
			return "Variable: type: " + varType;
		}

		bool getIsConst() { return isConst; }

		bool compare(Symbol *v) {
			VariableSymbol *var = dynamic_cast<VariableSymbol*>(v);
			return var != nullptr && varType == var->getVarType() && isConst == var->getIsConst(); 
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
			if(this->name == other->getName()) {
				return true;
			} else {
				return false;
			}
		}

		std::string getName() const { return name; }

		std::string to_string() const {
			return "Type declaration " + getName();
		}
};

class PrimitiveType : public Type {
	public:
		PrimitiveType(std::string n) : Type(n) {}
		bool compareType(Type *o) {
			PrimitiveType *other = dynamic_cast<PrimitiveType*>(o);
			return other != nullptr && this->name == other->getName();
		}
};

class VectorType : public Type {
	private:
		std::string fieldType;
		int size;
		
	public:
		VectorType() : Type("vector") {}
		VectorType(std::string t, int s) : Type("vector"), fieldType(t), size(s) {}

		bool compareType(Type *o) {
			VectorType *other = dynamic_cast<VectorType*>(o);
			return other != nullptr && size == other->getSize() && fieldType == other->getFieldType();
		}

		std::string getFieldType() { return fieldType; }
		int getSize() { return size; }
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