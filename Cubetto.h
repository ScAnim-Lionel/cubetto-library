/*
*      Cubetto Library v0.2
*
*
*
*      created on Jun 2014
*      by Matteo Loglio
*
*      0.2 by Matteo Loglio on Sep 14
*
*      This program is free software; you can redistribute it and/or modify
*      it under the terms of the GNU General Public License as published by
*      the Free Software Foundation; either version 2 of the License, or
*      (at your option) any later version.
*
*      This program is distributed in the hope that it will be useful,
*      but WITHOUT ANY WARRANTY; without even the implied warranty of
*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*      GNU General Public License for more details.
*
*      You should have received a copy of the GNU General Public License
*      along with this program; if not, write to the Free Software
*      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
*      MA 02110-1301, USA.
*/

#include "Arduino.h"
#include "printf.h"

#ifndef Cubetto_h
#define Cubetto_h

//NOTES
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


#define NRF24L01_MAX_PACKET_SIZE 32
static char packet[NRF24L01_MAX_PACKET_SIZE];

#define PRIMO_ID 0xDE1D8758 // NB This must be identical in Primo.

static long primo_random = 0;

#define STOP    0x00
#define FORWARD 0x01
#define LEFT    0x02
#define RIGHT   0x03

#define STEPPER_MAX_SPEED 1000 // Pulses per second.
#define STEPPER_ACCELERATION 500 // Pulses per second.

#define STEPPER_FORWARD 2700 // Total pulses.
#define STEPPER_TURN 1000 // Total pulses.

#define BUZZER_PIN 13

int in1Pin = 12;
int in2Pin = 10;
int in3Pin = 11;
int in4Pin = 9;

int on1Pin = 6;
int on2Pin = 4;
int on3Pin = 5;
int on4Pin = 3;

#define DEBUG_MODE 1

#ifdef DEBUG_MODE
#define debug_printf printf
#else
#define debug_printf(...) ((void)0)
#endif

AccelStepper stepper_left(AccelStepper::HALF4WIRE, in1Pin, in2Pin, in3Pin, in4Pin);
AccelStepper stepper_right(AccelStepper::HALF4WIRE, on1Pin, on2Pin, on3Pin, on4Pin);
static bool stepper_left_moving = 0;
static bool stepper_right_moving = 0;
RF24 radio(7,8);
volatile bool rx_event_handler_pending = 0;
const uint64_t pipe = 0xE8E8F0F0E1LL;


static uint32_t message_count = 0;


 void check_radio(void)
    {
        // What happened?
        bool tx,fail,rx;
        radio.whatHappened(tx,fail,rx);
        debug_printf("Got Interupt from the Radio\n\r");
        // Have we successfully transmitted?
        if ( tx )
        {
            debug_printf("Ack Payload:Sent\n\r");
        }
        
        // Have we failed to transmit?
        if ( fail )
        {
            debug_printf("Ack Payload:Failed\n\r");
        }
        
        // Did we receive a message?
        if ( rx )
        {
            // Signal the main event loop that there is an Rx event to handle.
            rx_event_handler_pending = 1;
            
            // Get this payload and dump it
            radio.read(packet, NRF24L01_MAX_PACKET_SIZE);
            debug_printf("Got payload\n\r");
            
            // Add an ack packet for the next time around.  This is a simple
            // packet counter
            radio.writeAckPayload( 1, &message_count, sizeof(message_count) );
            ++message_count;
        }
    }
    

/*
-----------------------------------------------------------------------------
Generic Classes
-----------------------------------------------------------------------------
*/

class Cubetto
{
    public:    

    void init(){
        pinMode(in1Pin, OUTPUT);
        pinMode(in2Pin, OUTPUT);
        pinMode(in3Pin, OUTPUT);
        pinMode(in4Pin, OUTPUT);
        
        pinMode(on1Pin, OUTPUT);
        pinMode(on2Pin, OUTPUT);
        pinMode(on3Pin, OUTPUT);
        pinMode(on4Pin, OUTPUT);
        
        stepper_left.setMaxSpeed(STEPPER_MAX_SPEED);
        stepper_right.setMaxSpeed(STEPPER_MAX_SPEED);
        stepper_left.setAcceleration(STEPPER_ACCELERATION);
        stepper_right.setAcceleration(STEPPER_ACCELERATION);
        
        // IMPORTANT stepper outputs are disabled here to minimise power usage whilst stationary,
        // as they are automatically enabled in the class constructor.
        stepper_left.disableOutputs();
        stepper_right.disableOutputs();
        
        //
        // Print preamble
        //
        
        Serial.begin(57600);
        printf_begin();
        //while (Serial.read()== -1)
        debug_printf("ROLE: Cubetto\n\r");
        
        //
        // Setup and configure rf radio
        //
        
        radio.begin();
        
        // We will be using the Ack Payload feature, so please enable it
        radio.enableAckPayload();
        
        //
        // Open pipes to other nodes for communication
        //
        
        radio.openReadingPipe(1,pipe);
        
        //
        // Start listening
        //
        
        radio.startListening();
        
        //
        // Dump the configuration of the rf unit for debugging
        //
        
        radio.printDetails();
        
        //
        // Attach interrupt handler to interrupt #0 (using pin 2)
        // on BOTH the sender and receiver
        //
        
        attachInterrupt(1, check_radio, CHANGE);

        // return void;
    }
    
