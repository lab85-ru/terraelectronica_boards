// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef INIFILE_H_INCLUDED
#define INIFILE_H_INCLUDED

#include "typedefs.h"
#include "allocator.h"
#include "inisection.h"
#include <cassert>
/// @cond PRIVATE

///Type of benchmark to perform. Self-explanatory.
enum BenchmarkType
{
    BT_STRING,
    BT_INT,
    BT_FLOAT,
    BT_BOOL,
    BT_MULTISTRING,
    BT_MULTIINT,
    BT_MULTIFLOAT,
    BT_MULTIBOOL,
    BT_STRINGS,
    BT_INTS,
    BT_FLOATS,
    BT_BOOLS
};
/// @endcond

///INI File
/**
 * Reads and stores data from an ini file and gives access to its sections.
 */
class INIFile
{
    friend class ReadMark;
    friend class INIFileSTL;

    private:

        ///Number of sections in the file.
        miniini_private::ui Length;
        ///Array of pointers to sections.
        INISection * * Sections;
        ///Allocator for character data of this file.
        miniini_private::Allocator * Alloc;


    public:

        ///Empty constructor.
        INIFile()
            :Length(0)
            ,Sections(NULL)
            ,Alloc(NULL)
        {}

        ///Loads given INI file.
        /**
		 * @note You can only call OpenFile on an INIFile that is not initialised, i.e. OpenFile or LoadBuffer was not called before.
         * @note File will be loaded even if it contains no ini sections, and even empty sections will be read.
         * @param fname Filename of the file to load. Must be a valid, zero terminated string.
         * @return true if the file is succesfully loaded.
         * @return false if the file does not exist, can not be accessed or is otherwise invalid.
         */
        bool OpenFile(const char * const fname);

        ///Loads given INI file. (STL version)
        /**
		 * @note You can only call LoadBuffer on an INIFile that is not initialised, i.e. OpenFile or LoadBuffer was not called before.
         * @note File will be loaded even if it contains no ini sections, and even empty sections will be read.
         * @param fname Filename of the file to load.
         * @return true if the file is succesfully loaded.
         * @return false if the file does not exist, can not be accessed or is otherwise invalid.
         */
        #ifndef INI_NO_STL
        bool OpenFile (const std::string & fname)
        {
            return OpenFile(fname.c_str());
        }
        #endif

        ///Loads INI file from given zero terminated buffer.
        /**
         * @note File will be loaded even if it contains no ini sections, and even empty sections will be read.
         * Useful e.g. for loading INI files from compressed files.
         * @param buf Buffer to load from. Must be zero terminated.
         * @param size Size of the buffer (including terminating zero).
		 * @return true if successfully loaded.
		 * @return false if there was an error and the buffer was not loaded.
         */
        bool LoadBuffer(const char * buf, unsigned size);

        ///Destructor.
        /**
         * Deletes loaded INI file (if any)
         */
        ~INIFile();

        ///Gets pointer to requested section.
        /**
         * @param name Name of the section to get. Must be a valid, zero terminated string.
         * @return pointer to requested section if the section exists.
         * @return NULL if the section does not exist.
         */
        INISection * GetSection(const char * const name) const;

        ///Loads given INI file. (STL version)
        /**
         * @param name Name of the section to get.
         * @return pointer to requested section if the section exists.
         * @return NULL if the section does not exist.
         */
        #ifndef INI_NO_STL
        INISection * GetSection(const std::string & name) const
        {
            assert(IsValid());
            return GetSection(name.c_str());
        }
        #endif

        ///[] operator : alias for GetSection().
        /**
         * @param name Name of the section to get. Must be a valid, zero terminated string.
         * @return pointer to requested section if the section exists.
         * @return NULL if the section does not exist.
         */
        INISection * operator [] (const char * const name) const
        {
            assert(IsValid());
            assert(name);
            return GetSection(name);
        }

        ///[] operator : alias for GetSection() (STL version).
        /**
         * @param name Name of the section to get.
         * @return pointer to requested section if the section exists.
         * @return NULL if the section does not exist.
         */
        #ifndef INI_NO_STL
        INISection * operator [] (const std::string & name) const
        {
            assert(IsValid());
            return GetSection(name);
        }
        #endif

        ///@return number of sections in the file.
        unsigned GetLength() const
        {
            return static_cast<unsigned>(Length);
        }

        ///@return true if this INIFile() is initialised, false otherwise.
        bool IsValid() const
        {
            //If any of these is 0/NULL, this INIFile was constructed
            //but not initialised.
            return Length && Sections && Alloc;
        }

    private:

        INIFile(const INIFile &);

        void operator = (const INIFile &);

        ///Perform specified benchmark on this file.
        /**
         * @param type Benchmark to perform.
         */
        void Benchmark(BenchmarkType type, bool stl);
};

#endif
