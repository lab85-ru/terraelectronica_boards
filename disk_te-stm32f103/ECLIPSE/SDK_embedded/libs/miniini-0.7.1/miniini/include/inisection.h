// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef INISECTION_H_INCLUDED
#define INISECTION_H_INCLUDED

#include <cstring>
#include <cstdlib>

#include "typedefs.h"
#include "allocator.h"
#include "linetoken.h"
#ifndef INI_NO_STL
#include <string>
#include <vector>
#endif


///Section of ini file.
/**
 * Stores tags of an ini file section.
 */
class INISection
{
    friend class INIFile;
    private:

        ///Name of the section. Taken from section header.
        miniini_private::c * Name;
        ///Length of the section, i.e. number of tags in it.
        miniini_private::ui Length;
        ///Tags in the section.
        /**
         * Each tag is stored as a char buffer containing tag name, a trailing
         * zero, tag value and another trailing zero.
         */
        miniini_private::c * * Tags;
        ///Allocator for character data in the section.
        miniini_private::Allocator * Alloc;


        ///Data that doesn't need to be reinitialized in every ctor call.
        ///Temp tags buffer capacity.
        static miniini_private::ui temptagscap;
        ///Temp tags buffer - stores tags during loading.
        static miniini_private::c * * temptags;
        ///Tag buffer capacity.
        static miniini_private::ui tagcap;
        ///Temp tag buffer - stores currently processed tag during loading.
        static miniini_private::c * tagbuf;

    public:

        ///@return name of this section.
        const char * GetName() const
        {
            return Name;
        }

        ///Read a string value from given tag.
        /**
         * @note Unlike other ReadXXX methods, ReadString only gives a
         * pointer to data inside the INISection. If you want to use this
         * data after INIFile is destroyed, you have to copy it.
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully.
         * @return false if the tag does not exist. (value was not read)
         */
        bool ReadString(const char * const name, const char * & out) const;

        ///Read a string value from given tag. (STL version)
        /**
         * Unlike non-STL ReadString, string data does get copied here
         * (using std::string constructor) so there is no need to copy
         * it manually.
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully.
         * @return false if the tag does not exist. (value was not read)
         */
        #ifndef INI_NO_STL
        bool ReadString(const std::string & name, std::string & out) const
        {
            const miniini_private::c * tempout;
            bool ret = ReadString(name.c_str(), tempout);
            if(ret)
            {
                out = tempout;
            }
            return ret;
        }
        #endif

        ///Read an int value from given tag.
        /**
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully
         * @return false if the tag does not exist (value was not read)
         */
        bool ReadInt(const char * const name, int & out) const;

        ///Read an int value from given tag. (STL version)
        /**
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully
         * @return false if the tag does not exist (value was not read)
         */
        #ifndef INI_NO_STL
        bool ReadInt(const std::string & name, int & out) const
        {
            return ReadInt(name.c_str(), out);
        }
        #endif

        ///Read a float value from given tag.
        /**
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully
         * @return false if the tag does not exist (value was not read)
         */
        bool ReadFloat(const char * const name, float & out) const;

        ///Read a float value from given tag. (STL version)
        /**
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully
         * @return false if the tag does not exist (value was not read)
         */
        #ifndef INI_NO_STL
        bool ReadFloat(const std::string & name, float & out) const
        {
            return ReadFloat(name.c_str(), out);
        }
        #endif

        ///Read a bool value from given tag.
        /**
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully
         * @return false if the tag does not exist (value was not read)
         */
        bool ReadBool(const char * const name, bool & out) const;

        ///Read a bool value from given tag. (STL version)
        /**
         * If the tag has multiple values, the first value is read.
         * @param name Name of the tag to read from.
         * @param out Reference to variable to read to. If tag not found, out won't be chaged.
         * @return true if the value was read succesfully
         * @return false if the tag does not exist (value was not read)
         */
        #ifndef INI_NO_STL
        bool ReadBool(const std::string & name, bool & out) const
        {
            return ReadBool(name.c_str(), out);
        }
        #endif

