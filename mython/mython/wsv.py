# whitspeace separated values

import re

def to_lol(filename, num_fields):
    'convert a wsv file to a list of lists'
    lines = []
    for line in file(filename).readlines():
        line = line.strip()
        if line == "": continue
        fields = []
        for field_num in range(0, num_fields-1):
            m = re.match('(\\S+)\\s*(.*)', line)
            fields.append(m.group(1))
            line = m.group(2)
        fields.append(line)
        lines.append(fields)
    return lines
