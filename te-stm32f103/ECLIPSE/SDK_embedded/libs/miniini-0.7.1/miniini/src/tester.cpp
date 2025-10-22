// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <cstdio>
#include <cstring>

#include <string>
#include <vector>
#include <iostream>

#include "../../miniini.h"

bool ReadStringNotHere(INISection * section)
{
    assert(section);
    const char * readstring;
    if(section->ReadString("nothere", readstring))
    {
        puts("Reading tag nothere= even though it's not there.");
        return false;
    }
    return true;
}

bool ReadString(INISection * section)
{
    assert(section);
    const char * readstring;
    if(!section->ReadString("valstring", readstring))
    {
        puts("Missing tag valstring= or error reading value");
        return false;
    }
    if(strcmp(readstring, "string"))
    {
        printf("Didn't read [%s] valstring= as \"string\" \n valstring was "
               "read as %s\n", section->GetName(), readstring);
        return false;
    }
    return true;
}

bool ReadInt(INISection * section)
{
    assert(section);
    int readint;
    if(!section->ReadInt("valint", readint))
    {
        puts("Missing tag valint= or error reading value");
        return false;
    }
    if(readint != -5)
    {
        printf("Didn't read [%s] valint= as -5 \n valint was read as %i\n"
                , section->GetName(), readint);
        return false;
    }
    return true;
}

bool ReadFloat(INISection * section)
{
    assert(section);
    float readfloat;
    if(!section->ReadFloat("valfloat", readfloat))
    {
        puts("Missing tag valfloat= or error reading value");
        return false;
    }
    if(readfloat != 1.5f)
    {
        printf("Didn't read [%s] valfloat= as 1.5 \n valfloat was read as %f\n"
                , section->GetName(), readfloat);
        return false;
    }
    return true;
}

bool ReadBool(INISection * section)
{
    assert(section);
    bool readbool;
    if(!section->ReadBool("valbool", readbool))
    {
        puts("Missing tag valbool= or error reading value");
        return false;
    }
    if(readbool != true)
    {
        printf("Didn't read [%s] valbool= as true \n valbool was read as %s\n"
                , section->GetName(), readbool ? "true" : "false");
        return false;
    }
    return true;
}

#ifndef INI_NO_STL
bool ReadStringSTL(INISection * section)
{
    assert(section);
    std::string readstring;
    if(!section->ReadString(std::string("valstring"), readstring))
    {
        puts("Missing tag valstring= or error reading value");
        return false;
    }
    if(readstring != "string")
    {
        printf("Didn't read [%s] valstring= as \"string\" \n valstring was "
               "read as %s\n", section->GetName(), readstring.c_str());
        return false;
    }
    return true;
}

bool ReadIntSTL(INISection * section)
{
    assert(section);
    int readint;
    if(!section->ReadInt(std::string("valint"), readint))
    {
        puts("Missing tag valint= or error reading value");
        return false;
    }
    if(readint != -5)
    {
        printf("Didn't read [%s] valint= as -5 \n valint was read as %i\n"
                , section->GetName(), readint);
        return false;
    }
    return true;
}

bool ReadFloatSTL(INISection * section)
{
    assert(section);
    float readfloat;
    if(!section->ReadFloat(std::string("valfloat"), readfloat))
    {
        puts("Missing tag valfloat= or error reading value");
        return false;
    }
    if(readfloat != 1.5f)
    {
        printf("Didn't read [%s] valfloat= as 1.5 \n valfloat was read as %f\n"
                , section->GetName(), readfloat);
        return false;
    }
    return true;
}

bool ReadBoolSTL(INISection * section)
{
    assert(section);
    bool readbool;
    if(!section->ReadBool(std::string("valbool"), readbool))
    {
        puts("Missing tag valbool= or error reading value");
        return false;
    }
    if(readbool != true)
    {
        printf("Didn't read [%s] valbool= as true \n valbool was read as %s\n"
                , section->GetName(), readbool ? "true" : "false");
        return false;
    }
    return true;
}
#endif

