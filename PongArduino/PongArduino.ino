/*
 Name:		PongArduino.ino
 Created:	8/24/2018 9:35:00 PM
 Author:	Mads
*/

#define LATCH_PIN 13
#define CLOCK_PIN 12
#define DATA_PIN 11

#define BALL_SPEED 100
#define PADDLE_SPEED 50

#define PADDLE_LEN 3

const PROGMEM bool Dash[] = { 0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 1, 1, 1, 1, 1, 1, 0,
							  0, 1, 1, 1, 1, 1, 1, 0,
							  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Zero[] = { 0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool One[] = {  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 1, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 1, 0, 0, 0,
							  0, 0, 0, 0, 1, 0, 0, 0,
							  0, 0, 0, 0, 1, 0, 0, 0,
							  0, 0, 0, 1, 1, 1, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Two[] = {  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 1, 0, 0, 0,
							  0, 0, 0, 1, 0, 0, 0, 0,
							  0, 0, 1, 1, 1, 1, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Three[] = {0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 1, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 1, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Four[] = { 0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 1, 1, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Five[] = { 0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 1, 1, 1, 1, 0, 0,
							  0, 0, 1, 0, 0, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 1, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Six[] = {  0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 1, 1, 1, 0, 0,
							  0, 0, 1, 0, 0, 0, 0, 0,
							  0, 0, 1, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Seven[] = {0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 1, 1, 1, 1, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 0, 0, 1, 0, 0, 0,
							  0, 0, 0, 1, 0, 0, 0, 0,
							  0, 0, 0, 1, 0, 0, 0, 0,
							  0, 0, 0, 1, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Eight[] = {0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Nine[] = { 0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0, 
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 1, 0, 0,
							  0, 0, 1, 1, 1, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };

const PROGMEM bool Cross[] = {0, 0, 0, 0, 0, 0, 0, 0,
							  0, 1, 0, 0, 0, 0, 1, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 0, 1, 1, 0, 0, 0,
							  0, 0, 1, 0, 0, 1, 0, 0,
							  0, 1, 0, 0, 0, 0, 1, 0,
							  0, 0, 0, 0, 0, 0, 0, 0 };


uint8_t score1 = 0;
uint8_t score2 = 0;

uint8_t paddle1Pos = 0;
uint8_t paddle2Pos = 0;

bool ballUp = false, ballRight = false;
uint8_t ballXPos = 4, ballYPos = 4;

uint8_t pattern = 0;

void applyShift()
{
	digitalWrite(LATCH_PIN, LOW);
	shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, pattern);
	digitalWrite(LATCH_PIN, HIGH);
}

void resetScreen()
{
	pattern = 0;
	applyShift();
	for (uint8_t i = 0; i < 8; ++i)
	{
		digitalWrite(i + 2, HIGH);
	}
}

void drawArray(const bool* array)
{
	
	for (uint8_t x = 0; x < 8; ++x)
	{
		resetScreen();
		for (uint8_t y = 0; y < 8; ++y)
		{
			bool b = pgm_read_byte_near(array + 8 * y + x );

			//bool b = array[8 * y + x];
			//Serial.print(b);
			if (b)
				digitalWrite(7 - y + 2, LOW);
		}
		//Serial.println();
		pattern = (1 << x);
		applyShift();
		delay(1);
	}
}

const bool* numberToArray(const uint8_t number)
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

	default: return Cross;
	}

}

void countDown()
{
	for (uint8_t i = 3; i > 0; --i)
	{
		const unsigned long before = millis();
		while (millis() - before < 1000)
		{
			drawArray(numberToArray(i));
		}
	}
}

void displayScore()
{

	if (score1 > 9 || score2 > 9)
	{
		score1 = 0;
		score2 = 0;

		countDown();
		return;
	}

	unsigned long before = millis();

	while(millis() - before < 1000)
	{
		drawArray(numberToArray(score1));
	}

	before = millis();

	while (millis() - before < 1000)
	{
		drawArray(Dash);
	}

	before = millis();

	while (millis() - before < 1000)
	{
		drawArray(numberToArray(score2));
	}

	
}

void resetBall()
{
	ballXPos = random(3, 5);
	ballYPos = random(3, 5);
	ballUp = ballYPos == 3;
	ballRight = ballXPos == 3;
}



void drawPaddle1()
{
	pattern = (1 << 0);

	for (uint8_t i = 0; i < PADDLE_LEN; i++)
	{
		digitalWrite(i + paddle1Pos + 2, LOW);
	}
}

void drawPaddle2()
{
	pattern = (1 << 7);

	for (uint8_t i = 0; i < PADDLE_LEN; i++)
	{
		digitalWrite(i + paddle2Pos + 2, LOW);
	}
}

void drawBall()
{
	pattern |= (1 << ballXPos);
	digitalWrite(ballYPos + 2, LOW);
}

void flashScreen()
{
	resetBall();

	for (uint8_t i = 0; i < 4; ++i)
	{
		pattern = 255;
		applyShift();

		for (uint8_t j = 0; j < 8; ++j)
		{
			digitalWrite(j + 2, LOW);
		}

		delay(100);

		pattern = 0;
		applyShift();

		for (uint8_t j = 0; j < 8; ++j)
		{
			digitalWrite(j + 2, HIGH);
		}

		delay(100);
	}

}

void updateBall()
{
	if (ballYPos == 7)
		ballUp = false;
	else if (ballYPos == 0)
		ballUp = true;
	if (ballXPos == 7)
		ballRight = false;
	else if (ballXPos == 0)
		ballRight = true;
	if (!ballRight && ballXPos <= 1 && paddle1Pos <= ballYPos && paddle1Pos + PADDLE_LEN >= ballYPos) ballRight = true;
	else if (ballXPos == 0) { score1++; displayScore(); resetBall(); }
	if (ballRight && ballXPos >= 6 && paddle2Pos <= ballYPos && paddle2Pos + PADDLE_LEN >= ballYPos) ballRight = false;
	else if (ballXPos == 7) { score2++; displayScore(); resetBall(); }

	//Serial.println(paddle1Pos);
	//Serial.println(ballYPos);


	if (ballUp) ballYPos++;
	else if (!ballUp) ballYPos--;
	if (ballRight) ballXPos++;
	else if (!ballRight) ballXPos--;
}

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

unsigned long ballTimer;
unsigned long paddleTimer;

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(9600);
	pinMode(DATA_PIN, OUTPUT);
	pinMode(CLOCK_PIN, OUTPUT);
	pinMode(LATCH_PIN, OUTPUT);

	digitalWrite(LATCH_PIN, LOW);
	shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, 0);
	digitalWrite(LATCH_PIN, HIGH);

	for (uint8_t i = 0; i < 8; ++i)
	{
		pinMode(i + 2, OUTPUT);
		digitalWrite(i + 2, HIGH);
	}
	ballTimer = millis();
	paddleTimer = ballTimer;

	resetBall();
	countDown();
}



// the loop function runs over and over again until power down or reset
void loop() {

	delay(1);
	resetScreen();
	drawPaddle1();
	applyShift();
	delay(1);
	resetScreen();
	drawPaddle2();
	applyShift();
	delay(1);
	resetScreen();
	drawBall();
	applyShift();

	if (millis() - ballTimer > BALL_SPEED)
	{
		updateBall();
		ballTimer = millis();
	}

	if (millis() - paddleTimer > PADDLE_SPEED)
	{
		updatePaddles();
		paddleTimer = millis();
	}

}
