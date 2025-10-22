/**
  ******************************************************************************
  * @file    demo_systeminfo.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Demo system information
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"
#include "DIALOG.h"

/* External variables --------------------------------------------------------*/
extern GUI_CONST_STORAGE GUI_BITMAP bmmicroSD[];
extern uint8_t microSD_OK;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_INFO   (GUI_ID_USER + 0x02)
#define ID_IMAGE_INFO      (GUI_ID_USER + 0x03)
#define ID_TEXT_BOARD       (GUI_ID_USER + 0x04)
#define ID_TEXT_CORE       (GUI_ID_USER + 0x05)
#define ID_TEXT_MCU       (GUI_ID_USER + 0x06)
#define ID_TEXT_CPU       (GUI_ID_USER + 0x07)
#define ID_TEXT_SDRAM       (GUI_ID_USER + 0x08)
#define ID_TEXT_VERSION       (GUI_ID_USER + 0x09)
#define ID_TEXT_COPYRIGHT       (GUI_ID_USER + 0x0A)
#define ID_BUTTON_INFO_CLOSE     (GUI_ID_USER + 0x0B)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Dialog resource using a WINDOW widget */
static const GUI_WIDGET_CREATE_INFO _aDialog[] = {
  { FRAMEWIN_CreateIndirect, "Show microSD card info", ID_FRAMEWIN_INFO, 0, 0, 320, 240-20, WM_CF_STAYONTOP, 0 },
  { IMAGE_CreateIndirect, "Image", ID_IMAGE_INFO, 210, 5, 90, 90, 0, 0, 0 },  
  { TEXT_CreateIndirect, "Card type:", ID_TEXT_BOARD, 5, 10, 200, 23, 0, 0, 0 },
  { TEXT_CreateIndirect, "Card Capacity:", ID_TEXT_MCU, 5, 50, 226, 30, 0, 0, 0 },
  { TEXT_CreateIndirect, "Card Block Size:", ID_TEXT_CORE, 5, 90, 226, 30, 0, 0, 0 },
  { BUTTON_CreateIndirect, "Close", ID_BUTTON_INFO_CLOSE, 125, 150, 80, 30, 0, 0, 0 }  
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Callback routine of the dialog
  * @param  pMsg: pointer to a data structure of type WM_MESSAGE
  * @retval None
  */
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int Id, NCode, mSD_cap;
  SD_CardInfo  mSD_Info;  
  static uint8_t TempStr[20];
  
 
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    

    /* Initialization of 'Show microSD card info' */
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_13HB_ASCII);
    
    /* Initialization of 'Image' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_INFO);
    IMAGE_SetBitmap(hItem, bmmicroSD);
    
    /* Initialization of 'Card type' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_BOARD);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);
    
    
    /* Initialization of 'Card Capacity' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_MCU);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);

 
    /* Initialization of 'Card Block Size' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_CORE);
    TEXT_SetFont(hItem, GUI_FONT_13HB_ASCII);
    TEXT_SetTextColor(hItem, 0x00804000);

    /* Initialization of 'Close' */
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_INFO_CLOSE);
    BUTTON_SetFont(hItem, GUI_FONT_13HB_ASCII);
    
    break;
    

    case WM_PAINT:

    if (SD_Detect() == SD_PRESENT | microSD_OK == 1)
      {
        SD_GetCardInfo(&mSD_Info);
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetColor(0x00703000);
        GUI_SetFont(GUI_FONT_13HB_ASCII);

        switch (mSD_Info.CardType)
        {
          case SDIO_STD_CAPACITY_SD_CARD_V1_1:
          GUI_DispStringAt("Standart SDCARD V1.1", 20, 30);
          break;

          case SDIO_STD_CAPACITY_SD_CARD_V2_0:
          GUI_DispStringAt("Standart SDCARD V2.0", 20, 30);
          break;

          case SDIO_HIGH_CAPACITY_SD_CARD:
          GUI_DispStringAt("HIGH CAPACITY SDCARD", 20, 30);
          break;
        } 
        mSD_cap = mSD_Info.CardCapacity/1048576;
        sprintf((char *)TempStr, "%d MByte", mSD_cap);
        GUI_DispStringAt((char *)TempStr, 20, 70);
        sprintf((char *)TempStr, "%d Byte", mSD_Info.CardBlockSize);
        GUI_DispStringAt((char *)TempStr, 20, 110);
      }
      else
      {
        GUI_SetTextMode(GUI_TM_TRANS);
        GUI_SetColor(0x00004080);
        GUI_SetFont(GUI_FONT_13HB_ASCII);
        GUI_DispStringAt("No card or no formating", 20, 30);
        GUI_DispStringAt("No card or no formating", 20, 70);
        GUI_DispStringAt("No card or no formating", 20, 110);
      }

    break;
    
    
      
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
    NCode = pMsg->Data.v;               /* Notification code */
    switch (NCode) {
    case WM_NOTIFICATION_RELEASED:      /* React only if released */
      switch (Id) {
      case ID_BUTTON_INFO_CLOSE:
        GUI_EndDialog(pMsg->hWin, 0);
        break;
      }
      break;
    case WM_NOTIFICATION_CHILD_DELETED:
      WM_NotifyParent(WM_GetParent(pMsg->hWin), 0x500);
      break;    
    }
    break;
    
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/**
  * @brief  DEMO_microSD
  * @param  hWin: Window handle
  * @retval None
  */
void DEMO_microSD(WM_HWIN hWin)
{
  GUI_CreateDialogBox(_aDialog, GUI_COUNTOF(_aDialog), _cbDialog, hWin, 0, 0);
}
/*************************** End of file ****************************/
