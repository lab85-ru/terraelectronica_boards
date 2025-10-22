// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <cassert>
#include <climits>

#include "typedefs.h"               
#include "globals.h"
#include "log.h"
#include "linetoken.h"
#include "inisection.h"
#include "util.h"

using namespace miniini_private;

ui INISection::temptagscap = 0;
c * * INISection::temptags = NULL;
ui INISection::tagcap = 0;
c * INISection::tagbuf = NULL;

/// @cond PRIVATE
///Perform given code 4 times. Used for manual loop unrolling.
#define FOR_4(instr) \
{\
  instr;\
  instr;\
  instr;\
  instr;\
}
/// @endcond

inline LineToken INISection::TagName(const c * & currentcharref, ui & tagsize)
{
    //ptr to the current character
    const c * currentchar = currentcharref;
    //current character
    register c ch;
    for(;;)
    {
        //reallocate header buffer if not enough space to add new chars
        //need 4 chars for unrolled part of the loop, 3 for trailing zeroes
        //since tag and value are stored in the same buffer separated by a zero
        //and the last tag is followed by two zeroes
        if(tagcap < tagsize + 7)
        {
            REALLOCATE(tagbuf, tagcap, tagsize, c);
        }
        //unrolled part of the loop (process 4 chars)
        FOR_4
        (
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
                    return LT_NAME;
                    break;
                }
                //header start found.
                case '[':
                {
                    currentcharref = currentchar;
                    return LT_HEADER;
                    break;
                }
                case '\0':
                {
                    currentcharref = currentchar;
                    return LT_NAME;
                    break;
                }
                default:
                {       
                    //value found, if tag name is not empty, start reading val
                    if(ch == namevalsep)
                    {
                        if(tagsize)
                        {
                            //adding trailing zero to the tag name
                            tagbuf[tagsize] = 0;
                            ++tagsize;
                            currentcharref = currentchar + 1;
                            return LT_VAL;
                        }
                        else
                        {
                            WARNING("Empty tag name.");
                            currentcharref = NextLine(currentchar);
                            return LT_NAME;
                        }
                    } 
                    if(ch == comment)
                    {
                        currentcharref = NextLine(currentchar);
                        return LT_NAME;
                    }
                    //add new char to tag name.
                    tagbuf[tagsize] = ch;
                    ++tagsize;
                    break;
                }     
            }
            ++currentchar;
        }
        )
    }
}

inline ui INISection::TagValue(const c * & currentcharref, ui tagsize)
{
    //ptr to the current character
    const c * currentchar = currentcharref;
    //current character
    register c ch;
    //reading characters of value
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
                return tagsize;
                break;
            }
            case '\0':
            {
                currentcharref = currentchar;
                return tagsize;
                break;
            }
            case ',':
            {
                //need 2 more chars for trailing 0s
                if(tagcap < tagsize + 3)
                {
                    REALLOCATE(tagbuf, tagcap, tagsize, c);
                }
                //add new char to value
                tagbuf[tagsize] = '\0';
                ++tagsize;
                break;
            }
            default:
            {
                if(ch == comment)
                {
                    currentcharref = NextLine(currentchar);
                    return tagsize;
                }
                //need 2 more chars for trailing 0s
                if(tagcap < tagsize + 3)
                {
                    REALLOCATE(tagbuf, tagcap, tagsize, c);
                }
                //add new char to value
                tagbuf[tagsize] = ch;
                ++tagsize;
            }
        }
    }
}

inline bool INISection::Header(const c * & currentcharref)
{
    //ptr to the current character
    const c * currentchar = currentcharref;
    //current character
    register c ch;
    //Name doesn't skip [ so we skip it here
    ++currentchar;
    //searching for first newline
    for(; ; ++currentchar)
    {
        ch = *currentchar;
        switch(ch)
        {
            //CR, LF
            case 10:
            case 13:
            {
                //not a header
                currentcharref = NextLine(currentchar);
                return false;
                break;
            }
            case ']':
            {
                //if empty, ignore
                return static_cast<bool>(currentchar - currentcharref);
                break;
            }
            case '\0':
            {
                //not a header
                currentcharref = currentchar;
                return false;
                break;
            }
            default:
            {
                if(ch == comment)
                {
                    currentcharref = NextLine(currentchar);
                    return false;
                }
                break;
            }
        }
    }
}

