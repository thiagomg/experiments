import sys

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
    
colors = { 
    'GREEN': bcolors.OKGREEN,
    'BLUE': bcolors.OKBLUE,
    'MAGENTA': bcolors.HEADER,
    'PURPLE': bcolors.HEADER,
    'YELLOW': bcolors.WARNING,
    'RED': bcolors.FAIL,
    'NONE': bcolors.ENDC
}

attribs = {
    'BOLD' : bcolors.BOLD,
    'UNDERLINE': bcolors.UNDERLINE,
}

exit_cond = lambda x: x in {'q', 'quit', 'leave', 'exit'}

def set_exit_cond(condition):
    global exit_cond
    exit_cond = condition

def get_char(s, char_list):
    while( True ):
        string = raw_input(s)
        if exit_cond(string):
            return None

        if string in char_list:
            return string

def get_number(s):
    while( True ):
        try:
            string = raw_input(s)
            if exit_cond(string):
                return None
            return int(string)
        except:
            print 'Not a number. Try again'
            
def get_word(s):
    string = raw_input(s)
    if exit_cond(string):
        return False
    return True

def ask_addition_question(m, n):
    for i in range(1, 4):
        result = get_number(str(m) + ' + ' + str(n) + ' = ')
        if result == None:
            return -1
        
        if result == (m+n):
            print 'Correct !'
            return 1
        else:
            print 'Wrong. try again!'
            
    return 0
    
def ask_multiplication_question(m, n):
    for i in range(1, 4):
        result = get_number(str(m) + ' x ' + str(n) + ' = ')
        if result == None:
            return -1
        
        if result == (m*n):
            print 'Correct !'
            return 1
        else:
            print 'Wrong. try again!'
            
    return 0
    
def ask_subtraction_question(m, n):
    for i in range(1, 4):
        if m < n:
            m, n = n, m
        result = get_number(str(m) + ' - ' + str(n) + ' = ')
        if result == None:
            return -1
        
        if result == (m-n):
            print 'Correct !'
            return 1
        else:
            print 'Wrong. try again!'
            
    return 0
    
def ask_word_question(word):
    return get_word('       ' + word + '       ')

def write(text, color=None, *attrib):

    prefix = ''
    sufix = ''
    if not color is None:
        prefix += colors[color.upper()]
    for at in attrib:
        prefix += attribs[at.upper()]
        
    if len(prefix) > 0:
        sufix = colors['NONE']

    print prefix + text + sufix
    