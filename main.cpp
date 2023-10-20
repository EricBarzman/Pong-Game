
#include "raylib.h"

//A choice of colors
Color Blue = Color{ 48, 50, 132, 255 };
Color Dark_Green = Color{ 35, 45, 105, 255 };
Color Turquoise = Color{ 115, 56, 196, 255 };
Color Red = Color { 225, 30, 42, 255 };

const char* whoWon = nullptr;

int leftScore = 0;
int rightScore = 0;
bool bHold = false;

const int ScreenWidth = 800;
const int ScreenHeight = 600;


struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		DrawCircle( (int)x, (int)y, radius, WHITE);
	}

	void Update()
	{
		x += speedX * GetFrameTime();
		y += speedY * GetFrameTime();

		if (y + radius >= GetScreenHeight() || y - radius <= 0)
		{
			speedY *= -1;
		}
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw()
	{
		DrawRectangleRounded(GetRect(), 0.8, 0, WHITE);
	}
};

int main()
{	
	InitWindow(ScreenWidth, ScreenHeight, "A Game of Pong");
	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball;
	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 6;
	ball.speedX = 220;
	ball.speedY = 330;

	//Starting pos and size
	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 30;
	leftPaddle.height = 120;
	leftPaddle.speed = 500;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 30;
	rightPaddle.height = 120;
	rightPaddle.speed = 500;

	while (!WindowShouldClose())
	{
		ball.Update();

		if (IsKeyDown(KEY_W))
		{
			if (leftPaddle.y - (leftPaddle.height / 2 ) - 10 > 0())
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_S))
		{
			if (leftPaddle.y + (leftPaddle.height / 2 ) - 5 < GetScreenHeight())
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP))
		{
			if (rightPaddle.y - (rightPaddle.height / 2) - 10 > 0())
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
		}
		if (IsKeyDown(KEY_DOWN))
		{
			if (rightPaddle.y + (rightPaddle.height / 2) - 5 < GetScreenHeight())
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * (- ball.speedX);
			}
		}

		if (ball.x < 0)
		{
			whoWon = "Right Player Won!";
			if (!bHold)
			{
				rightScore++;
				bHold = true;
			}
		}

		if (ball.x > GetScreenWidth())
		{
			whoWon = "Left Player Won!";
			if (!bHold)
			{
				leftScore++;
				bHold = true;
			}
		}

		BeginDrawing();
			ClearBackground(Dark_Green);
			DrawRectangle(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, Blue);
			DrawCircle(ScreenWidth / 2, ScreenHeight / 2, 280, Turquoise);
			DrawLine(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, WHITE);
			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			//It's over!
			if (whoWon)
			{
				int textWidth = MeasureText(whoWon, 60);
				DrawText(whoWon, GetScreenWidth()/2 - textWidth/2, GetScreenHeight() / 2 - 30, 60, WHITE);
			}

			//Reset ball
			if (whoWon && IsKeyPressed(KEY_SPACE))
			{
				ball.x = GetScreenWidth() / 2;
				ball.y = GetScreenHeight() / 2;
				ball.speedX = 300;
				ball.speedY = 300;
				whoWon = nullptr;
				bHold = false;
			}
			DrawFPS(10, 10);
			DrawText(TextFormat("%i", leftScore), ScreenWidth / 4, 20, 50, WHITE);
			DrawText(TextFormat("%i", rightScore), 3 * ScreenWidth / 4, 20, 50, WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}