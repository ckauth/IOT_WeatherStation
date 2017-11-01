#include "iothub.h"
#include "wiring.h"

#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_c_shared_utility/crt_abstractions.h"
#include "iothub_client.h"
#include "iothub_client_options.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"
#include <stdio.h>

// To control the program execution remotely
extern int DeviceMethodCallback(const char *method_name, const unsigned char *payload, size_t size, unsigned char **response, size_t *resp_size, void *userContextCallback);

// Delivery notification for message sent to the hub
static void MessageDeliveryCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
    if (IOTHUB_CLIENT_CONFIRMATION_OK != result)
    {
        printf("[Device] Failed to send message to Azure IoT Hub\r\n");
    }

    IoTHub *hub = (IoTHub*)userContextCallback;
    hub->MessageIsPending = false;
}

void InitializeIoTHub(IoTHub *hub, char *connectionstring)
{
    snprintf(hub->connectionstring , sizeof(hub->connectionstring), "%s", connectionstring);

    if (platform_init() != 0)
    {
        printf("[Device] Failed to initialize the platform.\r\n");
        platform_deinit();
        return;
    }
    if ((hub->clientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionstring, MQTT_Protocol)) == NULL)
    {
        (void)printf("[Device] ERROR: iotHubClientHandle is NULL!\r\n");
        IoTHubClient_LL_Destroy(hub->clientHandle);
        platform_deinit();
        return;
    }
    int receiveContext = 0;
    
    // Start listening to direct method invocation from the hub
    if (IoTHubClient_LL_SetDeviceMethodCallback(hub->clientHandle, DeviceMethodCallback, &receiveContext) != IOTHUB_CLIENT_OK)
    {
        (void)printf("ERROR: IoTHubClient_LL_SetDeviceMethodCallback..........FAILED!\r\n");
    }

    hub->MessageIsPending = false;
}

void DoWork(IoTHub *hub)
{
    IoTHubClient_LL_DoWork(hub->clientHandle);
    wait(100);
}

bool IsIoTHubConnectionEstablished(IoTHub *hub)
{
    return hub->clientHandle != NULL;
}

void SendMessageToIoTHub(IoTHub *hub, char message[])
{
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray(message, strlen(message));
    if (messageHandle == NULL)
    {
        printf("[Device] ERROR: unable to create a new IoTHubMessage\r\n");
        IoTHubMessage_Destroy(messageHandle);
        return;
    }
    if (IoTHubClient_LL_SendEventAsync(hub->clientHandle, messageHandle, MessageDeliveryCallback, hub) != IOTHUB_CLIENT_OK)
    {
        printf("[Device] ERROR: Failed to hand over the message to IoTHubClient\r\n");
        IoTHubMessage_Destroy(messageHandle);
        return;
    }
    IoTHubMessage_Destroy(messageHandle);

    hub->MessageIsPending = true;
}