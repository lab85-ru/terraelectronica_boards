#!/usr/bin/python

# Copyright (C) 2010 Ferdinand Majerech
# This file is part of MiniINI
# For conditions of distribution and use, see copyright notice in LICENSE.txt

import subprocess
import sys
import getopt
import ConfigParser
import os

def help():
    print "Script to merge outputs of multiple testutil.py runs"
    print "Copyright (C) 2009-2010 Ferdinand Majerech"
    print "Usage: testutil-merge.py [OPTIONS] [ARGUMENTS]"
    print " -h --help           display this help and exit"
    print " -o --output     val file to output merged data to."
    print "                     default: merged.sum.ini"
    print "ARGUMENTS must be directories of testutil.py outputs to merge"
    print "and the last argument must be the base file name used in those"
    print "testutil.py runs."

def merge(dirs, basefname, outfname):
    #output (merged) inifile
    out = ConfigParser.RawConfigParser()
    for dir in dirs:
        #filename of the .sim.ini file of the directory merged at the moment
        sumfname = dir + "/" + basefname
        #if the .sum.ini file exists, merge its contents into output.
        if(os.path.exists(sumfname)):
            input = ConfigParser.RawConfigParser()
            input.read(sumfname)
            for section in input.sections():
                #if there's no such section in output yet, create it
                if not out.has_section(section):
                    out.add_section(section)
                #merge tag into the output using directory name as a suffix separated by '|'
                for option in input.options(section):
                    out.set(section, option + "|" + dir, input.get(section, option))
    #write output out
    with open(outfname, "wb") as configfile:
        out.write(configfile)

def main():
    outfname = "merged.sum.ini"
    try:
        opts, args = getopt.getopt(sys.argv[1:], "ho:", 
                     ["help", "output="])
    except getopt.GetoptError:
        help()                          
        sys.exit(1)                               
    for opt, arg in opts:
        if opt in ("-h", "--help"):      
            help()                      
            sys.exit(0)
        if opt in ("-o", "--output"):      
            outfname = arg
    if len(args) < 2:
        print "Need at least 2 args: One directory and base input filename"
        sys.exit(2)
    merge(args[:-1], args[-1], outfname)

if __name__ == '__main__':
    main()
