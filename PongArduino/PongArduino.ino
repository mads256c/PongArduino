/*
 Name:		PongArduino.ino
 Created:	8/24/2018 9:35:00 PM
 Author:	Mads
*/

//Defines the pins we use on the shift register.
#define LATCH_PIN 13
#define CLOCK_PIN 12
#define DATA_PIN 11

//Ball and paddle speed are milliseconds per update.
#define BALL_SPEED 150
#define PADDLE_SPEED 50

//When the ball is reset, wait this amount before moving the ball
#define RESET_DELAY 1000

//The length of the paddles.
#define PADDLE_LEN 3

#pragma region

const PROGMEM uint8_t Dash[] = { B00000000,
								 B00000000,
								 B00000000,
								 B01111110,
								 B01111110,
								 B00000000,
								 B00000000,
								 B00000000 };

const PROGMEM uint8_t Zero[] = { B00000000,
								 B00011000,
								 B00100100,
								 B00100100,
								 B00100100,
								 B00100100,
								 B00011000,
								 B00000000 };

const PROGMEM uint8_t One[] = { B00000000,
								B00001000,
								B00011000,
								B00001000,
								B00001000,
								B00001000,
								B00011100,
								B00000000 };

const PROGMEM uint8_t Two[] = { B00000000,
								B00011000,
								B00100100,
								B00000100,
								B00001000,
								B00010000,
								B00111100,
								B00000000 };

const PROGMEM uint8_t Three[] = { B00000000,
								  B00111000,
								  B00000100,
								  B00011000,
								  B00000100,
								  B00000100,
								  B00111000,
								  B00000000 };

const PROGMEM uint8_t Four[] = { B00000000,
								 B00100100,
								 B00100100,
								 B00111100,
								 B00000100,
								 B00000100,
								 B00000100,
								 B00000000 };

const PROGMEM uint8_t Five[] = { B00000000,
								 B00111100,
								 B00100000,
								 B00011000,
								 B00000100,
								 B00000100,
								 B00111000,
								 B00000000 };

const PROGMEM uint8_t Six[] = { B00000000,
								B00011100,
								B00100000,
								B00111000,
								B00100100,
								B00100100,
								B00011000,
								B00000000 };

const PROGMEM uint8_t Seven[] = {B00000000,
								B00111100,
								B00000100,
								B00001000,
								B00010000,
								B00010000,
								B00010000,
								B00000000 };

const PROGMEM uint8_t Eight[] = { B00000000,
								  B00011000,
								  B00100100,
								  B00011000,
								  B00100100,
								  B00100100,
								  B00011000,
								  B00000000 };

const PROGMEM uint8_t Nine[] = { B00000000,
								 B00011000,
								 B00100100,
								 B00100100, 
								 B00011000,
								 B00000100,
								 B00111000,
								 B00000000 };

//This is used to debug certain stuff. If you see a cross on the led matrix; something is wrong.
const PROGMEM uint8_t Cross[] = { B00000000,
								  B01000010,
								  B00100100,
								  B00011000,
								  B00011000,
								  B00100100,
								  B01000010,
								  B00000000 };

const PROGMEM uint8_t Filled[] = {255,
								  255,
								  255,
								  255,
								  255,
								  255,
								  255,
								  255 };

#pragma endregion Bitmap definitions

uint8_t score1 = 0;
uint8_t score2 = 0;

uint8_t paddle1Pos = 0;
uint8_t paddle2Pos = 0;

bool ballUp = false, ballRight = false;
uint8_t ballXPos = 4, ballYPos = 4;

unsigned long ballTimer;
unsigned long paddleTimer;
unsigned long resetTimer;

//Shifts out pattern.
void applyShift(const uint8_t pattern, const uint8_t bitOrder = MSBFIRST)
{
	digitalWrite(LATCH_PIN, LOW);
	shiftOut(DATA_PIN, CLOCK_PIN, bitOrder, pattern);
	digitalWrite(LATCH_PIN, HIGH);
}

//Resets the screen.
void resetScreen()
{
	applyShift(0);

	//Set 0-7 to high
	PORTD |= B11111111; //PORTD controls digitalWrite for pins 0-7
}

//Draws a const PROGMEM array.
void drawArray(const uint8_t* array)
{
	resetScreen();

	for (uint8_t x = 0; x < 8; ++x)
	{
		const uint8_t b = pgm_read_byte_near(array + x); //Read the byte from the array stored in PROGMEM.

		if (b == 0)
			continue; //Skip so we have more time for the led that should be on. They will be brighter.

		applyShift(b, LSBFIRST); //Send voltage to the leds.

		digitalWrite(7 - x, LOW); //Set the column to low to turn on the right leds. 

		delay(1); //Wait for the leds to turn on.

		digitalWrite(7 - x, HIGH); //Set the column to high so we can move on to the next column.
	}
}

