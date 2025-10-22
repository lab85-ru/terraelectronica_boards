/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : bmp.h
 *    Description : Bit Map Header file. Support of 24-bit BMP
 *
 *    History :
 *    1. Date        : June 2012
 *       Author      : Stoyan Choynev
 *       Description : Initial revision.
 *
 *    $Revision: 57747 $
 **************************************************************************/
#include <stdint.h>

#ifndef __BMP_H
#define __BMP_H

#pragma pack(1)

typedef struct
{
  uint32_t  info_header_size;
  int32_t   bmp_width;
  int32_t   bmp_height;
  uint16_t  planes;
  uint16_t  bpp;
  uint32_t  compression;        
  uint32_t  bmp_data_size;        
  uint32_t  h_res;        
  uint32_t  v_res;        
  uint32_t  palette_colors;        
  uint32_t  imp_colors;
} dip_header_t;

typedef struct 
{
  char      str[2];
  uint32_t  bmp_size;
  uint8_t   :8;
  uint8_t   :8;
  uint8_t   :8;
  uint8_t   :8;
  uint32_t  pixarray_offset;
  dip_header_t dip_header;
} bmp_t;

typedef struct 
{
  uint8_t   blue;
  uint8_t   green;
  uint8_t   red;
} bmp_24bitpixel_t;

#pragma pack()

#endif // __IAR_LOGO_H
