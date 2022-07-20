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
Arduino_LoRaWAN::ReceivePortBufferCbFn receiveDone;

uint8_t receiveBuffer[4];

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name: setup()

Function:
        Initializes Si1133 Light Sensor

Definition:
        void setup (
            void
            )

Returns:
        Functions returning type void: nothing.
*/

void setup()
    {
    Serial.begin(115200);

    while (!Serial);
    //gCatena.SafePrintf("Si1133 LoRa Connectivity Test!\n");
    gCatena.SafePrintf("This is Receiver!\n");

    char sRegion[16];
    gCatena.SafePrintf("Target network: %s / %s\n",
        gLoRaWAN.GetNetworkName(),
        gLoRaWAN.GetRegionString(sRegion, sizeof(sRegion))
        );

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
    }

    if (! gLoRaWAN.IsProvisioned())
        {
        gCatena.SafePrintf("LoRaWAN not provisioned yet. Use the commands to set it up.\n");
        }
    else
        {
        // send a confirmed uplink
        if (gLoRaWAN.ReceivePortBufferCbFn(nullptr, 16, receiveBuffer, sizeof(receiveBuffer)))
            {
            gfTxStarted = true;
            }
        else
            {
            gCatena.SafePrintf("SendBuffer failed!\n");
            }
        }