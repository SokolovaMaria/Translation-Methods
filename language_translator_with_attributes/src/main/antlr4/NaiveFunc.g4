grammar NaiveFunc;

@header {
    import java.util.*;
}

@members {
    StringBuilder program = new StringBuilder();
}

parse
    : (function)*
    ;

function
    : LET f = var LPAREN as = args RPAREN ASSIGN ds = declarations e = expr END {
        program.append("def " + $f.map.getValue());
        if (!$as.names.isEmpty()) {
            program.append("(" + String.join(", ", $as.names) + ")");
        }
        program.append(" :\n");
        if (!$ds.decls.isEmpty()) {
            program.append(String.join("\n", $ds.decls));
        }
        program.append("\n\treturn " + $e.str);
        program.append("\n");
    }
    ;

declarations returns [List<String> decls]
    @init {
        $decls = new ArrayList<>();
    }
    : d = declaration IN ds = declarations {
        $decls.add($d.str);
        $decls.addAll($ds.decls);
    }
    |
    ;

declaration returns [String str]
    : LET v = var ASSIGN e = expr {
        $str = "\t" + $v.map.getValue() + " = " + $e.str;
    }
    ;

expr returns [String str]
    : e1 = expr bop = binOp e2 = expr {
        $str = $e1.str + " " + $bop.text + " " + $e2.str;
    }
    | uop = unaryOp e1 = expr {
        $str = $uop.text + " " + $e1.str;
    }
    | LPAREN e = expr RPAREN {
        $str = "(" + $e.str + ")";
    }
    | v = variable {
        $str = $v.text;
    }
    | c = constant {
        $str = $c.text;
    }
    | fc = funcCall {
        $str = $fc.str;
    }
    ;


funcCall returns [String str]
    : ID LPAREN as = funcCallArgs RPAREN {
        $str = $ID.text;
        if (!$as.fcArgs.isEmpty()) {
            $str += "(" + String.join(", ", $as.fcArgs) + ")";
        }
    }
    ;

funcCallArgs returns [List<String> fcArgs]
    @init {
        $fcArgs = new ArrayList<>();
    }
    : e = expr COMMA es = funcCallArgs {
        $fcArgs.add($e.str);
        $fcArgs.addAll($es.fcArgs);
    }
    | e = expr {
        $fcArgs.add($e.str);
    }
    ;

variable
    : ID
    ;

constant
    : INT
    | STRING
    | BOOL
    ;

args returns [List<String> names]
    @init {
        $names = new ArrayList<>();
    }
    : a = var COMMA as = args {
        $names.add($a.map.getValue());
        $names.addAll($as.names);
    }
    | a = var {
        $names.add($a.map.getValue());
    }
    |
    ;



var returns [AbstractMap.SimpleEntry<String, String> map]
    : ID {
        $map = new AbstractMap.SimpleEntry<>("", $ID.text);
    }
    | ID COLON t = type {
        $map = new AbstractMap.SimpleEntry<>($t.text, $ID.text);
    }
    ;

type
    : INT_T
    | STRING_T
    | BOOL_T
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
    | MOD
    | AND
    | OR
    ;

unaryOp
    : NEG
    ;


INT
   : ('0' .. '9') + (('.' ('0' .. '9') + (EXPONENT)?)? | EXPONENT)
   ;

fragment EXPONENT
   : ('e') ('+' | '-')? ('0' .. '9') +
   ;

STRING
    : '"' ( ~'"' | '\\' '"' )* '"'
    ;

BOOL: '1' | '0';

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

COLON
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

MOD
    : '%'
    ;

AND
    : 'and'
    ;

OR
    : 'or'
    ;

NEG
    : 'not'
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

IF
    : 'if'
    ;

THEN
    : 'then'
    ;

ELSE
    : 'else'
    ;

ID
    : LETTER (LETTER | DIGIT)*
    ;

fragment
COMMENT: '(*' .*? '*)';

fragment
WS: [ \r\t\u000C\n]+;

TRASH: (WS | COMMENT) -> channel(HIDDEN);
