/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: July 15 2019
 *  demo_v3.h
 *  This header file contains definitions corresponding to demo_v3 
 */
#ifndef demo_v3_h
#define demo_v3_h

  // Define the expected analog values for the Flight Mode Select button
  #define exp0 1014 // 1k
  #define exp1 931 // 10k
  #define exp2 840 // 22k
  #define exp3 770 // 33k

  // Pin Assignments
  const int joy = A0; 
  const int throttle = A1;
  const int pedal = A2;
  const int mode = A3;

  // LED pins
  const int s0 = 7;
  const int s1 = 6;
  const int s2 = 5;
  const int s3 = 4;

  // Motor PWM pin declaration
  const int m1 = 9; // L motor
  const int m2 = 10; // T motor
  const int m3 = 11; // R motor

  // Analog variables
  int throttle_out = 0;
  int pedal_out = 0;
  int joy_out = 0;

  // Miscellaneous 
  float op1, op2, op3, ftr;
  int old, sel, select; // Flight mode select variable

#endif
