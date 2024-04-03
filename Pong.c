/* ----------------------------------------------------------------------------
*
* Learning C and C++ Making a game of Pong 
* Author: Willeke Snijder
* 
* -----------------------------------------------------------------------------
*/

// includes
#include "raylib.h"


// some defines
const int BALL_NUM = 5;

//structs needed for game ------------------------------------------------------

typedef struct Ball {
	Vector2 position;
	Vector2 size;
	Vector2 speed;
	Color color;
	Rectangle ball;
}Ball;

typedef struct Paddle {
	Vector2 position;
	Vector2 size; 
	Vector2 speed;
	Color color;
	Rectangle paddle;
}Paddle;


// global var declaration -------------------------------------------------------
static bool gameover = false;
static bool pause = false;

static const int screenWidth = 1000;
static const int screenHeight = 600;

static Ball Pongball = {0};
static Paddle PadL = { 0 };
static Paddle PadR = { 0 };
static Rectangle top = {0,50, 1000, 10};
static Rectangle bottom = { 1, 550, 1000, 10 };
static int score_left = 0;
static int score_right = 0;
static int max_speed = 5;
static bool ball_shot = false;
static bool leftWon = false;
static bool reset = false;
static int ball_counter = 1;


//functions definitions ----------------------------------------------------------
static void initPong();
static void ResetBall();
static void DrawPong();
static void UpdatePong();

// main program -----------------------------------------------------------------
int main(void)
{
	InitWindow(screenWidth, screenHeight, "pong");
	initPong();
	SetTargetFPS(60);

	while (!WindowShouldClose())   

	{
		UpdatePong();
		DrawPong();
	}
	
	CloseWindow();        

	return 0;

}

// functions -------------------------------------------------------------------
void initPong()
{
	//game vars
	score_left = 0;
	score_right = 0;
	ball_counter = 1;
	ball_shot = false;

	// pad left

	PadL.size = (Vector2){ 10,100 };
	PadL.position = (Vector2){ 30,(screenHeight / 2) - (PadL.size.y / 2) };
	PadL.speed = (Vector2){ 0,7 };
	PadL.color = BLUE;
	PadL.paddle = (Rectangle){ PadL.position.x,PadL.position.y,PadL.size.x,PadL.size.y };

	// pad right
	PadR.size = (Vector2){ 10,100 };
	PadR.position = (Vector2){ ((screenWidth-30)-PadR.size.x),(screenHeight / 2) - (PadR.size.y / 2) };
	PadR.speed = (Vector2){ 0,7 };
	PadR.color = RED;
	PadR.paddle = (Rectangle){ PadR.position.x,PadR.position.y,PadR.size.x,PadR.size.y };

	// ball
	Pongball.size = (Vector2){ 25,25};
	Pongball.speed = (Vector2){ 5,0 };
	Pongball.color = WHITE;
	if (!leftWon)
	{
		Pongball.position = (Vector2){ PadR.position.x - Pongball.size.x, PadR.position.y + (PadR.size.y / 2) - (Pongball.size.y / 2) };
		Pongball.speed = (Vector2){ -5,0 };
	}
	else 
	{
		Pongball.speed = (Vector2){ 5,0 };
		Pongball.position = (Vector2){ PadL.position.x + PadL.size.x,PadL.position.y + (PadL.size.y / 2) - (Pongball.size.y / 2) };
	}
	Pongball.ball = (Rectangle){ Pongball.position.x,Pongball.position.y,Pongball.size.x,Pongball.size.y };

}

void ResetBall()
{
	// reset de ball naar nieuwe plek
	if (leftWon == false)
	{
		Pongball.ball.x = PadR.paddle.x - Pongball.size.x;
		Pongball.ball.y = PadR.paddle.y + (PadR.size.y / 2) - (Pongball.size.y / 2);
		Pongball.speed = (Vector2){ -5,0 };

	}
	if (leftWon == true)
	{
		Pongball.ball.x = PadL.paddle.x + PadL.size.x;
		Pongball.ball.y = PadL.paddle.y + (PadL.size.y / 2) - (Pongball.size.y / 2);
		Pongball.speed = (Vector2){ 5,0 };
	}
}

