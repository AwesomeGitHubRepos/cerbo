# http://www.payne.org/index.php/Reading_Google_Spreadsheets_in_Python



import gdata.docs
import gdata.docs.service
import gdata.spreadsheet.service
import re, os

import passwords
import picklemc




def getws(wbname, wsname):
    """Fetch a worksheet
    E.g. rows = google.getws('ACCTS2013', 'PorGo')
    """

    # Connect to Google
    gd_client = gdata.spreadsheet.service.SpreadsheetsService()
    cfgvals = passwords.section('google-pmax')
    gd_client.email = cfgvals['username']
    gd_client.password = cfgvals['password']
    gd_client.source = cfgvals['source']
    gd_client.ProgrammaticLogin()

    q = gdata.spreadsheet.service.DocumentQuery()
    q['title'] = wbname
    q['title-exact'] = 'true'
    feed = gd_client.GetSpreadsheetsFeed(query=q)
    spreadsheet_id = feed.entry[0].id.text.rsplit('/',1)[1]
    feed = gd_client.GetWorksheetsFeed(spreadsheet_id)
    worksheet_id = feed.entry[0].id.text.rsplit('/',1)[1]

    rows = gd_client.GetListFeed(spreadsheet_id, worksheet_id).entry
    #picklemc.dump(rows, 'spreadsheet.pck')
    return rows # , gd_client.GetCellsFeed(spreadsheet_id, worksheet_id)

def load():
    return picklemc.load('spreadsheet.pck')

def display(rows):
    for row in rows:
        for key in row.custom:
            print " %s: %s" % (key, row.custom[key].text)
        print

def main(fetch):
    if fetch:
        return download()
    else:
        return load()

if __name__ == "__main__":
    rows = main(True)
    display(rows)
    print "Finished"
