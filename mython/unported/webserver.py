#!/usr/bin/env python
# Very simple webserver, useful for testing purposes

import datetime
import platform
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
from optparse import OptionParser

html = '''<HTML>
<p>This is a test server from tacc.

<table border="1">
<tr> <td> Datetime </td> <td> DATETIME </td> </tr>
<tr> <td> Node     </td> <td> NODE     </td> </tr>
<tr> <td> Platform </td> <td> PLATFORM </td> </tr>
</table>
</HTML>'''

class MyHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        global html
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()
        contents = html.replace("DATETIME", str(datetime.datetime.now()))
        contents = contents.replace('NODE', platform.node())
        contents = contents.replace('PLATFORM', platform.platform())
        self.wfile.write(contents)
        


def main(port):
    try:
        server = HTTPServer(('', port), MyHandler)
        print 'started httpserver...'
        server.serve_forever()
    except KeyboardInterrupt:
        print '^C received, shutting down server'
        server.socket.close()

if __name__ == '__main__':
    usage="usage: %prog [options]"
    parser = OptionParser(usage=usage, version="%prog 1.0")
    parser.add_option("-p", "--port", dest="port", default=80, help="port number for server (default 80)")
    (options, args) = parser.parse_args()
    port = int(options.port)
    print "Using port ", port
    main(port)

