#include <Stepper.h>
#include <Servo.h>
#include "RotaryTable.h"
#include "Door.h"

/*
 * these two parameters assume all doors are equal. tune them to your liking.
 */
#define DOORS_LENGTH 35000u
#define TIME_SLICE 50
#define DOORS_SPEED 3800u

/*
 * pins
 */
#define LED1 14u
#define LED2 15u
#define LED3 16u
#define LED4 17u
#define LED5 18u

#define STUDYCURTAIN1 22u
#define STUDYCURTAIN2 23u
#define STUDYCURTAIN3 24u
#define BEDROOMCURTAIN 25u
#define CORRIDORCURTAIN 26u

#define BEDROOMLIGTH 36u
#define LIVINGROOMLIGHT1 37u
#define LIVINGROOMLIGHT2 38u
#define KITCHENLIGHT 39u
#define STUDYLIGHT 40u

#define LEFTEXTERNSPEAKER 41u
#define RIGHTEXTERNSPEAKER 42u
#define LEFTLIVINGROOMSPEAKER 47u
#define RIGHTLIVINGROOMSPEAKER 48u
#define LEFTSTUDYSPEAKER 49u
#define RIGHTSTUDYSPEAKER 50u
#define LEFTBEDROOMSPEAKER 51u
#define RIGHTBEDROOMSPEAKER 52u

/*
 * external interface (serial)
 */
#define EMERGENCYSTOP 0u

#define ROTATECORRIDORTOLIVINGROOM 1u
#define ROTATECORRIDORTOSTUDY 2u
#define ROTATECORRIDORTOBEDROOM 3u
#define ROTATEBEDROOMTOCORRIDOR 4u
#define ROTATEBEDROOMTOEXIT 5u

#define OPENENTRANCEDOOR 6u
#define CLOSEENTRANCEDOOR 7u
#define OPENTERRACEDOOR 8u
#define CLOSETERRACEDOOR 9u
#define OPENEXITDOOR 10u
#define CLOSEEXITDOOR 11u

#define OPENSTUDYCURTAINS 12u
#define CLOSESTUDYCURTAINS 13u
#define OPENBEDROOMCURTAIN 14u
#define CLOSEBEDROOMCURTAIN 15u
#define OPENCORRIDORCURTAIN 16u
#define CLOSECORRIDORCURTAIN 17u

#define TURNBEDROOMLIGHTON 18U
#define TURNBEDROOMLIGHTOFF 19U
#define TURNLIVINGROOMLIGHTSON 20u
#define TURNLIVINGROOMLIGHTSOFF 21u
#define TURNKITCHENLIGHTON 22u
#define TURNKITCHENLIGHTOFF 23u
#define TURNSTUDYLIGHTON 24u
#define TURNSTUDYLIGHTOFF 25u

#define ENABLEEXTERNSPEAKERS 26u
#define DISABLEEXTERNSPEAKERS 27u
#define ENABLELIVINGROOMSPEAKERS 28u
#define DISABLELIVINGROOMSPEAKERS 29u
#define ENABLESTUDYSPEAKERS 30u
#define DISABLESTUDYSPEAKERS 31u
#define ENABLEBEDROOMSPEAKERS 32u
#define DISABLEBEDROOMSPEAKERS 33u

enum doors {ENTRANCE, TERRACE, EXIT};
enum doorsInterruptStates {INTERRUPTABLE, INTERRUPTED};
volatile boolean doorsInterrupts[3][2] = {
  false, false, false,
  false, false, false
};

void handleInterruptEntrance (void) {
  doorsInterrupts[ENTRANCE][INTERRUPTED] = doorsInterrupts[ENTRANCE][INTERRUPTABLE];
}

void handleInterruptTerrace (void) {
  doorsInterrupts[TERRACE][INTERRUPTED] = doorsInterrupts[TERRACE][INTERRUPTABLE];
}

void handleInterruptExit (void) {
  doorsInterrupts[EXIT][INTERRUPTED] = doorsInterrupts[EXIT][INTERRUPTABLE];
}

