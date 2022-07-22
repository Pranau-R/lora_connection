/*
Module: lora_connection_receive.ino

Function:
        To receive the measurement from tranmit catena board sensor and
        to display it for the user.

Copyright notice:
        This file copyright (C) 2022 by
        MCCI Corporation
        3520 Krums Corners Road
        Ithaca, NY 14850
        An unpublished work. All rights reserved.
        This file is proprietary information, and may not be disclosed or
        copied without the prior permission of MCCI Corporation.

Author:
        Pranau R, MCCI Corporation   July 2022
*/

#include <Catena.h>
#include <Arduino_LoRaWAN.h>
#include <lmic.h>


/****************************************************************************\
|
|   Manifest constants & typedefs.
|
\****************************************************************************/

using namespace McciCatena;

/****************************************************************************\
|
|	VARIABLES
|
\****************************************************************************/

// the primary object
Catena gCatena;

// the LoRaWAN backhaul.  Note that we use the
// Catena version so it can provide hardware-specific
// information to the base class.
//
Catena::LoRaWAN gLoRaWAN;

// declare the callback function.
Arduino_LoRaWAN::ReceivePortBufferCbFn receiveMessage;

//uint8_t receiveBuffer[4];

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name: setup()

Function:
        Initializes Lora for Receiver.

Definition:
        void setup (
            void
            )

Returns:
        Functions returning type void: nothing.
*/

void setup()
    {
    gCatena.begin();

    Serial.begin(115200);

    while (!Serial);
    gCatena.SafePrintf("This is Receiver!\n");

    char sRegion[16];
    gCatena.SafePrintf("Target network: %s / %s\n",
        gLoRaWAN.GetNetworkName(),
        gLoRaWAN.GetRegionString(sRegion, sizeof(sRegion))
        );

    gLoRaWAN.SetReceiveBufferBufferCb(receiveMessage);

    // set up LoRaWAN
    gCatena.SafePrintf("LoRaWAN init: ");
    if (!gLoRaWAN.begin(&gCatena))
        {
        gCatena.SafePrintf("failed\n");
        }
    else
        {
        gCatena.SafePrintf("succeeded\n");
        }

    gCatena.registerObject(&gLoRaWAN);
    if (! gLoRaWAN.IsProvisioned())
        {
        gCatena.SafePrintf("LoRaWAN not provisioned yet. Use the commands to set it up.\n");
        }
    else
        {
        gCatena.SafePrintf("LoRaWAN is provisioned!.\n");
        }
    }

/*Name:   loop()

Function:
        Receive the data from the server.

Definition:
        void loop (
            void
            );

Returns:
        Functions returning type void: nothing.
*/

void loop()
    {
    gCatena.poll();
    }

void receiveMessage(
    void *pContext,
    uint8_t port,
    const uint8_t *receiveBuffer,
    size_t nBuffer)
    {
    nBuffer = sizeof(receiveBuffer);

    for (int i = 0; i <= sizeof(receiveBuffer); i++)
        {
        gCatena.SafePrintf("The Data %d is: %u", i, receiveBuffer[i]);
        }
    }
