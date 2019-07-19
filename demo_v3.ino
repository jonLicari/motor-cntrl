/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: July 15 2019
 *  demo_v3
 *  This code corresponds to the demo_v3 wiring diagram
 *  Contains 4 flight modes: Park, Pan, Flight, 1:1
 *  Modes are button activated and displayed on either LCD or 7 Seg
 */

#include "demo_v3.h"

int throttle_out = 0;
int pedal_out = 0;
int joy_out = 0;

float op1, op2, op3;

int old, sel, select; // Flight mode select variable

void setup() {
  
  select = 0; // Initialize flight mode to 0 - Idle state
  
  // Set pins to analog output
  pinMode(throttle, INPUT);
  pinMode(pedal, INPUT);
  pinMode(joy, INPUT);
  pinMode(mode, INPUT);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {

  // Read in the state variable
  sel = analogRead(mode);

  // Only run state check if there has been a change in ANALOG voltage
  if (sel != old) { // NOTE: This only works if the ANALOG signal is consistent and steady
    
    // Determine the state based on the analog voltage
    if (sel >= exp0*0.995 && sel =< exp0*1.005)
      select = 0;
    else if (sel >= exp1*0.995 && sel =< exp1*1.005)
      select = 1;
    else if (sel >= exp2*0.995 && sel =< exp2*1.005)
      select = 2;
    else if (sel >= exp3*0.995 && sel =< exp3*1.005)
      select = 3;
  }
    
  // output_var = analogRead(pin_var);
  throttle_out = analogRead(throttle);
  pedal_out = analogRead(pedal);
  joy_out = analogRead(joy);
  
  //rdVolt(); // Print the controls' analog voltage to the Serial monitor

  // Map the 10-bit ADC digital signal to an 8-bit digital signal for the DAC
  pedal_out = map(pedal_out, 0, 1023, 0, 255);
  throttle_out = map(throttle_out, 0, 1023, 0, 255);
  joy_out = map(joy_out, 0, 1023, 0, 255);

  // Switch case for flight mode selection
  switch (select) {
     case 0
      idle();
      break;

     case 1
      flight();
      break;

     case 2
      pan();
      break;

     case 3
      one();
      break;

     default // In the event of mode select failure, vehicle maintains its position
      idle();
      break;
  }

  // Prints the motor speed to the Serial Monitor
  mtrSpeed(); 

  // Store the current state 
  old = sel; // NOTE: sel is ANALOG 
  
  // Delay to complete computation
  delay(5);
}

void idle() {                         //Essentially a do-nothing state
  /* Do-nothing state
   * Retains previously set values of motors
   * Controls can still be manipulated and printed to Serial Monitor
   * Control input does not affect the motor speed
   */
  delay(5);
}

void flight() {                       //PWM output

  // Throttle input
  analogWrite(m2, pedal_out);         // P controls T motor

  // Joystick input
  if (joy_out < 128) {                // Left turn
    analogWrite(m1, (128-joy_out));   // L motor decreases speed
    analogWrite(m3, (256 - joy_out)); // R motor increases speed
  }
  else {                              // Right turn
    analogWrite(m1, joy_out);         // L motor increases speed
    analogWrite(m3, (255 - joy_out)); // R motor decreases speed
  }

  // Pedal input
  
  
  
  delay(5);
}

void pan() {                          //Joystick only 
  
  if (joy_out < 128) {                // Left turn
    analogWrite(m1, (127 - joy_out)); // L motor decreases speed
    analogWrite(m3, (255 - joy_out)); // R motor increases speed
  }
  else { // Right turn
    analogWrite(m1, joy_out);         // L motor increases speed
    analogWrite(m3, (255 - joy_out)); // R motor decreases speed
  }
  
  delay(5);
}

void one() {                          //1:1 control 
  
  // Send PWM output
  analogWrite(m3, joy_out);           // Joystick manipulates motor3
  analogWrite(m2, pedal_out);         // Pedal manipulates motor2
  analogWrite(m1, throttle_out);      // Throttle manipulates 
}

void rdVolt() {   // Print the values to the serial monitor

  Serial.print("Throttle voltage = ");
  Serial.print(throttle_out);

  Serial.print("\nPedal pot. voltage = ");
  Serial.print(pedal_out);

  Serial.print("\nJoystick x-axis position {0,1023} = ");
  Serial.print(joy_out);

  Serial.print("\n\n-----------------");
}

void mtrSpeed() {   // Prints the motor speed to the serial monitor
  
  // Show operating speed as a percentage of the maximum speed
  Serial.print("\nJoystick Motor operating speed = ");
  op1 = (joy_out/255.0)*100.00;
  Serial.print(op1);
  Serial.print("%");

  Serial.print("\npedal Motor operating speed = ");
  op2 = (pedal_out/255.0)*100.00;
  Serial.print(op2);
  Serial.print("%");

  Serial.print("\nthrottle Motor operating speed = ");
  op3 = (throttle_out/255.0)*100.00;
  Serial.print(op3);
  Serial.print("%");
  
  Serial.print("\n\n-----------------");
}
