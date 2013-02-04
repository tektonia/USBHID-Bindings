#include "mbed.h"
#include "USBHID.h"

/*
 * HidTest.c
 *
 *  Program to test the UsbHid with HidTest.java
 *  Java files in USBHID bindings page
 *
 *  This test program is allways sending a non blocking message and expecting to read a non blocking message
 *  Messages are sent with the last byte cycling through '0'-'9' and LED4 toggles with each send
 *  When a messsage is received:
 *    LED2 toggles
 *    Received message is sent, blocking.
 *    A message is read blocking
 *    The recived message is now the new non blocking message to be sent
 *
 * victorix 04.02.2013
*/
uint16_t vendor_id=0x1798;
uint16_t product_id=0x2013;
uint16_t product_release=0x0001;
bool connect=true;
#define HID_LEN 32

USBHID hid(HID_LEN, HID_LEN, vendor_id, product_id, product_release, connect);
HID_REPORT hidOut;
HID_REPORT hidIn;
 
DigitalOut l2(LED2);
DigitalOut l4(LED4);

int main() {

  char s[HID_LEN]="Have a nice day :)";
  memcpy(hidOut.data,s,HID_LEN);
  hidOut.length=HID_LEN;
  
  hidIn.length=HID_LEN;
    
  while(1) {
    l4=!l4;                 //toggle Led4
    hid.sendNB(&hidOut);    //send non blocking
    //last byte used as a simple frame counter
    if(hidOut.data[HID_LEN-1]>='0' && hidOut.data[HID_LEN-1]<'9')
      hidOut.data[HID_LEN-1]++;
    else  hidOut.data[HID_LEN-1]='0';
    wait(1);                //wait 1s
    if(hid.readNB(&hidIn)){                  //read non blocking
      l2=!l2;                                //toggle LED2
      memcpy(hidOut.data, hidIn.data, hidIn.length);  //copy input to output
      hid.send(&hidOut);                     //send blocking
      wait(2);                               //wait 2s
      if(hid.read(&hidIn)){                  //read blocking
        memcpy(hidOut.data, hidIn.data, hidIn.length); //copy input to output
      } 
    }  
  }
}
