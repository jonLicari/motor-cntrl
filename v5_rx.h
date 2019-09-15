/*  author: jlicari, UX - Electronics, Ryerson Helium
 *  Created: 14 Sep 2019
 *  demo_v5 > v5_rx.h
 *  This header file contains definitions corresponding to v5_rx.ino
 */

#ifndef v5_rx.h
#define v5_rx.h

  const int rx = 7; // Receive pin
  const int rx_led = 13; // Receive data led

  // Motor Variables
  int joy_out = 0;
  int throttle_out = 0;
  int pedal_out = 0;

  // Miscellaneous 
  float op1, op2, op3, ftr;
  int select; // Flight mode select variable

  // Motor PWM pin declaration
  const int m1 = 9; // L motor
  const int m2 = 10; // T motor
  const int m3 = 11; // R motor

  struct Data_Packet {
    // Receiving variables
    int f_mode; // receive flight mode
    int joy_dat; // receive joystick data
    int ped_dat; // receive pedal data
    int thr_dat; // Receive throttle data
  };

  Data_Packet data;

#endif
