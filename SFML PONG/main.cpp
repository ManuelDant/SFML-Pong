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

    // Definiendo las propiedades de las paletas
    Clock AITimer;
    const Time AITime = seconds(0.1f);
    const float paddleSpeed = 400.f;
    float rightPaddleSpeed = 0.f;
    const float ballSpeed = 400.f;
    float ballAngle = 0.f; // to be changed later

    Text pauseMessage;
    // Agregar Font despues -> pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(40);
    pauseMessage.setPosition(170.f, 150.f);
    pauseMessage.setFillColor(Color::White);
    pauseMessage.setString("Presione SPACE para empezar a jugar!");

    Clock clock;
    bool isPlaying = false;
    while (window.isOpen())
    {
        // Handle events
        Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == Event::Closed) ||
                ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: play
            if ((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Space))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    leftPaddle.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
                    rightPaddle.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
                    ball.setPosition(gameWidth / 2, gameHeight / 2);

                    // Reset the ball angle
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                    } while (std::abs(std::cos(ballAngle)) < 0.7f);
                }
            }
        }

        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (Keyboard::isKeyPressed(Keyboard::W) &&
                (leftPaddle.getPosition().y - paddleSize.y / 2 > 5.f))
            {
                leftPaddle.move(0.f, -paddleSpeed * deltaTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::S) &&
                (leftPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
                leftPaddle.move(0.f, paddleSpeed * deltaTime);
            }

            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightPaddle.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightPaddle.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                rightPaddle.move(0.f, rightPaddleSpeed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleSize.y / 2)
                    rightPaddleSpeed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleSize.y / 2)
                    rightPaddleSpeed = -paddleSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }

            // Move the ball
            float factor = ballSpeed * deltaTime;
            ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);

            // Check collisions between the ball and the screen
            if (ball.getPosition().x - ballRadius < 0.f)
            {
                isPlaying = false;
                pauseMessage.setString("Perdistes!!!\nPresiona SPACE para reiniciar\nESCAPE para salir.");
            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
                isPlaying = false;
                pauseMessage.setString("Ganastes !! \nPresiona SPACE para reiniciar\nESCAPE para salir.");
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                // Agregarle sonido  -> ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                // Agregarle sonido  -> ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }

            // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < leftPaddle.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > leftPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= leftPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= leftPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > leftPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                // Agregarle sonido  -> ballSound.play();
                ball.setPosition(leftPaddle.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > rightPaddle.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < rightPaddle.getPosition().x &&
                ball.getPosition().y + ballRadius >= rightPaddle.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= rightPaddle.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > rightPaddle.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

                // Agregarle sonido  -> ballSound.play();
                ball.setPosition(rightPaddle.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
        }
        // Clear the window
        window.clear(Color(0, 0, 0));
        if (isPlaying)
        {
            // Draw the paddles and the ball
            window.draw(leftPaddle);
            window.draw(rightPaddle);
            window.draw(ball);
        }
        else
        {
            // Draw the pause message
            window.draw(pauseMessage);
        }

        window.display();
    }

    return EXIT_SUCCESS;
}