//Int literal to led matrix array.
const uint8_t* numberToArray(const uint8_t number)
{
	switch (number)
	{
	case 0: return Zero;
	case 1: return One;
	case 2: return Two;
	case 3: return Three;
	case 4: return Four;
	case 5: return Five;
	case 6: return Six;
	case 7: return Seven;
	case 8: return Eight;
	case 9: return Nine;
	}

	__builtin_unreachable();

}

//Counts down from 3.
void countDown()
{
	for (uint8_t i = 3; i > 0; --i)
	{
		const auto before = millis();
		while (millis() - before < 1000)
		{
			drawArray(numberToArray(i));
		}
	}
}

//Displays the score.
void displayScore()
{
	if (score1 > 9 || score2 > 9)
	{
		score1 = 0;
		score2 = 0;

		countDown();
		return;
	}

	unsigned long timer = millis();

	while(millis() - timer < 500)
	{
		drawArray(numberToArray(score1));
	}

	timer = millis();

	while (millis() - timer < 300)
	{
		drawArray(Dash);
	}

	timer = millis();

	while (millis() - timer < 500)
	{
		drawArray(numberToArray(score2));
	}

	
}

//Resets the ball
void resetBall()
{
	ballXPos = random(3, 5);
	ballYPos = random(3, 5);
	ballUp = ballYPos == 3;
	ballRight = ballXPos == 3;

	paddle1Pos = 2;
	paddle2Pos = 2;

	resetTimer = millis();
}


//Draws the first paddle.
void drawPaddle1()
{
	//Set the bits paddle1Pos to paddle1Pos + PADDLE_LEN.
	const uint8_t nbits = ~(0xFFu << (paddle1Pos + PADDLE_LEN - paddle1Pos));
	PORTD &= ~(nbits << paddle1Pos); //Set the pins to low.

	applyShift(1 << 0);
}

//Draws the second paddle.
void drawPaddle2()
{
	//Set the bits paddle2Pos to paddle2Pos + PADDLE_LEN.
	const uint8_t nbits = ~(0xFFu << (paddle2Pos + PADDLE_LEN - paddle2Pos));
	PORTD &= ~(nbits << paddle2Pos); //Set the pins to low.

	applyShift(1 << 7);
}

//Draws the ball.
void drawBall()
{
	digitalWrite(ballYPos, LOW);
	applyShift(1 << ballXPos);
}


//Updates the ball (physics and score)
void updateBall()
{
	//Check if the paddle is in the way. If it is bounce the ball, if its not give point to the other player.
	if (!ballRight && ballXPos <= 1 && paddle1Pos <= ballYPos && paddle1Pos + (PADDLE_LEN - 1) >= ballYPos) ballRight = true;
	else if (ballXPos == 0) { score1++; displayScore(); resetBall(); return; }
	if (ballRight && ballXPos >= 6 && paddle2Pos <= ballYPos && paddle2Pos + (PADDLE_LEN - 1) >= ballYPos) ballRight = false;
	else if (ballXPos == 7) { score2++; displayScore(); resetBall(); return; }

	//Bounce the ball from the top
	if (ballYPos == 7)
		ballUp = false;
	//Bounce the ball from the bottom
	else if (ballYPos == 0)
		ballUp = true;
	//Bounce the ball from the right
	if (ballXPos == 7)
		ballRight = false;
	//Bounce the ball from the left
	else if (ballXPos == 0)
		ballRight = true;


	//Update the balls position based on the direction.
	if (ballUp) ballYPos++;
	else if (!ballUp) ballYPos--;
	if (ballRight) ballXPos++;
	else if (!ballRight) ballXPos--;
}

//Updates the paddles.
void updatePaddles()
{

	if (analogRead(A0) > 512 && paddle2Pos + PADDLE_LEN != 8)
		paddle2Pos++;


	if (analogRead(A1) > 512 && paddle2Pos != 0)
		paddle2Pos--;


	if (analogRead(A2) > 512 && paddle1Pos + PADDLE_LEN != 8)
		paddle1Pos++;


	if (analogRead(A3) > 512 && paddle1Pos != 0)
		paddle1Pos--;
}



// the setup function runs once when you press reset or power the board
void setup() {
	//Set 0-13 to output
	DDRD |= B11111111; // Controls pinMode for pins 0-7
	DDRB |= B00111111; // Controls pinMode for pins 8-13 last two bits are unused.

	applyShift(0, LSBFIRST);

	resetScreen();

	ballTimer = millis();
	paddleTimer = ballTimer;
	resetTimer = millis();

	resetBall();
	countDown();

}



// the loop function runs over and over again until power down or reset
void loop() {

	delay(1);
	resetScreen();
	drawPaddle1();
	delay(1);
	resetScreen();
	drawPaddle2();
	delay(1);
	resetScreen();
	drawBall();


	if (millis() - paddleTimer > PADDLE_SPEED)
	{
		updatePaddles();
		paddleTimer = millis();
	}

	if (millis() - resetTimer < RESET_DELAY)
		return;

	if (millis() - ballTimer > BALL_SPEED)
	{
		updateBall();
		ballTimer = millis();
	}

}
