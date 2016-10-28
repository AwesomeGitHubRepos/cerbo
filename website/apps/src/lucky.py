import base64
import io
from string import Template

# ORDER MAY BE IMPORTANT HERE
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt # sudo apt-get install python3-matplotlib


import cherrypy # sudo apt-get install python3-cherrypy3


from functools import partial
from scipy.stats import beta # sudo apt-get install python3-scipy


#import matplotlib

#import pylab
import numpy as np

def plot_beta(a, b, fill = False):
    betap = partial(beta.pdf, a, b)
    xs = np.linspace(0,1, num = 150)
    #y = list(map(betap, x))
    y = [beta.pdf(x, a, b) for x in xs]
    fills = [ x <= 0.5 for x in xs]    


    #sio = cStringIO.StringIO()
    sio = io.BytesIO()
    #sio = io.StringIO()
    fig = plt.figure()
    ax1 = fig.add_subplot(111)
    #ax1.set_ylim(bottom=0)

    #ax1.set_ylim(bottom
    if fill: ax1.fill_between(xs ,y,0,where=fills  , color='0.8')
    ax1.plot(xs, y)
    fig.savefig(sio, format='png')

    enc = base64.b64encode(sio.getvalue())
    plt.close("all")
    #hex = enc.strip()
    hex = enc.decode('utf-8')
    #hex = sio.getvalue().encode("base64").strip()
    prob = beta.cdf(0.5, a, b)
    return hex, prob


def validate(inp, form_name, desc):
    try: 
        f = float(inp)
        ok = True
        color = "white"
    except ValueError:
        f = None
        color = "red"
        ok = False

    fmt = '''<tr><td>$desc</td><td><input value="$value" style ="background-color : $color" name="$form_name" ></td></tr>'''
    s = Template(fmt)
    d = dict(color = color, desc = desc, form_name = form_name, value = inp)
    form_str = s.safe_substitute(d)
    return ok, f, form_str
    

def do_invalid_inputs():
    return "ERROR: Correct the inputs in red boxes and recalculate"

def do_valid_inputs(bsucc_f, bfail_f, succ_f, fail_f):
    img, prob = plot_beta(bsucc_f + succ_f, bfail_f + fail_f, fill= True)
    fmt = '''Probability it was all just luck: $luck%
    <p>A pretty picture of the likelihoods according to Bayesian Beta distribution:
    <p><img src="data:image/png;base64,$img" />'''
    s = Template(fmt)
    d = dict(luck = round(prob*100, 2), img = img)
    html = s.safe_substitute(d)
    return html


class Lucky(object):

    intro = '''<hr><h2>Introduction</h2>
    <p><b>Have you ever wondered whether a succession of outcomes was due to luck, or skill?</b> Well, with this web-app you can find out.

    <p><b>Quickstart</b> If you repeated a procedure a number of times, with S successes and F failures, then simply enter
    S in 'Number successes', and F in 'Number failures' in the box below, click the 'Calculate' button, and in the Outputs sections
    the probability that the number of outcomes was due to chance is displayed.

    <p><b>Example</b> You beat your brother in 6 out of the last 10 games you played with him. What is the chance that 
    you are better than him? To find the answer, enter the number 6 into 'Number successes', and 4 into 'Number fails', then
    click the 'Calculate' button. You should see the answer is 29.05%. You also see a fancy graph, which is a probability density
    function based on the data you enetered.

    <p><b>Getting fancy</b> If you want to know what the "base" values are about, and the methodology of the calculations, then
    read the 'Theory' section below.

    <p><b>Motivation</b> The motivation behind this little app was in order to answer a simple question: what is the
    probability that my investment performance was simply due to luck.

    <p><b>Contact me</b> Write me a direct message on my twitter account 
    <a href="https://twitter.com/mcturra2000">@mcturra2000</a>, or email me at <code>devnull at markcarter dot me dot uk</code>.
    Please note that my email account is disposable, and will reject email if people start spamming it.
    '''

    theory = '''<hr><h2>Theory</h2>
    There is too much mathematics to pack into a little section, but statisticians are often divided into 'frequentists' and
    'Bayesians'. Bayesian statistics is based on the ideas of Rev. Bayes (1701-1761). Bayesian statistics has one big
    philosophical hurdle that many find difficult to swallow: it depends on a prior assumption about how probabilities are
    distributed. You can essentially make up anything you want - although clearly there would be little sense in
    choosing unrealistic assumptions deliberately. As evidence for or against a hypothesis is gathered, the prior assumptions
    tends to 'get washed away', revealing a much more likely distribution of probability masses.

    <p>'Beta distributions' are often, but not exclusively, used to set the the a priori distribution, as it has a
    number of useful mathematical properties. The beta distribution is really a family of distributions. Its initial shape
    is set by the 'base successes' and 'base failures' parameter that you see above. If that confuses you, then just
    leave the default values as they are, and you will be fine. If you are more familiar with beta distributions then
    you can adjust the values up or down, equally, or relatively. It's up to you. If one value is greater than another, then
    it will tend to skew the shape of the plot to either the less or the right. As values rise, it will tend to reduce the
    spread of the distribution. This makes the prior assumptions more difficult to 'wash out'. 

    <p>I hope you find this interesting. Prof Allen Downey is some excellent writer on the Bayes distribution, and
    you may want to check out his blog <a href="http://allendowney.blogspot.co.uk/">Probably Overthinking it</a> for
    further inspiration.

    '''

    @cherrypy.expose
    def index(self, bsucc_str = "2", bfail_str = "2", succ_str = "0", fail_str = "0", **args):

        bsucc_ok, bsucc_f, input_bsucc = validate(bsucc_str, "bsucc_str", "Base successes:")
        bfail_ok, bfail_f, input_bfail = validate(bfail_str, "bfail_str", "Base fails:")
        succ_ok, succ_f, input_succ = validate(succ_str, "succ_str", "Number successes:")
        fail_ok, fail_f, input_fail = validate(fail_str, "fail_str", "Number fails:")
        form = Template('''
        <form method="get" action="/src/lucky">
        <table>
        $input_bsucc
        $input_bfail
        $input_succ
        $input_fail        
        </table>
        <input type='submit' value="Calculate">
        </form>
        ''')
        d = dict(input_bsucc = input_bsucc, input_bfail = input_bfail, input_succ = input_succ, input_fail = input_fail)
        form_str = form.safe_substitute(d)

        ok = bsucc_ok and bfail_ok and succ_ok and fail_ok
        if ok: calcs = do_valid_inputs(bsucc_f, bfail_f, succ_f, fail_f)
        else: calcs = do_invalid_inputs()


        s = Template('''<html><body>
        <h1><img src = "/images/lucky_shamrock.png"> Just luck?</h1>
        $intro
        <hr><h2>Inputs</h2>
        $form_str

        <hr><h2>Outputs</h2>
        <p>$calcs

        $theory
        <p>
        <hr><h2>Colophon</h2>
        <p>Powered by <img src="images/cplogo.jpg">
        <p>Author: Mark Carter
        <p>Created: 05-Mar-2015
        </body></html>''')
        d = dict(calcs = calcs, form_str=form_str, intro = self.intro, theory = self.theory)
        #s.substitute(form=form)
        txt = s.safe_substitute(d)
        return txt
    #index.exposed = True

    #@cherrypy.expose
    #def

#cherrypy.quickstart(Lucky(), "/", "lucky.config")
