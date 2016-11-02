import argparse
import ftplib
import sys

from mython.compat import princ





def main():
    parser = argparse.ArgumentParser(description='Send file to Netgear FTP')
    parser.add_argument('-c', dest = 'commands', 
                        default = False,
                        action= "store_true",
                        help = 'Print commands and exit')

    parser.add_argument('--put', dest = 'put', default = False,
                        action= "store_true", help= 'assumes upload')
    parser.add_argument('--source-dir', dest = 'sdir')
    parser.add_argument('--source-file', dest = 'sfile')
    parser.add_argument('--server', dest = 'server')
    parser.add_argument('--dest-dir' , dest ='ddir')
    args = parser.parse_args()
    if args.commands: princ(args) ; exit(0)

    #sys.stderr.write("ERR: args.server='{0}'\n".format(args.server))

    try:
        f = ftplib.FTP(args.server)
    except:
        sys.stderr.write("ERR: args.server='{0}'\n".format(args.server))
        raise
    f.login()
    try:
        f.cwd(args.ddir)
    except:
        print("ERR changing to dir:", args.ddir)
        raise
    fname = args.sfile
    fp = open(args.sdir + "/" + fname, 'rb')
    f.storbinary('STOR ' + fname, fp)
    fp.close()
    # f.retrlines('LIST')
    f.quit()

if __name__ == "__main__": main()
