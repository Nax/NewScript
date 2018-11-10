ast = {statement} ;

statement = expr ';'
          | ';'
          ;

expr = identifier
     | string
     | expr '.' identifier
     | expr '(' [expr {',' expr}] ')'
     ;

identifier = alpha {alnum} ;
string = '"' {char - '"'} '"'
       | "'" {char - "'"} "'"
       ;

char = ? Any valid UNICODE character ? ;
alnum = alpha | digit ;
alpha = 'a' | 'b' | 'c' ... | 'z' | 'A' | 'B' | 'C' ... | 'Z' ;
digit = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;
