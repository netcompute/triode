%{
#include <stdio.h>
#include <string>
#include "node.h"
#include "parser.hh"
#define SAVE_TOKEN yylval.string = new std::string(yytext)
#define TOKEN(t) ( yylval.token = t)
%}

%x C_COMMENT

%option noyywrap

%%

"\/\/".*		;
[ \t\n]			;



"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }

"if"			return TOKEN(TIF);
"else"			return TOKEN(TELSE);
"return"		return TOKEN(TRETURN);
"struct"		return TOKEN(TSTRUCT);
"int"           SAVE_TOKEN; return TYINT;

[a-zA-Z_][a-zA-Z0-9_]*	                        SAVE_TOKEN; return TIDENTIFIER;
[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}		SAVE_TOKEN; return TIP;
.{2}:.{2}:.{2}:.{2}:.{2}:.{2}	                    SAVE_TOKEN; return TMAC;
[0-9]+		                                        SAVE_TOKEN; return TINTEGER;

"("			return TOKEN(TLPAREN);
")"			return TOKEN(TRPAREN);
"{"			return TOKEN(TLBRACE);
"}"			return TOKEN(TRBRACE);
","			return TOKEN(TCOMMA);
"="			return TOKEN(TEQUAL);
"=="		return TOKEN(TCEQ);
"!="		return TOKEN(TCNE);
"<"			return TOKEN(TCLT);
"<="		return TOKEN(TCLE);
">"			return TOKEN(TCGT);
">="		return TOKEN(TCGE);
"+"			return TOKEN(TPLUS);
"-"			return TOKEN(TMINUS);
"*"			return TOKEN(TMUL);
"/"			return TOKEN(TDIV);
"&"			return TOKEN(TAND);
"|"			return TOKEN(TOR);
"^"			return TOKEN(TXOR);
"%"			return TOKEN(TMOD);
">>"		return TOKEN(TSHIFTR);
"<<"		return TOKEN(TSHIFTL);
"."         return TOKEN(TDOT);

