import csv
from sys import argv, exit


def main():
    if len(argv) != 3:
        print("Usage: python dna.py file1 file2")
        exit(1)

    strs = {"AGATC": 0, "TTTTTTCT": 0, "AATG": 0, "TCTAG": 0, "GATA": 0, "TATC": 0, "GAAA": 0, "TCTG": 0}
    keys = strs.keys()
    print(keys)
    print(len(keys[6]))

    with open(argv[2], "r") as seqFile:
        sequence = seqFile.read()
        for key in keys:
            frm = 0
            to = len(key)
            matches = 0
            while to < len(sequence):
                if sequence[frm:to] != key:
                    if matches != 0:
                        strs[key] = matches if matches > strs[key] else strs[key]
                    """ print(" no match ", frm, to) """
                    matches = 0
                    frm += 1
                    to += 1
                    continue
                if sequence[frm:to] == key:
                    """ print("match ", frm, to, matches) """
                    matches += 1
                    frm += len(key)
                    to += len(key)
            strs[key] = matches if matches > strs[key] else strs[key]
    print(strs.items())

    

    person = "No match"
    """ open csv file, database of people, to check for a match """
    with open(argv[1], "r") as data:
        db = csv.DictReader(data)
        for row in db:
            if int(row["AGATC"]) == strs["AGATC"] and int(row["AATG"]) == strs["AATG"] and int(row["TATC"]) == strs["TATC"]:
                person = row["name"]
                
    print(person)


main()