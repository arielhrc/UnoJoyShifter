#include <HX711.h>
#include "UnoJoy.h" 
 
//  G27 shifter to USB interface
//  based on a TEENSY
//  Original by pascalh http://insidesimracing.tv/forums/topic/13189-diy-g25-shifter-interface-with-h-pattern-sequential-and-handbrake-modes/
//  Adapted by jssting for UNO
//  Updated to include the POV Hat switch

//  1 operating mode
//  - H-pattern shifter
//
//  G27 shifter pinout
//
//  DB9  Color    Shifter Description       Teensy
//  1    Purple    1      Button Clock      pin 2
//  2    Grey     7       Button Data       pin 3
//  3    Yellow   5       Button !CS & !PL  pin 4
//  4    Orange   3       Shifter X axis    pin 38 (A0)
//  5    White      2       SPI input
//  6    Black    8       GND               GND
//  7    Red   6       +5V               VCC
//  8    Green    4       Shifter Y axis    pin 39 (A1)
//  9    Red    1       +5V

//  Other Items
//  Item                Pin
//  HandBrake           A5

// Teensy pin definitions
#define LED_PIN            13 //A2
#define DATA_IN_PIN        3 // A0
#define MODE_PIN           2 // A1
#define CLOCK_PIN          5 //A3
#define X_AXIS_PIN         A0 //A9
#define Y_AXIS_PIN         A2 //A8

// H-shifter mode analog axis thresholds
#define HS_XAXIS_12        400 //Gears 1,2
#define HS_XAXIS_56        675 //Gears 5,6, R
#define HS_YAXIS_135       700 //Gears 1,3,5
#define HS_YAXIS_246       375 //Gears 2,4,6, R

// HX711 circuit wiring
//#define HANDBRAKE          7 //A10

const int LOADCELL_DOUT_PIN = A4;
const int LOADCELL_SCK_PIN = A5;

HX711 scale;

// Digital inputs definitions
#define DI_REVERSE         1

#define DI_RED_CENTERRIGHT 4
#define DI_RED_CENTERLEFT  5
#define DI_RED_RIGHT       6
#define DI_RED_LEFT        7
#define DI_BLACK_TOP       8
#define DI_BLACK_RIGHT     9
#define DI_BLACK_LEFT      10
#define DI_BLACK_BOTTOM    11
#define DI_DPAD_RIGHT      12
#define DI_DPAD_LEFT       13
#define DI_DPAD_BOTTOM     14
#define DI_DPAD_TOP        15
#define AI_HANDBRAKE       16

bool enableDebug = false;
void setup()
{
  SetupPins();
  setupUnoJoy();
}

void loop()
{
  dataForController_t G27Shifter = getControllerData();
  setControllerData(G27Shifter);
}

void SetupPins(void){
  // G27 shifter analog inputs configuration
  pinMode(X_AXIS_PIN, INPUT_PULLUP);   // X axis
  pinMode(Y_AXIS_PIN, INPUT_PULLUP);   // Y axis
  //pinMode(HANDBRAKE, INPUT);
  pinMode(LOADCELL_DOUT_PIN, INPUT);
  
  // G27 shift register interface configuration
  pinMode(DATA_IN_PIN, INPUT);         // Data in
  pinMode(MODE_PIN, OUTPUT);           // Parallel/serial mode
  pinMode(CLOCK_PIN, OUTPUT);          // Clock

  // LED output mode configuration
  pinMode(LED_PIN, OUTPUT);            // LED

  // Virtual serial interface configuration
//  Serial.begin(38400); // use for debugging

  // Digital outputs initialization
  digitalRead(DATA_IN_PIN);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(MODE_PIN, HIGH);
  digitalWrite(CLOCK_PIN, HIGH);  

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_gain();
  scale.set_scale(-10000);
  scale.tare();                // reset the scale to 0
  
 
}

