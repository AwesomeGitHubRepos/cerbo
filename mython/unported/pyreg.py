import datetime, glob, pdb, json, os, os.path, StringIO, sys, text, zlib, _winreg

from time import strftime

def walk (top, writeable=False):
  """walk the registry starting from the key represented by
  top in the form HIVE\\key\\subkey\\..\\subkey and generating
  (key_name, key), subkey_names, values at each level.

  subkey_names are simply names of the subkeys of that key
  values are 3-tuples containing (name, data, data-type).
  See the documentation for _winreg.EnumValue for more details.
  """
  keymode = _winreg.KEY_READ
  if writeable:
    keymode |= _winreg.KEY_SET_VALUE
  if "\\" not in top: top += "\\"
  root, subkey = top.split ("\\", 1)
  try:
      key = _winreg.OpenKey (getattr (_winreg, root), subkey, 0, keymode)
  except WindowsError:
      print "WindowsError on OpenKey of ", subkey
      return

  subkeys = []
  i = 0
  while True:
    try:
      subkeys.append (_winreg.EnumKey (key, i))
      i += 1
    except EnvironmentError:
      break

  values = []
  i = 0
  while True:
    try:
      values.append (_winreg.EnumValue (key, i))
      i += 1
    except EnvironmentError:
      break

  yield (top, key), subkeys, values
  for subkey in subkeys:
    for result in walk (top.rstrip ("\\") + "\\" + subkey, writeable):
      yield result

def mkstr(s):
  result = ''

  # pre-conversion
  if not s:
    s = ''
  elif isinstance(s, int) or isinstance(s, float):
    s = str(s)

  for c in s:
    try: c1 = c.encode('ascii') #c1 = chr(ord(c))
    except: c1 = '?'
    result += c1
  return result


def walk_keypath(results, keypath):
    #print "keypath = " , keypath
    for (key_name, key), subkey_names, values in walk (keypath):
        #print "key_name:", key_name
        #level = key_name.count ("\\")
        #print "level:", level
        for name, data, datatype in values:
            #print "name", name
            #edata = enc_data(data)
            key = mkstr(key_name)
            name = mkstr(name)
            data = mkstr(data)
            if len(data) > 500:
              check = zlib.adler32(data)
              data = "BIG, LEN %d, CRC %d" % (len(data), check)

            results.append((key, name, data))

def walk_keypaths():
    results = []
    keypaths = ['HKEY_CLASSES_ROOT', 'HKEY_CURRENT_USER', 'HKEY_LOCAL_MACHINE',
                'HKEY_USERS', 'HKEY_CURRENT_CONFIG']
    for keypath in keypaths:
        walk_keypath(results, keypath)
    return results

def setup_file():
    outdir = os.getenv("LOCALAPPDATA") + "\\pyreg\\"
    if not os.access(outdir, os.F_OK): os.makedirs(outdir)

    resultsdir = outdir + "results\\"
    if not os.access(resultsdir, os.F_OK): os.makedirs(resultsdir)

    files = glob.glob(resultsdir + "????.json")
    def fnum(x):
        bname = os.path.basename(x)
        bnum = bname[0:4]
        return int(bnum)

    if len(files) == 0:
        outnum =0
    else:
        outnum = max( map(fnum, files))
    outfile = "%04.4d.json" % (outnum + 1)

    print "Enter a description: ",
    desc = sys.stdin.readline().strip()
    desc_text = "%s\n%s\n%s\n\n" % (outfile, strftime("%Y-%m-%d %H:%M:%S"), desc)
    text.append_file(outdir + "desc.txt",desc_text)

    datafile = "%s%s" % (resultsdir , outfile)
    return datafile

def main():
    fname = setup_file()
    tree = walk_keypaths()
    txt = json.dumps(tree, indent=4)
    file(fname, "w").write(txt)
    print 'Filename:', fname
    print 'Finished'


if __name__ == '__main__': main()
