/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: June 1 2019
 *  demo_v2
 *  This code corresponds to the demo_v2 wiring diagram
 *  Contains joystick, slider pot, and radial pot control of hobby DC motors
 *  Prints analog voltage to serial monitor
 */

// Variable declarations 
const int slider = A1;
const int radial = A2;
const int joy = A0;

// Motor PWM pin declaration
const int m1 = 9;
const int m2 = 10;
const int m3 = 11;

int slide_out = 0;
int rad_out = 0;
int joy_out = 0;

float op1, op2, op3;

void setup() {
  // Set pins to analog output
  pinMode(slider, INPUT);
  pinMode(radial, INPUT);
  pinMode(joy, INPUT);

  // Initialize Serial monitor
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // output_var = analogRead(pin_var);
  slide_out = analogRead(slider);
  rad_out = analogRead(radial);
  joy_out = analogRead(joy);
/*
  // Print the values to the serial monitor
  Serial.print("Slider voltage = ");
  Serial.print(slide_out);

  Serial.print("\nRadial pot. voltage = ");
  Serial.print(rad_out);

  Serial.print("\nJoystick x-axis position {0,1023} = ");
  Serial.print(joy_out);

  //Serial.print("\n\n-----------------");
  */
  // Map the 10-bit ADC digital signal to an 8-bit digital signal for the DAC
  rad_out = map(rad_out, 0, 1023, 0, 255);
  slide_out = map(slide_out, 0, 1023, 0, 255);
  joy_out = map(joy_out, 0, 1023, 0, 255);
  
  // Send PWM output
  analogWrite(m3, joy_out);
  analogWrite(m2, rad_out);
  analogWrite(m1, slide_out);

  // Show operating speed as a percentage of the maximum speed
  Serial.print("\nJoystick Motor operating speed = ");
  op1 = (joy_out/255.0)*100.00;
  Serial.print(op1);
  Serial.print("%");

  Serial.print("\nRadial Motor operating speed = ");
  op2 = (rad_out/255.0)*100.00;
  Serial.print(op2);
  Serial.print("%");

  Serial.print("\nSlider Motor operating speed = ");
  op3 = (slide_out/255.0)*100.00;
  Serial.print(op3);
  Serial.print("%");
  
  Serial.print("\n\n-----------------");

  // Delay to complete computation
  delay(10);
}