void INISection::Init(const c * const sectionname, const c * * const currentcharptr, 
                      Allocator * const alloc)
{
    assert(sectionname);
    assert(alloc);
    assert(currentcharptr);
    assert(*currentcharptr);
    assert(temptags);
    assert(tagbuf);
    //Copying allocator ptr
    Alloc = alloc;
    //Copying name of the section
    const ui namelen = strlen(sectionname) + 1;
    assert(namelen > 1);
    Name = Alloc->NewSpace(namelen);
    memcpy(Name, sectionname, namelen);
    //ptr to the current character in buffer
    const c * currentchar = *currentcharptr;
    //Iterating through lines in the buffer
    while(*currentchar != '\0')
    {
        //size of tag name (if any) read
        ui namesize = 0;
        //goes to start of next line if nothing found, 
        //if value found, stops right after name=value separator,
        //if header found, stays at beginning of line
        LineToken token = TagName(currentchar, namesize);
        //value found (this line is a name=value pair)
        if(token == LT_VAL)
        {
            //size of name and value
            ui tagsize = TagValue(currentchar, namesize);
            //value is empty
            if(namesize == tagsize)
            {
                WARNING("Empty value in a tag (no characters after name=value " 
                        "separator. Ignoring. Section: %s", Name);
                continue;
            }
            //adding trailing zero to the tag value
            tagbuf[tagsize] = tagbuf[tagsize + 1] = 0;
            tagsize += 2;
            //if needed, reallocate the temp tags buffer
            if(Length >= temptagscap)
            {
                REALLOCATE(temptags, temptagscap, Length, c *);
            }
            //Add new tag to temp tags buffer. 
            c * newtag = Alloc->NewSpace(tagsize);
            memcpy(newtag, tagbuf, tagsize * sizeof(c));
            Insert(temptags, Length, newtag);
            ++Length;
        }
        //header found
        else if(token == LT_HEADER)
        {
            //if this line is a header, we're finished loading the section
            if(Header(currentchar))
            {
                break;
            }
            //else Header() leaves currentchar at the start of next line
        }
    }
    //Updating line token ptr of the caller
    *currentcharptr = currentchar;
    //There are no tags in the section
    if(!Length)
    {
        WARNING("Empty section. Section: %s", Name);
    }
    //Copy temp lines buffer to Lines
    Tags = new c * [Length];
    memcpy(Tags, temptags, Length * sizeof(c *));
}

INISection::~INISection()
{
    //There is no need to tell allocator to delete data here,
    //since this is only called on INIFile destruction and that's
    //when Allocator gets destroyed and deletes the data anyway.
    //However, if/when section deletion is implemented, there
    //will need to be a separate dtor to handle that case.
    if(Tags)
    {
        delete [] Tags;
    }
}

inline bool INISection::ReadString(const char * const name, const char * & out) const
{
    assert(name);
    i pos = BinarySearch(Tags, Length, name);
    if(pos >= 0)
    {
        out = Tags [pos] + strlen(Tags[pos]) + 1;
        return true;
    }
    return false;
}

bool INISection::ReadInt(const char * const name, int & out) const
{
    assert(name);
    //Points to char where strtol finished processing. Used to check for errors
    c * tail;
    errno = 0;
    const c * valstr;
    //Requested tag does not exist
    if(!ReadString(name, valstr))
    {
        return false;
    }
    const long tempout = strtol(valstr, &tail, 0);
    const c tailc = tail[0];
    if(tailc == valstr[0])
    {
        ERROR("Non-integer value in a tag where integer is expected."
              "Section: %s Tag: %s Value: %s", Name, name, valstr);
        return false;
    }
    if(errno)
    {
        //this is an error because in my tests strtol returned -1 on overflow.
        ERROR("Integer value out of range."
              "Section: %s Tag: %s Value: %s", Name, name, valstr);
        return false;
    }
    #ifdef INI_DEBUG
    if(tailc)
    {
        WARNING("Redunant characters in a tag where integer is "
                "expected. Reading integer."
                "Section: %s Tag: %s Value: %s", Name, name, valstr);
    }
    #endif
    out = static_cast<int>(tempout);
    return true;
}

