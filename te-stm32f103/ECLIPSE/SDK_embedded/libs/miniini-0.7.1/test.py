#!/usr/bin/python

# Copyright (C) 2009-2010 Ferdinand Majerech
# This file is part of MiniINI
# For conditions of distribution and use, see copyright notice in LICENSE.txt

import subprocess
import sys
import copy
import getopt
import ConfigParser
import os


def help():
    print "Aggregate benchmark script"
    print "Copyright (C) 2009-2010 Ferdinand Majerech"
    print "Usage: test.py [OPTIONS] [ARGUMENTS]"
    print " -h --help           display this help and exit"
    print " -f --file       val ini file containing tests to run - file must be"
    print "                     present in test/ini directory - if this is used,"
    print "                     test.py needs one argument - directory to output"
    print "                     test results to - this will be created as a"
    print "                     subdirectory of test/runs"
    print " -c --compare    val directory to output comparison of two test.py"
    print "                     runs to - this will be created as a subdirectory"
    print "                     of test/comps - if this is used, test.py needs"
    print "                     two arguments - directories containing results"
    print "                     of test.py runs to compare - these are"
    print "                     subdirectories of test/runs directory"

def runcmd(cmd):
    print cmd
    subprocess.call(cmd, shell=True)

def compare(outdir, dir1, dir2):
    rundir = "test/runs/"
    dir1 = rundir + dir1 + "/testsum"
    dir2 = rundir + dir2 + "/testsum"
    outdir = "test/comps/" + outdir
    for target in ["opt", "dbg"]:
        toutdir = outdir + "/" + target
        runcmd("mkdir -p " + toutdir)
        tdir1 = dir1 + "/" + target
        tdir2 = dir2 + "/" + target
        for file in os.listdir(tdir1):
            if file in os.listdir(tdir2):
                runcmd("./testutil-merge-compare.py -o " + 
                       toutdir + "/" + "comp-" + file + ".txt" + " " + 
                       tdir1 + "/" + file + " " + tdir2 + "/" + file)