dataForController_t getControllerData(void){

  dataForController_t G27Shifter = getBlankDataForController();

  // Reading of button states from G27 shift register
  int b[16];

  digitalWrite(MODE_PIN, LOW);         // Switch to parallel mode: digital inputs are read into shift register
  delayMicroseconds(5);               // Wait for signal to settle
  digitalWrite(MODE_PIN, HIGH);        // Switch to serial mode: one data bit is output on each clock falling edge


  for (int i = 0; i < 16; i++)         // Iteration over both 8 bit registers
  {
    digitalWrite(CLOCK_PIN, LOW);      // Generate clock falling edge
    delayMicroseconds(5);             // Wait for signal to settle

    b[i] = digitalRead(DATA_IN_PIN);   // Read data bit and store it into bit array

    digitalWrite(CLOCK_PIN, HIGH);     // Generate clock rising edge
    delayMicroseconds(5);             // Wait for signal to settle
  }


  //Hand Brake Section

  int hbState = 0;
  hbState = abs(scale.get_units(1));
  if (hbState>20) G27Shifter.leftStickX = hbState; //Serial.println(Val);
  else G27Shifter.leftStickX =0 ; 
 
  //hbState =  analogRead(HANDBRAKE);
  //Serial.println(hbState);
 //// if (hbState > 20 ) {
 //  G27Shifter.button16 = 0;  }
//  else {
///    G27Shifter.button16 = 0;   
 // }

  //Shifter Section
  // Reading of shifter position
  int x = analogRead(X_AXIS_PIN);      // X axis
  int y = analogRead(Y_AXIS_PIN);      // Y axis

  //String valToPrint = String("Y: " + y + ".X: " + x);
  if (enableDebug){
    Serial.print("Y: ");
    Serial.print(y);
    Serial.print(", X: ");
    Serial.println(x);
  }
  // Current gear calculation
  int gear = 0;                        // Default value is neutral

  if (x < HS_XAXIS_12)               // Shifter on the left?
  {
    if (y > HS_YAXIS_135) gear = 1;  // 1st gear
    if (y < HS_YAXIS_246) gear = 2;  // 2nd gear
  }
  else if (x > HS_XAXIS_56)          // Shifter on the right?
  {
    if (y > HS_YAXIS_135) gear = 5;  // 5th gear
    if (y < HS_YAXIS_246) gear = 6;  // 6th gear
  }
  else                               // Shifter is in the middle
  {
    if (y > HS_YAXIS_135) gear = 3;  // 3rd gear
    if (y < HS_YAXIS_246) gear = 4;  // 4th gear
  }

  if (gear != 6) b[DI_REVERSE] = 0;  // Reverse gear is allowed only on 6th gear position
  if (b[DI_REVERSE] == 1) gear = 7;  // 6th gear is deactivated if reverse gear is engaged


  if (gear < 1) {

    for (int gr = 1;gr<8;gr++)
    {
    //  G27Shifter.clearButton(gr, G27Shifter);
      clearButton(gr, G27Shifter);
    }
  }
  /*G27Shifter.triangleOn = 0;
  G27Shifter.circleOn = 0;
  G27Shifter.squareOn = 0;
  G27Shifter.crossOn = 0;
  G27Shifter.l1On = 0;
  G27Shifter.r1On = 0;
  G27Shifter.selectOn =0;
  G27Shifter.startOn = 0;
  G27Shifter.homeOn = 0; 
  } */
 
  // Depress virtual button for current gear
  if (gear > 0) setButton(gear, G27Shifter);  
//  //Depress virtual button for current gear


 /*
  switch(gear){  
    case 0: //neutral  // Release virtual buttons for all gears
      
      G27Shifter.triangleOn = 0;
      G27Shifter.circleOn = 0;
      G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 0;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 0;
      G27Shifter.selectOn =0;
      break;
    case 1: //1st Gear
      G27Shifter.squareOn = 1;
      G27Shifter.crossOn = 0;
      G27Shifter.circleOn = 0;
      G27Shifter.triangleOn = 0;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 0;
      G27Shifter.l2On = 0;
      break;
    case 2: //2nd Gear
      G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 1;
      G27Shifter.circleOn = 0;
      G27Shifter.triangleOn = 0;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 0;
      G27Shifter.l2On = 0;
      break;
    case 3: //3rd gear
      G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 0;
      G27Shifter.circleOn = 1;
      G27Shifter.triangleOn = 0;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 0;
      G27Shifter.l2On = 0;
      break;
    case 4: //4th Geat
    G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 0;
      G27Shifter.circleOn = 0;
      G27Shifter.triangleOn = 1;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 0;
      G27Shifter.l2On = 0;
        break;
    case 5: //5th gear
      G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 0;
      G27Shifter.circleOn = 0;
      G27Shifter.triangleOn = 0;
      G27Shifter.l1On = 1;
      G27Shifter.r1On = 0;
      G27Shifter.l2On = 0;
      break;
    case 6: //6th Gear
      G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 0;
      G27Shifter.circleOn = 0;
      G27Shifter.triangleOn = 0;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 1;
      G27Shifter.l2On = 0;
      break;
    case 7: //Reverse
      G27Shifter.squareOn = 0;
      G27Shifter.crossOn = 0;
      G27Shifter.circleOn = 0;
      G27Shifter.triangleOn = 0;
      G27Shifter.l1On = 0;
      G27Shifter.r1On = 0;
      G27Shifter.l2On = 1;
      break;
  }
  */
  
  //all other normal button states for buttons 8 - 16
      // Set state of virtual buttons for all the physical buttons (Excluding Gears and the hat switch)
      for (int i = 4; i < 12; i++) setButton((3 + i),G27Shifter);

 // Hat Switch Section
  for (int i = 12; i < 16; i++)
  {
    //release hat switch
    if (!b[DI_DPAD_TOP] && !b[DI_DPAD_RIGHT] && !b[DI_DPAD_BOTTOM] && !b[DI_DPAD_LEFT]) {
      G27Shifter.dpadUpOn = 0;
      G27Shifter.dpadLeftOn = 0;
      G27Shifter.dpadDownOn = 0;
      G27Shifter.dpadRightOn = 0;
    }

    //primary button only directions
    else if (b[DI_DPAD_TOP] && !b[DI_DPAD_RIGHT] && !b[DI_DPAD_LEFT]) G27Shifter.dpadUpOn=1 ;
    else if (b[DI_DPAD_RIGHT] && !b[DI_DPAD_TOP] && !b[DI_DPAD_BOTTOM]) G27Shifter.dpadRightOn=1;
    else if (b[DI_DPAD_BOTTOM] && !b[DI_DPAD_RIGHT] && !b[DI_DPAD_LEFT]) G27Shifter.dpadDownOn=1;
    else if (b[DI_DPAD_LEFT] && !b[DI_DPAD_BOTTOM] && !b[DI_DPAD_TOP]) G27Shifter.dpadLeftOn=1;

    //combined button directions
    else if (b[DI_DPAD_TOP] && b[DI_DPAD_RIGHT]) {
      G27Shifter.dpadRightOn=1;
      G27Shifter.dpadUpOn=1;
    }
    else if (b[DI_DPAD_RIGHT] && b[DI_DPAD_BOTTOM]) {
      G27Shifter.dpadRightOn=1;
      G27Shifter.dpadDownOn=1;
    }
    else if (b[DI_DPAD_BOTTOM] && b[DI_DPAD_LEFT]) {
      G27Shifter.dpadLeftOn=1;
      G27Shifter.dpadDownOn=1;
    }
    else if (b[DI_DPAD_TOP] && b[DI_DPAD_LEFT]) {
      G27Shifter.dpadLeftOn=1;
      G27Shifter.dpadUpOn=1;
    }
  } 
    
      /*
      G27Shifter.r2On = b[4];
      G27Shifter.selectOn = b[5];
      G27Shifter.startOn = b[6];
      G27Shifter.homeOn = b[7];
      G27Shifter.l3On = b[8];
      G27Shifter.r3On = b[9];
      G27Shifter.l4On = b[10];   
      G27Shifter.r4On = b[11];
      */

  // Write new virtual G27Shifter state
return G27Shifter;
}