bool INISection::ReadFloat(const char * const name, float & out) const
{
    assert(name);
    //Points to char where strtof finished processing. Used to check for errors
    c * tail;
    errno = 0;
    const c * valstr;
    //Requested tag does not exist
    if(!ReadString(name, valstr))
    {
        return false;
    }
    const float tempout = strtof(valstr, &tail);
    const c tailc = tail[0];
    if(tailc == valstr[0])
    {
        ERROR("Non-float value in a tag where float is expected."
              "Section: %s Tag: %s Value: %s", Name, name, valstr);
        return false;
    }
    if(errno)
    {
        ERROR("Float value out of range."
              "Section: %s Tag: %s Value: %s", Name, name, valstr);
        return false;
    }
    #ifdef INI_DEBUG
    if(tailc)
    {
        WARNING("Redunant characters in a tag where float is "
                "expected. Reading float."
                "Section: %s Tag: %s Value: %s", Name, name, valstr);
    }
    #endif
    out = tempout;
    return true;
}

bool INISection::ReadBool( const char * const name, bool & out) const
{
    assert(name);
    const c * valstr;
    //Requested tag does not exist
    if(!ReadString(name, valstr))
    {
        return false;
    }
    //Parsing bool using the first character only
    switch(valstr[0])
    {
        case 't':
        case 'T':
        case 'y':
        case 'Y':
        case '1':
            out = true;
            return true;
        case 'f':
        case 'F':
        case 'n':
        case 'N':
        case '0':
            out = false;
            return true;
        default:
            return false;
    }
    return false;
}

inline ui INISection::ParseInts(const c * * strings, int * out, const ui numstrings)
{
    //Ptr to current string in strings
    const c * const * str = strings;
    //When string reaches this, we've iterated over all the strings
    const c * const * maxstr = strings + numstrings;
    //Number of actual valid ints written to out
    unsigned elems = 0;
    //Points to char where strtol finished processing. Used to check for errors
    c * tail;
    //Iterating through strings read by ReadStrings and converting them to ints
    for(; str < maxstr; ++str)
    {    
        errno = 0;
        const long tempelem = strtol(*str, &tail, 0);
        const c tailc = tail[0];
        if(tailc == *str[0])
        {
            ERROR("Non-integer value in an array or multi value tag where "
                  "integer is expected. Terminating array / multi value"
                  "reading. Value: %s", *str);
            break;
        }
        if(errno)
        {
            //this is an error because in some of my tests strtol returned -1 on overflow.
            ERROR("Integer value in an array or multi value tag out of range."
                  "Terminating array / multi value reading. Value: %s", *str);
            break;
        }
        #ifdef INI_DEBUG
        if(tailc)
        {
            WARNING("Redunant characters in a tag where integer is expected. "
                    "Reading integer. Value: %s", *str);
        }
        #endif
        out[elems] = static_cast<int>(tempelem);   
        ++elems;
    }
    return elems;
}

inline ui INISection::ParseFloats(const c * * strings, float * out, const ui numstrings)
{
    //Ptr to current string in strings
    const c * const * string = strings;
    //When string reaches this, we've iterated over all the strings
    const c * const * const maxstring = strings + numstrings;
    //Number of actual valid floats written to out
    unsigned elems = 0;
    //Points to char where strtof finished processing. Used to check for errors
    c * tail;
    //Iterating through strings read by ReadStrings and converting them to floats
    for(; string < maxstring; ++string)
    {    
        errno = 0;
        const f tempelem = strtof(*string, &tail);
        const c tailc = tail[0];
        if(tailc == *string[0])
        {
            ERROR("Non-float value in an array tag where float is expected. "
                  "Terminating array reading. Value: %s", *string);
            break;
        }
        if(errno)
        {
            ERROR("Float value in an array tag out of range. Terminating array "
                  "reading. Value: %s", *string);
            break;
        }
        #ifdef INI_DEBUG
        if(tailc)
        {
            WARNING("Redunant characters in a tag where float is expected. "
                    "Reading float. Value: %s", *string);
        }
        #endif
        out[elems] = tempelem;   
        ++elems;
    }
    return elems;
}

inline ui INISection::ParseBools(const c * * strings, bool * out, const ui numstrings)
{
    //Ptr to current string in strings
    const c * const * string = strings;
    //When string reaches this, we've iterated over all the strings
    const c * const * const maxstring = strings + numstrings;
    //Number of actual valid bools written to out
    unsigned elems = 0;
    //Iterating through strings read by ReadStrings and converting them to bools
    for(; string < maxstring; ++string, ++elems)
    {
        switch(*string[0])
        {
            case 't':
            case 'T':
            case 'y':
            case 'Y':
            case '1':
                out[elems] = true;
                break;
            case 'f':
            case 'F':
            case 'n':
            case 'N':
            case '0':
                out[elems] = false;
                break;
            default:
                goto BREAK_FOR;
        }
    }
    BREAK_FOR:;
    return elems;
}

