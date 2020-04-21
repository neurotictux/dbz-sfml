#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using sf::Color;

sf::RenderWindow window(sf::VideoMode(1000, 600), "Dragon Ball!");

enum GameState
{
  Initial,
  Arena
};

class InitialScreen
{
  sf::Sprite sprite;
  sf::Texture texture;
  sf::Font font;
  sf::Text text;
  sf::Color color;
  sf::Music music;
  sf::RectangleShape backgroundShape;
  sf::Color backgroundColor;
  sf::Music theme;

  bool darken;
  int increment = 1;
  bool gameStarted = false;
  float alphaBackground;

public:
  InitialScreen()
  {
    if (texture.loadFromFile("textures/start_background.jpg"))
      sprite.setTexture(texture);

    music.openFromFile("sounds/burning_fire.wav");
    if (theme.openFromFile("sounds/theme.wav"))
      theme.play();

    if (font.loadFromFile("fonts/CracklingFire.ttf"))
      text.setFont(font);

    backgroundShape.setSize(sf::Vector2f(1000, 600));
    backgroundColor = sf::Color::Black;

    text.setString("Press Start");
    text.setCharacterSize(80);
    text.setPosition(300, 500);
    color = sf::Color::Red;
    color.a = 0;
  };

  int startGame = false;

  void draw()
  {
    color.a = darken ? color.a - increment : color.a + increment;
    if (color.a >= 255 && !darken)
      darken = true;

    if (color.a <= 0 && darken)
      darken = false;

    text.setFillColor(color);

    if (gameStarted && alphaBackground < 250)
      alphaBackground += 0.3;
    else if (alphaBackground >= 250)
      startGame = true;

    backgroundShape.setFillColor(backgroundColor);
    backgroundColor.a = alphaBackground;
    window.clear();
    window.draw(sprite);
    window.draw(text);
    window.draw(backgroundShape);
    window.display();
  }

  void initGame()
  {
    if (!gameStarted)
    {
      increment = 5;
      music.play();
      theme.stop();
    }
    gameStarted = true;
  }
};

class ArenaScreen
{
  sf::Sprite sprite;
  sf::Texture texture;
  sf::RectangleShape backgroundShape;
  Color backgroundColor;

  bool screenLoaded = false;
  float alphaBackground = 255;

public:
  ArenaScreen()
  {
    if (texture.loadFromFile("textures/background_arena1.png"))
      sprite.setTexture(texture);
    backgroundColor = Color::Black;
    backgroundColor.a = alphaBackground;
    backgroundShape.setSize(sf::Vector2f(1000, 600));
  }

  void draw()
  {
    if (!screenLoaded && alphaBackground > 0)
      alphaBackground -= 0.2;
    else if (alphaBackground <= 0)
      screenLoaded = true;

    backgroundColor.a = alphaBackground;
    backgroundShape.setFillColor(backgroundColor);

    window.clear();
    window.draw(sprite);
    window.draw(backgroundShape);
    window.display();
  }
};

int main()
{
  InitialScreen initialScreen;
  ArenaScreen arenaScreen;

  GameState gameState = GameState::Initial;

  while (window.isOpen())
  {
    switch (gameState)
    {
    case GameState::Initial:
      initialScreen.draw();
      if (initialScreen.startGame)
        gameState = GameState::Arena;
      break;
    case GameState::Arena:
      arenaScreen.draw();
      break;
    }

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && gameState == GameState::Initial)
        initialScreen.initGame();
    }
  }

  return 0;
}