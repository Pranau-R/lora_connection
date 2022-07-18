/*
Module: receive_LoRa.ino

Function:
        To receive the measurement from tranmit catena board sensor and
        to display it for the user by using LoRa and LoRa.h library.

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

/****************************************************************************\
|
|   Code.
|
\****************************************************************************/

/*
Name: setup()

Function:
        Initializes LoRa for Receiver.

Definition:
        void setup (
            void
            )

Returns:
        Functions returning type void: nothing.
*/

void setup()
    {
    //initialize Serial Monitor
    Serial.begin(115200);
    while (!Serial);
    gCatena.SafePrintf("This is LoRa Receiver!\n");

    //setup LoRa transceiver module
    LoRa.setPins(ss, rst, dio0);

    if(!LoRa.begin(866E6))
        {
        gCatena.SafePrintf("Starting LoRa failed!\n");
        while (1);
        }

    // Change sync word (0xF3) to match the receiver
    // The sync word assures you don't get LoRa messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    gCatena.SafePrintf("LoRa Initializing OK!\n");
    }

/*
Name:   loop()

Function:
        Receives the data packets from transfer boards and display it in serial monitor.

Definition:
        void loop (
            void
            );

Returns:
        Functions returning type void: nothing.
*/

void loop()
    {
    String LoRaData; 
    int packetSize = LoRa.parsePacket();
    if (packetSize) 
        {
        // received a packet
        gCatena.SafePrintf("Received packet '");

        // read packet
        while (LoRa.available())
            {
            LoRaData = LoRa.read();
            gCatena.SafePrintf("The Message is: ", LoRaData); 
            }

        // print RSSI of packet
        gCatena.SafePrintf("' with RSSI ", LoRa.packetRssi());
        }
    }
