#!/usr/bin/python

# Copyright (C) 2009-2010 Ferdinand Majerech
# This file is part of MiniINI
# For conditions of distribution and use, see copyright notice in LICENSE.txt

import subprocess
import getopt
import sys
import re
import time
    
tests = "vc"
outdir = "testout"
name = "testout"
commands = None
dosummarize = True
sumname = name + ".sum.ini"
keep = 0
timeruns = 64

def help():
    print "Benchmark/command run analyzer"
    print "Copyright (C) 2009-2010 Ferdinand Majerech"
    print "Usage: testutil.py [OPTION...] COMMAND TO TEST [CUSTOM]"
    print "Runs memcheck, cachegrind, callgrind and so on on given command"
    print "Optionally, CUSTOM must be a list of custom tag=value pairs to write"
    print "to the [custom] section in the summary output file."
    print " -h --help          display this help and exit"
    print " -t --tests     val tests to perform - every character specifies a"
    print "                    test to run: "
    print "                    m : memcheck"
    print "                    c : callgrind"
    print "                    C : cachegrind"
    print "                    M : massif"
    print "                    t : run time length test"
    print "                    Example: -t mcC will run memcheck, callgrind and"
    print "                    cachegrind on given command"
    print " -f --out-file  val base file name of output files"  
    print " -o --out-dir   val directory to save output files to"  
    print " -k --keep          keep more files produced by tests performed,"
    print "                    not just summary and callgrind/cachegrind files"
    print " -T --time-runs val if the run time length test is used, this option"
    print "                    can change the number of times the command is"
    print "                    run to get average run time"

def runcmd(cmd):
    print cmd
    subprocess.call(cmd, shell=True)

def grep(exp, fname):
    file = open(fname, "r")
    grepre = re.compile(exp)
    out = []
    for line in file:
        if(grepre.search(line)):
            out.append(line)
    return out

def newline(file):
    file.write("\n")

def section(str, file):
    file.write("\n[" + str + "]\n")

def tag(tag, val, file):
    file.write(tag + "=" + str(val) + "\n")

def comment(str, file):
    file.write(";" + str + "\n")

def extractnums(exp, file):
    numre = re.compile("\d+\.?\d*")
    line = grep(exp, file)[0]
    line = line.replace(",", "")
    return numre.findall(line)

def memcheck(command):
    fname = outdir + "/" + name + "-memcheck.txt"
    memcheck = "valgrind " + command + " > " + fname + " 2>&1"
    runcmd(memcheck)

    file = open(outdir + "/" + sumname, 'a+')
    section("memcheck", file)
    mematend = extractnums("in use at exit", fname)
    tag("bytes_at_exit", mematend[1], file)
    tag("blocks_at_exit", mematend[2], file)
    heapuse = extractnums("total heap usage", fname)
    tag("total_allocs", heapuse[1], file)
    tag("total_frees", heapuse[2], file)
    tag("total_bytes_allocated", heapuse[3], file)
    errorsum = extractnums("ERROR SUMMARY", fname)
    tag("errors", errorsum[1], file)
    if keep < 1:
        runcmd("rm " + fname)
    file.close()

def callgrind(command):
    fname = outdir + "/" + name + "-callgrind"
    callgrind = ("valgrind --tool=callgrind --callgrind-out-file=" + fname + ".out " 
		 + command + " > " + fname + ".txt 2>&1")
    runcmd(callgrind)

    fname += ".txt"
    file = open(outdir + "/" + sumname, 'a+')
    section("callgrind", file)
    irefs = extractnums("I   refs", fname)
    tag("instruction_refs", irefs[1], file)
    if keep < 1:
        runcmd("rm " + fname)
    file.close()

