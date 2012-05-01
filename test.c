
#include <stdio.h>
#include <strings.h>

#include "Prtcl_USB_HID_SHT.h"
#include "USB_HID_Def_SHT.h"

#define _PRODUCT_ID      0x10AA     // Target device's product ID
#define _SERIAL_NUMBER   "A2010037" // Target device's serial number
#define _TIMEOUT         20000      // in milliseconds


void main(void)
{
    long ret;
    COMMAND_SHT command;                // command message to send
    REPLY_SHT reply;                    // reply message to receive
    LIB_INFORMATION_SHT libinfo;        // info about the current SHT library
    unsigned char data[] = { 0x30, 0x30, 0x30,}; //init command data
    char currentserial[16];

    bzero(currentserial, sizeof(currentserial));

    printf("Getting library information...\n");
    USB_HID_SHT_GetLibraryRevision(&libinfo);
    printf("Interface: %s %s \n",
           libinfo.InterfaceLib.Filename, libinfo.InterfaceLib.Revision);
    printf("Protocol: %s %s \n",
           libinfo.ProtocolLib.Filename, libinfo.ProtocolLib.Revision);


// establishes communications between the host computer and the card dispenser
    printf("Connecting...\n");
    ret = USB_HID_SHT_ConnectDevice(_PRODUCT_ID,
                                    _SERIAL_NUMBER,
                                    currentserial);

    printf("current serial: %s\n", currentserial);

    if(ret == _USB_HID_SHT_NO_ERROR)
    {
        printf("command finished with no error\n");
        // init command successfully finished, now parse reply
        if( reply.replyType == PositiveReply )
        {
            printf("received positive reply\n");
        }
        else if( reply.replyType == NegativeReply )
        {
            printf("received negative reply\n");
        }
        else
        {
            printf("received other reply\n");
        }
    }
    else
    {
        printf("command finished with ERROR\n");
    }
    

    command.CommandCode = 0x30;    //init command
    command.ParameterCode = 0x30;
    command.Data.pBody = NULL;     // start address of region where data is stored
    command.Data.Size = 0;         // data size in bytes

    
// Executes Initialize command, and then receives a reply for the command 

    printf("Executing...\n");
    ret = USB_HID_SHT_ExecuteCommand(_PRODUCT_ID,
                                     currentserial,
                                     &command,
                                     _TIMEOUT,
                                     &reply);
    if(ret == _USB_HID_SHT_NO_ERROR)
    {
        printf("command finished with no error\n");
        // init command successfully finished, now parse reply
        if( reply.replyType == PositiveReply )
        {
            printf("received positive reply\n");
        }
        else if( reply.replyType == NegativeReply )
        {
            printf("received negative reply\n");
        }
        else
        {
            printf("received other reply\n");
        }
    }
    else
    {
        printf("command finished with ERROR\n");
    }


    printf("Disconnecting...\n");
    ret = USB_HID_SHT_DisconnectDevice(_PRODUCT_ID,
                                       _SERIAL_NUMBER);
    if(ret == _USB_HID_SHT_NO_ERROR)
    {
        printf("command finished with no error\n");
        // init command successfully finished, now parse reply
        if( reply.replyType == PositiveReply )
        {
            printf("received positive reply\n");
        }
        else if( reply.replyType == NegativeReply )
        {
            printf("received negative reply\n");
        }
        else
        {
            printf("received other reply\n");
        }
    }
    else
    {
        printf("command finished with ERROR\n");
    }

}