void setButton(int button, dataForController_t dataForController){
dataForController_t controllerData = dataForController;
    switch(button){
    case 1:
     controllerData.button1 = 1; 
     break;
      
    case 2:             
      controllerData.button2 = 1; 
      break; 

    case 3:    
      controllerData.button3 = 1; 
      break; 
    
    case 4:    
      controllerData.button4 = 1; 
      break; 

    case 5:    
      controllerData.button5 = 1; 
      break; 

    case 6:    
      controllerData.button6 = 1; 
      break; 

    case 7:    
      controllerData.button7 = 1; 
      break; 

    case 8:    
      controllerData.button8 = 1; 
      break; 

    case 9:    
      controllerData.button9 = 1; 
      break; 
    
    case 10:    
      controllerData.button10 = 1; 
      break; 
    
    case 11:   
      controllerData.button11 = 1; 
      break; 
    
    case 12:    
      controllerData.button12 = 1; 
      break; 
    
    case 13:
      controllerData.button13 = 1; 
      break; 
    
    case 14:
      controllerData.button14 = 1;
      break; 
    
    case 15:    
      controllerData.button15 = 1; 
      break;
    
    case 16:    
      controllerData.button16 = 1; 
      break;
    }
  //return controllerData;
  }

void clearButton(int button, dataForController_t dataForController){
    dataForController_t controllerData = dataForController;
    switch(button){
    case 1:
     controllerData.button1 = 0; 
     break;
      
    case 2:             
      controllerData.button2 = 0; 
      break; 

    case 3:    
      controllerData.button3 = 0; 
      break; 
    
    case 4:    
      controllerData.button4 = 0; 
      break; 

    case 5:    
      controllerData.button5 = 0; 
      break; 

    case 6:    
      controllerData.button6 = 0; 
      break; 

    case 7:    
      controllerData.button7 = 0; 
      break; 

    case 8:    
      controllerData.button8 = 0; 
      break; 

    case 9:    
      controllerData.button9 = 0; 
      break; 
    
    case 10:    
      controllerData.button10 = 0; 
      break; 
    
    case 11:   
      controllerData.button11 = 0; 
      break; 
    
    case 12:    
      controllerData.button12 = 0; 
      break; 
    
    case 13:
      controllerData.button13 = 0; 
      break; 
    
    case 14:
      controllerData.button14 = 0;
      break; 
    
    case 15:    
      controllerData.button15 = 0; 
      break;
    
    case 16:    
      controllerData.button16 = 0; 
      break;
    }
  //return controllerData;
  }  