class ini_file:
    benchinidir = "test/benchmark/"
    basedir = "test/runs/"
    inidir = "test/ini/"
    stl = False
    sumdir = "testsum"
    types_map = {"s" : "string", "i" : "int", "f" : "float", "b" : "bool"}
    files = []
    benchmarks = []
    benchsizes = []
    tests = []
    testbase = "./testutil.py -t "
    bench_all = bench_separate = bench_rules = bench_tester = False
    def __init__(self, fname, basedir):
        file = ConfigParser.RawConfigParser()
        file.read(self.inidir + fname)
        self.basedir += basedir
        for section in file.sections():
            try:
                self.sections = file.getint(section, "sections")
                self.mode = file.get(section, "mode")
                self.types = file.get(section, "types")
                self.tags_min = file.getint(section, "tags-min")
                self.tags_max = file.getint(section, "tags-max")
                self.tag_len_min = file.getint(section, "tag-len-min")
                self.tag_len_max = file.getint(section, "tag-len-max")
                self.tools = file.get(section, "tools")
                if(self.mode == "arrays"):
                    self.arrays_min = file.getint(section, "arrays-min")
                    self.arrays_max = file.getint(section, "arrays-max")
                if(self.mode == "multitags"):
                    self.multitags_min = file.getint(section, "multitags-min")
                    self.multitags_max = file.getint(section, "multitags-max")
                if(file.has_option(section, "bench-all")):
                    self.bench_all = file.getboolean(section, "bench-all")
                if(file.has_option(section, "bench-separate")):
                    self.bench_separate = file.getboolean(section, "bench-separate")
                if(file.has_option(section, "bench-rules")):
                    self.bench_rules = file.getboolean(section, "bench-rules")
                if(file.has_option(section, "bench-tester")):
                    self.bench_tester = file.getboolean(section, "bench-tester")
                if(file.has_option(section, "stl")):
                    self.stl = file.getboolean(section, "stl")
            except ConfigParser.NoOptionError:
                print "Error reading ini file. Aborting"
                sys.exit(2)
            self.gen_files()
            self.run_tests(section)
            self.finish_run()
        self.summarize(file.sections())

    def gen_files(self):
        cmd = ("./testgen.py -s " + str(self.sections) + " -t " + 
               str(self.tags_min) + " -T " + str(self.tags_max) + " -l " +
               str(self.tag_len_min) + " -L " + str(self.tag_len_max))
        if(self.mode == "arrays"):
            cmd += " -a " + str(self.arrays_min) + " -A " + str(self.arrays_max) + " -v "
            for type in self.types:
                fname = self.benchinidir + self.types_map[type] + "s.ini"
                self.files.append(fname)
                self.benchmarks.append("read" + self.types_map[type] + "s")
                runcmd(cmd + self.types_map[type] + "s" + " " + fname)
                self.benchsizes.append(os.path.getsize(fname))
        elif(self.mode == "multitags"):
            cmd += (" -a " + str(self.multitags_min) + " -A " + 
                    str(self.multitags_max) + " -v ")
            for type in self.types:
                fname = self.benchinidir + self.types_map[type] + "m.ini"
                self.files.append(fname)
                self.benchmarks.append("read" + self.types_map[type] + "m")
                runcmd(cmd + self.types_map[type] + "m" + " " + fname)
                self.benchsizes.append(os.path.getsize(fname))
        else:
            cmd += " -v "
            for type in self.types:
                fname = self.benchinidir + self.types_map[type] + ".ini"
                self.files.append(fname)
                self.benchmarks.append("read" + self.types_map[type])
                runcmd(cmd + " " + self.types_map[type] + " " + fname)
                self.benchsizes.append(os.path.getsize(fname))
                            
    def finish_run(self):
        for file in self.files:
            runcmd("rm " + file)
        self.files = []
        self.benchmarks = []
        self.benchsizes = []
        self.bench_all = self.bench_separate = self.bench_rules = self.bench_tester = False

    def run_tests(self, subdir):
        dir = os.path.join(self.basedir, subdir) 
        runcmd("rm -r -f " + dir)
        for target, benchbase in [("dbg", "bin/benchmark-dbg"), 
                                  ("opt", "bin/benchmark")]:
            if self.stl:
                benchbase += " --stl"
            targetdir = dir + "/" + target
            if(self.bench_all):
                self.tests.append("bench-all")
                benchstr = " '" + benchbase + " " + " ".join(self.benchmarks) + "' "
                fsize = 0
                for s in self.benchsizes:
                    fsize += s
                fsizestr = "filesize=" + str(fsize)
                runcmd(self.testbase + self.tools + " -f bench-all -o " + 
                       targetdir + benchstr + " " + fsizestr)
            if(self.bench_separate):
                for bench in range(len(self.benchmarks)):
                    self.tests.append("bench-" + self.benchmarks[bench])
                    benchstr = " '" + benchbase + " " + self.benchmarks[bench] + "' "
                    fsizestr = "filesize=" + str(self.benchsizes[bench])
                    runcmd(self.testbase + self.tools + " -f bench-" + self.benchmarks[bench] + 
                           " -o " + targetdir + benchstr + " " + fsizestr )
            if(self.bench_rules):
                self.tests.append("bench-readstringrules")
                benchstr = " '" + benchbase + " " + "readstringrules" + "' "
                runcmd(self.testbase + self.tools + " -f bench-readstringrules" +
                       " -o " + targetdir + benchstr)
            if(target == "dbg" and self.bench_tester):
                self.tests.append("tester")
                testerstr = " 'bin/tester' "
                runcmd(self.testbase + self.tools + " -f tester -o " + targetdir + testerstr)

    def summarize(self, dirs):
        cdin = "cd " + self.basedir
        cdout = "cd .."
        for target in ["opt", "dbg"]:
            targetdirs= []
            for dir in dirs:
                targetdirs.append(dir + "/" + target)
            runcmd(cdin + " && mkdir -p " + self.sumdir + "/" + target + "/ && " + cdout)
            for test in self.tests:
                cmd = "../../../testutil-merge.py -o "
                out = self.sumdir + "/" + target + "/" + test + ".sum.ini"
                cmd += out + " " + " ".join(targetdirs) + " " + test + ".sum.ini"
                runcmd(cdin + " && " + cmd + " && " + cdout)

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hf:c:", 
                     ["help", "file=", "compare="])
    except getopt.GetoptError:
        help()                          
        sys.exit(1)                               
    for opt, arg in opts:
        if opt in ("-h", "--help"):      
            help()                      
            sys.exit(0)
        elif opt in ("-f", "--file"):  
            if len(args) < 1:
                help()
                sys.exit(1)
            ini_file(arg, args[0])                      
        elif opt in ("-c", "--compare"):
            if len(args) < 2:
                help()
                sys.exit(1)
            compare(arg, args[0], args[1])

if __name__ == '__main__':
    main()