        ///Determine number of values in given tag.
        /**
         * @param name Name of tag. Must be a valid zero terminated string.
         * @return number of values in the tag.
         */
        unsigned MultiValSize(const char * const name) const;

        ///Read an array of strings from a multi value tag with given name.
        /**
         * Reads strings from comma separated values in the tag.
         * @note Unlike other ReadMultiXXX methods, ReadMultiString only gives a
         * pointer to data inside the INISection. If you want to use this
         * data after INIFile is destroyed, you have to copy it.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read.
         */
        unsigned ReadMultiString(const char * const name, const char * * out,
                                 const unsigned cap) const;

        ///Read an array of strings from a multi value tag with given name. (STL version)
        /**
         * Reads strings from comma separated values in the tag.
         * @note Elements read are appended to the given vector.
         * Unlike non-STL ReadMultiString, string data does get copied here
         * (using std::string constructor) so there is no need to copy
         * it manually.
         * @param name Name of the tag to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read.
         */
        #ifndef INI_NO_STL
        unsigned ReadMultiString(const std::string & name,
                                 std::vector<std::string> & out) const;
        #endif

        ///Read an array of ints from a multi value tag with given name.
        /**
         * Reads ints from comma separated values in the tag.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read.
         */
        unsigned ReadMultiInt(const char * const name, int * out,
                              const unsigned cap) const;

        ///Read an array of ints from a multi value tag with given name. (STL version)
        /**
         * Reads ints from comma separated values in the tag.
         * @note Elements read are appended to the given vector.
         * @param name Name of the tag to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read.
         */
        #ifndef INI_NO_STL
        unsigned ReadMultiInt(const std::string & name,
                              std::vector<int> & out) const;
        #endif

        ///Read an array of floats from a multi value tag with given name.
        /**
         * Reads floats from comma separated values in the tag.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read.
         */
        unsigned ReadMultiFloat(const char * const name, float * out,
                                const unsigned cap) const;

        ///Read an array of floats from a multi value tag with given name. (STL version)
        /**
         * Reads floats from comma separated values in the tag.
         * @note Elements read are appended to the given vector.
         * @param name Name of the tag to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read.
         */
        #ifndef INI_NO_STL
        unsigned ReadMultiFloat(const std::string & name,
                                std::vector<float> & out) const;
        #endif

        ///Read an array of bools from a multi value tag with given name.
        /**
         * Reads bools from comma separated values in the tag.
         * @param name Name of the tag to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read.
         */
        unsigned ReadMultiBool(const char * const name, bool * out,
                               const unsigned cap) const;

        ///Read an array of bools from a multi value tag with given name. (STL version)
        /**
         * Reads bools from comma separated values in the tag.
         * @note Elements read are appended to the given vector.
         * @param name Name of the tag to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read.
         */
        #ifndef INI_NO_STL
        unsigned ReadMultiBool(const std::string & name,
                               std::vector<bool> & out) const;
        #endif

        ///Determine size of array of tags with given base name.
        /**
         * Counts tags named name1=, name2=, etc. where name is the
         * given base name for the tags.
         * @param name Base name of tags in the array. Must be a valid zero terminated string.
         * @return number of elements in the array.
         */
        unsigned ArraySize(const char * const name) const;

        ///Read an array of strings with given base name.
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags.
         * If any array tag has multiple values, the first value is read.
         * @note Unlike other ReadXXXs methods, ReadStrings only gives a
         * pointer to data inside the INISection. If you want to use this
         * data after INIFile is destroyed, you have to copy it.
         * @note After a call to ReadStrings, contents of out after
         * out[return value] are undefined.
         * @param name Base name of the tags to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read.
         */
        unsigned ReadStrings(const char * const name, const char * * out,
                             const unsigned cap) const;

