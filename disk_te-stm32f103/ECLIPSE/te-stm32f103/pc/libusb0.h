//---------------------------------------------------------------------------

#ifndef libusb0H
#define libusb0H

// libusb0 API
#include "usb.h"
#include <vector>
#include <string>

using namespace std ;
#include <string>

using namespace std ;

struct TCtrlSetup
{
 struct usb_ctrl_setup setup ;  // control_setup
 void* setup_data ;      // control data
} ;


class TLibUsb0
{
private:


        usb_bus* USB_bus ;  // связанный список структур шин и устройств
        usb_dev_handle *handle ; // дескриптор устройства
        int FBytes ;



public:		// User declarations

        unsigned short VID ;
        unsigned short PID ;

        unsigned int BulkInEP ;
        unsigned int BulkOutEP ;

        unsigned int IntInEP ;
        unsigned int IntOutEP ;

        unsigned int BusCount ;
        unsigned int DeviceCount ;
        unsigned int Timeout ;

        int Configuration;
        int AltInterface;

        TCtrlSetup* CtrlSetup ;
        vector <string>  DescriptorStrings ;

        TLibUsb0();
        ~TLibUsb0();

        void ScanUSB(void);

        //Device operations
        bool IsOpened();
        void Open();  // открытие устройств
        void Close(); // закрытие устройств
        void Claim(int interface); // захват интерфейса
        void Release(int interface); // сброс захвата интерфейса
        void ResetEndPoint ( unsigned int ep ) ;
        void ClearHaltEndPoint (unsigned int ep) ;
        void Reset();

        void SetConfiguration(int configuration);
        void SetAltInterface(int altinterface);

        // Control Transfers
        int ControlMsg () ;// Send a control message to a device
        int DescriptorString(int index, int langid, char* buff , int length );

        //Bulk Transfers
        int BulkWrite(char *bytes, int size); //Write data to a bulk endpoint
        int BulkRead (char *bytes, int size);  //Read data from a bulk endpoint

        //Interrupt Transfers
        int InterruptWrite(char *bytes, int size); // Write data to an interrupt endpoint
        int InterruptRead (char *bytes, int size);  // Read data from a interrupt endpoint
        int usb_get_descriptor(usb_dev_handle *dev, unsigned char type, unsigned char index, void *buf, int size);
        char* StrError();

};

#endif
