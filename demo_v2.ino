/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: June 1 2019
 *  demo_v2
 *  This code corresponds to the demo_v2 wiring diagram
 *  Contains joystick, slider pot, and radial pot control of hobby DC motors
 *  Prints analog voltage to serial monitor
 */

// Variable declarations 
const int slider = 11;
const int radial = 12;
const int joy = 13;

int slide_out = 0;
int  rad_out = 0;
int joy_out = 0;
int joy_map = 0;

void setup() {
  // Set pins to analog output
  pinMode(slider, OUTPUT);
  pinMode(radial, OUTPUT);
  pinMode(joy, OUTPUT);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // output_var = analogRead(pin_var);
  slide_out = analogRead(slider);
  rad_out = analogRead(radial);
  joy_out = analogRead(joy);

  // Map the values of the joystick from {0,5}[V] to {0,1023}[bits]
  joy_map = map(joy_out, 0, 5, 0, 1023);

  //Necessary to map slider and radial? 

  // Print the values to the serial monitor
  Serial.print("Slider voltage = ");
  Serial.print(slide_out);

  Serial.print("\nRadial pot. voltage = ");
  Serial.print(rad_out);

  Serial.print("\nJoystick x-axis position {0,1023} = ");
  Serial.print(joy_map);

  // Delay to allow A/D converter to complete computation
  delay(5);
}
