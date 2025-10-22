/*************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_glcd.c
 *    Description : Graphical LCD driver
 *
 *    History :
 *    1. Date        : 6, March 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 57747 $
 **************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include "LPC407x_8x.h"
#include "drv_glcd.h"
#include "board.h"

#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK          (C_GLCD_CLK_PER_LINE*C_GLCD_LINES_PER_FRAME*C_GLCD_REFRESH_FREQ)

extern uint32_t SDRAM_VRAM_BASE_ADDR;

#define LCD_VRAM_BASE_ADDR ((uint32_t)&SDRAM_VRAM_BASE_ADDR)
#define LCD_CURSOR_BASE_ADDR ((uint32_t)0x20088800)

static pFontType_t pCurrFont = NULL;
static LdcPixel_t TextColour;
static LdcPixel_t TextBackgndColour;

static uint32_t TextX_Pos = 0;
static uint32_t TextY_Pos = 0;

static uint32_t XL_Win = 0;
static uint32_t YU_Win = 0;
static uint32_t XR_Win = C_GLCD_H_SIZE-1;
static uint32_t YD_Win = C_GLCD_V_SIZE-1;

static uint32_t TabSize = TEXT_DEF_TAB_SIZE;

static uint32_t WindY_Size, WindX_Size;
static uint32_t CurrY_Size, CurrX_Size;
static uint32_t * pWind;
static uint32_t * pPix;

#define EXP_COEF -0.011

uint32_t testtbl[8] = {0x00000000, 0x00000001, 0x00000101,
                     0x00010001, 0x00000100, 0x00010100,
                     0x00010000, 0x00010101};
/*************************************************************************
 * Function Name: GLCD_Cursor_Cnfg
 * Parameters:
 *
 * Return: none
 *
 * Description: Configure the cursor
 *
 *************************************************************************/
void GLCD_Cursor_Cfg(int Cfg)
{
  LPC_LCD->CRSR_CFG = Cfg;
}
/*************************************************************************
 * Function Name: GLCD_Cursor_En
 * Parameters: cursor - Cursor Number
 *
 * Return: none
 *
 * Description: Enable Cursor
 *
 *************************************************************************/
void GLCD_Cursor_En(int cursor)
{
  LPC_LCD->CRSR_CTRL = ((cursor & 0x3)<<4) | (1<<0);
}

/*************************************************************************
 * Function Name: GLCD_Cursor_Dis
 * Parameters: None
 *
 * Return: none
 *
 * Description: Disable Cursor
 *
 *************************************************************************/
void GLCD_Cursor_Dis(int cursor)
{
  LPC_LCD->CRSR_CTRL &= ~(1<<0);
}

/*************************************************************************
 * Function Name: GLCD_Move_Cursor
 * Parameters: x - cursor x position
 *             y - cursor y position
 *
 * Return: none
 *
 * Description: Moves cursor on position (x,y). Negativ values are posible.
 *
 *************************************************************************/
void GLCD_Move_Cursor(int x, int y)
{
  if(0 <= x)
  {//no clipping
    LPC_LCD->CRSR_CLIP = LPC_LCD->CRSR_CLIP & ~(0x3F);
    LPC_LCD->CRSR_XY = (LPC_LCD->CRSR_XY & ~(0x3FF)) | (x & (0x3FF));
  }
  else
  {//clip x
    LPC_LCD->CRSR_CLIP = (LPC_LCD->CRSR_CLIP & ~(0x3F)) | ((-x) & (0x3F));
    LPC_LCD->CRSR_XY = LPC_LCD->CRSR_XY & ~(0x3FF);
  }

  if(0 <= y)
  {//no clipping
    LPC_LCD->CRSR_CLIP = LPC_LCD->CRSR_CLIP & ~(0x3F<<8);
    LPC_LCD->CRSR_XY = (LPC_LCD->CRSR_XY & ~(0x3FF<<16)) | (((y) & (0x3FF))<<16);
  }
  else
  {//clip y
    LPC_LCD->CRSR_CLIP = (LPC_LCD->CRSR_CLIP & ~(0x3F<<8)) | (((-y) & (0x3F))<<8);
    LPC_LCD->CRSR_XY = LPC_LCD->CRSR_XY & ~(0x3FF<<16);
  }
}

