grammar SimpleOCaml;

parse
    : (declaration | definition)* (main)?
    ;

// DECLARATION

declaration
    : VAL ID COLOMN argTypes resType
    ;

argTypes
    : (type ARROW)*
    ;

resType
    : type
    ;

type: INT_T | STRING_T | BOOL_T;


//DEFINITION

local_definition
    : LET ID args ASSIGN body IN
    ;

definition
    : LET ID args ASSIGN body END
    ;

args
    : (ID (COMMA ID)*)?
    ;

body
    : (local_definition)* expr
    ;


//MAIN

main
    : (funcCall END)*
    ;


//EXPR

expr
    : expr binOp expr
    | unaryOp expr
    | variable
    | constant
    | parenExpr
    | funcCall
    ;

funcCall
    : ID LPAREN (expr (COMMA expr)*)? RPAREN
    ;

binOp
    : EQUAL
    | NOT_EQUAL
    | LT
    | GT
    | LE
    | GE
    | ADD
    | SUB
    | MUL
    | DIV
    | AND
    | OR
    ;

unaryOp
    : NEG
    ;

constant
    : INT
    | STRING
    | BOOL
    ;

variable
    : ID
    ;

parenExpr
    : LPAREN expr RPAREN
    ;

//LEXEMS

INT
   : ('0' .. '9') + (('.' ('0' .. '9') + (EXPONENT)?)? | EXPONENT)
   ;

fragment EXPONENT
   : ('e') ('+' | '-')? ('0' .. '9') +
   ;

STRING
    : '"' ( ~'"' | '\\' '"' )* '"'
    ;

BOOL: 'true' | 'false';

fragment LETTER
    : [a-zA-Z]
    ;
fragment DIGIT
    : [0-9]
    ;

LPAREN
    : '('
    ;
RPAREN
    : ')'
    ;

ARROW
    : '->'
    ;

COLOMN
    : ':'
    ;

COMMA
    : ','
    ;

END
    : ';;'
    ;


ASSIGN
    : '='
    ;

EQUAL
    : '=='
    ;

NOT_EQUAL
    : '!='
    ;

LT
    : '<'
    ;

GT
    : '>'
    ;

LE
   : '<='
   ;

GE
    : '>='
    ;

ADD
    : '+'
    ;

SUB
    : '-'
    ;

MUL
    : '*'
    ;

DIV
    : '/'
    ;

AND
    : '&&'
    ;

OR
    : '||'
    ;

NEG
    : '!'
    ;

INT_T
    : 'int'
    ;

STRING_T
    : 'string'
    ;

BOOL_T
    : 'bool'
    ;

VAL
    : 'val'
    ;

LET
    : 'let'
    ;

IN
    : 'in'
    ;


ID
    : LETTER (LETTER | DIGIT)*
    ;

fragment
COMMENT: '(*' .*? '*)';

fragment
WS: [ \r\t\u000C\n]+;

TRASH: (WS | COMMENT) -> channel(HIDDEN);

