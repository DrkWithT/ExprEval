# README: Math Expr Evaluator

### Brief:
An arithmetic expression evaluator written in C++. It uses a recursive descent parser & AST approach for basic syntax checking and later support for additional syntax if needed.

### Grammar:
```bnf
; Some tokens!
whitespace ::= (SP | TAB | CR | LF)+
number ::= (DIGIT | ".")+
operators ::= "+" | "-" | "*" | "/"

; Note: Start parsing from lower precedence and descend to tighter binding precedence rule if needed!
expr ::= term
term ::= factor (("+" | "-") factor)*
factor ::= power (("*" | "/") power)*
power ::= unary ("^" unary)*
unary ::= ?("-") number | "(" expr ")"
```

### Some extras:
 - ~~I must add parenthesis support soon.~~ **DONE**
 - ~~Add a REPL for expressions?~~ **DONE**
 - Make error messages more detailed?
