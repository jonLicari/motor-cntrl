/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: July 15 2019
 *  demo_v3
 *  This code corresponds to the demo_v3 wiring diagram
 *  Contains 4 flight modes: Park, Pan, Flight, 1:1
 *  Modes are button activated and displayed on either LCD or 7 Seg
 */

// Variable declarations 
const int throttle = 11;
const int pedal = 12;
const int joy = 13;

int throttle_out = 0;
int pedal_out = 0;
int joy_out = 0;
int joy_map = 0;

void setup() {
  // Set pins to analog output
  pinMode(throttle, OUTPUT);
  pinMode(pedal, OUTPUT);
  pinMode(joy, OUTPUT);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // output_var = analogRead(pin_var);
  throttle_out = analogRead(throttle);
  pedal_out = analogRead(pedal);
  joy_out = analogRead(joy);

  // Map the values of the joystick from {0,5}[V] to {0,1023}[bits]
  joy_map = map(joy_out, 0, 5, 0, 1023);

  //Necessary to map throttle and pedal? 

  // Print the values to the serial monitor
  Serial.print("Slider voltage = ");
  Serial.print(throttle_out);

  Serial.print("\nRadial pot. voltage = ");
  Serial.print(pedal_out);

  Serial.print("\nJoystick x-axis position {0,1023} = ");
  Serial.print(joy_map);

  // Delay to allow A/D converter to complete computation
  delay(5);
}
