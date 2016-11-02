"""Module that works on a list of dictionaries"""

import operator

def floatify(alod):
    "Convert, where possible, elements of a list of dicts to floats"
    result = []
    for row in alod:
        for k, v in row.items():
            try: v = float(row[k])
            except ValueError: v = row[k]
            row[k] = v
        result.append(row)
    return result


def col(alod, key_name):
    "Extract a list of entries in a list of directories with key_name"
    return map(operator.itemgetter(key_name), alod)

def sorteds(alod, key_name):
    "Return a sorted list of dictionaries by key name"
    return sorted(alod, key=operator.itemgetter(key_name))