/*************************************************************************
 * Function Name: GLCD_Copy_Cursor
 * Parameters: pCursor - pointer to cursor conts image
 *             cursor - cursor Number (0,1,2 or 3)
 *                      for 64x64(size 256) pix cursor always use 0
 *             size - cursor size in words
 * Return: none
 *
 * Description: Copy Cursor from const image to LCD RAM image
 *
 *************************************************************************/
void GLCD_Copy_Cursor (const uint32_t *pCursor, int cursor, int size)
{
   uint32_t * pDst = (uint32_t *)LCD_CURSOR_BASE_ADDR;
   pDst += cursor*64;

   for(int i = 0; i < size ; i++) *pDst++ = *pCursor++;
}
/*************************************************************************
 * Function Name: GLCD_Init
 * Parameters: const uint32_t *pPain, const uint32_t * pPallete
 *
 * Return: none
 *
 * Description: GLCD controller init
 *
 *************************************************************************/
void GLCD_Init (pPic_t pPic, const uint32_t * pPallete)
{
uint32_t * pDst = (uint32_t *) LCD_VRAM_BASE_ADDR;

  LCD0_IOCON = 0x27;
  LCD1_IOCON = 0x27;
  LCD2_IOCON = 0x27;
  LCD3_IOCON = 0x27;
  LCD4_IOCON = 0x27;
  LCD5_IOCON = 0x27;
  LCD6_IOCON = 0x27;
  LCD7_IOCON = 0x27;
  LCD8_IOCON = 0x27;
  LCD9_IOCON = 0x27;
  LCD10_IOCON = 0x27;
  LCD11_IOCON = 0x27;
  LCD12_IOCON = 0x27;
  LCD13_IOCON = 0x27;
  LCD14_IOCON = 0x27;
  LCD15_IOCON = 0x27;
  LCD16_IOCON = 0x27;
  LCD17_IOCON = 0x27;
  LCD18_IOCON = 0x27;
  LCD19_IOCON = 0x27;
  LCD20_IOCON = 0x27;
  LCD21_IOCON = 0x27;
  LCD22_IOCON = 0x27;
  LCD23_IOCON = 0x27;
//  LCD_PWR_IOCON = 0x27;
  LCD_DCLK_IOCON = 0x27;
  LCD_VS_IOCON = 0x27;
  LCD_ENAB_IOCON = 0x27;
  LCD_HS_IOCON = 0x27;

  LCD_BL_E_IOCON = 0x20;
  LCD_BL_AJ_IOCON = 0x20;
  
  /*Back light enable*/
  LCD_BL_E_FDIR |= (1<<0);
  LCD_BL_E_FSET = (1<<0);
  LCD_BL_AJ_FDIR |= (1<<0);
  LCD_BL_AJ_FCLR = (1<<1);
  
  /* Init GLCD cotroller */
  LPC_SC->PCONP |= (1<<0);  /* enable LCD controller clock */
  GLCD_Cursor_Dis(0);       /* Disable cursor */
  LPC_LCD->CTRL = (0<<0) |  /* disable GLCD controller */
                  (5<<1) |  /* 24 bpp */
                  (1<<5) |  /* TFT panel */
                  (0<<7) |  /* single panel */
                  (0<<8) |  /* notmal output */
                  (0<<9) |  /* little endian byte order */
                  (0<<10)|  /* little endian pix order */
                  (0<<11);  /* disable power */
  /* init pixel clock */
  LPC_SC->LCD_CFG = SystemCoreClock / (uint32_t)C_GLCD_PIX_CLK;
  LPC_LCD->POL =  (1<<26)|  /* bypass inrenal clk divider */
                  (0<<5) |  /* clock source for the LCD block is HCLK */
                  (1<<11)|  /* LCDFP pin is active LOW and inactive HIGH */
                  (1<<12)|  /* LCDLP pin is active LOW and inactive HIGH */
                  (0<<13)|  /* data is driven out into the LCD on the rising edge */
                  (0<<14)|  /* active high */
                  ((C_GLCD_H_SIZE-1)<<16);
  /* init Horizontal Timing */
  LPC_LCD->TIMH = ((C_GLCD_H_BACK_PORCH - 1)<<24) |
                  ((C_GLCD_H_FRONT_PORCH - 1)<<16)|
                  ((C_GLCD_H_PULSE - 1)<<8)       |
                  (((C_GLCD_H_SIZE/16) - 1)<<2)   ;
  /* init Vertical Timing */
  LPC_LCD->TIMV = (C_GLCD_V_BACK_PORCH<<24) |
                  (C_GLCD_V_FRONT_PORCH<<16)|
                  (C_GLCD_V_PULSE<<10)      |
                  ((C_GLCD_V_SIZE - 1)<<0)  ;
  /* Frame Base Address doubleword aligned */
  LPC_LCD->UPBASE =  LCD_VRAM_BASE_ADDR & ~7UL ;
  LPC_LCD->LPBASE =  LCD_VRAM_BASE_ADDR & ~7UL ;
  /* init colour pallet */

  GLCD_Ctrl (TRUE);

  if(NULL != pPallete)
  {
    GLCD_SetPallet(pPallete);
  }

  if (NULL == pPic)
  {
    uint32_t pix = 0;
    /* Set test picture */
    for(uint32_t v = 0; (C_GLCD_V_SIZE) > v; v++)
    {
      pDst = (uint32_t *)LCD_VRAM_BASE_ADDR + v*C_GLCD_H_SIZE;
      uint32_t i = 0;

      for(uint32_t n = 0; 8 > n; n++)
      {
        pix = 0;
        for(/*empty*/ ; i < C_GLCD_H_SIZE*(n+1)/8;i++)
        { 
           *(pDst+i) = pix;
        }
      }
    }
  }
  else
  {
    // set display memory
    
   GLCD_ShowPic(0,0,pPic,0);
   /*
    for(uint32_t i = 0; (C_GLCD_H_SIZE * C_GLCD_V_SIZE) > i; i++)
    {
      *pDst++ = *pPain++;
    }*/
  }

  for(volatile uint32_t i = C_GLCD_ENA_DIS_DLY; i; i--);
}

