import csv
from sys import argv, exit


def main():
    if len(argv) != 3:
        print("Usage: python dna.py file1 file2")
        exit(1)

    strs = {"AGAT": 0, "AATG": 0, "TATC": 0, "none": 0}
    matches = 0
    matched = "none"

    """ tally up matches """
    with open(argv[2], "r") as seqFile:
        sequence = seqFile.read()
        frm = 0
        to = 4
        while to < len(sequence):
            """ get to start of STRs """
            if sequence[frm:to] not in ["AGAT", "AATG", "TATC"]:
                frm += 1
                to += 1
                continue
            if sequence[frm:to] == "AGAT":
                if matched != "AGAT":
                    strs[matched] = matches if matches > strs[matched] else strs[matched]
                    matched = "AGAT"
                    matches = 1
                else:
                    matches += 1
                frm += 4
                to += 4
            elif sequence[frm:to] == "AATG":
                if matched != "AATG":
                    strs[matched] = matches if matches > strs[matched] else strs[matched]
                    matched = "AATG"
                    matches = 1
                else:
                    matches += 1
                frm += 4
                to += 4
            elif sequence[frm:to] == "TATC":
                if matched != "TATC":
                    strs[matched] = matches if matches > strs[matched] else strs[matched]
                    matched = "TATC"
                    matches = 1
                else:
                    matches += 1
                frm += 4
                to += 4
            else:
                frm += 4
                to += 4
        """ input values of what is in matches and matched in strs after the loop """
        strs[matched] = matches if matches > strs[matched] else strs[matched]

    person = "No match"
    """ open csv file, database of people, to check for a match """
    with open(argv[1], "r") as data:
        db = csv.DictReader(data)
        for row in db:
            if int(row["AGAT"]) == strs["AGAT"] and int(row["AATG"]) == strs["AATG"] and int(row["TATC"]) == strs["TATC"]:
                person = row["name"]
                
    print(person)

    
main()