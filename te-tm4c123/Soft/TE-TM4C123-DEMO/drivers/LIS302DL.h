//*****************************************************************************
//
// LIS302DL.h - Prototypes for the MEMS digital output motion sensor.
//
//*****************************************************************************

#ifndef __LIS302DL_H__
#define __LIS302DL_H__

//*****************************************************************************
//
// Register address map.
//
//*****************************************************************************
#define WHO_AM_I					0x0F
#define	CTRL_REG1					0x20
#define	CTRL_REG2					0x21
#define	CTRL_REG3					0x22
#define	HP_FILTER_RESET		0x23
#define STATUS_REG				0x27
#define	OUT_X							0x29
#define	OUT_Y							0x2B
#define	OUT_Z							0x2D
//*****************************************************************************
//
// Bit map CTRL_REG1
//
//*****************************************************************************
#define CTRL_REG1_Xen						0x01
#define CTRL_REG1_Yen						0x02
#define CTRL_REG1_Zen						0x04
#define CTRL_REG1_STM						0x08
#define CTRL_REG1_STP						0x10
#define CTRL_REG1_FS						0x20
#define CTRL_REG1_PD						0x40
#define CTRL_REG1_DR						0x80
//*****************************************************************************
//
// Bit map STATUS_REG
//
//*****************************************************************************
#define XDA						0x01
#define YDA						0x02
#define ZDA						0x04
#define ZYXDA					0x08
#define XOR						0x10
#define YOR						0x20
#define ZOR						0x40
#define ZYXOR					0x80
//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void LIS302DL_Init(void);
extern uint32_t LIS302DL_Read_Reg(uint8_t Adr_Reg);
extern void LIS302DL_Write_Reg(uint8_t Adr_Reg, uint8_t Data);
#endif // __LIS302DL_H__
