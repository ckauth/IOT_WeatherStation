#ifndef __IOTHUB_H__
#define __IOTHUB_H__

#include "iothub_client.h"

typedef struct IoTHub
{
    IOTHUB_CLIENT_LL_HANDLE clientHandle;
    char connectionstring[256];
    bool MessageIsPending;
} IoTHub;

void InitializeIoTHub(IoTHub *hub, char *connectionstring);

void DoWork(IoTHub *hub);

bool IsIoTHubConnectionEstablished(IoTHub *hub);

void SendMessageToIoTHub(IoTHub *hub, char message[]);

#endif // __IOTHUB_H__