    void waitInstructions() {
        // Check if there is an Rx event to handle.
        if(rx_event_handler_pending == 1)
        {
            int i;
            long packet_id = 0;
            long packet_random = 0;
            
            debug_printf("Event Handler start\n\r");
            
            debug_printf("packet");
            for(i = 0; i < 32; i++)
            {
                debug_printf(" %x", packet[i]);
            }
            debug_printf("\n\r");
            
            memcpy(&packet_id, (const long*)&packet[0], 4);
            memcpy(&packet_random, (const long*)&packet[4], 4);
            
            
            // Check the packet is valid.
            if(packet_id == PRIMO_ID)
            {
                debug_printf("PRIMO_ID good\n\r");
                
                if(primo_random == 0)
                {
                    // (The UID could be used to set the packet address in the radio, but this would
                    // make it necessary to un-pair Primo/Cubetto at BOTH ends).
                    primo_random = packet_random;
                    debug_printf("PRIMO_RANDOM set\n\r");
                }
                
                if(primo_random == packet_random)
                {
                    debug_printf("PRIMO_RANDOM good\n\r");
                    
                    // Carry out movement instructions here.
                    // IMPORTANT stepper outputs are enabled/disabled to minimise power usage whilst stationary.
                    stepper_left.enableOutputs();
                    stepper_right.enableOutputs();
                    move();
                    stepper_left.disableOutputs();
                    stepper_right.disableOutputs();
                }
            }
            
            debug_printf("Event Handler end\n\r");
            
            // Finally clear the event flag, so that the next interrupt event can register.
            rx_event_handler_pending = 0;
            
            // FIXME Would it be better to disable the radiointerrupt in the interrupt handler,
            // and re-enable it here?
        }
    }
    
    void forward(int amount) {
        stepper_left.move(-amount);
        stepper_right.move(amount);
        tone(BUZZER_PIN, 3000);
        delay(50);
        noTone(BUZZER_PIN);
        stepper_left_moving = 1;
        stepper_right_moving = 1;
        
        while(stepper_left_moving || stepper_right_moving)
        {
            stepper_left_moving = stepper_left.run();
            stepper_right_moving = stepper_right.run();
        }
    }
    
    void left(int amount) {
        stepper_left.move(-amount);
        stepper_right.move(-amount);
        tone(BUZZER_PIN, 5000);
        delay(50);
        noTone(BUZZER_PIN);
        stepper_left_moving = 1;
        stepper_right_moving = 1;
        
        while(stepper_left_moving || stepper_right_moving)
        {
            stepper_left_moving = stepper_left.run();
            stepper_right_moving = stepper_right.run();
        }
    }
    
    void right(int amount) {
        stepper_left.move(amount);
        stepper_right.move(amount);
        tone(BUZZER_PIN, 4000);
        delay(50);
        noTone(BUZZER_PIN);
        stepper_left_moving = 1;
        stepper_right_moving = 1;
        
        while(stepper_left_moving || stepper_right_moving)
        {
            stepper_left_moving = stepper_left.run();
            stepper_right_moving = stepper_right.run();
        }
    }
    
    
    void move(void)
    {
        int i;
        
        for(i = 8; i < 32; i++)
        {
            switch(packet[i])
            {
                case STOP:
                happy();
                break;
                
                case FORWARD:
                stepper_left.move(STEPPER_FORWARD);
                stepper_right.move(-STEPPER_FORWARD);
                tone(BUZZER_PIN, 3000);
                delay(50);
                noTone(BUZZER_PIN);
                break;
                
                case LEFT:
                stepper_left.move(-STEPPER_TURN);
                stepper_right.move(-STEPPER_TURN);
                tone(BUZZER_PIN, 5000);
                delay(50);
                noTone(BUZZER_PIN);
                break;
                
                case RIGHT:
                stepper_left.move(STEPPER_TURN);
                stepper_right.move(STEPPER_TURN);
                tone(BUZZER_PIN, 4000);
                delay(50);
                noTone(BUZZER_PIN);
                break;
                
                default:
                break;
            }
            
            stepper_left_moving = 1;
            stepper_right_moving = 1;
            
            while(stepper_left_moving || stepper_right_moving)
            {
                stepper_left_moving = stepper_left.run();
                stepper_right_moving = stepper_right.run();
            }
        }
    }
    
   
    void happy() {
        tone(BUZZER_PIN, NOTE_C7, 150);
        delay(20);
        
        tone(BUZZER_PIN, NOTE_GS7, 600);
        delay(30);
        
        //tone(BUZZER_PIN, NOTE_C8, 500);
        //  delay(10);
    }
};


#endif