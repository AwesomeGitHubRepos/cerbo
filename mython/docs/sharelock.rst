Sharelock
=========

Fixing the CSV file
-------------------

The raw form downloaded from Sharelock Holmes contains a number of formatting errors. As of 23-Oct-2015, the standard is to fix the file in place::

    import mython.sharelock
    mython.sharelock.fixfile()

This fixes file `~/.fortran/StatsList.csv` on Linux systems, and returns a list of lists.

Alternatively::

  mython sharelock --momo

This also creates a `momo.csv`  file.


Module documents
----------------

.. automodule:: mython.sharelock
   :members:
