// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <cstdio>
#include <cstring>

#ifndef INI_NO_STL
#include <string>
#endif

#include "typedefs.h"
#include "globals.h"
#include "log.h"
#include "linetoken.h"
#include "inisection.h"
#include "inifile.h"
#include "util.h"

using namespace miniini_private;

bool INIFile::OpenFile(const char * const fname)
{
    assert(fname);
    assert(!IsValid());
    //opening file
    FILE * const file = fopen( fname, "rb" );
    if(file == NULL)
    {
        ERROR("Couldn't open file. Maybe it does not exist? File: %s",
              fname);
        return false;
    }
    //determining filesize
    i seek = fseek(file, 0, SEEK_END);
    if(seek != 0)
    {
        fclose(file);
        ERROR("Couldn't reach end of file. File: %s", fname);
        return false;
    }
    const i size = ftell(file);
    if(size == -1)
    {
        fclose(file);
        ERROR("Couldn't determine size of file. File: %s", fname);
        return false;
    }
    seek = fseek(file, 0, SEEK_SET);
    if(seek!=0)
    {
        fclose(file);
        ERROR("Couldn't reach start of file. File: %s", fname);
        return false;
    }
    //reading file to a buffer
    c * const buf = new c [size + 1];
    const i readnum = fread(buf, size, 1, file);
    //if 0 blocks succesfully read
    if(readnum == 0)
    {
        fclose(file);
        delete [] buf;
        ERROR("Could open but could not read from file. File might be "
              "corrupted or you might not have sufficient rights to "
              "read from it. File: %s", fname);
        return false;
    }
    //adding trailing zero to the buffer
    buf[size] = 0;
    bool out = LoadBuffer(buf, static_cast<unsigned>(size + 1));
    delete [] buf;
    fclose(file);
    return out;
}

///Tries to read a header from line starting by given character pointer.
/**
 * Updates character pointer to point to next line.
 * If needed, reallocates header buffer and updates its pointer and capacity.
 * @param currentcharref Reference to pointer to current character in a buffer.
 * @param header Reference to buffer where to read the header name (if any) to.
 * @param headercap Reference to the capacity of header buffer.
 * @return size of header or 0 if there is no header in the line or its size is 0 (which is invalid)
 */
static inline ui Header(const c * & currentcharref, c * & header, ui & headercap)
{
    //ptr to the current character
    const c * currentchar = currentcharref;
    //current character
    register c ch;
    //size of header in characters
    ui headersize = 0;
    //searching for [
    for(; ; ++currentchar)
    {
        ch = *currentchar;
        switch(ch)
        {
            case ' ':
            case '\t':
            {
                //ignore spaces
                break;
            }
            //CR, LF
            case 10:
            case 13:
            {
                currentcharref = NextLine(currentchar);
                return 0;
                break;
            }
            //header start found, we can start reading header
            case '[':
            {
                //move to next char
                ++currentchar;
                goto HEADER_START;
            }
            case '\0':
            {
                currentcharref = currentchar;
                return 0;
                break;
            }
            default:
            {
                //comment char
                if(ch == comment)
                {
                    currentcharref = NextLine(currentchar);
                    return 0;
                }
                break;
            }
        }
    }
    HEADER_START:;
    //searching for ] and reading header characters
    for(; ; ++currentchar)
    {
        ch = *currentchar;
        switch(ch)
        {
            case ' ':
            case '\t':
            {
                //ignore spaces
                break;
            }
            //CR, LF
            case 10:
            case 13:
            {
                currentcharref = NextLine(currentchar);
                return 0;
                break;
            }
            //header end found, we're finished reading
            case ']':
            {
                header[headersize] = 0;
                currentcharref = NextLine(currentchar);
                return headersize;
                break;
            }
            case '\0':
            {
                currentcharref = currentchar;
                return 0;
                break;
            }
            default:
            {
                if(ch == comment)
                {
                    currentcharref = NextLine(currentchar);
                    return 0;
                }
                //reallocate header buffer if not enough space to add new char
                //need 1 more char for trailing 0
                if(headercap < headersize + 2)
                {
                    REALLOCATE(header, headercap, headersize, c);
                }
                //add new char to header
                header[headersize] = ch;
                ++headersize;
                break;
            }
        }
    }
}

