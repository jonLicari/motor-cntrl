/*  author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: 14 Sep 2019
 *  demo_v5
 *  This code corresponds to the demo_v5 transmitter wiring diagram
 *  Contains 4 flight mode buttons, indicator LEDs, 3 controls, 433MHz Tx
 */

#include <VirtualWire.h>
#include "v5_tx.h"

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
      //Serial.println("Enter Idle State");
      digitalWrite(s0, HIGH);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

     case 1:
      //Serial.println("Enter Flight State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, LOW);
      digitalWrite(s3, LOW);
      break;

     case 2:
      //Serial.println("Enter Pan State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, LOW);
      break;

     case 3:
      //Serial.println("Enter 1:1 State");
      digitalWrite(s0, LOW);
      digitalWrite(s1, LOW);
      digitalWrite(s2, LOW);
      digitalWrite(s3, HIGH);
      break;

     default: // In the event of mode select failure, vehicle maintains its position
      //Serial.println("Enter Default(Idle) State");
      digitalWrite(s0, HIGH);
      digitalWrite(s1, HIGH);
      digitalWrite(s2, HIGH);
      digitalWrite(s3, HIGH);
      break;
  } 

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
  delay(100);
}

void set_data() {
  data.f_mode = select;
  data.joy_dat = joy_out;
  data.ped_dat = pedal_out;
  data.thr_dat = throttle_out;
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
