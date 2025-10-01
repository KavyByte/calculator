from parser import parser
from lexical_analyzer import lexical_analyzer

with open("input.txt", "r") as f:
    for line in f:
        line = line.strip()
        print("Input : "+str(line))

        tokens = lexical_analyzer(line)
        parser(tokens)