bool ReadMultiString(INISection * section)
{
    assert(section);
    int stringcount;
    if(!section->ReadInt("stringcount", stringcount))
    {
        puts("Missing tag stringcount= or error reading value");
        return false;
    }
    const char * * readstrings = new const char * [stringcount];
    int readcount = section->ReadMultiString("multistring", readstrings, stringcount);
    if(readcount != stringcount)
    {
	    printf("ReadMultiString didn't read stringcount= elements\n");
	    printf("stringcount= : %d elements read : %d\n", stringcount, readcount);
        return false;
    }
    if(strcmp(readstrings[2], "strc"))
    {
	    printf("ReadMultiString didn't read the third string in\n");
	    printf("multistring= as \"strc\" : was read as: %s\n", readstrings[2]);
    }
    delete [] readstrings;
    return true;
}

bool ReadMultiInt(INISection * section)
{
    assert(section);
    int intcount;
    if(!section->ReadInt("intcount", intcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    int * readints = new int [intcount];
    int readcount = section->ReadMultiInt("multiint", readints, intcount);
    if(readcount != intcount)
    {
	    printf("ReadMultiInt didn't read intcount= elements\n");
	    printf("intcount= : %d elements read : %d\n", intcount, readcount);
        return false;
    }
    delete [] readints;
    return true;
}

bool ReadMultiFloat(INISection * section)
{
    assert(section);
    int floatcount;
    if(!section->ReadInt("floatcount", floatcount))
    {
        puts("Missing tag floatcount= or error reading value");
        return false;
    }
    float * readfloats = new float [floatcount];
    int readcount = section->ReadMultiFloat("multifloat", readfloats, floatcount);
    if(readcount != floatcount)
    {
	    printf("ReadMultiFloat didn't read floatcount= elements\n");
	    printf("floatcount= : %d elements read : %d\n", floatcount, readcount);
        return false;
    }
    delete [] readfloats;
    return true;
}

bool ReadMultiBool(INISection * section)
{
    assert(section);
    int boolcount;
    if(!section->ReadInt("boolcount", boolcount))
    {
        puts("Missing tag boolcount= or error reading value");
        return false;
    }
    bool * readbools = new bool [boolcount];
    int readcount = section->ReadMultiBool("multibool", readbools, boolcount);
    if(readcount != boolcount)
    {
	    printf("ReadMultiBool didn't read boolcount= elements\n");
	    printf("boolcount= : %d elements read : %d\n", boolcount, readcount);
        return false;
    }
    delete [] readbools;
    return true;
}

#ifndef INI_NO_STL
bool ReadMultiStringSTL(INISection * section)
{
    assert(section);
    int stringcount;
    if(!section->ReadInt(std::string("stringcount"), stringcount))
    {
        puts("Missing tag stringcount= or error reading value");
        return false;
    }
    std::vector<std::string> readstrings;
    int readcount = section->ReadMultiString(std::string("multistring"), readstrings);
    if( readcount != stringcount)
    {
	    printf("ReadMultiStringSTL didn't read stringcount= elements\n");
	    printf("stringcount= : %d elements read : %d\n", stringcount, readcount);
        return false;
    }
    return true;
}

bool ReadMultiIntSTL(INISection * section)
{
    assert(section);
    int intcount;
    if(!section->ReadInt(std::string("intcount"), intcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    std::vector<int> readints;
    int readcount = section->ReadMultiInt(std::string("multiint"), readints);
    if( readcount != intcount)
    {
	    printf("ReadMultiIntSTL didn't read intcount= elements\n");
	    printf("intcount= : %d elements read : %d\n", intcount, readcount);
        return false;
    }
    return true;
}

bool ReadMultiFloatSTL(INISection * section)
{
    assert(section);
    int floatcount;
    if(!section->ReadInt(std::string("floatcount"), floatcount))
    {
        puts("Missing tag floatcount= or error reading value");
        return false;
    }
    std::vector<float> readfloats;
    int readcount = section->ReadMultiFloat(std::string("multifloat"), readfloats);
    if( readcount != floatcount)
    {
	    printf("ReadMultiFloatSTL didn't read floatcount= elements\n");
	    printf("floatcount= : %d elements read : %d\n", floatcount, readcount);
        return false;
    }
    return true;
}

bool ReadMultiBoolSTL(INISection * section)
{
    assert(section);
    int boolcount;
    if(!section->ReadInt(std::string("boolcount"), boolcount))
    {
        puts("Missing tag boolcount= or error reading value");
        return false;
    }
    std::vector<bool> readbools;
    int readcount = section->ReadMultiBool(std::string("multibool"), readbools);
    if( readcount != boolcount)
    {
	    printf("ReadMultiBoolSTL didn't read boolcount= elements\n");
	    printf("boolcount= : %d elements read : %d\n", boolcount, readcount);
        return false;
    }
    return true;
}
#endif

bool ReadStrings(INISection * section)
{
    assert(section);
    int stringcount;
    if(!section->ReadInt("stringcount", stringcount))
    {
        puts("Missing tag stringcount= or error reading value");
        return false;
    }
    const char * * readstrings = new const char * [stringcount];
    int readcount = section->ReadStrings("string", readstrings, stringcount);
    if( readcount != stringcount)
    {
	    printf("ReadStrings didn't read stringcount= elements\n");
	    printf("stringcount= : %d elements read : %d\n", stringcount, readcount);
        return false;
    }
    delete [] readstrings;
    return true;
}

bool ReadInts(INISection * section)
{
    assert(section);
    int intcount;
    if(!section->ReadInt("intcount", intcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    int * readints = new int [intcount];
    int readcount = section->ReadInts("int", readints, intcount);
    if( readcount != intcount)
    {
	    printf("ReadInts didn't read intcount= elements\n");
	    printf("intcount= : %d elements read : %d\n", intcount, readcount);
        return false;
    }
    delete [] readints;
    return true;
}

bool ReadFloats(INISection * section)
{
    assert(section);
    int floatcount;
    if(!section->ReadInt("floatcount", floatcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    float * readfloats = new float [floatcount];
    int readcount = section->ReadFloats("float", readfloats, floatcount);
    if( readcount != floatcount)
    {
	    printf("ReadFloats didn't read floatcount= elements\n");
	    printf("floatcount= : %d elements read : %d\n", floatcount, readcount);
        return false;
    }
    delete [] readfloats;
    return true;
}

bool ReadBools(INISection * section)
{
    assert(section);
    int boolcount;
    if(!section->ReadInt("boolcount", boolcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    bool * readbools = new bool [boolcount];
    int readcount = section->ReadBools("bool", readbools, boolcount);
    if( readcount != boolcount)
    {
	    printf("ReadBools didn't read boolcount= elements\n");
	    printf("boolcount= : %d elements read : %d\n", boolcount, readcount);
        return false;
    }
    delete [] readbools;
    return true;
}

#ifndef INI_NO_STL
bool ReadStringsSTL(INISection * section)
{
    assert(section);
    int stringcount;
    if(!section->ReadInt(std::string("stringcount"), stringcount))
    {
        puts("Missing tag stringcount= or error reading value");
        return false;
    }
    std::vector<std::string> readstrings;
    int readcount = section->ReadStrings(std::string("string"), readstrings);
    if( readcount != stringcount)
    {
	    printf("ReadStrings didn't read stringcount= elements\n");
	    printf("stringcount= : %d elements read : %d\n", stringcount, readcount);
        return false;
    }
    return true;
}

bool ReadIntsSTL(INISection * section)
{
    assert(section);
    int intcount;
    if(!section->ReadInt(std::string("intcount"), intcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    std::vector<int> readints;
    int readcount = section->ReadInts(std::string("int"), readints);
    if( readcount != intcount)
    {
	    printf("ReadInts didn't read intcount= elements\n");
	    printf("intcount= : %d elements read : %d\n", intcount, readcount);
        return false;
    }
    return true;
}

bool ReadFloatsSTL(INISection * section)
{
    assert(section);
    int floatcount;
    if(!section->ReadInt(std::string("floatcount"), floatcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    std::vector<float> readfloats;
    int readcount = section->ReadFloats(std::string("float"), readfloats);
    if( readcount != floatcount)
    {
	    printf("ReadFloats didn't read floatcount= elements\n");
	    printf("floatcount= : %d elements read : %d\n", floatcount, readcount);
        return false;
    }
    return true;
}

bool ReadBoolsSTL(INISection * section)
{
    assert(section);
    int boolcount;
    if(!section->ReadInt(std::string("boolcount"), boolcount))
    {
        puts("Missing tag intcount= or error reading value");
        return false;
    }
    std::vector<bool> readbools;
    int readcount = section->ReadBools(std::string("bool"), readbools);
    if( readcount != boolcount)
    {
	    printf("ReadBools didn't read boolcount= elements\n");
	    printf("boolcount= : %d elements read : %d\n", boolcount, readcount);
        return false;
    }
    return true;
}
#endif

bool TagCount(INISection * section)
{
    assert(section);
    int numtags;
    if(!section->ReadInt("numtags", numtags))
    {
        puts("Missing tag numtags= or error reading value");
        return false;
    }
    int tagsloaded = section->GetLength();
    if(numtags != tagsloaded)
    {
        printf("numtags= in doesn't match with number of tags actually "
               "loaded. numtags= : %d tags loaded : %d\n", numtags, tagsloaded);
        return false;
    }
    return true;
}

bool Spaces(INISection * section)
{
    assert(section);
    const char * spacesinaval;
    if(!section->ReadString("spacesinaname", spacesinaval))
    {
        puts("Missing tag spacesinaname= or error reading value");
        return false;
    }
    if(strcmp(spacesinaval, "spacesinaval"))
    {
        puts("Whitespaces do not seem to be ignored correctly");
        return false;
    }
    return true;
}

bool LongTag(INISection * section)
{
    assert(section);
    const char * longval;
    if(!section->ReadString("aridiculouslylongname", longval))
    {
        puts("Missing tag aridiculouslylongname= or error reading value");
        return false;
    }
    if(strcmp(longval, "anevenmoreridiculouslylongvalueofabsolutelyawesometestingvalueforminiini"))
    {
        puts("Long tags do not seem to be read correctly.");
        return false;
    }
    return true;
}

class TestSuite
{
    private:
        char * Section;
        bool(**Tests )(INISection *);
        char (* TestNames) [64];
        INIFile & INI;
        
    public:
        TestSuite(char * section, char (* testnames) [64],
                  bool(**tests )(INISection *), INIFile & ini)
            :Section(section)
            ,Tests(tests)
            ,TestNames(testnames)
            ,INI(ini)
        {
            assert(section);
        }

        bool Run()
        {
            bool success = true;
            printf("Starting run of test suite [%s]...\n", Section);
            INISection * section = INI.GetSection(Section);
            if(!section)
            {
                printf("ERROR: Missing section %s: can't run test suite\n",
                        Section);
                return false;
            }
            unsigned successes = 0;
            unsigned failures = 0;
            bool(**test)(INISection *) = Tests;
            char (* name) [64] = &TestNames[0];
            for(;*test; ++test, ++name)
            {
                puts("Running test:");
                puts(*name);
                if((*test)(section))
                {
                    ++successes;
                    puts("Test succeeded");
                }
                else
                {
                    success = false;
                    ++failures;
                    puts("Test FAILED");
                }
            }
            printf("Test suite %s: %u tests: Successes: %u Failures: %u\n\n", 
                   Section, successes + failures, successes, failures);
            return success;
        }

};

bool RunTests(INIFile & ini)
{         
    bool success = true;
    puts("Testing count of loaded sections...");
    INISection * section = ini.GetSection("[DEFAULT]");
    if(!section)
    {
        success = false;
        puts("ERROR: Default section was not read");
    }
    const char * def;
    if(!section->ReadString("default", def))
    {
        success = false;
        puts("ERROR: Can't read tag default= from the default section");
    }
    if(strcmp(def, "default"))
    {
        success = false;
        printf("ERROR: Didn't read default= from default section as \"default\""
               "\ndefault was read as %s\n", def);
    }
    section = ini.GetSection("SETTINGS");
    if(!section)
    {
        printf("ERROR: Missing section SETTINGS: can't run ANY test suite\n");
        return false;
    }
    int sections;
    if(!section->ReadInt("sections", sections))
    {
        puts("ERROR: Missing tag sections= in [SETTINGS] section or error "
             "reading value: can't run ANY test suite");
        return false;
    }
    int sectionsloaded = ini.GetLength();
    if(sections != sectionsloaded)
    {
        printf("ERROR: sections= doesn't match with number of sections "
               "loaded. sections=: %d Sections loaded: %d \ncan't run ANY "
               "test suite", sections, sectionsloaded);
        return false;
    }
    puts("Count of loaded sections test succeeded\n");
    
    char valtypes_name [] = "valtypes";

    bool(*valtypes_tests [])(INISection *) = {&ReadInt, &ReadFloat, &ReadBool, 
                                              &ReadString,
                                              &ReadStringNotHere
    #ifndef INI_NO_STL
                                              , &ReadIntSTL, 
                                              &ReadFloatSTL, &ReadBoolSTL, 
                                              &ReadStringSTL
    #endif                                        
                                              , NULL};
    char valtypes_testnames [] [64] = {"ReadInt", "ReadFloat", "ReadBool", 
                                       "ReadString",
                                       "ReadStringNotHere", "ReadIntSTL", 
                                       "ReadFloatSTL", "ReadBooSTL", 
                                       "ReadStringSTL"};

    char multivals_name [] = "multivals";

    bool(*multivals_tests [])(INISection *) = {&ReadMultiString, &ReadMultiInt, 
                                               &ReadMultiFloat, &ReadMultiBool
    #ifndef INI_NO_STL
                                               , &ReadMultiStringSTL, &ReadMultiIntSTL, 
                                               &ReadMultiFloatSTL, &ReadMultiBoolSTL
    #endif                                        
                                               , NULL};

    char multivals_testnames [] [64] = {"ReadMultiString", "ReadMultiInt", 
                                        "ReadMultiFloat", "ReadMultiBool",
                                        "ReadMultiStringSTL", "ReadMultiIntSTL", 
                                        "ReadMultiFloatSTL", "ReadMultiBoolSTL" };

    char arrays_name [] = "arrays";
    bool(*arrays_tests [])(INISection *) = {&ReadInts, &ReadFloats, &ReadBools,
                                            &ReadStrings
    #ifndef INI_NO_STL
                                            , &ReadIntsSTL, 
                                            &ReadFloatsSTL, &ReadBoolsSTL,
                                            &ReadStringsSTL
    #endif                                        
                                            , NULL};
    char arrays_testnames [] [64] = {"ReadInts", "ReadFloats", "ReadBools",
                                     "ReadStrings"
    #ifndef INI_NO_STL
                                     , "ReadIntsSTL", 
                                     "ReadFloatsSTL", "ReadBoolsSTL",
                                     "ReadStringsSTL"
    #endif                                        
                                     };
    char garbage_name [] = "garbage";
    bool(*garbage_tests [])(INISection *) = {&TagCount, &Spaces, &LongTag,  NULL};
    char garbage_testnames [] [64] = {"TagCount", "Spaces", "LongTag"};
        
    TestSuite ValTypes(valtypes_name, valtypes_testnames, valtypes_tests, ini);
    TestSuite MultiVals(multivals_name, multivals_testnames, multivals_tests, ini);
    TestSuite Arrays(arrays_name, arrays_testnames, arrays_tests, ini);
    TestSuite Garbage(garbage_name, garbage_testnames, garbage_tests, ini);
    if(!ValTypes.Run())
    {
        success = false;
    }
    if(!MultiVals.Run())
    {
        success = false;
    }
    if(!Arrays.Run())
    {
        success = false;
    }
    if(!Garbage.Run())
    {
        success = false;
    }
    return success;
}

void Log(const char * const log)
{
    std::cout << log << std::endl;
}

int main()
{
    char fname [] = "test/test.ini";
    puts("INI regression test: Loading test file:");
    puts(fname);
    INILogCallback(Log);
    INIFile ini;
    if(!ini.OpenFile(fname))
    {
        puts("ERROR:\nCould not open file");
    }
    if(!RunTests(ini))
    {
        return -1;
    }
    return 0;
}         