Door entranceDoor (44u, 27u, 28u, 29u, 4u, &(doorsInterrupts[ENTRANCE][INTERRUPTED]), DOORS_LENGTH, 0);
Door terraceDoor (45u, 30u, 31u, 32u, 3u, &(doorsInterrupts[TERRACE][INTERRUPTED]), DOORS_LENGTH, 0);
Door exitDoor (46u, 33u, 34u, 35u, 2u, &(doorsInterrupts[EXIT][INTERRUPTED]), DOORS_LENGTH, 0);
RotaryTable corridorTable (10u, 16u, 45u,
                           11u, 10u, 135u,
                           90u,
                           2u, 4u, 3u, 5u, 0);
RotaryTable bedroomTable (12u, 10u, 45u,
                          13u, 10u, 135u,
                          90u,
                          6u, 8u, 7u, 9u, 0);


void setup() {
  Serial.begin(9600);

  //attachInterrupt(entranceDoor.getInterrupt(), handleInterruptEntrance, CHANGE);
  //attachInterrupt(terraceDoor.getInterrupt(), handleInterruptTerrace, CHANGE);
  //attachInterrupt(exitDoor.getInterrupt(), handleInterruptExit, CHANGE);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);

  pinMode(STUDYCURTAIN1, OUTPUT);
  pinMode(STUDYCURTAIN2, OUTPUT);
  pinMode(STUDYCURTAIN3, OUTPUT);
  pinMode(BEDROOMCURTAIN, OUTPUT);
  pinMode(CORRIDORCURTAIN, OUTPUT);

  pinMode(BEDROOMLIGTH, OUTPUT);
  pinMode(LIVINGROOMLIGHT1, OUTPUT);
  pinMode(LIVINGROOMLIGHT2, OUTPUT);
  pinMode(KITCHENLIGHT, OUTPUT);
  pinMode(STUDYLIGHT, OUTPUT);

  pinMode(LEFTEXTERNSPEAKER, OUTPUT);
  pinMode(RIGHTEXTERNSPEAKER, OUTPUT);
  pinMode(LEFTLIVINGROOMSPEAKER, OUTPUT);
  pinMode(RIGHTLIVINGROOMSPEAKER, OUTPUT);
  pinMode(LEFTSTUDYSPEAKER, OUTPUT);
  pinMode(RIGHTSTUDYSPEAKER, OUTPUT);
  pinMode(LEFTBEDROOMSPEAKER, OUTPUT);
  pinMode(RIGHTBEDROOMSPEAKER, OUTPUT);
  
  digitalWrite(LEFTEXTERNSPEAKER, HIGH);
  digitalWrite(RIGHTEXTERNSPEAKER, HIGH);
  digitalWrite(LEFTLIVINGROOMSPEAKER, HIGH);
  digitalWrite(RIGHTLIVINGROOMSPEAKER, HIGH);
  digitalWrite(LEFTSTUDYSPEAKER, HIGH);
  digitalWrite(RIGHTSTUDYSPEAKER, HIGH);
  digitalWrite(LEFTBEDROOMSPEAKER, HIGH);
  digitalWrite(RIGHTBEDROOMSPEAKER, HIGH);
        
  digitalWrite(STUDYCURTAIN1, HIGH);
  digitalWrite(STUDYCURTAIN2, HIGH);
  digitalWrite(STUDYCURTAIN3, HIGH);
  digitalWrite(BEDROOMCURTAIN, HIGH);
  digitalWrite(CORRIDORCURTAIN, HIGH);
}

unsigned char action;
signed long pendingSteps[3] = {0, 0, 0};

/*
 * implements a safe way to close doors, checking for interruptions
 */
void closeDoor(int DOOR_ID, Door door) {
  Serial.print("cerrandop ");
  doorsInterrupts[DOOR_ID][INTERRUPTABLE] = true;
  Serial.println(door.getSteps());
  if (! door.rotate(-DOORS_LENGTH, DOORS_SPEED)) {
    Serial.println("error cerrando");
    if (doorsInterrupts[DOOR_ID][INTERRUPTED]) {
      Serial.println("interrupcion");
      openDoor(DOOR_ID, door);
    }
  }
  doorsInterrupts[DOOR_ID][INTERRUPTABLE] = false;
  doorsInterrupts[DOOR_ID][INTERRUPTED] = false;
}