        ///Read an array of strings with given base name. (STL version)
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags. This version of ReadStrings will read
         * as many elements as there are in the ini file to given vector.
         * If any array tag has multiple values, the first value is read.
         * @note Elements read are appended to the given vector.
         * Unlike non-STL ReadString, string data does get copied here
         * (using std::string constructor) so there is no need to copy
         * it manually.
         * @param name Base name of the tags to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read.
         */
        #ifndef INI_NO_STL
        unsigned ReadStrings(const std::string & name,
                             std::vector<std::string> & out) const;
        #endif

        ///Read an array of ints with given base name.
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags.
         * If any array tag has multiple values, the first value is read.
         * @param name Base name of the tags to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read
         */
        unsigned ReadInts(const char * const name, int * out,
                          const unsigned cap) const;

        ///Read an array of ints with given base name. (STL version)
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags. This version of ReadInts will read
         * as many elements as there are in the ini file to given vector.
         * If any array tag has multiple values, the first value is read.
         * @note Elements read are appended to the given vector.
         * @param name Base name of the tags to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read
         */
        #ifndef INI_NO_STL
        unsigned ReadInts(const std::string & name,
                          std::vector<int> & out) const;
        #endif

        ///Read an array of floats with given base name.
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags.
         * in wrong order, e.g. name4= after name2= .
         * If any array tag has multiple values, the first value is read.
         * @param name Base name of the tags to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read
         */
        unsigned ReadFloats(const char * const name, float * out,
                            const unsigned cap) const;

        ///Read an array of floats with given base name. (STL version)
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags. This version of ReadFloats will read
         * as many elements as there are in the ini file to given vector.
         * If any array tag has multiple values, the first value is read.
         * @note Elements read are appended to the given vector.
         * @param name Base name of the tags to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read
         */
        #ifndef INI_NO_STL
        unsigned ReadFloats(const std::string & name,
                            std::vector<float> & out) const;
        #endif

        ///Read an array of bools with given base name.
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags.
         * If any array tag has multiple values, the first value is read.
         * @param name Base name of the tags to read from. Must be a valid zero terminated string.
         * @param out Pointer to buffer to read to.
         * @param cap Capacity of the buffer.
         * @return number of values read
         */
        unsigned ReadBools(const char * const name, bool * out,
                           const unsigned cap) const;

        ///Read an array of bools with given base name. (STL version)
        /**
         * Reads values from tags named name1=, name2=, etc. where name is the
         * given base name for the tags. This version of ReadBools will read
         * as many elements as there are in the ini file to given vector.
         * If any array tag has multiple values, the first value is read.
         * @note Elements read are appended to the given vector.
         * @param name Base name of the tags to read from.
         * @param out Reference to the vector to read to.
         * @return number of values read
         */
        #ifndef INI_NO_STL
        unsigned ReadBools(const std::string & name,
                           std::vector<bool> & out) const;
        #endif

        ///@return number of tags in the section.
        unsigned GetLength() const
        {
            return static_cast<unsigned>(Length);
        }

    private:

        ///Empty constructor.
        INISection()
            :Name(NULL)
            ,Length(0)
            ,Tags(NULL)
            ,Alloc(NULL)
        {}

        ///Destructor.
        ~INISection();

        ///Tries to read a tag name from line starting by given character pointer.
        /**
         * Updates character pointer and tag size according to data processed.
         * Tag name is read to static member tagbuf, which is reallocated if needed.
         * @param currentcharref Reference to pointer to current character in a buffer.
         * @param tagsize Reference to size of the tag.
         * @return LT_VAL if this is a valid tag with a value.
         * Current char ptr is updated to point to the first character after the '='
         * and tag size is updated to number of characters read to tagbuf.
         * @return LT_HEADER if this line contains a header. Current char ptr is updated
         * to point to '[', NOT after it.
         * @return LT_NAME otherwise. In this case the line contains nothing interesting
         * and we can ignore it. Current char ptr is updated to point to start of next
         * line.
         */
        static inline miniini_private::LineToken TagName(const miniini_private::c * & currentcharref,
                                                         miniini_private::ui & tagsizeref);

