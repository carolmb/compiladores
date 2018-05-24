#ifndef _UTILITIES_
#define _UTILITIES_

#include <vector>
#include <string>
#include <iostream>

class Field {
	std::string name;
	std::string type;
	
	bool compare(Field o){
		return this->type==o.type;
	}
	
};

class Type {
	public:
		virtual bool compare(Type *type) = 0;
};

class PrimitiveType : public Type {
	public :
		Field field;
	
		bool compare(Type *o) {
			PrimitiveType *other = dynamic_cast<PrimitiveType*>(o);
			if(other != nullptr){
				return this->field.compare(*other);
			}
			
			return false;
		}
};

class VectorType : public UserType {
	struct Range {
		int begin;
		int end;
	};
	
	public:
		std::vector<Range> range;
};

class UserType : public Type {
	public: 
		std::vector<PrimitiveType> primitives;
		std::vector<UserType> users;
		
		bool compare(Type *o) {
			UserType *other = dynamic_cast<UserType*>(o);
			if(other != nullptr){
				for(int i=0;i<primitives.size();i++){
					if (this->primitives[i].compare(other->primitives[i])==false){
						return false;
					} 
				}
				for(int i=0;i<userType.size();i++){
					if (this->userType[i].compare(other->userType[i])==false){
						return false;
					} 
				}
				
			}	
			return true; 
		}
};

#endif