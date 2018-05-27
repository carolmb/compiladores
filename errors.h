#ifndef __ERRORS__
#define __ERRORS__
void yyerror (char *s); 
void yyerrorDuplicateIdentifier (std::string sym_name);
void yyerrorSize (int expSize, int currentSize);
void yyerrorType (std::string expType, std::string currenType);
void yyerrorUnknownType(std::string label);
void yyerrorInvalidType(std::string label);
void yyerrorUnknownVar(std::string label);
#endif