/*************************************************************************
 * Function Name: GLCD_SetPallet
 * Parameters: const uint32_t * pPallete
 *
 * Return: none
 *
 * Description: GLCD init colour pallete
 *
 *************************************************************************/
void GLCD_SetPallet (const uint32_t * pPallete)
{
uint32_t * pDst = (uint32_t *)LPC_LCD->PAL;
  assert(pPallete);
  for (uint32_t i = 0; i < 128; i++)
  {
    *pDst++ = *pPallete++;
  }
}

/*************************************************************************
 * Function Name: GLCD_Ctrl
 * Parameters: uint32_t bEna
 *
 * Return: none
 *
 * Description: GLCD enable disabe sequence
 *
 *************************************************************************/
void GLCD_Ctrl (uint32_t bEna)
{
  if (bEna)
  {
    LPC_LCD->CTRL |= (1<<0);
    for(volatile uint32_t i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL |= (1<<11);   /* enable power */
  }
  else
  {
    LPC_LCD->CTRL &= ~(1<<11);   /* disable power */
    for(volatile uint32_t i = C_GLCD_PWR_ENA_DIS_DLY; i; i--);
    LPC_LCD->CTRL &= ~(1<<0);
  }
}

/*************************************************************************
 * Function Name: GLCD_SetFont
 * Parameters: pFontType_t pFont, LdcPixel_t Color
 *              LdcPixel_t BackgndColor
 *
 * Return: none
 *
 * Description: Set current font, font color and background color
 *
 *************************************************************************/
void GLCD_SetFont(pFontType_t pFont, LdcPixel_t Color, LdcPixel_t BackgndColor)
{
  pCurrFont = pFont;
  TextColour = Color;
  TextBackgndColour = BackgndColor;
}

/*************************************************************************
 * Function Name: GLCD_SetWindow
 * Parameters: uint32_t X_Left, uint32_t Y_Up,
 *             uint32_t X_Right, uint32_t Y_Down
 *
 * Return: none
 *
 * Description: Set draw window XY coordinate in pixels
 *
 *************************************************************************/
void GLCD_SetWindow(uint32_t X_Left, uint32_t Y_Up,
                    uint32_t X_Right, uint32_t Y_Down)
{
  assert(X_Right < C_GLCD_H_SIZE);
  assert(Y_Down < C_GLCD_V_SIZE);
  assert(X_Left < X_Right);
  assert(Y_Up < Y_Down);
  XL_Win = X_Left;
  YU_Win = Y_Up;
  XR_Win = X_Right;
  YD_Win = Y_Down;
}

/*************************************************************************
 * Function Name: GLCD_TextSetPos
 * Parameters: uint32_t X_UpLeft, uint32_t Y_UpLeft,
 *             uint32_t X_DownLeft, uint32_t Y_DownLeft
 *
 * Return: none
 *
 * Description: Set text X,Y coordinate in characters
 *
 *************************************************************************/
void GLCD_TextSetPos(uint32_t X, uint32_t Y)
{
  TextX_Pos = X;
  TextY_Pos = Y;
}

/*************************************************************************
 * Function Name: GLCD_TextSetTabSize
 * Parameters: uint32_t Size
 *
 * Return: none
 *
 * Description: Set text tab size in characters
 *
 *************************************************************************/
void GLCD_TextSetTabSize(uint32_t Size)
{
  TabSize = Size;
}

/*************************************************************************
 * Function Name: LCD_SET_WINDOW
 * Parameters: int c
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
static
void LCD_SET_WINDOW (uint32_t X_Left, uint32_t X_Right,
                     uint32_t Y_Up, uint32_t Y_Down)
{
  pPix = pWind = ((uint32_t *)LCD_VRAM_BASE_ADDR) + X_Left + (Y_Up*C_GLCD_H_SIZE);
  WindX_Size = X_Right - X_Left;
  WindY_Size = Y_Down - Y_Up;
  CurrX_Size = CurrY_Size = 0;
}

/*************************************************************************
 * Function Name: LCD_SET_WINDOW
 * Parameters: int c
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
static
void LCD_WRITE_PIXEL (uint32_t Pixel)
{
  *pPix++ = Pixel;
  if (++CurrX_Size > WindX_Size)
  {
    CurrX_Size = 0;
    if(++CurrY_Size > WindY_Size)
    {
      CurrY_Size = 0;
    }
    pPix = pWind + CurrY_Size * C_GLCD_H_SIZE;
  }
}

/*************************************************************************
 * Function Name: GLCD_TextCalcWindow
 * Parameters: uint32_t * pXL, uint32_t * pXR,
 *             uint32_t * pYU, uint32_t * pYD,
 *             uint32_t * pH_Size, uint32_t * pV_Size
 *
 * Return: uint32_t
 *          FALSE - out of window coordinate aren't valid
 *          TRUE  - the returned coordinate are valid
 *
 * Description: Calculate character window
 *
 *************************************************************************/
static
uint32_t GLCD_TextCalcWindow (uint32_t * pXL, uint32_t * pXR,
                             uint32_t * pYU, uint32_t * pYD,
                             uint32_t * pH_Size, uint32_t * pV_Size)
{
  *pH_Size = pCurrFont->H_Size;
  *pV_Size = pCurrFont->V_Size;
  *pXL = XL_Win + (TextX_Pos*pCurrFont->H_Size);
  if(*pXL > XR_Win)
  {
    return(FALSE);
  }
  *pYU = YU_Win + (TextY_Pos*pCurrFont->V_Size);
  if(*pYU > YD_Win)
  {
    return(FALSE);
  }

  *pXR   = XL_Win + ((TextX_Pos+1)*pCurrFont->H_Size) - 1;
  if(*pXR > XR_Win)
  {
  	*pH_Size -= *pXR - XR_Win;
    *pXR = XR_Win;
  }

  *pYD = YU_Win + ((TextY_Pos+1)*pCurrFont->V_Size) - 1;
  if(*pYD > YD_Win)
  {
    *pV_Size -= *pYD - YD_Win;
    *pYD = YD_Win;
  }

  return(TRUE);
}

/*************************************************************************
 * Function Name: putchar
 * Parameters: int c
 *
 * Return: none
 *
 * Description: Put char function
 *
 *************************************************************************/
int GLCD_putchar (int c)
{
uint8_t * pSrc;
uint32_t H_Line;
uint32_t xl,xr,yu,yd,Temp,V_Size, H_Size, SrcInc = 1;
uint32_t WhiteSpaceNumb;
  if(pCurrFont == NULL)
  {
    return(EOF);
  }
  H_Line = (pCurrFont->H_Size / 8) + ((pCurrFont->H_Size % 8)?1:0);
  switch(c)
  {
  case '\n':  // go to begin of next line (NewLine)
    ++TextY_Pos;
    break;
  case '\r':  // go to begin of this line (Carriage Return)
  	// clear from current position to end of line
  	while(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
  	{
      LCD_SET_WINDOW(xl,xr,yu,yd);
	    for(uint32_t i = 0; i < V_Size; ++i)
	    {
	      for(uint32_t j = 0; j < H_Size; ++j)
	      {
	        LCD_WRITE_PIXEL(TextBackgndColour);
	      }
	    }
  		++TextX_Pos;
  	}
    TextX_Pos = 0;
    break;
  case '\b': // go back one position (BackSpace)
    if(TextX_Pos)
    {
      --TextX_Pos;
      // del current position
	  	if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
	  	{
        LCD_SET_WINDOW(xl,xr,yu,yd);
		    for(uint32_t i = 0; i < V_Size; ++i)
		    {
		      for(uint32_t j = 0; j < H_Size; ++j)
		      {
		        LCD_WRITE_PIXEL(TextBackgndColour);
		      }
		    }
	  	}
    }
    break;
  case '\t':  // go to next Horizontal Tab stop
  	WhiteSpaceNumb = TabSize - (TextX_Pos%TabSize);
  	for(uint32_t k = 0; k < WhiteSpaceNumb; ++k)
  	{
      LCD_SET_WINDOW(xl,xr,yu,yd);
	  	if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
	  	{
		    for(uint32_t i = 0; i < V_Size; ++i)
		    {
		      for(uint32_t j = 0; j < H_Size; ++j)
		      {
		        LCD_WRITE_PIXEL(TextBackgndColour);
		      }
		    }
		    ++TextX_Pos;
	  	}
	  	else
	  	{
	  		break;
	  	}
  	}
    break;
  case '\f':  // go to top of page (Form Feed)
  	// clear entire window
  	H_Size = XR_Win - XL_Win;
  	V_Size = YD_Win - YU_Win;
    // set character window X left, Y right
    LCD_SET_WINDOW(XL_Win,XR_Win,YU_Win,YD_Win);
    // Fill window with background font color
    for(uint32_t i = 0; i <= V_Size; ++i)
    {
      for(uint32_t j = 0; j <= H_Size; ++j)
      {
        LCD_WRITE_PIXEL(TextBackgndColour);
      }
    }
  	
  	TextX_Pos = TextY_Pos = 0;
    break;
  case '\a':  // signal an alert (BELl)
    TEXT_BEL1_FUNC();
    break;
  default:
    // Calculate the current character base address from stream
    // and the character position
    if((c <  pCurrFont->CharacterOffset) &&
    	 (c >= pCurrFont->CharactersNuber))
   	{
   		c = 0;
    }
    else
    {
    	c -= pCurrFont->CharacterOffset;
    }
    pSrc = pCurrFont->pFontStream + (H_Line * pCurrFont->V_Size * c);
    // Calculate character window and fit it in the text window
    if(GLCD_TextCalcWindow(&xl,&xr,&yu,&yd,&H_Size,&V_Size))
    {
	    // set character window X left, Y right
	    LCD_SET_WINDOW(xl,xr,yu,yd);
	    // Send char data
	    for(uint32_t i = 0; i < V_Size; ++i)
	    {
        SrcInc = H_Line;
        for(uint32_t j = 0; j < H_Size; ++j)
	      {
	        Temp = (*pSrc & (1UL << (j&0x7)))?TextColour:TextBackgndColour;
	        LCD_WRITE_PIXEL(Temp);
	        if((j&0x7) == 7)
	        {
	          ++pSrc;
            --SrcInc;
	        }
	      }
        // next line of character
	      pSrc += SrcInc;
	    }
    }
    ++TextX_Pos;
  }
  return(c);
}

/*************************************************************************
 * Function Name: GLCD_print
 * Parameters: char *s, const char *fmt, ...
 *
 * Return: none
 *
 * Description: Print formated string on the LCD
 *
 *************************************************************************/
void GLCD_print (const char *fmt, ...)
{
va_list ap;
char s[MAX_GLCD_STR_SIZE];
char *p_char = s;
  va_start(ap, fmt);
  vsprintf(s, fmt,ap);
  va_end(ap);
  while(0 != *p_char)
  {
    if(-1 == GLCD_putchar(*p_char++))
    {
      break;
    }
  }
}
#define min(a,b)  (a<b?a:b)
/*************************************************************************
 * Function Name: GLCD_LoadPic
 * Parameters: bmp_t * pBmp
 *
 * Return: pPic_t
 *
 * Description: Read bmp pic and place it in SDRAM (currentrly supports
 *              only 24bits BMP)
 *
 *************************************************************************/
pPic_t GLCD_LoadPic (bmp_t * pBmp)
{
  
lcd_24bitpixel_t * pData; 
bmp_24bitpixel_t * pSrc;
uint32_t PicLineSize;
pPic_t pPic;
uint32_t X_size;
uint32_t Y_size;

  if(('B' != pBmp->str[0]) || ('M' != pBmp->str[1]))
  {
     return NULL;
  }

  if(NULL == (pPic = malloc(1*sizeof(Pic_t))))
  {
    return NULL;
  }
          
  pPic->H_Size = pBmp->dip_header.bmp_width;
  pPic->V_Size = pBmp->dip_header.bmp_height;
  pPic->BitsPP = pBmp->dip_header.bpp;
  pPic->BytesPP = sizeof(lcd_24bitpixel_t);
  
  pPic->pPalette = NULL;
  if(NULL ==  (pPic->pPicStream = malloc(pPic->H_Size*pPic->V_Size*sizeof(lcd_24bitpixel_t))))
  {
    free(pPic);
    return NULL;
  }
  pPic->pPicDesc = NULL;
    
  pData =  pPic->pPicStream;
  
  /*BMP Padding data*/
  PicLineSize = pBmp->dip_header.bmp_data_size/pBmp->dip_header.bmp_height;
  pSrc = (bmp_24bitpixel_t *)((uint32_t)pBmp + pBmp->pixarray_offset);
  pSrc = (bmp_24bitpixel_t *)((uint32_t)pSrc +(pBmp->dip_header.bmp_height-1)*PicLineSize);
  
  X_size = pPic->H_Size;
  Y_size = pPic->V_Size;
  

  for(uint32_t i = 0; i < Y_size; i++)
  { 
    for(uint32_t j = 0; j < X_size; j++,pData++,pSrc++)
    {
      pData->red = pSrc->red;
      pData->green = pSrc->green;
      pData->blue = pSrc->blue;
    }
    pSrc -= X_size;
    pSrc = (bmp_24bitpixel_t *)((uint32_t)pSrc - PicLineSize);
  }
  
  return pPic;
}

/*************************************************************************
 * Function Name: GLCD_DeletePic
 * Parameters: pPic_t * pPic
 *
 * Return: none
 *
 * Description: Free memory alocated for the picture pPic
 *
 *************************************************************************/
void GLCD_DeletePic (pPic_t pPic)
{
  free(pPic->pPicStream);
  free(pPic);
}

/*************************************************************************
 * Function Name: GLCD_FreePic
 * Parameters: 
 *
 * Return: none
 *
 * Description: 
 *
 *************************************************************************/
void GLCD_FreePic (pPic_t pPic)
{
  
  free(pPic->pPicStream);
  free(pPic);

}
     
/*************************************************************************
 * Function Name: GLCD_LoadPic
 * Parameters: uint32_t X_Left, uint32_t Y_Up,  pPic_t pPic, uint32_t Mask
 *
 * Return: none
 *
 * Description: Load picture in VRAM memory area
 *
 *************************************************************************/
void GLCD_ShowPic (int32_t X, int32_t Y, pPic_t pPic, uint32_t Mask)
{
uint32_t * pData = (uint32_t *)LCD_VRAM_BASE_ADDR;
uint32_t * pSrc = (uint32_t *)(pPic->pPicStream);                           
uint32_t X_size;
uint32_t Y_size;
  
  if(X >= 0)
  {
    pData += X;
    X_size = min(pPic->H_Size,C_GLCD_H_SIZE-X);
  }
  else
  {
    pSrc -= X;
    X_size = min(pPic->H_Size+X,C_GLCD_H_SIZE);
  }   
  
  if(Y >=0)
  {
    pData += (Y * C_GLCD_H_SIZE);
    Y_size = min(pPic->V_Size,C_GLCD_V_SIZE-Y);
  }
  else
  {
    pSrc -= Y*pPic->H_Size;
    Y_size = min(pPic->V_Size+Y,C_GLCD_V_SIZE);
  }

  for(uint32_t i = 0; i < Y_size; i++)
  { 
    for(uint32_t j = 0; j < X_size; j++,pData++,pSrc++)
    {
      *pData = *pSrc^Mask;
    }
    pData += C_GLCD_H_SIZE-X_size;
    pSrc += pPic->H_Size-X_size;
  }
}
