#include <bits/stdc++.h>
#include "ast.h"
using namespace std;

Expression* grammar_1(string lex,Expression* e1,bool isprimary,bool isliteral);   //grammar that has only one 
Expression* cond_qn_co(string lex,Expression* e1,Expression* e2,Expression* e3); //conditional_or_expression QN_OP expression COLON_OP condtional_expression
Expression* evalOR_AND(string lex,Expression* e1,string op,Expression* e2);
Expression* evalBITWISE(string lex,Expression* e1,string op,Expression* e2);
Expression* evalEN(string lex,Expression* e1,string op,Expression* e2);
Expression* evalRELATIONAL(string lex,Expression* e1,Expression* e2);
Expression* evalSHIFT(string lex,Expression* e1,string op,Expression* e2);
Expression* evalARITHMETIC(string lex,Expression* e1,string op,Expression* e2);
Expression* evalUNARY(string lex,string op,Expression* e1);
Expression* evalIC_DC(string lex,string op,Expression* e1);
Expression* evalTL(string lex,Expression* e1);
Expression* evalEX(string lex,Expression* e1);

