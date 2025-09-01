import re

def tokenize(expression):
    """
    look for numbers or brackets or operators
    """
    token_pattern = r"(\d+|[()\^+\-*/])"
    tokens = re.findall(token_pattern, expression)
    return tokens

# parse functions for each non terminal
def parse_factor(tokens):
    """
    F -> (E) | number
    """
    token = tokens.pop(0)
    if token == "(":
        result = parse_expression(tokens)
        # HACK: prolly should check below statement
        tokens.pop(0) # get rid of )
        return result
    else:
        return float(token)

def parse_exponent(tokens):
    """
    X -> F ^ X | F
    """
    left = parse_factor(tokens)
    while tokens and tokens[0] == "^":
        op = tokens.pop(0)
        right = parse_exponent(tokens)
        if op == "^":
            left **= right
    return left

def parse_term(tokens):
    """
    T -> X*T | X/T | X
    """
    left = parse_exponent(tokens)
    while tokens and tokens[0] in "*/":
        op = tokens.pop(0)
        right = parse_exponent(tokens)
        # NOTE: backtracking happening here
        # to eliminate start predicting shit (LL(1))
        if op == "*":
            left *= right
        else:
            left /= right
    return left

def parse_expression(tokens):
    """
    E -> T + E | T - E | T
    """
    left = parse_term(tokens)
    while tokens and tokens[0] in "+-":
        op = tokens.pop(0)
        right = parse_term(tokens)
        if op == "+":
            left += right
        else: 
            left -= right
    return left

def evaluate_expression(expression):
    tokens = tokenize(expression)
    result = parse_expression(tokens)
    return result

expr = "init"
while expr:
    try:
        print("parser: ", end='')
        expr = input()
        print(evaluate_expression(expr))
    except EOFError:
        print()
        print("parser: exiting")
        break