bool INIFile::LoadBuffer(const char * buf, unsigned size)
{
    assert(buf);
    assert(!IsValid());
    //Allocating memory for ini sections' strings in 16 blocks-
    //that results in a little speed overhead but potential memory
    //overhead of the allocator is decreased to a bit more than
    //1/16 of file size.
    Alloc = new Allocator(size, 16);
    //capacity of temporary buffer of pointers to sections
    ui tempsectionscap = 16;
    INISection::InitTempData();
    //temporary buffer of pointers to sections
    INISection * * tempsections = new INISection * [tempsectionscap];
    //ptr to the current character in buffer
    const c * currentchar = buf;
    //allocated capacity of headername
    ui headercap = 64;
    //buffer to load section header name to.
    c * headername = new c [headercap];
    //number of chars in headername
    ui headersize;
    //Loading the default INI section
    INISection * newsection = new INISection();
    newsection->Init("[DEFAULT]", &currentchar, Alloc);
    Insert(tempsections, Length, newsection);
    ++Length;
    //Iterating through lines in buffer, reading sections found
    while(*currentchar != '\0')
    {
        //Header() leaves currentchar at start of next line
        headersize = Header(currentchar, headername, headercap);
        //header found
        if(headersize)
        {
            //if temp section ptrs buffer runs out of space, reallocate it
            if(Length + 1 > tempsectionscap)
            {
                REALLOCATE(tempsections, tempsectionscap, Length, INISection *);
            }
            //Try to load new section
            newsection = new INISection();
            newsection->Init(headername, &currentchar, Alloc);
            //Insert new section
            Insert(tempsections, Length, newsection);
            ++Length;
        }
    }
    delete [] headername;
    if(!Length)
    {
        WARNING("Empty INI file/buffer.");
    }
    //copy pointers from tempsections to Sections and delete tempsections
    Sections = new INISection * [Length];
    memcpy(Sections, tempsections, Length * sizeof(INISection *));
    delete [] tempsections;
    INISection::DestroyTempData();
    //Remove any unused memory blocks from the allocator.
    Alloc->Trim();
    return true;
}

INIFile::~INIFile()
{
    if(!(Sections && Alloc))
    {
        return;
    }
    for(ui section = 0; section < Length; ++section)
    {
        delete Sections[section];
    }
    delete [] Sections;
    delete Alloc;
}

INISection * INIFile::GetSection(const char * const name) const
{
    assert(IsValid());
    assert(name);
    i sectionidx = BinarySearch(Sections, Length, name);
    if(sectionidx >= 0)
    {
        return Sections[sectionidx];
    }
    ERROR("Missing requested section. Section: %s", name);
    return NULL;
}

void INIFile::Benchmark(BenchmarkType type, bool stl)
{
    INISection * section;
    for(ui s = 0; s < Length; ++s)
    {
        if(stl)
        {
            section = GetSection(Sections[s]->Name);
        }
        #ifndef INI_NO_STL
        else
        {
            section = GetSection(std::string(Sections[s]->Name));
        }
        #endif
        switch(type)
        {
            case BT_STRING:
                section->BenchString(stl);
                break;
            case BT_INT:
                section->BenchInt(stl);
                break;
            case BT_FLOAT:
                section->BenchFloat(stl);
                break;
            case BT_BOOL:
                section->BenchBool(stl);
                break;
            case BT_MULTISTRING:
                section->BenchMultiString(stl);
                break;
            case BT_MULTIINT:
                section->BenchMultiInt(stl);
                break;
            case BT_MULTIFLOAT:
                section->BenchMultiFloat(stl);
                break;
            case BT_MULTIBOOL:
                section->BenchMultiBool(stl);
                break;
            case BT_STRINGS:
                section->BenchStrings(stl);
                break;
            case BT_INTS:
                section->BenchInts(stl);
                break;
            case BT_FLOATS:
                section->BenchFloats(stl);
            case BT_BOOLS:
                section->BenchBools(stl);
                break;
            default:
                ERROR("Unknown benchmark type");
                break;
        }
    }
}
