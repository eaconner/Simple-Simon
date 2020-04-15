# Simple Simon Kit #

![Simple Simon](http://ericconnerapps.com/Apps/SimpleSimon.png)

Red then green...no, blue?...wait!...BUZZ! Remember the classic game of Simon?
Well, here is a build-your-own Simon kit that will sharpen your reaction time while teaching you basic soldering (a useful skill in its own right).

After you have successfully built a working Simon game, you will have a greater knowledge of through-hole soldering and the tools, techniques, and terminology required to populate your own PCB prototype. Additionally, the source code and design files are available, making this project a great entryway into microcontrollers and embedded programming. Game on!

## Parts List ##
|	Qty	|	Part				|	Description				|	Digikey Part #	|
|:-----:|-----------------------|---------------------------|-------------------|
|	1	|	U1					|	Atmel ATtiny45			|	ATTINY45-20PU	|
|	4	|	R1, R2, R3, R4		|	470ohm 1/4w Resistor	|	CF14JT470RCT	|
|	1	|	D1					|	5mm Red LED				|	160-1678		|
|	1	|	D2					|	5mm Green LED			|	160-1677		|
|	1	|	D3					|	5mm Blue LED			|	160-1610		|
|	1	|	D4					|	5mm Yellow LED			|	160-1686		|
|	1	|	SG1					|	12mm Piezo Buzzer		|	102-3752		|
|	5	|	S1, S2, S3, S4, S5	|	Pushbutton				|	450-1644		|
|	1	|	BAT1				|	Battery Holder			|	BS-7			|
|	1	|	BAT1				|	CR2032 Battery			|	P189			|

## Instructions ##
1. Insert the ATtiny45 microcontroller into U1. Make sure the dot in the corner of the chip is aligned with the white silk screen dot on the board. Flip the board over, then solder into place.
2. Insert the (4) four resistors into R1, R2, R3, & R4 by bending the legs down. Flip the board over and solder into place.
3. Insert the (4) four LEDs into D1, D2, D3, & D4. Each LED has a short leg and a long leg. The long leg goes into the hole closest to the edge of the board. Also make sure it sits flush with the PCB. Then solder into place. **Note: You can check the color of the LED by touching the long leg to the "+" side of the CR2032 battery and the short leg to the "-" side.**
4. Insert the buzzer into SG1. Flip the board over and solder into place.
5. Insert the battery holder on the back of the board into BAT1. Flip the board over and solder into place.
6. Insert the battery into the holder.

## Gameplay ##

Press the "Start" button to begin. Memorize and repeat an increasing sequence of colors and tones. In higher levels the game goes faster. If you make a mistake, the correct LED blinks three times and game-over animation is played. If you have beaten the best score, your score is saved and the best-score animation is played.

- If the **red** button is held during when the start button is pressed, the best score is erased.
- If the **green** button is held during when the start button is pressed, starts a random sequence of lights and tones.
- If the **blue** button is held during when the start button is pressed, starts a new game at the highest achieved level.
- If the **yellow** button is held during when the start button is pressed, toggles the sound on or off.
