/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_glcd.h
 *    Description : Graphical LCD driver include file
 *
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 57747 $
 **************************************************************************/
#include "arm_comm.h"
#include "bmp.h"

#ifndef __GLCD_DRV_H
#define __GLCD_DRV_H

typedef struct 
{
  uint8_t   red;
  uint8_t   green;
  uint8_t   blue;
  uint8_t   :8;
} lcd_24bitpixel_t;

typedef struct _Pic_t {
  uint32_t  H_Size;
  uint32_t  V_Size;
  uint32_t  BitsPP;
  uint32_t  BytesPP;
  uint32_t * pPalette;
  lcd_24bitpixel_t * pPicStream;
  uint8_t *  pPicDesc;
} Pic_t, *pPic_t;


typedef struct _FontType_t {
  uint32_t H_Size;
  uint32_t V_Size;
  uint32_t CharacterOffset;
  uint32_t CharactersNuber;
  uint8_t * pFontStream;
  uint8_t * pFontDesc;
} FontType_t, *pFontType_t;

typedef uint32_t LdcPixel_t, *pLdcPixel_t;


#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           640
#define C_GLCD_H_PULSE          10 
#define C_GLCD_H_FRONT_PORCH    16
#define C_GLCD_H_BACK_PORCH     134
#define C_GLCD_V_SIZE           480
#define C_GLCD_V_PULSE          2
#define C_GLCD_V_FRONT_PORCH    32
#define C_GLCD_V_BACK_PORCH     10
/*
#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           800
#define C_GLCD_H_PULSE          48
#define C_GLCD_H_FRONT_PORCH    88
#define C_GLCD_H_BACK_PORCH     41
#define C_GLCD_V_SIZE           480
#define C_GLCD_V_PULSE          3
#define C_GLCD_V_FRONT_PORCH    32
#define C_GLCD_V_BACK_PORCH     28
*/

#define C_GLCD_PWR_ENA_DIS_DLY  50000
#define C_GLCD_ENA_DIS_DLY      50000

#define CRSR_PIX_32     0
#define CRSR_PIX_64     1
#define CRSR_ASYNC      0
#define CRSR_FRAME_SYNC 2

#define TEXT_DEF_TAB_SIZE 5

#define TEXT_BEL1_FUNC()

#define MAX_GLCD_STR_SIZE       256

void GLCD_Init(pPic_t pPic, const uint32_t * pPallete);
void GLCD_SetPallet (const uint32_t * pPallete);
void GLCD_Ctrl (uint32_t bEna);
void GLCD_Cursor_Cfg(int Cfg);
void GLCD_Cursor_En(int cursor);
void GLCD_Cursor_Dis(int cursor);
void GLCD_Move_Cursor(int x, int y);
void GLCD_Copy_Cursor (const uint32_t *pCursor, int cursor, int size);
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor);
void GLCD_SetWindow(uint32_t X_Left, uint32_t Y_Up,
                    uint32_t X_Right, uint32_t Y_Down);
void GLCD_TextSetPos(uint32_t X, uint32_t Y);
void GLCD_TextSetTabSize(uint32_t Size);
static void LCD_SET_WINDOW (uint32_t X_Left, uint32_t X_Right,
                            uint32_t Y_Up, uint32_t Y_Down);
static void LCD_WRITE_PIXEL (uint32_t Pixel);
static uint32_t GLCD_TextCalcWindow (uint32_t * pXL, uint32_t * pXR,
                                    uint32_t * pYU, uint32_t * pYD,
                                    uint32_t * pH_Size, uint32_t * pV_Size);
pPic_t GLCD_LoadPic (bmp_t * pBmp);
void GLCD_DeletePic (pPic_t pPic);

void GLCD_ShowPic (int32_t X, int32_t Y, pPic_t pPic, uint32_t Mask);
void GLCD_FreePic (pPic_t pPic);
void GLCD_print (const char *fmt, ...);

#endif // __GLCD_DRV_H