def cachegrind(command):
    basestr = "valgrind --tool=cachegrind --cachegrind-out-file="
    fname = outdir + "/" + name + "-cachegrind"
    cachegrind = basestr + fname + ".out " + command + " > " + fname + ".txt 2>&1"
    cmd = cachegrind
    runcmd(cmd)
    
    fname += ".txt"
    file = open(outdir + "/" + sumname, 'a+')
    section("cachegrind", file)
    irefs = extractnums("I   refs", fname)
    tag("instruction_refs", irefs[1], file)
    drefs = extractnums("D   refs", fname)
    tag("data_refs", drefs[1], file)
    newline(file)
    comment("L1 cache:", file)
    i1misses = extractnums("I1  misses", fname)
    tag("l1_instruction_misses", i1misses[2], file)
    i1rate = extractnums("I1  miss rate", fname)
    tag("l1_instruction_miss_rate", i1rate[2], file)
    d1misses = extractnums("D1  misses", fname)
    tag("l1_data_misses", d1misses[2], file)
    d1rate = extractnums("D1  miss rate", fname)
    tag("l1_data_miss_rate", d1rate[2], file)
    comment("L1 cache totals:", file)
    l1misses = extractnums("L2 refs", fname)
    tag("l1_total_misses", l1misses[2], file)
    newline(file)
    comment("L2 cache:", file)
    i2misses = extractnums("L2i misses", fname)
    tag("l2_instruction_misses", i2misses[2], file)
    i2rate = extractnums("L2i miss rate", fname)
    tag("l2_instruction_miss_rate", i2rate[2], file)
    d2misses = extractnums("L2d misses", fname)
    tag("l2_data_misses", d2misses[2], file)
    d2rate = extractnums("L2d miss rate", fname)
    tag("l2_data_miss_rate", d2rate[2], file)
    comment("L2 cache totals:", file)
    l2misses = extractnums("L2 misses", fname)
    tag("l2_total_misses", l2misses[2], file)
    l2rate = extractnums("L2 miss rate", fname)
    tag("l2_total_miss_rate", l2rate[2], file)
    newline(file)
    comment("Branches:", file)
    branches = extractnums("Branches", fname)
    tag("branches", branches[1], file)
    mispredicts = extractnums("Mispredicts", fname)
    tag("mispredicts", mispredicts[1], file)
    misrate = extractnums("Mispred rate", fname)
    tag("mispredict_rate", misrate[1], file)
    if keep < 1:
        runcmd("rm " + fname)
    file.close()

def massif(command):
    basestr = "valgrind --tool=massif --massif-out-file="
    fname = outdir + "/" + name + "-massif"
    massif = basestr + fname + ".out " + command + " > " + fname + ".txt 2>&1"
    ms_print = ("ms_print --x=80 --y=20 " + fname + ".out > " + fname + ".out.txt" + " 2>&1")
    remove = "rm " + fname + ".out && rm " + fname + ".txt" 
    cmd = massif + " && " + ms_print + " && " + remove
    runcmd(cmd)

    fname += ".out.txt"
    file = open(outdir + "/" + sumname, 'a+')
    section("massif", file)
    unitline = grep("^\s+\S+",fname)[0]
    peak = float(extractnums("^\d+\.?\d*", fname)[0])
    if("KB" in unitline):
        peak = int(peak * 1024)
    elif("MB" in unitline):
        peak = int(peak * 1024 * 1024)
    elif("GB" in unitline):
        peak = int(peak * 1024 * 1024 * 1024)
    elif("B" in unitline):
        peak = int(peak)
    else:
        comment("Peak memory usage unit unknown.", file)
    tag("peak_memory_usage", peak, file)
    if keep < 1:
        runcmd("rm " + fname)
    file.close()

def runtime(command):
    start = time.time()
    for run in range(0, timeruns):
        runcmd(command + " > /dev/null")
    end = time.time()

    file = open(outdir + "/" + sumname, 'a+')
    section("run_time", file)
    tag("average_run_time",(end - start) / timeruns, file)
    tag("run_count", timeruns, file)
    file.close()

def custom(args):
    file = open(outdir + "/" + sumname, 'a+')
    section("custom", file)
    for arg in args:
        file.write(arg + "\n")
    file.close()


def main():
    global tests
    global name
    global outdir
    global sumname
    global keep
    global timeruns
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hkt:f:o:", 
                     ["help", "keep", "tests=", "out-file=", "out-dir="])
    except getopt.GetoptError:
        help()                          
        sys.exit(1)                               
    for opt, arg in opts:
        if opt in ("-h", "--help"):      
            help()                      
            sys.exit(0)
        elif(opt in ("-t", "--tests")):
            tests = arg
        elif(opt in ("-f", "--out-file")):
            name = arg
            sumname = name + ".sum.ini"
        elif(opt in ("-o", "--out-dir")):
            outdir = arg
        elif(opt in ("-k", "--keep")):
            keep += 1  
        elif(opt in ("-T", "--time-runs")):
            timeruns = int(arg)
    if len(args):
        runcmd("mkdir -p " + outdir)
        if(dosummarize):
            sumfile = open(outdir + "/" + sumname, 'w')
            comment("testutil.py run summary:\n", sumfile)
            sumfile.close()
        if "t" in tests:
            runtime(args[0])
        if "c" in tests:
            callgrind(args[0])
        if "C" in tests:
            cachegrind(args[0])
        if "M" in tests:
            massif(args[0])
        if "m" in tests:
            memcheck(args[0]) 
        custom(args[1:])
    else:
        help()
        sys.exit(1)
    
if __name__ == '__main__':
    main()
