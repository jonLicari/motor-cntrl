/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: 10 August 2019
 *  demo_v4
 *  This code corresponds to the demo_v4 wiring diagram
 *  Contains 4 flight modes: Park, Pan, Flight, 1:1
 *  RF Transmission of PWM Output
 */

#include <VirtualWire.h>
#include "demo_v4.h"

void setup() {
  
  select = 0; // Initialize flight mode to 0 - Idle state
  
  // Set pins to analog input
  pinMode(throttle, INPUT);
  pinMode(pedal, INPUT);
  pinMode(joy, INPUT);
  pinMode(mode, INPUT);

  // Set LED flight mode LEDs as output
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  // Tx initialization
  vw_set_tx_pin(tx); // Set transmit pin
  vw_setup(2000); // Bits per second
  pinMode(tx_led, OUTPUT);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {

  // Read in the state variable
  sel = analogRead(mode);
  
  // Only run state check if there has been a change in ANALOG voltage
  if (sel != old && sel != 0) { // NOTE: This only works if the ANALOG signal is consistent and steady
    
    // Determine the state based on the analog voltage
    if (sel >= exp0*0.995 && sel <= exp0*1.005)
      select = 0;
    else if (sel >= exp1*0.995 && sel <= exp1*1.005)
      select = 1;
    else if (sel >= exp2*0.995 && sel <= exp2*1.005)
      select = 2;
    else if (sel >= exp3*0.995 && sel <= exp3*1.005)
      select = 3;

    old = sel;
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
     case 0:
      idle();
      //Serial.println("Enter Idle State");
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

     case 1:
      flight();
      //Serial.println("Enter Flight State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

     case 2:
      pan();
      //Serial.println("Enter Pan State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      break;

     case 3:
      one();
      //Serial.println("Enter 1:1 State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, HIGH);
      break;

     default: // In the event of mode select failure, vehicle maintains its position
      idle();
      //Serial.println("Enter Default(Idle) State");
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, HIGH);
      break;
  }

  // Prints the motor speed to the Serial Monitor
  // mtrSpeed(); 

  send_data();
  
  // Delay to complete computation
  delay(10);
}

void send_data() {
  digitalWrite(tx_led, HIGH); // Begin transmit
  set_data(); // Substitute for reading values
  vw_send((uint8_t *)&data, sizeof(data));
  vw_wait_tx(); // Wait until the whole message sends
  digitalWrite(tx_led, LOW);
  delay(1000);
}

void set_data() {
  data.f_mode = select;
  data.joy_dat = joy_out;
  data.ped_dat = pedal_out;
  data.thr_dat = throttle_out;
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

  /* Throttle input
   *  PROBLEM: The throttle is supposed to modify current motor 
   *  speeds without changing the motor speed ratio. This is not
   *  possible as there's no way of reading current duty cycles.
   *  SOL'N: Instead of having the throttle_out value set to a 
   *  PWM pin it will be used to create a factor which will be
   *  mutliplied to every analogWrite() statement.
   */

  // Throttle input
  if (throttle_out < 128) {                 // Slow Down
    ftr = -1*(throttle_out/10);
  }
  else {                                    // Speed Up
    ftr = throttle_out/10;
  }
  
  // Pedal input
  analogWrite(m2, pedal_out + ftr);         // P controls T motor

  // Joystick input
  if (joy_out < 120) {                      // Left turn
    analogWrite(m1, (128-joy_out) + ftr);   // L motor decreases speed
    analogWrite(m3, (256 - joy_out) + ftr); // R motor increases speed
  }
  else if (joy_out > 136) {                 // Right turn
    analogWrite(m1, joy_out + ftr);         // L motor increases speed
    analogWrite(m3, (255 - joy_out) + ftr); // R motor decreases speed
  }
  else {                                    // Forward, rest position  
    analogWrite(m1, pedal_out + ftr);       // Mimic m2 speed for fwd
    analogWrite(m3, pedal_out + ftr);       // and vert. movement
  }
  
  delay(10);
}

void pan() {                          //Joystick only 
  
  if (joy_out < 120) {                // Left turn
    analogWrite(m1, (127 - joy_out)); // L motor decreases speed
    analogWrite(m3, (255 - joy_out)); // R motor increases speed
  }
  else if (joy_out > 136) {           // Right turn
    analogWrite(m1, joy_out);         // L motor increases speed
    analogWrite(m3, (255 - joy_out)); // R motor decreases speed
  }
  else {                              // Forward, rest position
    analogWrite(m1, joy_out);
    analogWrite(m3, joy_out);
  }
  
  delay(5);
}

void one() {                          //1:1 control 
  
  // Send PWM output
  analogWrite(m3, joy_out);           // Joystick manipulates motor3
  analogWrite(m2, pedal_out);         // Pedal manipulates motor2
  analogWrite(m1, throttle_out);      // Throttle manipulates 
}

void rdVolt() {                       // Print the values to the serial monitor

  Serial.print("Throttle voltage = ");
  Serial.print(throttle_out);

  Serial.print("\nPedal pot. voltage = ");
  Serial.print(pedal_out);

  Serial.print("\nJoystick x-axis position {0,1023} = ");
  Serial.print(joy_out);

  Serial.print("\n\n-----------------");
}

void mtrSpeed() {                     // Prints the motor speed to the serial monitor
    
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
