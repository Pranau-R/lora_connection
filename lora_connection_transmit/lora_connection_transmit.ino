/*
Module: Lora_connection_transmit.ino

Function:
        To get the measurement from on-board catena sensor and
        to transmit those measurement to another catena board using LoRa.

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
//#include <Catena_Si1133.h>
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
Arduino_LoRaWAN::SendBufferCbFn uplinkDone;

bool gfSuccess;
bool gfTxStarted;
bool gfTxDone;

uint8_t uplinkBuffer[] = { 0xCA, 0xFE, 0xBA, 0xBE };

//   The LUX sensor
//Catena_Si1133 gSi1133;

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
    gCatena.SafePrintf("This is Sender!\n");

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

    if (! gLoRaWAN.IsProvisioned())
        {
        gCatena.SafePrintf("LoRaWAN not provisioned yet. Use the commands to set it up.\n");
        }
    else
        {
        // send a confirmed uplink
        if (gLoRaWAN.SendBuffer(uplinkBuffer, sizeof(uplinkBuffer), uplinkDone, nullptr, true, /* port */ 16))
            {
            gfTxStarted = true;
            }
        else
            {
            gCatena.SafePrintf("SendBuffer failed!\n");
            }
        }

    /*if (gSi1133.begin())
        {
        gSi1133.configure(0, CATENA_SI1133_MODE_SmallIR);
        gSi1133.configure(1, CATENA_SI1133_MODE_White);
        gSi1133.configure(2, CATENA_SI1133_MODE_UV);
        }
    else
        {
        gCatena.SafePrintf("No Si1133 found: check hardware\n");
        }*/
    }

/*
Name:   loop()

Function:
        Performs different light calculation such as
        Infrared Light, White Light, UV Light.

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
    /* Get a new sensor event */
    /*uint16_t data[3];

    while (! gSi1133.isOneTimeReady())
        {
        yield();
        }

    gSi1133.readMultiChannelData(data, 3);
    gSi1133.stop();
    gCatena.SafePrintf(
        "Si1133:  %u IR, %u White, %u UV\n",
        data[0],
        data[1],
        data[2]
        );*/

    if (gfTxStarted && gfTxDone)
        {
        gfTxStarted = false;
        if (gfSuccess)
            {
            gCatena.SafePrintf("Transmit succeeded.\n");
            }
        else
            {
            gCatena.SafePrintf("Message uplink failed!\n");
            }
        }
    }

void uplinkDone(void *pCtx, bool fSuccess)
    {
    gfTxDone = true;
    gfSuccess = fSuccess;
    }
