/*
Module: transmit_LoRa.ino

Function:
        To get the measurement from on-board catena sensor and
        to transmit those measurement to another catena board using LoRa and LoRa.h library.

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
#include <Catena_Si1133.h>
#include <LoRa.h>


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

//   The LUX sensor
Catena_Si1133 gSi1133;

int counter = 0;

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name: setup()

Function:
        Initializes Si1133 Light Sensor.

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
    gCatena.SafePrintf("Si1133 LoRa Connectivity Test!\n");
    gCatena.SafePrintf("This is Sender!\n");

    if (gSi1133.begin())
        {
        gSi1133.configure(0, CATENA_SI1133_MODE_SmallIR);
        gSi1133.configure(1, CATENA_SI1133_MODE_White);
        gSi1133.configure(2, CATENA_SI1133_MODE_UV);
        }
    else
        {
        gCatena.SafePrintf("No Si1133 found: check hardware\n");
        }

    if (!LoRa.begin(868E6))
        {
        gCatena.SafePrintf("Starting LoRa failed!");
        while (1);
        }
    LoRa.setSyncWord(0xF3);
    LoRa.setTxPower(20);
    }

/*
Name:   loop()

Function:
        Performs different light calculation such as
        Infrared Light, White Light and UV Light.

Definition:
        void loop (
            void
            );

Returns:
        Functions returning type void: nothing.
*/

void loop()
    {
    /* Get a new sensor event */
    uint16_t data[3];

    while (! gSi1133.isOneTimeReady())
        {
        yield();
        }

    gSi1133.readMultiChannelData(data, 3);
    gSi1133.stop();

    Serial.print("Sending packet: ");
    Serial.println(counter);

    // send packet
    LoRa.beginPacket();
    LoRa.print("Data:");
    LoRa.print(counter);

    LoRa.print("Si1133: ");
    LoRa.print(data[0]);
    LoRa.print(" IR, ");

    LoRa.print(data[1]);
    LoRa.print(" White, ");

    LoRa.print(data[2]);
    LoRa.println(" UV, ");

    LoRa.endPacket();

    counter++;

    delay(5000);
    }