unsigned INISection::MultiValSize(const char * const name) const
{
    assert(name);
    //ptr to current position in tag
    const c * tag;
    //Number of elements in the tag
    ui elems = 0;  
    i pos = BinarySearch(Tags, Length, name);
    if(pos >= 0)
    {
        //get to the start of the first value
        tag = Tags[pos];
        while(*tag != '\0')
        {
            ++tag;
        }
        ++tag;
        //now we're at the first char of value
        while(*tag != '\0')
        {
            ++elems;
            while(*tag != '\0')
            {
                ++tag;
            }
            ++tag;
            //now we're at the char after the trailing zero of previous value
        }
        return static_cast<unsigned>(elems);
    }
    return 0;
}

unsigned INISection::ReadMultiString(const char * const name, const char * * out, 
                                     const unsigned cap) const
{
    assert(name);
    assert(out);
    //ptr to current position in tag
    const c * tag;
    //Number of elements in the tag
    ui elems = 0;  
    i pos = BinarySearch(Tags, Length, name);
    //tag exists
    if(pos >= 0)
    {
        //get to the start of the first value
        tag = Tags[pos];
        while(*tag != '\0')
        {
            ++tag;
        }
        ++tag;
        //now we're at the first char of value
        while(*tag != '\0')
        {
            //out of capacity, don't read any more values
            if(elems == cap)
            {
                WARNING("Multi value tag element out of capacity."
                        "Section: %s Tag name: %s Capacity: %d", Name, name, cap);
                break;
            }
            //add element
            out[elems] = tag;
            ++elems;
            //get to the zero after the current value
            while(*tag != '\0')
            {
                ++tag;
            }
            //go to the next character. if it's 0, we're at the end of the tag, 
            //otherwise, there is another value.
            ++tag;
            //now we're at the char after the trailing zero of previous value
        }
        return static_cast<unsigned>(elems);
    }
    return 0;

}

unsigned INISection::ReadMultiInt(const char * const name, int * out, 
                                  const unsigned cap) const
{
    assert(name);
    assert(out);
    //Array of value strings to be converted to ints
    const c * * const valstrs = new const c * [cap];
    //Number of strings read by ReadStrings to valstrs
    const ui tempelems = ReadMultiString(name, valstrs, cap);
    ui output = ParseInts(valstrs, out, tempelems);
    delete [] valstrs;
    return static_cast<unsigned>(output);
}

unsigned INISection::ReadMultiFloat(const char * const name, float * out, 
                                    const unsigned cap) const
{
    assert(name);
    assert(out);
    //Array of value strings to be converted to floats
    const c * * const valstrs = new const c * [cap];
    //Number of strings read by ReadStrings to valstrs
    const ui tempelems = ReadMultiString(name, valstrs, cap);
    ui output = ParseFloats(valstrs, out, tempelems);
    delete [] valstrs;
    return static_cast<unsigned>(output);
}

unsigned INISection::ReadMultiBool(const char * const name, bool * out, 
                                   const unsigned cap) const
{
    assert(name);
    assert(out);
    //Array of value strings to be converted to floats
    const c * * const valstrs = new const c * [cap];
    //Number of strings read by ReadStrings to valstrs
    const ui tempelems = ReadMultiString(name, valstrs, cap);
    ui output = ParseBools(valstrs, out, tempelems);
    delete [] valstrs;
    return static_cast<unsigned>(output);
}

#ifndef INI_NO_STL
unsigned INISection::ReadMultiString(const std::string & name, 
                                     std::vector<std::string> & out) const
{
    const char * cname = name.c_str();
    unsigned numelems = MultiValSize(cname);
    //Read data to this array first
    const char * * tempstrs = new const char * [numelems];
    numelems = ReadMultiString(cname, tempstrs, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui str = 0; str < numelems; ++str)
    {
        out.push_back(std::string(tempstrs[str]));
    }
    delete [] tempstrs;
    return numelems;
}

