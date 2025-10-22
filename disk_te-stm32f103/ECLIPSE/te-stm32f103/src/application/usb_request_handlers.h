#ifndef __USB_REQUEST_HANDLERS__
#define __USB_REQUEST_HANDLERS__

typedef uint8_t* (*REQUEST_ROUTINE)(uint16_t) ;
extern REQUEST_ROUTINE CopyRoutineTable[] ;

#endif /* __USB_REQUEST_HANDLERS__ */
