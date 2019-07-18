/* author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: July 15 2019
 *  demo_v3.h
 *  This header file contains definitions corresponding to demo_v3 
 */
// Define the expected values for the Flight Mode Select button
#define exp0 = 1014
#define exp1 = 974
#define exp2 = 952
#define exp3 = 931

 // Pin Assignments
const int joy = A0; 
const int throttle = A1;
const int pedal = A2;
const int mode = A3;

// Motor PWM pin declaration
const int m1 = 9;
const int m2 = 10;
const int m3 = 11;
