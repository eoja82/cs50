import csv
from sys import argv, exit


if len(argv) != 3:
    print("Usage: python dna.py database.csv sequence.txt")
    exit(1)

strs = {}

""" determine which database is used """
if argv[1] == "databases/small.csv":
    strs = {"AGATC": 0, "AATG": 0, "TATC": 0}
elif argv[1] == "databases/large.csv":
    strs = {"AGATC": 0, "TTTTTTCT": 0, "AATG": 0, "TCTAG": 0, "GATA": 0, "TATC": 0, "GAAA": 0, "TCTG": 0}
else:
    print("Wrong database")

keys = strs.keys()

""" tally up matches for strs """
with open(argv[2], "r") as seqFile:
    sequence = seqFile.read()
    for key in keys:
        frm = 0
        to = len(key)
        matches = 0
        while to < len(sequence):
            """ get to start of a STR """
            if sequence[frm:to] != key:
                if matches != 0:
                    strs[key] = matches if matches > strs[key] else strs[key]
                matches = 0
                frm += 1
                to += 1
                continue
            if sequence[frm:to] == key:
                matches += 1
                frm += len(key)
                to += len(key)
        strs[key] = matches if matches > strs[key] else strs[key]

person = "No match"

""" open csv file, database of people, to check for a match """
with open(argv[1], "r") as data:
    db = csv.DictReader(data)
    for row in db:
        strMatches = 0
        for key in keys:
            if int(row[key]) == strs[key]:
                strMatches += 1
            else:
                break
        if strMatches == len(keys):
            person = row["name"]
            break
        
print(person)