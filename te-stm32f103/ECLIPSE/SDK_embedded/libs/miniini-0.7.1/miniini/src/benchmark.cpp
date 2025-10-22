#include <cstdio>
#include <cstring>

#include "../../miniini.h"

/// @cond PRIVATE

class ReadMark
{
    public:
        ReadMark(int argc, char * argv [], bool stl)
        {
            for(int arg = 0; arg < argc; ++arg)
            {
                INIFile ini;
                if(!strcmp(argv[arg], "readstring"))
                {
                    if(!ini.OpenFile("test/benchmark/string.ini")) return;
                    else ini.Benchmark(BT_STRING, stl);
                }
                else if(!strcmp(argv[arg], "readint"))
                {
                    if(!ini.OpenFile("test/benchmark/int.ini")) return;
                    else ini.Benchmark(BT_INT, stl);
                }
                else if(!strcmp(argv[arg], "readfloat"))
                {
                    if(!ini.OpenFile("test/benchmark/float.ini")) return;
                    else ini.Benchmark(BT_FLOAT, stl);
                }
                else if(!strcmp(argv[arg], "readbool"))
                {
                    if(!ini.OpenFile("test/benchmark/bool.ini")) return;
                    else ini.Benchmark(BT_BOOL, stl);
                }
                else if(!strcmp(argv[arg], "readstringm"))
                {
                    if(!ini.OpenFile("test/benchmark/stringm.ini")) return;
                    else ini.Benchmark(BT_MULTISTRING, stl);
                }
                else if(!strcmp(argv[arg], "readintm"))
                {
                    if(!ini.OpenFile("test/benchmark/intm.ini")) return;
                    else ini.Benchmark(BT_MULTIINT, stl);
                }
                else if(!strcmp(argv[arg], "readfloatm"))
                {
                    if(!ini.OpenFile("test/benchmark/floatm.ini")) return;
                    else ini.Benchmark(BT_MULTIFLOAT, stl);
                }
                else if(!strcmp(argv[arg], "readboolm"))
                {
                    if(!ini.OpenFile("test/benchmark/boolm.ini")) return;
                    else ini.Benchmark(BT_MULTIBOOL, stl);
                }
                else if(!strcmp(argv[arg], "readstrings"))
                {
                    if(!ini.OpenFile("test/benchmark/strings.ini")) return;
                    else ini.Benchmark(BT_STRINGS, stl);
                }
                else if(!strcmp(argv[arg], "readints"))
                {
                    if(!ini.OpenFile("test/benchmark/ints.ini")) return;
                    else ini.Benchmark(BT_INTS, stl);
                }
                else if(!strcmp(argv[arg], "readfloats"))
                {
                    if(!ini.OpenFile("test/benchmark/floats.ini")) return;
                    else ini.Benchmark(BT_FLOATS, stl);
                }
                else if(!strcmp(argv[arg], "readbools"))
                {
                    if(!ini.OpenFile("test/benchmark/bools.ini")) return;
                    else ini.Benchmark(BT_BOOLS, stl);
                }
                else if(!strcmp(argv[arg], "readstringrules"))
                {
                    if(!ini.OpenFile("test/benchmark/rulesmd2.ini")) return;
                    else ini.Benchmark(BT_STRING, stl);
                }
            }
        }
};

void help()
{
    puts
    (
        "MiniINI benchmark\n"
        "Copyright (C) 2009-2010 Ferdinand Majerech\n"
        "Usage: bench [OPTIONS] [BENCHMARKS]\n"
        "Requires files to be present in test/benchmark for benchmarks to run\n"
        "Benchmarks and files they require:\n"
        " readstring         string.ini\n"
        " readint            int.ini\n"
        " readfloat          float.ini\n"
        " readbool           bool.ini\n"
        " readstringm        stringm.ini\n"
        " readintm           intm.ini\n"
        " readfloatm         floatm.ini\n"
        " readboolm          boolm.ini\n"
        " readstrings        strings.ini\n"
        " readints           ints.ini\n"
        " readfloats         floats.ini\n"
        " readbools          bools.ini\n"
        " readstringrules    rulesmd2.ini\n"
        "Options:\n"
        "    --stl           STL API will be benchmarked instead of cstdlib API.\n"
    );
}

int main(int argc, char * argv [])
{
    if(!strcmp(argv[1], "--stl"))
    {
        if(argc < 3)
        {
            help();
            return -1;
        }
        ReadMark(argc - 2, argv + 2, true);
    }
    else
    {
        if(argc < 2)
        {
            help();
            return -2;
        }
        ReadMark(argc - 1, argv + 1, false);
    }
    return 0;
}
/// @endcond
