
    /*
     *      Cubetto Library v0.1
     *      
     *
     *
     *      created on Jun 2014
     *      by Matteo Loglio
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

    #ifndef Cubetto_h
    #define Cubetto_h

    #define LLED_PIN 13

    #define USER_LED_1_PIN 2
    #define USER_LED_2_PIN 11

    #define BUZZER_PIN 10

    #define ENC_1_PIN 3
    #define ENC_2_PIN 7

    #define ENABLE_PIN 5
    #define DIR_1Y_PIN 6
    #define DIR_2Y_PIN 4
    #define DIR_3Y_PIN 8
    #define DIR_4Y_PIN 9

    #define XBEE Serial1
    #define XBEE_SPEED 9600

    #define DELAY 1000
    #define POT_DELAY 300

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

    /* 
    -----------------------------------------------------------------------------
                                    Generic Classes
    -----------------------------------------------------------------------------
    */

    class Cubetto
    {
    public:        

        void init() {
            pinMode(LLED_PIN, OUTPUT);
          pinMode(USER_LED_1_PIN, OUTPUT);
          pinMode(USER_LED_2_PIN, OUTPUT);
          pinMode(BUZZER_PIN, OUTPUT);
          pinMode(ENABLE_PIN, OUTPUT);
          pinMode(DIR_1Y_PIN, OUTPUT);
          pinMode(DIR_2Y_PIN, OUTPUT);
          pinMode(DIR_3Y_PIN, OUTPUT);
          pinMode(DIR_4Y_PIN, OUTPUT);

          pinMode(ENC_1_PIN, INPUT);
          pinMode(ENC_2_PIN, INPUT);

          digitalWrite(ENABLE_PIN, LOW);
          digitalWrite(DIR_1Y_PIN, LOW);
          digitalWrite(DIR_2Y_PIN, LOW);
          digitalWrite(DIR_3Y_PIN, LOW);
          digitalWrite(DIR_4Y_PIN, LOW);

          digitalWrite(LLED_PIN, HIGH);
          digitalWrite(USER_LED_1_PIN, LOW);
          digitalWrite(USER_LED_2_PIN, LOW);

          stopTime = 0UL;
          motorsRunning = false;
          countA = 0;
          countB = 0;

          XBEE.begin(XBEE_SPEED);
          XBEE.println("Starting power on test...");
          doPowerOnTest();

          while (XBEE.available() > 0) XBEE.read();

          XBEE.println("Ready!");
          happy();
        }

      void waitInstructions() {
          if (XBEE.available() > 0)
          {
            char ch = XBEE.read();
            int mil = millis();

            switch (ch)
            {
                //Forward
              case 'F' :
              forward(40);
              break;

                //Right
              case 'R':
              right(15);
              break;

                //Left
              case 'L' :
              left(14);
              break;

              case '4' :
              stopAllMotors();
              startMotorB(true);
              break;

              case 'H' :
              happy();
              XBEE.println(":)");
              break;

              case '6' :
              noTone(BUZZER_PIN);
              XBEE.println("Buzzer OFF");
              break;

              case '7' :
              digitalWrite(LLED_PIN, LOW);
              XBEE.println("L LED ON");
              break;

              case '8' :
              digitalWrite(LLED_PIN, HIGH);
              XBEE.println("L LED OFF");
              break;
          }
      }
    }

    void moveSteps(int steps, boolean reverseA, boolean reverseB) {

      stopAllMotors();

      startMotorA(reverseA);
      startMotorB(reverseB);

      countA = 0;
      countB = 0;

      while ((countA < steps) || (countB < steps)) {

        AEnc = digitalRead(ENC_1_PIN);
        BEnc = digitalRead(ENC_2_PIN);
        Serial.println("move");
        delay(10);
        if (countB < steps) {
          if (BEnc != pBEnc) {
            countB++;
        }
    }
    if (countA < steps) {
      if (AEnc != pAEnc) {
        countA++;
    }
    }
    pBEnc = BEnc;
    pAEnc = AEnc;
    }
    stopAllMotors();
    }

    void forward(int steps) {
      moveSteps(steps, true, true);
      stopAllMotors();
      tone(BUZZER_PIN, 3000);
      delay(50);
      noTone(BUZZER_PIN);
    }

    void left(int steps) {
      moveSteps(steps, false, true);
      tone(BUZZER_PIN, 5000);
      delay(50);
      noTone(BUZZER_PIN);
    }

    void right(int steps) {
      moveSteps(steps, true, false);
      tone(BUZZER_PIN, 4000);
      delay(50);
      noTone(BUZZER_PIN);
    }


    void starWars() {
      tone(BUZZER_PIN, NOTE_G2, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_G2, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_G2, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_DS2, 500);
      delay(500);

      tone(BUZZER_PIN, NOTE_AS2, 150);
      delay(150);

      tone(BUZZER_PIN, NOTE_G2, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_DS2, 500);
      delay(500);

      tone(BUZZER_PIN, NOTE_AS2, 150);
      delay(150);

      tone(BUZZER_PIN, NOTE_G2, 1200);
      delay(1200);

      tone(BUZZER_PIN, NOTE_D3, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_D3, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_D3, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_DS3, 500);
      delay(500);

      tone(BUZZER_PIN, NOTE_C3, 150);
      delay(150);

      tone(BUZZER_PIN, NOTE_FS2, 600);
      delay(600);

      tone(BUZZER_PIN, NOTE_D2, 500);
      delay(500);

      tone(BUZZER_PIN, NOTE_AS2, 150);
      delay(150);

      tone(BUZZER_PIN, NOTE_G2, 1200);
      delay(1200);

      noTone(BUZZER_PIN);
    }

    void happy() {
      tone(BUZZER_PIN, NOTE_C7, 150);
      delay(60);

      tone(BUZZER_PIN, NOTE_GS7, 600);
      delay(100);

      tone(BUZZER_PIN, NOTE_C8, 500);
      delay(40);
    }

    protected:

        unsigned long stopTime;
        bool motorsRunning;

        int countA, countB;

        int AEnc, BEnc;
        int pAEnc, pBEnc;

        void printEncoderCount() {
          Serial.print("Right: ");
          Serial.print(countB);
          Serial.print(" Left: ");
          Serial.println(countA);
      }

      void ledEncoderDebug() {
          digitalWrite(USER_LED_1_PIN, AEnc);
          digitalWrite(USER_LED_2_PIN, BEnc);
      }

      void printEncoderReadings() {
          Serial.print("1: ");
          Serial.print(digitalRead(ENC_1_PIN));
          Serial.print(" 2: ");
          Serial.println(digitalRead(ENC_2_PIN));
          delay(50);
      }

      void doPowerOnTest()
      {
          digitalWrite(USER_LED_1_PIN, HIGH);
          delay(POT_DELAY);
          digitalWrite(USER_LED_1_PIN, LOW);
          XBEE.println("USR1 LED");

          digitalWrite(USER_LED_2_PIN, HIGH);
          delay(POT_DELAY);
          digitalWrite(USER_LED_2_PIN, LOW);
          XBEE.println("USR2 LED");

          digitalWrite(LLED_PIN, LOW);

          startMotorA(false);
          delay(POT_DELAY);
          stopAllMotors();

          startMotorA(true);
          delay(POT_DELAY);
          stopAllMotors();

          startMotorB(false);
          delay(POT_DELAY);
          stopAllMotors();

          startMotorB(true);
          delay(POT_DELAY);
          stopAllMotors();

          digitalWrite(LLED_PIN, HIGH);
          XBEE.println("L LED");

          tone(BUZZER_PIN, 4000);
          delay(POT_DELAY);
          noTone(BUZZER_PIN);
          XBEE.println("Buzzer");
      }

      void startMotorA (bool reverse)
      {
          if (reverse)
          {
            //one motor was spinning faster than the other
            digitalWrite(DIR_1Y_PIN, HIGH);
            //analogWrite(DIR_1Y_PIN, 230);
            digitalWrite(DIR_2Y_PIN, LOW);
        }
        else
        {
            digitalWrite(DIR_1Y_PIN, LOW);
            analogWrite(DIR_2Y_PIN, 240);
        }

        analogWrite(ENABLE_PIN, 255);

        stopTime = millis() + DELAY;

        XBEE.print("Motor A running ");
        if (reverse) XBEE.print('C');
        XBEE.println("CW");

        motorsRunning = true;
    }

    void startMotorB (bool reverse)
    {
      if (reverse)
      {
        digitalWrite(DIR_3Y_PIN, HIGH);
        digitalWrite(DIR_4Y_PIN, LOW);
    }
    else
    {
        digitalWrite(DIR_3Y_PIN, LOW);
        digitalWrite(DIR_4Y_PIN, HIGH);
    }

    analogWrite(ENABLE_PIN, 255);

    stopTime = millis() + DELAY;

    XBEE.print("Motor B running ");
    if (reverse) XBEE.print('C');
    XBEE.println("CW");

    motorsRunning = true;
    }

    void stopAllMotors()
    {
      if (motorsRunning)
      {
        digitalWrite(DIR_1Y_PIN, LOW);
        digitalWrite(DIR_2Y_PIN, LOW);
        digitalWrite(DIR_3Y_PIN, LOW);
        digitalWrite(DIR_4Y_PIN, LOW);
        delay(100);
        digitalWrite(ENABLE_PIN, LOW);

        XBEE.println("All motors stopped");
    }

    motorsRunning = false;
    }

    void stopMotorB() {
      digitalWrite(DIR_3Y_PIN, LOW);
      digitalWrite(DIR_4Y_PIN, LOW);
    }

    void stopMotorA() {
      digitalWrite(DIR_1Y_PIN, LOW);
      digitalWrite(DIR_2Y_PIN, LOW);
    }
    };


    #endif

