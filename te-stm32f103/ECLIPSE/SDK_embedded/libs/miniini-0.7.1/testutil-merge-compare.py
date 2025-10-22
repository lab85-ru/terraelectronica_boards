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
    print "Script to compare two outputs of testutil-merge.py"
    print "Copyright (C) 2009-2010 Ferdinand Majerech"
    print "Usage: testutil-merge-compare.py [OPTIONS] [ARGUMENTS]"
    print " -h --help           display this help and exit"
    print " -o --output     val file to output comparison results to"
    print "                     default: merged-comparison.txt"
    print "ARGUMENTS must be files to compare."

def percent_change_str(val1, val2):
    f1 = float(val1)
    f2 = float(val2)
    if(f1 == 0):
        if(f2 > 0):
            return "inf%"
        elif(f2 == 0):
            return "100%"
        else:
            return "-inf%"
    return str(100 * (f2 / f1)) + "%"

def text_run(out, run):
    col = 20
    outstr = ""
    #align the values
    for st in run:
        if len(st) > col:
            st = st [-col:]
        elif len(st) < col:
            st +=  (col - len(st)) * " "
        outstr += st + "    "
    outstr += percent_change_str(run[1], run[2])
    out.write(outstr + "\n")

def text_data(out, data):
    out.write("Data type: " + data[0] + "\n")
    for run in data[1]:
        text_run(out, run)

def text_out(datas, fname, in1, in2):
    out = open(fname, "w")
    out.write("testutil-merge-compare.py : text output\n")
    out.write("comparing two series of identical testutil.py runs\n") 
    out.write("using different binaries:\n")
    out.write(in1 + " " + in2 + "\n\n")
    for data in datas:
        text_data(out, data)
        out.write("\n")
    out.close()

def find_name(datas, name):
    for data in range(len(datas)):
        if datas[data][0] == name:
            return data
    return sys.maxint

#Compare given input files and write results to given output file
def compare(infname1, infname2, outfname):
    input1 = ConfigParser.RawConfigParser()
    input1.read(infname1)
    input2 = ConfigParser.RawConfigParser()
    input2.read(infname2)
    #List containing (name, data) tuples, where name is type of value output
    #by testutil.py and data is a list of (dir, val1, val2) tuples where dir is
    #directory of a testutil.py run and val1, val2 are values of "name" in "dir"
    #in compared files.
    datas = []
    for section in input1.sections():
        for option in input1.options(section):
            name = section + "-" + option[:option.find("|")] 
            nameidx = find_name(datas, name)
            #new name
            if nameidx == sys.maxint:
                datas.append([name, []])
                nameidx = find_name(datas, name)
            newelem = (option[option.find("|") + 1:]
                       , input1.get(section, option),
                       input2.get(section, option))
            datas[nameidx][1].append(newelem)

    def comparedata(x, y):
        if(x[0] > y[0]):
            return 1
        elif(x[0] < y[0]):
            return -1
        else:
            return 0
    datas.sort(comparedata)
    for data in datas:
        data[1].sort(comparedata)
    text_out(datas, outfname, infname1, infname2)
 
def main():
    output = "merged-comparison.py"
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
            output = arg
    if not len(args) == 2:
        print "Need 2 args: Merged summaries to compare."
        sys.exit(2)
    compare(args[0], args[1], output)

if __name__ == '__main__':
    main()
