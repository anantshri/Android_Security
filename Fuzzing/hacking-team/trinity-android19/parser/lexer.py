# Lexer to tokenize the struct list

import ply.lex as lex


reserved = {
  'typedef' : 'TYPEDEF',
  'struct' : 'STRUCT',
  'union' : 'UNION',   
  'asmlinkage' : 'ASMLINKAGE',
  'enum' : 'ENUM'   
}

tokens = [
  'ARRAY',
  'TYPE',
  'NAME',
  'NUMBER',
  'OPEN',
  'CLOSE',
  'END',
  'COMMA',
  'POINTER'
] + list(reserved.values())

literals = ['[',']',',',')','(','|',':']

t_ARRAY =  r'\[' + r'\d+' + r'\]'
t_NUMBER = r'\-*' + r'\d+'
t_OPEN = r'\{'
t_CLOSE = r'\}'
t_END = r'\;'
t_COMMA = r'\,'
t_POINTER = r'\*' + r'[a-zA-Z_][a-zA-Z0-9_]*' 

def t_NAME(t):
     r'[a-zA-Z_][a-zA-Z0-9_]*'
     t.type = reserved.get(t.value,'NAME')
     return t

# Lets count the line
def t_newline(t):
    r'\n+'
    t.lexer.lineno += len(t.value)

# Comments
def t_comment(t):
    r'(/\*( .|\n)*?\*/)|(//.*)'
    pass


# Ignore the spaces
t_ignore  = ' \t'


# Print an error
def t_error(t):
    print "Line  " + str(t.lineno) + ": illegal character " + "'" + t.value[0] + "'"
    t.lexer.skip(1)