void openDoor(int DOOR_ID, Door door) {
  doorsInterrupts[DOOR_ID][INTERRUPTABLE] = false;
  doorsInterrupts[DOOR_ID][INTERRUPTED] = false;
  door.rotate(door.getDoorLength() - door.getSteps(), DOORS_SPEED);
}

void loop() {
  while (Serial.available() > 0) { //Everything in the serial monitor
    action = Serial.read();
    //action = action - 97;
    Serial.println(action);
    switch (action) {
      case EMERGENCYSTOP: {
        Serial.println("EMERGENCYSTOP");
        break;
      }
      case ROTATECORRIDORTOLIVINGROOM: {
        Serial.println("ROTATECORRIDORTOLIVINGROOM");
        corridorTable.unlock();
        //corridorTable.stepperMotor.rotate(-750);
        corridorTable.lock();
        break;
      }
      case ROTATECORRIDORTOSTUDY: {
        Serial.println("ROTATECORRIDORTOSTUDY");
        corridorTable.unlock();
        //corridorTable.stepperMotor.rotate(750);
        corridorTable.lock();
        break;
      }
      case ROTATECORRIDORTOBEDROOM: {
        Serial.println("ROTATECORRIDORTOBEDROOM");
        corridorTable.unlock();
        //corridorTable.stepperMotor.rotate(750);
        corridorTable.lock();
        break;
      }
      case ROTATEBEDROOMTOCORRIDOR: {
        Serial.println("ROTATEBEDROOMTOCORRIDOR");
        bedroomTable.unlock();
        //bedroomTable.stepperMotor.rotate(-750);
        bedroomTable.lock();
        break;
      }
      case ROTATEBEDROOMTOEXIT: {
        Serial.println("ROTATEBEDROOMTOEXIT");
        bedroomTable.unlock();
        //bedroomTable.stepperMotor.rotate(750);
        bedroomTable.lock();
        break;
      }
      case OPENENTRANCEDOOR: {
        entranceDoor.rotate(entranceDoor.getDoorLength() - terraceDoor.getSteps(), DOORS_SPEED);
        break;
      }
      case CLOSEENTRANCEDOOR: {
        entranceDoor.getSteps();
        entranceDoor.rotate(-entranceDoor.getSteps(), DOORS_SPEED);
        break;
      }
      case OPENTERRACEDOOR: {
        terraceDoor.rotate(terraceDoor.getDoorLength() - terraceDoor.getSteps(), DOORS_SPEED);
        break;
      }
      case CLOSETERRACEDOOR: {
        terraceDoor.rotate(-terraceDoor.getSteps(), DOORS_SPEED);
        break;
      }
      case OPENEXITDOOR: {
        exitDoor.rotate(exitDoor.getDoorLength() - exitDoor.getSteps(), DOORS_SPEED);
        break;
      }
      case CLOSEEXITDOOR: {
        exitDoor.rotate(-exitDoor.getSteps(), DOORS_SPEED);
        break;
      }
      case OPENSTUDYCURTAINS: {
        digitalWrite(STUDYCURTAIN1, LOW);
        digitalWrite(STUDYCURTAIN2, LOW);
        digitalWrite(STUDYCURTAIN3, LOW);
        break;
      }
      case CLOSESTUDYCURTAINS: {
        digitalWrite(STUDYCURTAIN1, HIGH);
        digitalWrite(STUDYCURTAIN2, HIGH);
        digitalWrite(STUDYCURTAIN3, HIGH);
        break;
      }
      case OPENBEDROOMCURTAIN: {
        digitalWrite(BEDROOMCURTAIN, LOW);
        break;
      }
      case CLOSEBEDROOMCURTAIN: {
        digitalWrite(BEDROOMCURTAIN, HIGH);
        break;
      }
      case OPENCORRIDORCURTAIN: {
        digitalWrite(CORRIDORCURTAIN, LOW);
        break;
      }
      case CLOSECORRIDORCURTAIN: {
        digitalWrite(CORRIDORCURTAIN, HIGH);
        break;
      }
      case TURNBEDROOMLIGHTON: {
        digitalWrite(BEDROOMLIGTH, HIGH);
        break;
      }
      case TURNBEDROOMLIGHTOFF: {
        digitalWrite(BEDROOMLIGTH, LOW);
        break;
      }
      case TURNLIVINGROOMLIGHTSON: {
        digitalWrite(LIVINGROOMLIGHT1, HIGH);
        digitalWrite(LIVINGROOMLIGHT2, HIGH);
        break;
      }
      case TURNLIVINGROOMLIGHTSOFF: {
        digitalWrite(LIVINGROOMLIGHT1, LOW);
        digitalWrite(LIVINGROOMLIGHT2, LOW);
        break;
      }
      case TURNKITCHENLIGHTON: {
        digitalWrite(KITCHENLIGHT, HIGH);
        break;
      }
      case TURNKITCHENLIGHTOFF: {
        digitalWrite(KITCHENLIGHT, LOW);
        break;
      }
      case TURNSTUDYLIGHTON: {
        digitalWrite(STUDYLIGHT, HIGH);
        break;
      }
      case TURNSTUDYLIGHTOFF: {
        digitalWrite(STUDYLIGHT, LOW);
        break;
      }
      case ENABLEEXTERNSPEAKERS: {
        digitalWrite(LEFTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(RIGHTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(LEFTSTUDYSPEAKER, HIGH);
        digitalWrite(RIGHTSTUDYSPEAKER, HIGH);
        digitalWrite(LEFTBEDROOMSPEAKER, HIGH);
        digitalWrite(RIGHTBEDROOMSPEAKER, HIGH);
        digitalWrite(LEFTEXTERNSPEAKER, LOW);
        digitalWrite(RIGHTEXTERNSPEAKER, LOW);
        break;
      }
      case DISABLEEXTERNSPEAKERS: {
        digitalWrite(LEFTEXTERNSPEAKER, HIGH);
        digitalWrite(RIGHTEXTERNSPEAKER, HIGH);
        break;
      }
      case ENABLELIVINGROOMSPEAKERS: {
        digitalWrite(LEFTLIVINGROOMSPEAKER, LOW);
        digitalWrite(RIGHTLIVINGROOMSPEAKER, LOW);
        digitalWrite(LEFTSTUDYSPEAKER, HIGH);
        digitalWrite(RIGHTSTUDYSPEAKER, HIGH);
        digitalWrite(LEFTBEDROOMSPEAKER, HIGH);
        digitalWrite(RIGHTBEDROOMSPEAKER, HIGH);
        digitalWrite(LEFTEXTERNSPEAKER, HIGH);
        digitalWrite(RIGHTEXTERNSPEAKER, HIGH);
        break;
      }
      case DISABLELIVINGROOMSPEAKERS: {
        digitalWrite(LEFTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(RIGHTLIVINGROOMSPEAKER, HIGH);
        break;
      }
      case ENABLESTUDYSPEAKERS: {
        digitalWrite(LEFTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(RIGHTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(LEFTSTUDYSPEAKER, LOW);
        digitalWrite(RIGHTSTUDYSPEAKER, LOW);
        digitalWrite(LEFTBEDROOMSPEAKER, HIGH);
        digitalWrite(RIGHTBEDROOMSPEAKER, HIGH);
        digitalWrite(LEFTEXTERNSPEAKER, HIGH);
        digitalWrite(RIGHTEXTERNSPEAKER, HIGH);
        break;
      }
      case DISABLESTUDYSPEAKERS: {
        digitalWrite(LEFTSTUDYSPEAKER, HIGH);
        digitalWrite(RIGHTSTUDYSPEAKER, HIGH);
        break;
      }
      case ENABLEBEDROOMSPEAKERS: {
        digitalWrite(LEFTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(RIGHTLIVINGROOMSPEAKER, HIGH);
        digitalWrite(LEFTSTUDYSPEAKER, HIGH);
        digitalWrite(RIGHTSTUDYSPEAKER, HIGH);
        digitalWrite(LEFTBEDROOMSPEAKER, LOW);
        digitalWrite(RIGHTBEDROOMSPEAKER, LOW);
        digitalWrite(LEFTEXTERNSPEAKER, HIGH);
        digitalWrite(RIGHTEXTERNSPEAKER, HIGH);
        break;
      }
      case DISABLEBEDROOMSPEAKERS: {
        digitalWrite(LEFTBEDROOMSPEAKER, HIGH);
        digitalWrite(RIGHTBEDROOMSPEAKER, HIGH);
        break;
      }
    }
  }
}