unsigned INISection::ReadMultiInt(const std::string & name, 
                                  std::vector<int> & out) const
{
    const char * cname = name.c_str();
    unsigned numelems = MultiValSize(cname);
    //Read data to this array first
    int * tempints = new int [numelems];
    numelems = ReadMultiInt(cname, tempints, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui in = 0; in < numelems; ++in)
    {
        out.push_back(tempints[in]);
    }
    delete [] tempints;
    return numelems;
}

unsigned INISection::ReadMultiFloat(const std::string & name, 
                                   std::vector<float> & out) const
{
    const char * cname = name.c_str();
    unsigned numelems = MultiValSize(cname);
    //Read data to this array first
    float * tempfloats = new float [numelems];
    numelems = ReadMultiFloat(cname, tempfloats, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui fl = 0; fl < numelems; ++fl)
    {
        out.push_back(tempfloats[fl]);
    }
    delete [] tempfloats;
    return numelems;
}

unsigned INISection::ReadMultiBool(const std::string & name, 
                                   std::vector<bool> & out) const
{
    const char * cname = name.c_str();
    unsigned numelems = MultiValSize(cname);
    //Read data to this array first
    bool * tempbools = new bool [numelems];
    numelems = ReadMultiBool(cname, tempbools, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui bo = 0; bo < numelems; ++bo)
    {
        out.push_back(tempbools[bo]);
    }
    delete [] tempbools;
    return numelems;
}
#endif

unsigned INISection::ArraySize(const char * const name) const
{
    assert(name);
    ui namelen = strlen(name);
    //ptr to current tag
    const c * tag;
    //Points to char where strtol finished processing. Used to check for errors
    c * tail;
    //Index of current array element
    ui elemidx;  
    ui indicescap = 32;
    ui indicessize = 0;
    //Stores indices of found elements
    ui * indices = new ui [indicescap];
    for(ui idx = 0; idx < Length; ++idx)
    {
        tag = Tags[idx];
        //if tag name starts by name
        if(!strncmp(name, tag, namelen))
        {
            errno = 0;
            elemidx = strtol(tag + namelen, &tail, 0);    
            //Note: If tag contains no integer, elemidx will be set to 0 which 
            //will never be read.
            //Redunant chars after int - probably a different tag name or error 
            if(tail[0])
            {
                WARNING("Redunant characters after integer in tag name. "
                        "Ignoring. Section: %s Tag name: %s", Name, tag);
                continue;
            }
            #ifdef INI_DEBUG
            //Huge (out of range) integer in tag name- 
            //Probably a different tag name or error
            if(errno == ERANGE)
            {
                WARNING("Integer in tag name out of range. Ignoring. "
                        "Section: %s Tag name: %s", Name, tag);
                continue;
            }
            #endif
            //elemidx can be 0 if tag contains no integer
            if(elemidx)
            {
                //Reallocating indices if out of space
                if(indicessize >= indicescap)
                {
                    REALLOCATE(indices, indicescap, indicessize, ui);
                }
                //Correct index, counting this array element
                indices[indicessize] = elemidx - 1;
                ++indicessize;
            }
        }
    }
    ui elems = 0;
    bool finished = false;
    //Count number of consecutive indices
    while(!finished)
    {
        finished = true;
        for(ui idx = 0; idx < indicessize; ++idx)
        {
            if(indices[idx] == elems)
            {
                ++elems;
                finished = false;
            }
        }
    }
    return static_cast<unsigned>(elems);
}

unsigned INISection::ReadStrings(const char * const name, const char * * out, 
                                 const unsigned cap) const
{
    assert(name);
    assert(out);
    ui namelen = strlen(name);
    //we fill out with zeroes, so we can search for null ptr to determine how 
    //many elements we've read
    memset(out, 0, sizeof(const char *) * cap);
    //ptr to current tag
    const c * tag;
    //Points to char where strtol finished processing. Used to check for errors
    c * tail;
    //Index of current array element
    ui elemidx;  
    for(ui idx = 0; idx < Length; ++idx)
    {
        tag = Tags[idx];
        //if tag name starts by name
        if(!strncmp(name, tag, namelen))
        {
            errno = 0;
            elemidx = strtol(tag + namelen, &tail, 0);    
            //Note: If tag contains no integer, elemidx will be set to 0 which 
            //will never be read.
            //Redunant chars after int - probably a different tag name or error 
            if(tail[0])
            {
                WARNING("Redunant characters after integer in tag name. "
                        "Ignoring. Section: %s Tag name: %s", Name, tag);
                continue;
            }
            #ifdef INI_DEBUG
            //Huge (out of range) integer in tag name- 
            //Probably a different tag name or error
            if(errno == ERANGE)
            {
                WARNING("Integer in tag name out of range. Ignoring. "
                        "Section: %s Tag name: %s", Name, tag);
                continue;
            }
            #endif
            //Out of capacity  
            if(elemidx > cap)
            {
                WARNING("Array element out of capacity."
                        "Section: %s Tag name: %s Capacity: %d", Name, tag, cap);
                continue;
            }
            //elemidx can be 0 if tag contains no integer
            if(elemidx)
            {
                //Correct index at the end of name, reading array element
                out[elemidx - 1] = (tag) + strlen(tag) + 1;
            }
        }
    }
    //return number of valid elements read 
    //(if, say, elem 19 is missing, only elems 0-18 are valid)
    for(unsigned elem = 0; elem < cap; ++elem)
    {
        if(!out[elem])
        {
            return elem;
        }
    }
    return cap;
}

