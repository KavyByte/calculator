import re
import ply.lex as lex

tokens = (
    'ID',
    'INT',
    'FLOAT',
    'STRING',
    'PLUS', 'MINUS', 'TIMES', 'DIVIDE',
    'ASSIGN', 'EQ', 'NEQ', 'LT', 'GT', 'LE', 'GE',
    'LPAREN', 'RPAREN', 'LBRACE', 'RBRACE',
    'COMMA', 'SEMI', 'COLON'
)

reserved = {
    'if': 'IF',
    'else': 'ELSE',
    'while': 'WHILE',
    'return': 'RETURN',
    'def': 'DEF'
}

tokens = tokens + tuple(reserved.values())

t_PLUS    = r'\+'
t_MINUS   = r'-'
t_TIMES   = r'\*'
t_DIVIDE  = r'/'
t_EQ      = r'=='
t_NEQ     = r'!='
t_LE      = r'<='
t_GE      = r'>='
t_LT      = r'<'
t_GT      = r'>'
t_ASSIGN  = r'='
t_LPAREN  = r'\('
t_RPAREN  = r'\)'
t_LBRACE  = r'\{'
t_RBRACE  = r'\}'
t_COMMA   = r','
t_SEMI    = r';'
t_COLON   = r':'

def t_FLOAT(t):
    r'\d+\.\d+'
    t.value = float(t.value)
    return t

def t_INT(t):
    r'\d+'
    t.value = int(t.value)
    return t

def t_STRING(t):
    r'(\"([^\\\n]|(\\.))*?\")|(\'([^\\\n]|(\\.))*?\')'
    
    s = t.value[1:-1]
    t.value = bytes(s, "utf-8").decode("unicode_escape")
    return t

def t_ID(t):
    r'[A-Za-z_][A-Za-z0-9_]*'
    
    t.type = reserved.get(t.value, 'ID')
    return t


def t_COMMENT_singleline(t):
    r'//.*'
    pass  

def t_COMMENT_multiline(t):
    r'/\*([^*]|\*(?!/))*\*/'
    
    t.lexer.lineno += t.value.count('\n')
    pass


def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)


t_ignore  = ' \t\r'


def t_error(t):
    print(f"Illegal character {t.value[0]!r} at line {t.lexer.lineno}")
    t.lexer.skip(1)

def build_lexer(**kwargs):
    return lex.lex(**kwargs)

if __name__ == '__main__':
    test_input = r'''
    // This is a single line comment
    def add(a, b) {
        return a + b;
    }

    /* multi-line
       comment */
    x = 123;
    y = 3.14;
    s = "hello\nworld";
    if x >= 10 {
        y = y * 2;
    }
    '''

    lexer = build_lexer()
    lexer.input(test_input)

    while True:
        tok = lexer.token()
        if not tok:
            break
        print(f"{tok.type:<12} {repr(tok.value):<20} {tok.lineno}")

