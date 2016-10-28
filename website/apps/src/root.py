import cherrypy # sudo apt-get install python3-cherrypy3

from src.lucky import Lucky
from src.vat import Vat

class Root(object):

    index_htm = open("static/index.htm").read()

    @cherrypy.expose
    def index(self, **args):
        return self.index_htm

cherrypy.tree.mount(Lucky(), "/src/lucky")
cherrypy.tree.mount(Vat(), "/src/vat")

cherrypy.quickstart(Root(), "/", "apps.config")