unsigned INISection::ReadInts(const char * const name, int * out, 
                              const unsigned cap) const
{
    assert(name);
    assert(out);
    //Array of value strings to be converted to ints
    const c * * const valstrs = new const c * [cap];
    //Number of strings read by ReadStrings to valstrs
    const ui tempelems = ReadStrings(name, valstrs, cap);
    ui output = ParseInts(valstrs, out, tempelems);
    delete [] valstrs;
    return static_cast<unsigned>(output);
}       

unsigned INISection::ReadFloats(const char * const name, float * out, 
                                const unsigned cap) const
{
    assert(name);
    assert(out);
    //Array of value strings to be converted to floats
    const c * * const valstrs = new const c * [cap];
    //Number of strings read by ReadStrings to valstrs
    const ui tempelems = ReadStrings(name, valstrs, cap);
    ui output = ParseFloats(valstrs, out, tempelems);
    delete [] valstrs;
    return static_cast<unsigned>(output);
}     

unsigned INISection::ReadBools(const char * const name, bool * out, 
                               const unsigned cap) const
{
    assert(name);
    assert(out);
    //Array of value strings to be converted to bools 
    const c * * const valstrs = new const c * [cap];
    //Number of strings read by ReadStrings to valstrs
    const ui tempelems = ReadStrings(name, valstrs, cap);
    ui output = ParseBools(valstrs, out, tempelems);
    delete [] valstrs;
    return static_cast<unsigned>(output);
}

#ifndef INI_NO_STL
unsigned INISection::ReadStrings(const std::string & name, 
                                 std::vector<std::string> & out) const
{
    const char * cname = name.c_str();
    unsigned numelems = ArraySize(cname);
    //Read data to this array first
    const char * * tempstrs = new const char * [numelems];
    numelems = ReadStrings(cname, tempstrs, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui str = 0; str < numelems; ++str)
    {
        out.push_back(std::string(tempstrs[str]));
    }
    delete [] tempstrs;
    return numelems;
}

unsigned INISection::ReadInts(const std::string & name, 
                                 std::vector<int> & out) const
{
    const char * cname = name.c_str();
    unsigned  numelems = ArraySize(cname);
    //Read data to this array first
    int * tempints = new int [numelems];
    numelems = ReadInts(cname, tempints, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui in = 0; in < numelems; ++in)
    {
        out.push_back(tempints[in]);
    }
    delete [] tempints;
    return numelems;
}

unsigned INISection::ReadFloats(const std::string & name, 
                                 std::vector<float> & out) const
{
    const char * cname = name.c_str();
    unsigned  numelems = ArraySize(cname);
    //Read data to this array first
    float * tempfloats = new float [numelems];
    numelems = ReadFloats(cname, tempfloats, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui fl = 0; fl < numelems; ++fl)
    {
        out.push_back(tempfloats[fl]);
    }
    delete [] tempfloats;
    return numelems;
}

unsigned INISection::ReadBools(const std::string & name, 
                                 std::vector<bool> & out) const
{
    const char * cname = name.c_str();
    unsigned numelems = ArraySize(cname);
    //Read data to this array first
    bool * tempbools = new bool [numelems];
    numelems = ReadBools(cname, tempbools, numelems);
    out.reserve(numelems);
    //Move data to output vector
    for(ui bo = 0; bo < numelems; ++bo)
    {
        out.push_back(tempbools[bo]);
    }
    delete [] tempbools;
    return numelems;
}
#endif
