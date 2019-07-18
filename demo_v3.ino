/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: July 15 2019
 *  demo_v3
 *  This code corresponds to the demo_v3 wiring diagram
 *  Contains 4 flight modes: Park, Pan, Flight, 1:1
 *  Modes are button activated and displayed on either LCD or 7 Seg
 */

// Variable declarations 
const int throttle = A1;
const int pedal = A2;
const int joy = A0;

// Motor PWM pin declaration
const int m1 = 9;
const int m2 = 10;
const int m3 = 11;

int throttle_out = 0;
int pedal_out = 0;
int joy_out = 0;

float op1, op2, op3;

void setup() {
  // Set pins to analog output
  pinMode(throttle, INPUT);
  pinMode(pedal, INPUT);
  pinMode(joy, INPUT);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {
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

  mtrSpeed(); // Prints the motor speed to the Serial Monitor
  
  // Delay to complete computation
  delay(10);
}

void idle() {     //Essentially a do-nothing state
  
  delay(5);
}

void flight() {   //PWM output
}

void pan() {   //pan code
}

void one() {   //1:1 control 
  // Send PWM output
  analogWrite(m3, joy_out); // Joystick manipulates motor3
  analogWrite(m2, pedal_out); // 
  analogWrite(m1, throttle_out);
}

void rdVolt() [   // Print the values to the serial monitor

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
