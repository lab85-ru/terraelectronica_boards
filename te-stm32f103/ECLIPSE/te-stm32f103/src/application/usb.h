#ifndef __USB_H__
#define __USB_H__

#include "appdefs.h"
#include "task.hpp"

#include "appdefs.h"
#include "task.hpp"

class TUsbTask : public TTask
{
  public:
  private:
     void Code() ;
};

extern TUsbTask* UsbTask ;

#endif /*__USB_H__*/
