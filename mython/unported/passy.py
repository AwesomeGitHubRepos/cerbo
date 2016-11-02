"""Very simple encryption of a file using the RC4 algorithm

Features:
* You can encrypt/decrypt binary data. Typically, this will be string. If you
want to encrypt a different data structure, for example a dictionary,
then you should first express it as a json string
* The 'payload' (i.e. your data) is stored in a file in base64. Whilst this is
not storage-efficient, it does make the storage more reliable (less
json encoding problems, for example).
* Adds some obfuscation when producing your password's fingerprint. This should
improve security a little, and make the hash code completely distinct from
other programs which might use MD5 to hash passwords.

Usage:

* Conventional: You can use pass.py as a conventional module, and use the
load() and save() functions.

* Interactively: A simple interactive session might go like this:
    # python -i passy.py
    >>> Password:
    >>> Again:
    >>> pay = load() # load the default file contents into a variable
    >>> print pay # what was store there?
    u'how now brown cow'
    >>> # define completely new contents
    >>> pay = { "yahoo" : "username: fred, password: flintstone" }
    >>> save(pay) # save the dictionary to the file
    >>> quit() # that's enought for now

  If you have the file ~/.ssh/id_rsa, then you can use it as a keyfile:
    # python -i passy.py -r

  passy.py uses the default file passy.jsn as its password store. You can open
  it with a text editor to see what the contents look like. If you want to
  use some file other than the default, use the -f option.

  To obtain help on using passy,py, you can type:
    # python passy,py -h
    

RISKS: Here are the likely risks of using this module
* RC4 is not without its weaknesses
* This program does not attempt to overwrite memory - so may be
susceptible to core dump analysis, and suchlike

In summary, I don't claim that this module is some kind of perfect security
tool, only that it is likely to be "good enough" for everyday low-risk
needs. Good enough for storing your Facebook password, but insufficient
to store your bank details. For that I suggest a proper tool like Keepassx.

Enjoy!"""

# 2010-10-16 mcarter created

import base64
import getpass
import json
import hashlib
import optparse
import os.path
import sys


#############################################################################

## {{{ http://code.activestate.com/recipes/576736/ (r5)
#!/usr/bin/env python
#
#       RC4, ARC4, ARCFOUR algorithm
#
#       Copyright (c) 2009 joonis new media
#       Author: Thimo Kraemer <thimo.kraemer@joonis.de>
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 2 of the License, or
#       (at your option) any later version.
#       
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#       
#       You should have received a copy of the GNU General Public License
#       along with this program; if not, write to the Free Software
#       Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#       MA 02110-1301, USA.
#

def rc4crypt(data, key):
    x = 0
    box = range(256)
    for i in range(256):
        x = (x + box[i] + ord(key[i % len(key)])) % 256
        box[i], box[x] = box[x], box[i]
    x = 0
    y = 0
    out = []
    for char in data:
        x = (x + 1) % 256
        y = (y + box[x]) % 256
        box[x], box[y] = box[y], box[x]
        out.append(chr(ord(char) ^ box[(box[x] + box[y]) % 256]))
    
    return ''.join(out)
## end of http://code.activestate.com/recipes/576736/ }}}

#############################################################################

class PassyError(Exception):
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return repr(self.value)

#############################################################################

    
# constants
schema = 'tacc-passy'
version = 1 # file version
salt = 'mixing it up'

# values for an interactive environment
m_password = None
m_filename = None



#############################################################################

def fingerprint(password):
    global salt
    return hashlib.md5(salt + str(version) + password).hexdigest()
    
def load(filename = None, password = None):
    "load a passy-formatted file"
    global schema, version, m_filename, m_password
    if not filename: filename = m_filename
    assert(filename)
    if not password: password = m_password
    assert(password)
    raw = file(filename, 'rb').read()
    data = json.loads(raw)
    
    try:
        assert(data["schema"] == schema)
    except AssertionError as e:
        raise PassyError("File format unrecognised")
    
    if data['version'] != version:
        raise PassyError("File format version undecodable")

    if data['password-fingerprint'] != fingerprint(password):
        raise PassyError("Password is wrong")

    b64 = base64.b16decode(data['payload'])
    jstring = rc4crypt(b64, password)
    data = json.loads(jstring)    
    return data
    

def save(data, filename = None, password = None):
    "Save a string to a passy-formatted file"
    global schema, version, m_filename, m_password
    if not filename: filename = m_filename
    assert(filename)
    if not password: password = m_password
    assert(password)

    data_as_json = json.dumps(data)
    payload = base64.b16encode(rc4crypt(data_as_json, password))
    output = {}
    output['password-fingerprint'] = fingerprint(password)
    output['payload'] = payload
    output['schema']=  schema
    output['version'] = version
    contents = json.dumps(output)
    file(filename, 'wb').write(contents)

#############################################################################

# set up an interactive environment

   
def interactive():
    global m_filename, m_password
    parser = optparse.OptionParser()
    parser.add_option("-f", "--file", dest="filename", default='passy.jsn',
                      help="Use file FILE, creating it if necessary (default=passy.jsn)")
    parser.add_option("-r", "--use-std-rsa", dest='use_std_rsa',
                      default = False, action='store_true', 
                      help="Use ~/.ssh/id_rsa private keyfile as a password")

    (options, args) = parser.parse_args()

    # Verify password
    if options.use_std_rsa:
        rsa_filename = os.path.expanduser("~/.ssh/id_rsa")
        assert(os.path.isfile(rsa_filename))
        m_password = file(rsa_filename, 'rb').read()
    else:
        while True:
            pass1 = getpass.getpass()
            if len(pass1) ==0:
                print "Can't use a blank password"
                continue
            pass2 = getpass.getpass('Again:')
            if pass1 == pass2: break
            print "Passwords don't match. Try again.foo\n"
        m_password = pass1
    assert(len(m_password)>0)

    m_filename = options.filename



#############################################################################

if __name__ == "__main__":
    interactive()
