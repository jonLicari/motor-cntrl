/*  author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: 14 Sep 2019
 *  demo_v5
 *  This code corresponds to the demo_v5 receiver wiring diagram
 *  Contains 3 motors, 433MHz Tx
 */

#include <VirtualWire.h>
#include "v5_rx.h"

void setup() {
  // put your setup code here, to run once:
  vw_set_rx_pin(rx);  // Set receive pin
  vw_setup(2000);
  vw_rx_start();  // Start running the receiver PLL 
  pinMode(rx_led, OUTPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Receive the wireless data
  get_data();

  // Switch case for flight mode selection
  switch (select) {
     case 0:
      idle();
      //Serial.println("Enter Idle State");
      break;

     case 1:
      flight();
      //Serial.println("Enter Flight State");
      break;

     case 2:
      pan();
      //Serial.println("Enter Pan State");
      break;

     case 3:
      one();
      //Serial.println("Enter 1:1 State");
      break;

     default: // In the event of mode select failure, vehicle maintains its position
      idle();
      //Serial.println("Enter Default(Idle) State");
      break;
  }  
  
  // Prints the motor speed to the Serial Monitor
  // mtrSpeed();
  
  delay(10);
}

void get_data() {
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Array where message is copied
  uint8_t buflen = VW_MAX_MESSAGE_LEN; 
  // ^^^ should have arrays max size upon input,return#of bytes actually copied
  // ^^^ function itself returns true if the message was verified correct, 
  // ^^^ or false if a message was received but appears to have been corrupted.

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      int i;

      digitalWrite(13, true); // Flash a light to show received good message
      // Message with a good checksum received, dump it.
      Serial.print("Got: ");
  
      //for (i = 0; i < buflen; i++)
      //{
       //   Serial.print(buf[i], HEX);
         // Serial.print(" ");
      //}
      data.f_mode = buf[0];
      data.joy_dat = buf[2];
      data.ped_dat = buf[4];
      data.thr_dat = buf[6];
      Serial.print("Mode ");
      Serial.println(data.f_mode);
      Serial.print("Joystick ");
      Serial.println(data.joy_dat);
      Serial.print("Pedal ");
      Serial.println(data.ped_dat);
      Serial.print("Throttle ");
      Serial.println(data.thr_dat);
      
      digitalWrite(13, false);
    }
    
  set_data();
    
  delay (2000);
}

void set_data() {
  
  select = data.f_mode;
  joy_out = data.joy_dat;
  pedal_out = data.ped_dat;
  throttle_out = data.thr_dat;
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
  if (pedal_out + ftr > 255)
    analogWrite(m2, 255);
  else if (pedal_out + ftr < 1)
    analogWrite(m2, 0);
  else
    analogWrite(m2, pedal_out + ftr);       // P controls T motor

  // Joystick input
  if (joy_out < 120) {                      // Left turn
    if ((128-joy_out) + ftr < 0)
      analogWrite(m1, 0);
    else  
      analogWrite(m1, (128-joy_out) + ftr);   // L motor decreases speed

    if ((256 - joy_out) + ftr > 255)
      analogWrite(m3, 255);
    else  
      analogWrite(m3, (256 - joy_out) + ftr); // R motor increases speed
  }
  else if (joy_out > 136) {                 // Right turn
    if (joy_out + ftr > 255)
      analogWrite(m1, 255);
    else
      analogWrite(m1, joy_out + ftr);         // L motor increases speed
    
    if ((255-joy_out)+ftr < 1)
      analogWrite(m3, 0);
    else
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
