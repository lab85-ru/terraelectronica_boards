// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "typedefs.h"

/// @cond PRIVATE
namespace miniini_private
{

///Single line comment character. ';' by default. Can be defined by user.
extern c comment;                                                   

///Separator char between tag and value. '=' by default. Can be defined by user.
extern c namevalsep;

///Characters parsed as line separators
/**
 * CR, LF, trailing zero
 */
extern c linesep[3];

}
/// @endcond

///Sets comment character 
/**
 * Default comment character is ';'. This function changes it to given character.
 * Comment cannot be set space, tab, carriage return, line feed, '[', ']', null
 * character or name=value separator (which is '=' by default).
 * @param comment_char New comment character.
 * @return true if comment character was set succesfully.
 * @return false if comment could not be set to this character.
 */
bool INISetComment(const char comment_char);

///Sets name=value separator character.
/**
 * Default separator character is '='. This function changes it to given 
 * character.
 * Separator cannot be set space, tab, carriage return, line feed, '[', ']', 
 * null character or comment character (which is ';' by default).
 * @param comment_char New separator character.
 * @return true if separator character was set succesfully.
 * @return false if separator could not be set to this character.
 */
bool INISetSeparator(const char sep);
#endif
