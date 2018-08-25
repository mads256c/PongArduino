/*
 Name:		PongArduino.ino
 Created:	8/24/2018 9:35:00 PM
 Author:	Mads
*/

#define LATCH_PIN 13
#define CLOCK_PIN 12
#define DATA_PIN 11

#define DELAY 100
#define BALL_SPEED 100
#define PADDLE_SPEED 50

#define PADDLE_LEN 3

uint8_t paddle1Pos = 0;
uint8_t paddle2Pos = 0;

bool ballUp = false, ballRight = false;
uint8_t ballXPos = 4, ballYPos = 4;

int pattern = 0;

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
	for (int i = 0; i < 8; ++i)
	{
		digitalWrite(i + 2, HIGH);
	}
}

void drawPaddle1()
{
	pattern |= (1 << 0);

	for (int i = 0; i < PADDLE_LEN; i++)
	{
		digitalWrite(i + paddle1Pos + 2, LOW);
	}
}

void drawPaddle2()
{
	pattern |= (1 << 7);

	for (int i = 0; i < PADDLE_LEN; i++)
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
	ballUp = false;
	ballRight = false;
	ballXPos = 4;
	ballYPos = 4;

	for (int i = 0; i < 4; ++i)
	{
		pattern = 255;
		applyShift();

		for (int j = 0; j < 8; ++j)
		{
			digitalWrite(j + 2, LOW);
		}

		delay(100);

		pattern = 0;
		applyShift();

		for (int j = 0; j < 8; ++j)
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
	else if (ballXPos == 0) flashScreen();
	if (ballRight && ballXPos >= 6 && paddle2Pos <= ballYPos && paddle2Pos + PADDLE_LEN >= ballYPos) ballRight = false;
	else if (ballXPos == 7) flashScreen();

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

	for (int i = 0; i < 8; ++i)
	{
		pinMode(i + 2, OUTPUT);
		digitalWrite(i + 2, HIGH);
	}
	ballTimer = millis();
	paddleTimer = ballTimer;
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
