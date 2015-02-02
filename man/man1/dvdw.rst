====
dvdw
====

-------------------
write a file to dvd
-------------------

:Author: mark carter
:Date: 2013-04-07
:Copyright: public domain
:Version: 1
:Manual section: 1
:Manual group: mcarter


SYNOPSIS
========

    dvdm file

DESCRIPTION
===========

Wite a file to DVD. It is a simple script that mostly just does this:

    growisofs -M /dev/sr0 -R -J $1

In order to initialise a DVD, issue the following command:

    growisofs -Z /dev/sr0 -R -J ~/.bashrc

SEE ALSO
========

mcarter(1)