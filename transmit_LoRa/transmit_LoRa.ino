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

//define the pins used by the transceiver module
#define ss D7
#define rst D8
#define dio0 D25

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

// LoRa object
//LoRaClass LoRa;

//   The LUX sensor
//Catena_Si1133 gSi1133;

uint8_t uplinkBuffer = 0xCA;

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name: setup()

Function:
        Initializes Si1133 Light Sensor and LoRa for Transmission.

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
    //gCatena.SafePrintf("Si1133 LoRa Connectivity Test!\n");
    gCatena.SafePrintf("This is Lora Sender!\n");

    //setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);

    if (!LoRa.begin(866E6))
        {
        gCatena.SafePrintf("Starting LoRa failed!\n");
        while (1);
        }

    gCatena.SafePrintf("LoRa Initializing OK!\n");
    LoRa.setSyncWord(0xF3);
    LoRa.setTxPower(20);
    }

/*
Name:   loop()

Function:
        Sends the data in a packet.

Definition:
        void loop (
            void
            );

Returns:
        Functions returning type void: nothing.
*/

void loop()
    {
    gCatena.SafePrintf("Before Transfer!\n");
    // send packet
    LoRa.beginPacket();

    gCatena.SafePrintf("Before write!\n");
    LoRa.write(uplinkBuffer);
    gCatena.SafePrintf("After write!\n");

    gCatena.SafePrintf("Before print!\n");
    LoRa.print(uplinkBuffer);
    gCatena.SafePrintf("After print!\n");

    LoRa.endPacket();

    gCatena.SafePrintf("Transfer Done!\n");
    //LoRa.onTxDone(onTxDone);
    //counter++;

    delay(5000);
    }
