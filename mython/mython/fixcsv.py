import argparse
import csv
import io
import os.path

import mython.listmc

def clean(infile, outfile, cols = None):
    """Clean up from INFILE to OUTFILE. for Sharelock Holmes.
    OUTFILE = None => use infile
    COLS = "col1, col2, ..." selects subcols. None => all columns
    """
    
    # read input file, and play with the columns
    full_infile = os.path.expanduser(infile)
    with open(full_infile,'r') as fp: text = fp.read()
    text = text.replace("\r", "\n")
    text = text.replace(',\n', '\n')
    #print(len(text))
    text = text.replace(' "', '"')
    text = text.replace('RS_6Month', 'rs6mb')
    text = text.replace('RS_Year', 'rs1y')
    text = text.replace('RS_5Year', 'rs5y')
    text = text.replace('F.EPIC', 'epic')
    text = text.replace('F.Sector', 'sector')
    text = text.replace('F.Sub_Sector', 'subsector')
    text = text.replace('MarketCap', 'mkt')
    text = text.replace('Piotroski_Score', 'pio')
    
    with io.StringIO(text) as csvin:
        reader = csv.reader(csvin)
        byCols = mython.listmc.transpose([r for r in reader])
        
    # collect the wanted columns
    subset = []
    if cols == None:
        wantedCols = ""
    else:
        wantedCols = cols.split(",")
    for c in byCols:
        if (cols == None) or (c[0] in wantedCols): 
            subset.append(c)
    subset = mython.listmc.transpose(subset)
    
    # write to file
    if outfile is None: outfile = infile
    full_outfile = os.path.expanduser(outfile)
    with open(full_outfile, "w") as csvout:
        writer = csv.writer(csvout)
        for r in subset: writer.writerow(r)


if __name__ == "__main__":
    parser = argparse.ArgumentParser("Fix Sharelock Holmes CSV file")
    parser.add_argument("infile", help = "input file to process")
    parser.add_argument("--cols", 
                        help = "Command-separated (no spaces!) list of column headers")
    parser.add_argument("-o", help = "output file name")
    parser.add_argument('-d', action = "store_true", 
                        help = "Print arguments and exit")
    args = parser.parse_args()
    
    if args.d: 
        print(args)
        exit(0)

    clean(args.infile, args.o, args.cols)
