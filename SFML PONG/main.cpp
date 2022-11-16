#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    const float pi = 3.14159f;
    const int gameWidth = 800;
    const int gameHeight = 600;
    Vector2f paddleSize(25, 100);
    float ballRadius = 10.f;


    RenderWindow window(VideoMode(gameWidth, gameHeight, 32), "SFML Pong by Manuel Dantuono",
        Style::Titlebar | Style::Close);
    window.setVerticalSyncEnabled(true);

    // Paddle Izquierdo
    RectangleShape leftPaddle;
    leftPaddle.setSize(paddleSize - Vector2f(3, 3));
    leftPaddle.setOutlineThickness(1);
    leftPaddle.setOutlineColor(Color::White);
    leftPaddle.setFillColor(Color(255, 0, 0));
    leftPaddle.setOrigin(paddleSize / 2.f);

    // Paddle Derecho
    RectangleShape rightPaddle;
    rightPaddle.setSize(paddleSize - Vector2f(3, 3));
    rightPaddle.setOutlineThickness(1);
    rightPaddle.setOutlineColor(Color::White);
    rightPaddle.setFillColor(Color(0, 0, 255));
    rightPaddle.setOrigin(paddleSize / 2.f);

    // Pelota
    CircleShape ball;
    ball.setRadius(ballRadius - 3);
    ball.setOutlineThickness(1);
    ball.setOutlineColor(Color::White);
    ball.setFillColor(Color::Green);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);



}