void DrawPong()
{
	BeginDrawing();


	// Draw playing field
	ClearBackground(GRAY);
	DrawRectangleRec(top, LIGHTGRAY);
	DrawRectangleRec(bottom, LIGHTGRAY);
	DrawLine(500, 50, 500, 550, LIGHTGRAY);
	int titlesize = 40;
	int Pongsize = MeasureText("PONG", titlesize);
	DrawText("PONG", screenWidth / 2 - Pongsize / 2, 10, titlesize, BLACK);

	if (!gameover)
	{
	
		// draw the ball
		DrawRectangleRec(Pongball.ball, Pongball.color);
		// draw the paddles
		DrawRectangleRec(PadL.paddle, PadL.color);
		DrawRectangleRec(PadR.paddle, PadR.color);
		// draw the score
		DrawText(TextFormat("%i", score_left ),100,10,titlesize,BLACK);
		DrawText(TextFormat("%i", score_right), screenWidth - 100 - (MeasureText(TextFormat("%i", score_right), titlesize)), 10, titlesize, BLACK);
		// draw remaining balls.

		if (pause)
		{
			DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 40) / 2, screenHeight / 2 - 40, 40, LIGHTGRAY);
		}
		
	}
	else
	{
		DrawText("END GAME", screenWidth / 2 - MeasureText("END GAME", 40) / 2, screenHeight / 2 - 40, 40, LIGHTGRAY);
		if (score_left > score_right)
		{
			DrawText("BLUE WON", screenWidth / 2 - MeasureText("BLUE WON", 40) / 2, screenHeight / 2, 40, BLUE);
		}
		else
		{
			DrawText("RED WON", screenWidth / 2 - MeasureText("RED WON", 40) / 2, screenHeight / 2, 40, RED);
		}
		if (IsKeyPressed(KEY_ENTER))
		{
			gameover = false;
			initPong();
		}
	}
	

	EndDrawing();

}


void UpdatePong()
{
	if (!gameover)
	{

		if (IsKeyPressed('P'))
		{
			pause = !pause;
		}

		if (!pause)
		{
			// ball logic 
			
			if (!ball_shot && leftWon==false)
			{			
				Pongball.ball.y = PadR.paddle.y + (PadR.size.y / 2) - (Pongball.size.y / 2);			
			}
			if (!ball_shot && leftWon==true)
			{
				Pongball.ball.y = PadL.paddle.y + (PadL.size.y / 2) - (Pongball.size.y / 2);
			}
			
			if (IsKeyPressed(KEY_SPACE) && !ball_shot)
			{
				ball_shot = true;
			}

			if (ball_shot)
			{
				Pongball.ball.x -= Pongball.speed.x;
				Pongball.ball.y += Pongball.speed.y;
			}

				

				if (CheckCollisionRecs(top, Pongball.ball) || CheckCollisionRecs(bottom, Pongball.ball))
				{
					Pongball.speed.y *= -1;
				}
				if (CheckCollisionRecs(PadL.paddle, Pongball.ball) || CheckCollisionRecs(PadR.paddle, Pongball.ball))
				{

					Pongball.speed.x *= -1;
					
					
					if (CheckCollisionRecs(PadL.paddle, Pongball.ball))
					{
						if (PadL.position.y + (PadL.size.y / 2) < Pongball.ball.y + (Pongball.size.y / 2))
						{
							Pongball.speed.y = (PadL.position.y + (PadL.size.y / 2)) / (Pongball.ball.y + (Pongball.size.y / 2)) * -max_speed;

						}
						else
						{
							Pongball.speed.y = (PadL.position.y + (PadL.size.y / 2)) / (Pongball.ball.y + (Pongball.size.y / 2)) * max_speed;
						}
					}
					if (CheckCollisionRecs(PadR.paddle, Pongball.ball))
					{

						if (PadR.position.y + (PadR.size.y / 2) < Pongball.ball.y + (Pongball.size.y / 2))
						{
							Pongball.speed.y = (PadR.position.y + (PadR.size.y / 2)) / (Pongball.ball.y + (Pongball.size.y / 2)) * -max_speed;

						}
						else
						{
							Pongball.speed.y = (PadR.position.y + (PadR.size.y / 2)) / (Pongball.ball.y + (Pongball.size.y / 2)) * max_speed;
						}
						
					}
					
				}
				if (Pongball.ball.x < 0)
				{
					score_right++;
					ball_counter++;
					Pongball.speed = (Vector2){ 0,0 };
					leftWon = false;
					ball_shot = false;
					ResetBall();
					
				}
				if (Pongball.ball.x > screenWidth)
				{
					score_left++;
					ball_counter++;
					Pongball.speed = (Vector2){ 0,0 };
					leftWon = true;
					ball_shot = false;
					ResetBall();			
				}

				if (ball_counter > BALL_NUM)
				{
					gameover = true;
				}


		}
	}


			// paddle logic
		if (IsKeyDown(KEY_UP) && !CheckCollisionRecs(top,PadR.paddle))
		{	
			PadR.paddle.y -= PadR.speed.y;
		}
		if (IsKeyDown(KEY_DOWN) && !CheckCollisionRecs(bottom, PadR.paddle))
		{
			PadR.paddle.y += PadR.speed.y;
		}
		if (IsKeyDown(KEY_W) && !CheckCollisionRecs(top, PadL.paddle))
		{
			PadL.paddle.y -= PadL.speed.y;
		}
		if (IsKeyDown(KEY_S) && !CheckCollisionRecs(bottom, PadL.paddle))
		{
			PadL.paddle.y += PadL.speed.y;
		}
	}

