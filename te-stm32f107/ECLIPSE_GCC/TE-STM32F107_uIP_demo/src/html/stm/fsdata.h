/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: fsdata.h,v 1.4.2.1 2003/10/04 22:54:06 adam Exp $
 */
#ifndef __FSDATA_H__
#define __FSDATA_H__

#include "uipopt.h"

#define NULL (void*)0

typedef struct FSDATA_FILE{
  const struct FSDATA_FILE *next;
  const char *name;
  const char *data;
  const int len;
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
  u16_t count;
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
} fsdata_file ;



typedef struct  {
  struct fsdata_file *next;
  char *name;
  char *data;
  int len;
#ifdef FS_STATISTICS
#if FS_STATISTICS == 1
  u16_t count;
#endif /* FS_STATISTICS */
#endif /* FS_STATISTICS */
} fsdata_file_noconst ;

extern const fsdata_file file_404_html ;
extern const fsdata_file file_index_html ;
extern const fsdata_file file_STM32_Home_Webserver_Demo_files_st766_gif ;
extern const fsdata_file file_STM32_Home_Webserver_Demo_files_stm32_extends_gif ;
extern const fsdata_file file_STM32_LED_html ;
extern fsdata_file_noconst file_STM32_StatusBar_html ;

#define FS_ROOT file_STM32_StatusBar_html
#define FS_NUMFILES 8

#endif /* __FSDATA_H__ */