        ///Tries to read a tag value from line starting by given character pointer.
        /**
         * Updates character pointer to the start of the next line or end of the buffer,
         * and tag size according to data processed.
         * Tag value is read to static member tagbuf, which is reallocated if needed.
         * @param currentcharref Reference to pointer to current character in a buffer.
         * @param tagsize Reference to size of the tag.
         * @return size of tag buffer after value is read.
         */
        static inline miniini_private::ui TagValue(const miniini_private::c * & currentcharref,
                                                   miniini_private::ui tagsize);

        ///Determines if the line staring by given character pointer contains a header.
        /**
         * @param currentcharref Reference to pointer to current character in a buffer.
         * @returns true if header is found and not empty.
         * @returns false otherwise, and updates current char pointer to the next line.
         */
        static inline bool Header(const miniini_private::c * & currentcharref);

        ///Parses given strings as ints.
        /**
         * Will stop parsing when a non-int string is encountered.
         * @param strings Strings to parse.
         * @param out Buffer to output ints to.
         * @param numstrings Number of strings.
         * @return Number of parsed strings.
         */
        static inline miniini_private::ui ParseInts(const miniini_private::c * * strings,
                                                    int * out,
                                                    const miniini_private::ui numstrings);

        ///Parses given strings as floats.
        /**
         * Will stop parsing when a non-float string is encountered.
         * @param strings Strings to parse.
         * @param out Buffer to output floats to.
         * @param numstrings Number of strings.
         * @return Number of parsed strings.
         */
        static inline miniini_private::ui ParseFloats(const miniini_private::c * * strings,
                                                      float * out,
                                                      const miniini_private::ui numstrings);

        ///Parses given strings as bools.
        /**
         * Will stop parsing when a non-bool string is encountered.
         * @param strings Strings to parse.
         * @param out Buffer to output bools to.
         * @param numstrings Number of strings.
         * @return Number of parsed strings.
         */
        static inline miniini_private::ui ParseBools(const miniini_private::c * * strings,
                                                     bool * out,
                                                     const miniini_private::ui numstrings);

        ///Loads the section from given position in a raw ini file buffer.
        /*
         * Loads the section and changes currentcharptr so that the caller can
         * start with another section.
         * @param sectionname Name of the section to initialize
         * @param currentcharptr Start of the section in a raw ini file buffer.
         * @return true if the section is valid.
         * @return false if the section is empty.
         */
        void Init(const miniini_private::c * const sectionname,
                  const miniini_private::c * * const currentcharptr,
                  miniini_private::Allocator * const alloc);

        ///Allocates/initializes static data
        static void InitTempData()
        {
            assert(!temptags);
            assert(!tagbuf);
            temptagscap = 8;
            temptags = new miniini_private::c * [temptagscap];
            //MUST be over 6
            tagcap = 64;
            tagbuf = new miniini_private::c [tagcap];
        }

        ///Destroys static data
        static void DestroyTempData()
        {
            assert(temptags);
            assert(tagbuf);
            temptagscap = 0;
            delete [] temptags;
            temptags = NULL;
            tagcap = 0;
            delete [] tagbuf;
            tagbuf = NULL;
        }

        /// @cond PRIVATE

        ///ReadXXX benchmark. Read every element in the section as given type.
        #ifndef INI_NO_STL
        #define BENCH(method, type, stltype)\
        for(miniini_private::ui tag = 0; tag < Length; ++tag)\
        {\
            if(stl)\
            {\
                stltype out;\
                method(std::string(Tags[tag]), out);\
            }\
            else\
            {\
                type out;\
                method(Tags[tag], out);\
            }\
        }
        #else
        #define BENCH(method, type, stltype)\
        for(miniini_private::ui tag = 0; tag < Length; ++tag)\
        {\
            type out;\
            method(Tags[tag], out);\
        }
        #endif

