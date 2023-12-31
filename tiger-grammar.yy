// Note: to learn how to invoke bison/flex in Eclipse, see https://www.eclipse.org/forums/index.php/t/1075215/
// And refer to the example at https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html#A-Complete-C_002b_002b-Example

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
%defines
%define api.parser.class {tigerParser}

/* next two from https://www.gnu.org/software/bison/manual/html_node/Complete-Symbols.html#Complete-Symbols */
%define api.value.type variant
%define api.token.constructor

%code requires
{
class tigerParseDriver;
}
%param { tigerParseDriver& driver }

// According to the Example, this turns on "location tracking"
%locations
// Thanks to Ian Fisher HC '19 for explaining the details of the parts I'd failed to get working


%{
#include "tigerParseDriver.h"
%}

%token END  0
%token <bool> BOOL
%token <int>  INT
%token <std::string> ID STRING

// NOTE that bison complains if you have the same symbol listed as %token (above) and %type (below)
//      so if you want to add attributes to a token, remove it from the list below

%token 
  COMMA COLON SEMICOLON LPAREN RPAREN L_SQUARE_BRACKET R_SQUARE_BRACKET 
  L_CURLY_BRACE R_CURLY_BRACE 
  ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END_LET OF 
  BREAK NIL
  FUNCTION VAR TYPE DOT
  PLUS MINUS TIMES DIVIDE ASSIGN EQ NEQ LT LE GT GE OR AND NOT
;

/* precedence (stickiness) ... put the stickiest stuff at the bottom of the list */

%left PLUS MINUS
%left TIMES DIVIDE
%left COMMA

/* Attributes types for nonterminals are next, e.g. struct's from tigerParseDriver.h */
%type <expAttrs> exp
%type <seqAttrs> seq


// The line below means our grammar must not have conflicts
//  (no conflicts means it is "an LALR(1) grammar",
//   meaning it must be unambiguous and have some other properties).
%expect 0


%%

%start program;
program: exp[main]	{ EM_debug("Got the main expression of our tiger program.", $main.AST->pos());
		 			  driver.AST = new A_root_($main.AST);
		 			}
	
// Lecture notes 2/1:
// 		Using the PTattr structue to compute a parse tree
// 		$$.type = Ty_int(), $$height = 1 (To think about)
// 		Down the line use the type to produce a typechecker error

exp:  INT[i]					{ $$.AST = A_IntExp(Position::fromLex(@i), $i);
								  EM_debug("Got int " + str($i), $$.AST->pos());
								}
	| STRING[i]					{$$.AST = A_StringExp(Position::fromLex(@i), $i);
								  EM_debug("Got string " + $i, $$.AST->pos());
								}
	| exp[exp1] PLUS exp[exp2]	{ $$.AST = A_OpExp(Position::range($exp1.AST->pos(), $exp2.AST->pos()),
												   A_plusOp,  $exp1.AST,$exp2.AST);
								  EM_debug("Got plus expression.", $$.AST->pos());
								}
	| exp[exp1] TIMES exp[exp2]	{ $$.AST = A_OpExp(Position::range($exp1.AST->pos(), $exp2.AST->pos()),
											   A_timesOp, $exp1.AST,$exp2.AST);
								  EM_debug("Got times expression.", $$.AST->pos());
								}
	| exp[exp1] MINUS exp[exp2] { $$.AST = A_OpExp(Position::range($exp1.AST->pos(), $exp2.AST->pos()),
	                                               A_minusOp, $exp1.AST,$exp2.AST);
								  EM_debug("Got minus expression.", $$.AST->pos());
	                            }

	| exp[exp1] DIVIDE exp[exp2] { $$.AST = A_CallExp($exp1.AST->pos(),
										to_Symbol("div"),
										A_ExpList($exp1.AST, A_ExpList($exp2.AST, 0)));
								EM_debug("Got divison expression" , $$.AST->pos());	
								}

	| ID LPAREN exp[exp1] RPAREN 	{$$.AST = A_CallExp($exp1.AST->pos(), 
									to_Symbol($ID),
									A_ExpList($exp1.AST, 0));
									EM_debug("Got Function "+ $ID, $$.AST->pos());
									}
	| LPAREN seq[seq1] RPAREN   {$$.AST = A_SeqExp($seq1.AST->pos(), $seq1.AST);
								EM_debug("Got Sequence", $$.AST->pos());
								}

seq: exp[exp1]					{$$.AST = A_ExpList($exp1.AST, 0);
								EM_debug("Got expression", $$.AST->pos());
								}
	| exp[exp1] SEMICOLON seq[exp2] {$$.AST = A_ExpList($exp1.AST, $exp2.AST);
									}
	| exp[exp1] COMMA seq[exp2]
    							 { $$.AST = A_ExpList($exp1.AST, $exp2.AST);
    							    }


//
// Note: In older compiler tools, instead of writing $exp1 and $exp2, we'd write $1 and $3,
//        to refer to the first and third elements on the right-hand-side of the production.
//        We can still use this notation (and note that Appel uses it in his book), e.g. the above is like
//					$$.AST = A_OpExp(Position::range($exp1.AST->pos(), $exp2.AST->pos()),
//									 A_timesOp, $1.AST,$3.AST);
// Also: Bison's location system produces information about nonterminals as well as terminals,
//        so we could use @exp1 to get it's information about the locations of exp1
//        writing, e.g., Position::fromLex(@exp1) or instead of $exp1.AST->pos()
//

	;

%%

void
yy::tigerParser::error(const location_type& l,
          	       const std::string& m)
  {
	  EM_debug("In yy::tigerParser::error");
	  EM_error(m, true, Position::fromLex(l));
  }
