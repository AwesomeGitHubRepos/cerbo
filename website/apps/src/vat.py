from string import Template
import cherrypy
#from Cheetah.Template import Template
import pdb


class Vat(object):
    def index(self, amount = '100.00', grossp = "CHECKED", netp = "", rate = '20.0', **args ):
        templateDef = '''
<h1>Vat Calculator</h1>

<h2>Inputs</h2>

<form action="index" method="GET">
<table><tbody>
  <tr><td>Amount</td> 
      <td><input type="text" name="amount" style="text-align: right"  value="$amount" /></td>
  </tr>
  <tr><td>Amount is:</td> 
      <td><input type="radio" name="which" value="gross" $grossp >Gross <br>
          <input type="radio" name="which" value="net" $netp >Net</td> 
  </tr>
  <tr><td>Rate %</td> 
      <td><input type="text" name="rate" style="text-align: right"  value="$rate" /></td></tr>
</tbody></table>
<p>
<input type="submit" value="Calculate" />

</form>



<h2>Outputs</h2>

<table><tbody>
  <tr><td>Net</td> <td style="text-align: right" >$net</td></tr>
  <tr><td>VAT</td> <td style="text-align: right" >$vat</td></tr>
  <tr><td>Gross</td> <td style="text-align: right" >$gross</td></tr>
</tbody></table>

<p>
Finished
'''

        #nameSpace = {'amount': amount}
        #pdb.set_trace()
        t = Template(templateDef) #, searchList=[nameSpace])

        amount = amount
        try: amountn = float(amount)
        except: amountn = "Error"

        rate = rate
        try: rate1= 1.0+ float(rate)/100
        except: rate1 = "Error"

        #which = "gross" if grossp == "CHECKED" else "netp"
        if "which" in args:
            which = args['which']
        else:
            which = "gross"
        print("which = ", which)
        #pdb.set_trace()
        # calcs
        if which == "net":
            netp = "CHECKED"
            grossp = ""
            net = amountn
            try: gross = round(net * rate1, 2)
            except: gross = "Error"
        else: # gross
            netp = ""
            grossp ="CHECKED"
            gross = amountn
            try: net = round(gross / rate1, 2)
            except: net = "Error"
        
        try: vat = gross - net
        except: vat = "Error"

        def stringf(num):
            if (type(num) is int) or (type(num) is float):
                return "{0:.2f}".format(num)
            else:
                return num

        net = stringf(net)
        vat = stringf(vat)
        gross = stringf(gross)
        
        #print str(t)
        d = dict(net = net, vat = vat, gross = gross, amount = amount, rate = rate, grossp = grossp, netp = netp, which = which)
        return t.safe_substitute(d)
    index.exposed = True


# http://www.cherrypy.org/wiki/ModPython
def setup_apache_server():
    # Set up site-wide config. Do this first so that,
    # if something goes wrong, we get a log.
    cherrypy.config.update({'environment': 'production',
                            'log.screen': False,
                            'log.error_file': 'site.log',
                            'show_tracebacks': False})
    
    cherrypy.tree.mount(Vat())



def main():
    cherrypy.server.socket_host = '0.0.0.0'
    cherrypy.server.socket_port = 8080
    cherrypy.quickstart(vat())

if __name__ == "__main__":
    main()