        void BenchString(bool stl)
        {
            BENCH(ReadString, const char *, std::string);
        }

        void BenchInt(bool stl)
        {
            BENCH(ReadInt, int, int);
        }

        void BenchFloat(bool stl)
        {
            BENCH(ReadFloat, float, float);
        }

        void BenchBool(bool stl)
        {
            BENCH(ReadBool, bool, bool);
        }

        ///ReadMultiXXX benchmarks.
        ///Can't handle more than 1024 values in one tag.
        #ifndef INI_NO_STL
        #define BENCHMULTI(method, type, stltype)\
        for(miniini_private::ui tag = 0; tag < Length; ++tag)\
        {\
            if(stl)\
            {\
                std::vector<stltype> out;\
                method(std::string(Tags[tag]), out);\
            }\
            else\
            {\
                unsigned elemcount = 1024;\
                type * out = new type [elemcount];\
                method(Tags[tag], out, elemcount);\
                delete [] out;\
            }\
        }
        #else
        #define BENCHMULTI(method, type, stltype)\
        for(miniini_private::ui tag = 0; tag < Length; ++tag)\
        {\
            unsigned elemcount = 1024;\
            type * out = new type [elemcount];\
            method(Tags[tag], out, elemcount);\
            delete [] out;\
        }
        #endif

        void BenchMultiString(bool stl)
        {
            BENCHMULTI(ReadMultiString, const char *, std::string);
        }

        void BenchMultiInt(bool stl)
        {
            BENCHMULTI(ReadMultiInt, int, int);
        }

        void BenchMultiFloat(bool stl)
        {
            BENCHMULTI(ReadMultiFloat, float, float);
        }

        void BenchMultiBool(bool stl)
        {
            BENCHMULTI(ReadMultiBool, bool, bool);
        }


        ///ReadXXXs benchmarks. These require specific inifiles.
        ///Each section has to contain tags a=, b= , etc.
        ///that specify lengths of arrays with same base name,
        ///i.e. a=2 means there's going to be a1= and a2=.
        #ifndef INI_NO_STL
        #define BENCHARRAY(method, type, stltype)\
        for(char tagname [] = "a"; tagname[0] <= 'z'; ++(tagname[0]))\
        {\
            if(stl)\
            {\
                std::vector<stltype> out;\
                method(std::string(tagname), out);\
            }\
            else\
            {\
                int elemcount;\
                if(!ReadInt(tagname, elemcount))\
                {\
                    break;\
                }\
                type * out = new type [elemcount];\
                method(tagname, out, elemcount);\
                delete [] out;\
            }\
        }
        #else
        #define BENCHARRAY(method, type, stltype)\
        for(char tagname [] = "a"; tagname[0] <= 'z'; ++(tagname[0]))\
        {\
            int elemcount;\
            if(!ReadInt(tagname, elemcount))\
            {\
                break;\
            }\
            type * out = new type [elemcount];\
            method(tagname, out, elemcount);\
            delete [] out;\
        }
        #endif

        void BenchStrings(bool stl)
        {
            BENCHARRAY(ReadStrings, const char *, std::string);
        }

        void BenchInts(bool stl)
        {
            BENCHARRAY(ReadInts, int, int);
        }

        void BenchFloats(bool stl)
        {
            BENCHARRAY(ReadFloats, float, float);
        }

        void BenchBools(bool stl)
        {
            BENCHARRAY(ReadBools, bool, bool);
        }

        #undef BENCH
        #undef BENCHARRAY
        #undef BENCHMULTI
        /// @endcond

        ///No assignment or copyconstruction.

        INISection(const INISection &);

        void operator = (const INISection &);
};

#endif
