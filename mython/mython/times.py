import datetime
import time

#This is a bit screwy. pkg_resources may not even load
#import pkg_resources
#pdtv = pkg_resources.get_distribution("parsedatetime").version
#if pdtv == '1.2':
#    import parsedatetime as pdt
#else:
#    import parsedatetime.parsedatetime as pdt
import parsedatetime.parsedatetime as pdt





def iso_days_ago(delta = 0):
    "Return datestamp in form YYYY-MM-DD DELTA days ago"
    s = str(delta) + " days ago"
    t, what = pdt.Calendar().parse(s)
    if what in (1,2):
        # result is struct_time
        dt = datetime.datetime( *t[:6] )
    elif what == 3:
        # result is a datetime
        dt = t
    s1 = dt.strftime("%Y-%m-%d")
    return s1

def curr_date_iso():
    "return current date in YYYY-MM-DD format"
    return time.strftime("%Y-%m-%d")

def now_str():
    "Example output: 2013-06-25 15:07:09.614593"
    return str(datetime.datetime.now())

def days_since_epoch(a_datetime):
    epoch = datetime.datetime.utcfromtimestamp(0)
    return (a_datetime - epoch).days


def str_to_date(yyyymmdd, nparray = False):
    """Convert a string of the form YYYY-MM-DD to a date object
    Also handles case where yyyymmdd is a HDF5 dataset
    if nparray = True, will return a numpy array.
    """
    
    # Handle case of HDF5 dataset
    #print(yyyymmdd)
    try:
        import h5py
        import numpy
        if isinstance(yyyymmdd, h5py.Dataset):
            l = [ str_to_date(d.tostring()) for d in yyyymmdd]
            if nparray: l = numpy.array(l)
            return l
    except ImportError:
        pass
    
    if type(yyyymmdd) == bytes: yyyymmdd = yyyymmdd.decode()
    l = yyyymmdd.split("-")
    l = list(map(int, l))
    d = datetime.date(*l)
    return d
