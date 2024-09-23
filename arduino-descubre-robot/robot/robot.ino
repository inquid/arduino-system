#include "StepperMotor.h"

/*
 * pins
 */
//Horizontal Motor

#define PULSEA 4u
#define DIRECTIONA 5u
#define ENAA 6u
#define LOWA 2u

#define PULSEB 46u
#define DIRECTIONB 45u
#define ENAB 22u
#define LOWB 7u

#define RELAYPROJECTOR 14u
#define RELAYLENS 15u
#define RELAYLAMP 44u
#define RELAYLEDS 9u

#define RELAYDCMOTORONE 38u
#define RELAYDCMOTORTWO 40u
#define RELAYDCMOTORTHREE 42u

// variables de giros
#define ROTATE25LEFT 100
#define ROTATE25RIGHT -100
#define ROTATE45LEFT 400
#define ROTATE45RIGHT -400
#define ROTATE90LEFT 800
#define ROTATE90RIGHT -800
#define ROTATE180LEFT 1600
#define ROTATE180RIGHT -1600
#define ROTATETALKMIN 15
#define ROTATETALKMAX 70

//variables de movimientos
#define MOVEONEMETTER 3333u
#define MOVETWOMETTER 6666u
#define MOVETHREEMETTER 9999u
#define MOVEFOURMETTER 13332u
#define MOVEFIVEMETTER 16665u
#define MOVESIXMETTER 19998u

#define LEDSTOOLOW 50u
#define LEDSLOW 75u
#define LEDSLOWMEDIUM 100u
#define LEDSNORMAL 150u
#define LEDSHIGHMEDIUM 175u
#define LEDSLOWHIGH 200u
#define LEDSHIGH 225u
#define LEDSTOOHIGH 250u

//velocity variables
#define MOVESLOW 50u
#define MOVENORMAL 360u
#define MOVEFAST 500u

/* Define function codes */
#define EMERGENCYSTOP 0u
//Rotate Functions
	#define ROTATEROBOTOSEEVISITORS 1u
	#define ROTATETOPROJECTAREAONE 2u
	#define ROTATETOTV 3u 
	#define ROTATETOTERRACE 4u 
	#define ROTATETOPROJECTIONAREATWO 5u
	#define ROTATETOWALK 6u
	#define ROTATESLIGTHLYLEFT 26u
	#define ROTATESLIGTHLYRIGHT 27u
//Projector & leds Functions
	#define TURNPROJECTORON 7u
	#define TURNPROJECTOROOFF 8u
	#define TURNLENSON 9u
	#define TURNLENSOFF 10u	
	#define TURNLEDSEYETOSTARTTOUR 11u
	#define MAKEROBOTTALK 12u
	#define TURNLAMPON 13u
	#define TURNLAMPOFF 14u
//Move functions
	#define MOVEINITIALPOINTTOPROJECTIONAREAONE 15u
	#define MOVEPROJECTONETOTABLEONE 16u
	#define MOVETABLEONETOSTUDIOSTANDBY 17u
	#define MOVESTUDIOSTANDBYTOTABLEONE 18u
	#define MOVETABLEONETOPROJECTIONAREATWO 19u
	#define MOVEPROJECTIONAREATWOTOSTART 20u
	#define STOPANYMOVE 21u
//Custom Functions used for tests
	#define WALKDIRECTIONONE 22u
	#define WALKDIRECTIONTWO 23u
	#define TURNONLEDS 24u
	#define TURNOFFLEDS 25u
unsigned char action;
	
