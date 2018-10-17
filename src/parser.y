%{
  #include <stdio.h>
  #include "node.h"
  using namespace triode;
  NBlock* programBlock;
  extern FILE *yyin;
  extern int yylex();
  void yyerror(const char* s) {
    fprintf(stderr, "yyerror: %s\n", s);
  }
%}

%error-verbose

%union {
  triode::NBlock* block;
  triode::NStatement* stmt;
  triode::NExpression* expr;
  triode::NIdentifier* ident;
  triode::NVariableDeclaration* var_decl;
  std::vector<triode::NVariableDeclaration*>* varvec;
  std::vector<triode::NExpression*>* exprvec;
  std::string* string;
  int token;
}
%token <string> TIDENTIFIER TINTEGER TIP TMAC TYINT
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TEQUAL TIF TELSE TCEQ TCNE TCLT TCLE TCGT TCGE TPLUS TMINUS TMUL TDIV TRETURN TAND TOR TXOR TMOD TSHIFTR TSHIFTL TSTRUCT TDOT
%type <ident> ident primary_type typename
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl if_stmt struct_decl
%type <expr> expr numeric
%type <varvec> func_decl_args struct_members
%type <exprvec> call_args
%type <token> binary
%start program

%%

program : stmts {programBlock = $1; }
;

stmts : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
| stmts stmt { $1->statements.push_back($<stmt>2); }
;

stmt : var_decl
| struct_decl
| func_decl
| if_stmt
| expr
| TRETURN expr { $$ = new NReturnStatement(*$2); }
;

var_decl : typename ident { $$ = new NVariableDeclaration(*$1, *$2); }
| typename ident TEQUAL expr { $$ = new NVariableDeclaration(*$1, *$2, $4); }
;

typename : primary_type { $$ = $1; }
| TSTRUCT ident { $$ = $2; }
;

primary_type : TYINT { $$ = new NIdentifier(*$1); delete $1; }
;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
;

expr : numeric
| ident { $<ident>$ = $1; }
| TLPAREN expr TRPAREN { $$ = $2; }
| expr binary expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
| ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
| ident TDOT ident { $$ = new NStructMember(*$<ident>1, *$<ident>3); }
| ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
;

numeric : TINTEGER { $$ = new NInteger(atol($1->c_str())); }
;

binary : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE | TPLUS | TMINUS | TMUL | TDIV | TAND | TOR | TXOR | TSHIFTL | TSHIFTR
;

struct_decl : TSTRUCT ident TLBRACE struct_members TRBRACE { $$ = new NStructDeclaration(*$2, *$4); }
;

struct_members : /* blank */ { $$ = new VariableList(); }
| var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
| struct_members var_decl { $1->push_back($<var_decl>2); }
;

call_args : /*blank*/  { $$ = new ExpressionList(); }
| expr { $$ = new ExpressionList(); $$->push_back($1); }
| call_args TCOMMA expr  { $1->push_back($3); }
;

func_decl : ident ident TLPAREN func_decl_args TRPAREN block { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6); }
;

func_decl_args : /*blank*/ { $$ = new VariableList(); }
| var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
| func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
;

block : TLBRACE stmts TRBRACE { $$ = $2; }
| TLBRACE TRBRACE { $$ = new NBlock(); }
;

if_stmt : TIF expr block { $$ = new NIfStatement(*$2, *$3); }
| TIF expr block TELSE block { $$ = new NIfStatement(*$2, *$3, *$5); }
| TIF expr block TELSE if_stmt
;

%%
