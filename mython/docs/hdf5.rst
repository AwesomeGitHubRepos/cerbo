HDF5
====

Installation
------------

Problem
~~~~~~~

The distribution-supplied version of HDF5 doesn't work well, and you
want to compile and install your own copy

Solution
~~~~~~~~

Download the latest version of the sources, and issue the command::

    ./configure --enable-fortran --prefix=/usr/local


CSV to HDF5
-----------

Issue the command::

  mython hdf --to5 <filename>

This assumes that `filename` is in `~/.fortran` and is in CSV format. It will then create a file  named `filename.h5`. You might then want to aggregate it up into a bigger repository using `h5copy`


Fortran
-------

Problem
~~~~~~~

You want to read an array of fixed-length strings using Fortran.

Solution
~~~~~~~~

Use ``h5ex_t_string_F03.f90`` , discussed in an `example
<http://www.hdfgroup.org/HDF5/examples/api18-fortran.html>`_.


Viewing
-------

Problem
~~~~~~~

You want to view the contents of HDF5 files in a GUI.

Solution
~~~~~~~~

Download and install either of the following applications:

* `HDF5View <http://www.hdfgroup.org/products/java/release/download.html>`_
* `ViTables <http://vitables.org/>`_

Module documents
----------------

.. automodule:: mython.hdf
   :members:
      
