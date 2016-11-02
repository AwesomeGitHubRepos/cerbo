"""Retrieve stored passwords"""

import ConfigParser
import os

def slurp():
    "Read in the whole passwords file"
    cfg = ConfigParser.ConfigParser()
    cfg.read([os.path.expanduser('~/docs/secret/passwords.cfg')])
    return cfg

# def items():
#    return slurp().items()

def section(name):
    "Return a dictionary of entries for a given section. E.g. section('google-pmax')"""
    cfg = slurp()
    sec = cfg.items(name)
    sec = { k: v for (k, v) in sec}
    return sec

def auth(section_name):
    """Return user and pass tuple for a section, suitable for passing into
    the auth field of requests.get()
    """
    d = section(section_name)
    return (d['username'], d['password'])