/* Define motors */
StepperMotor horizontal (PULSEB, DIRECTIONB, ENAB, LOWB); //Motor Eje Horizontal
StepperMotor vertical (PULSEA, DIRECTIONA, ENAA, LOWA); //Motor Eje Vertical
//Funciones Personalizadas
	void moveDirectionOne(int time){
	    //Prende el relay para generar movimiento en una dirección.
	    digitalWrite(RELAYDCMOTORONE, HIGH);
	    //Lo mueve cierto tiempo
	    delay(time);
	    //Detiene el movimiento
	    digitalWrite(RELAYDCMOTORONE, LOW);
	}

	void moveDirectionTwo(int time){
	  //Se asegura de tener todos los relays abajo para evitar cortos
	  digitalWrite(RELAYDCMOTORONE, LOW); 
	  digitalWrite(RELAYDCMOTORTWO, LOW);
	  digitalWrite(RELAYDCMOTORTHREE, LOW);
	  //Prende los relays de cambio de dirección
	  digitalWrite(RELAYDCMOTORTWO, HIGH);
	  digitalWrite(RELAYDCMOTORTHREE, HIGH);
	  //Prende el relay de encendido
	  digitalWrite(RELAYDCMOTORONE, HIGH); 
	  // Lo mueve cierto tiempo
	  delay(time);
	  //Apaga los relays y el movimiento.
	  digitalWrite(RELAYDCMOTORONE, LOW); 
	  digitalWrite(RELAYDCMOTORTWO, LOW);
	  digitalWrite(RELAYDCMOTORTHREE, LOW);
	}

	//Stop any moving
	void stopAnyDirection(){
	    //Prende el relay para generar movimiento en una dirección.
	  digitalWrite(RELAYDCMOTORONE, LOW); 
	  digitalWrite(RELAYDCMOTORTWO, LOW);
	  digitalWrite(RELAYDCMOTORTHREE, LOW);
	}

	//Move direction one without stop
	void moveDirectionOneNonStop(){
	    //Prende el relay para generar movimiento en una dirección.
	    digitalWrite(RELAYDCMOTORONE, HIGH);
	}

	//Move direction two withoutstop
	void moveDirectionTwoNonStop(){
	  //Se asegura de tener todos los relays abajo para evitar cortos
	  digitalWrite(RELAYDCMOTORONE, LOW); 
	  digitalWrite(RELAYDCMOTORTWO, LOW);
	  digitalWrite(RELAYDCMOTORTHREE, LOW);
	  //Prende los relays de cambio de dirección
	  digitalWrite(RELAYDCMOTORTWO, HIGH);
	  digitalWrite(RELAYDCMOTORTHREE, HIGH);
	  //Prende el relay de encendido
	  digitalWrite(RELAYDCMOTORONE, HIGH); 
	}
	/**
	 * Prende el proyector, simula la presion del botón de encendido por medio segundo.
	 */
	void turnOnProjector(){
	      digitalWrite(RELAYPROJECTOR, HIGH);
	      delay(500);
	      digitalWrite(RELAYPROJECTOR, LOW);
	}

	/**
	 * Prende la lente del proyecto
	 */
	void turnOnLens(){
	      digitalWrite(RELAYLENS, LOW);
	}

	/**
	 * Apaga la lente del proyector
	 */
	void turnOffLens(){
	      digitalWrite(RELAYLENS, HIGH);
	}

	/**
	 * Prende la lámpara del proyector
	 */
	void turnLampOn(){
	      digitalWrite(RELAYLAMP, HIGH);
	}

	/**
	 * Apaga la lámpara del proyector
	 */
	void turnLampOff(){
	      digitalWrite(RELAYLAMP, LOW);
	}



	/**
	 * Prende el proyector, simula la presion del botón de encendido por medio segundo.
	 */
	void turnOffProjector() {
	      digitalWrite(RELAYPROJECTOR, HIGH);
	      delay(500);
	      digitalWrite(RELAYPROJECTOR, LOW);
	      delay(100);
	      digitalWrite(RELAYPROJECTOR, HIGH);
	      delay(500);
	      digitalWrite(RELAYPROJECTOR, LOW);
	}

	//Controled current
	void lightLedsEye(int value){
	    analogWrite(RELAYLEDS,value);
	}

	//Max current
	void lightLedsEye(){
	    analogWrite(RELAYLEDS,HIGH);
	}

	//Turn Off leds
	void turnOffLedsEye(){
	    analogWrite(RELAYLEDS,LOW);
	}
	
    void makeRobotTalk (int times) {
        for(int x = 0; x < times; x++) {
            delay(400);
            //horizontal.rotate(ROTATETALKMAX, MOVENORMAL);
            lightLedsEye(random(LEDSLOW, LEDSHIGH));
            char random1 = random(ROTATETALKMIN, ROTATETALKMAX);
            vertical.rotate((unsigned int) random1);
            lightLedsEye(random(LEDSLOW, LEDSHIGH));
            //horizontal.rotate(ROTATETALKMAX, MOVENORMAL);
            char random2 = random(ROTATETALKMIN, ROTATETALKMAX);
            vertical.rotate((unsigned int) -random1);
            delay(1000);
            vertical.rotate((unsigned int) random2);
            lightLedsEye(random(LEDSLOW, LEDSHIGH));
            delay(400);
            vertical.rotate((unsigned int) -random2);
        }
        turnOffLedsEye();
    }
        
    /* Rota de 90° a la posición inicial */
    void flashLeds(int times){
	    lightLedsEye();
	    for(int x=0; x<times; x++){
	        delay(300);
	        lightLedsEye(LEDSNORMAL);
	    }
	    turnOffLedsEye();
    }
        
	void setup() {
	  Serial.begin(9600);
	  pinMode(RELAYPROJECTOR, OUTPUT);
	  pinMode(RELAYLENS, OUTPUT);
	  pinMode(RELAYLAMP, OUTPUT);
	  pinMode(RELAYLEDS, OUTPUT);

	  pinMode(RELAYDCMOTORONE, OUTPUT);
	  pinMode(RELAYDCMOTORTWO, OUTPUT);
	  pinMode(RELAYDCMOTORTHREE, OUTPUT);
	  randomSeed(analogRead(0));
	}
	
	void loop() {
	  while (Serial.available() > 0) { 
	    action = Serial.read();
	    action = action; 
	    Serial.println(action);

	    switch (action) {
	      /* Detener todos los motores, bajar todos los relays y sonar alarma */
	      case EMERGENCYSTOP: {
    		break;
	      }
	      case ROTATEROBOTOSEEVISITORS: {
    		/* Rota de 90° a la posición inicial */ 	
    		vertical.rotate(ROTATE90LEFT);
    		break;
	      }
        case ROTATETOPROJECTAREAONE: {
    		vertical.rotate(ROTATE90RIGHT);
    		break;
	      }
        case ROTATETOTV: {
    		vertical.rotate(ROTATE45LEFT);
    		break;
	      }
        case ROTATETOTERRACE: {
    		vertical.rotate(ROTATE180RIGHT);
    		break;
	      }
        case ROTATETOPROJECTIONAREATWO: {
    		vertical.rotate(ROTATE180LEFT);
    		break;
	      }
	    case ROTATETOWALK: {
		    vertical.rotate(ROTATE180RIGHT);
		    break;
	      }
	      case ROTATESLIGTHLYLEFT: {
		    vertical.rotate(ROTATE25LEFT);
		    break;
	      }
          case ROTATESLIGTHLYRIGHT: {
		    vertical.rotate(ROTATE25RIGHT);
		    break;
	      }
	      case TURNPROJECTORON: {
		turnOnProjector();
		break;
	      }
	      case TURNPROJECTOROOFF: {
		turnOffProjector();
		break;
	      }
	      case TURNLENSON: {
		turnOnLens();
		break;
	      }
	      case TURNLENSOFF: {
		turnOffLens();
		break;
	      }
	      case TURNLAMPON: {
		turnLampOn();
		break;
	      }
	      case TURNLAMPOFF: {
		turnLampOff();
		break;
	      }
		case STOPANYMOVE: {
		 stopAnyDirection();
		break;
	      }
	      case TURNLEDSEYETOSTARTTOUR: {
                  flashLeds(10);
		break;
	      }
	       case MAKEROBOTTALK: {
                  makeRobotTalk(5);
		break;
	      }
	       case WALKDIRECTIONONE: {
		moveDirectionOne(MOVESIXMETTER);
		break;
	      }
	       case WALKDIRECTIONTWO: {
		moveDirectionTwoNonStop();
		break;
	      }
		case MOVEINITIALPOINTTOPROJECTIONAREAONE: {
		 moveDirectionOne((4 * MOVEONEMETTER) / 5);
		break;
	      }
		case MOVEPROJECTONETOTABLEONE: {
		 moveDirectionOne(MOVESIXMETTER);
		break;
	      }
		case MOVETABLEONETOSTUDIOSTANDBY: {
		 moveDirectionOne(MOVEONEMETTER);
		break;
	      }
		case MOVESTUDIOSTANDBYTOTABLEONE: {
		 moveDirectionOne(MOVEONEMETTER);
		break;
	      }
		case MOVETABLEONETOPROJECTIONAREATWO: {
		 moveDirectionOne(MOVEONEMETTER);
		break;
	      }
		case MOVEPROJECTIONAREATWOTOSTART: {
		 moveDirectionOne(MOVEONEMETTER);
		break;
	      }
		case TURNONLEDS: {
		 lightLedsEye();
		break;
	      }case TURNOFFLEDS: {
		 turnOffLedsEye();
		break;
	      }
	    }     
	  }
	}


