#!/bin/sh

rmoctype=0
cd /home/repos/svn/oops/olibc/head

[ ! -f "mainpage.dox" ] && ln -sf doxygen/mainpage.dox ./
if [ ! -f oc_type.h ]; then
   	ln -sf oc_type.h.in oc_type.h
	rmoctype=1
fi

/usr/bin/doxygen doxygen/olibc.conf

rm -f mainpage.dox
[ $rmoctype -eq 1 ] && rm -f oc_type.h
cd -
