# motor-cntrl
Arduino DC motor control w/ 4 flight modes

A throttle, pedal, and joystick will enable VTOL flight controls (3 low power DC motors).

The 4 flight modes are as follows:
  0. Idle: Controls input has no effect on motor speed, do-nothing state, retains previously set motor speeds
  1. Flight: Input to all controls manipulates motor speed,
  2. Pan: Only joystick input manipulates motor
  3. One-to-one: Each control type (Throttle, Pedal, Joystick) manipulates its own motor 

LEGEND
	motors		(L)eft (T)op (R)ight
	controls	(P)edal (Th)rottle (J)oystick

On startup, 
	Selector value will be 0 (Park/ Idle)
	4 buttons to correspond to flight modes 0 - park, 1 - flight, 2 - pan, 3 - 1:1
	That's 4 separate pins to the uno, with 4 extra resistors
	Every time the selector changes the state, corresponding LED will turn on, all others off
	Alternative to LEDs is single digit 7 seg, requires 10 pins, 7 resistors, more power than LEDs
	// On the box, engrave holes for buttons like a controller w corresponding LEDs inside each button
	
Buttons will manipulate a Selector State implemented as switch case

Case 1 //Flight

	Full control input, all motors active
	P - vert acc
	Th - forward speed 	
	J - L/R 

Buttons being read as analog voltage, need to match voltages to int, set the state to int value

solution:

if (analogVoltage >= expectedVoltage*0.995 && analogVoltage =< expectedVoltage*1.005)
	select = 0,1,2,3;

100k [Ohm] pull down

0 --- 1k  	------ 0.9901 -- 1014
1 --- 5.1k	------ 0.9515 -- 974
2 --- 7.5k	------ 0.9302 -- 952
3 --- 10k	------ 0.9091 -- 931


PAN
	- joystick controls L and R motor, T motor holds previously set speed
	- [Left] L motor drops speed, R increases speed
	- [Right] R motor drops speed, L motor speeds up

FLIGHT
	- JOYSTICK controls L and R motor, T motor holds previously set speed
	- [Left] L motor drops speed, R increases speed
	- [Right] R motor drops speed, L motor speeds up
	- THROTTLE controls L, R, T motors and increases their speed by a factor //allows motors to keep speed ratios
	- [Fast] factor > 1
	- [Slow] factor < 1
	- PEDAL controls T motor and SETS PWM
	- [Fwd] 0-max speed

Issues
- holding previously set value can be problematic; if the value was 0, scaling by a facto0r would have no effect
