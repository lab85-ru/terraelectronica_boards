// Copyright (C) 2009-2010 Ferdinand Majerech
// This file is part of MiniINI
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#include "log.h"

namespace miniini_private
{

//No logging by default.
void(* __Log)(const char * const) = NULL;

}

void INILogCallback(void(* callback)(const char * const))
{
    miniini_private::__Log = callback;
}
