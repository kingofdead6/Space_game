#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <Windows.h>
#include <mmsystem.h>

using namespace sf;
using namespace std;

// Classes

class Bullet {
public:
    Sprite shape;
    Bullet(Texture* texture, Vector2f pos) {
        this->shape.setTexture(*texture);
        this->shape.setScale(0.2f, 0.2f);
        this->shape.setPosition(pos);
    }
    ~Bullet() {}
};

class Player {
public:
    Sprite shape;
    Texture* texture;
    bool isAlive;
    int HP;
    int HPmax;
    vector<Bullet> bullets;

    Player(Texture* texture) {
        this->HPmax = 10;
        this->HP = this->HPmax;
        this->texture = texture;
        this->shape.setTexture(*texture);
        this->shape.setScale(0.4f, 0.4f);
        this->isAlive = true;
    }
    ~Player() {}
};

class Enemy {
public:
    int HP;
    int HPmax;
    Sprite shape;

    Enemy(Texture* texture, Vector2u windowSize) {
        this->HPmax = rand() % 3 + 1;
        this->HP = this->HPmax;
        this->shape.setTexture(*texture);
        this->shape.setScale(0.4f, 0.4f);
        this->shape.setPosition(windowSize.x - this->shape.getGlobalBounds().width, rand() % (windowSize.y - (int)this->shape.getGlobalBounds().height));
    }
    ~Enemy() {}
};

// Explosion struct
struct Explosion {
    Sprite sprite;
    Clock clock;
};

// Functions

void movePlayer(Player& player , float dt , float dtmultiplier); 

void WinCol(Player& player, RenderWindow& window) ;

void backgrounddraw(RenderWindow &window);

int main() {
    // Music
    bool played = PlaySound(TEXT("Assets/space_shooting/spacemusic.wav"), NULL, SND_FILENAME | SND_ASYNC);
    // Init
    srand(time(NULL));
    int score = 0;
    bool isPaused = false;

    // Window initialization
    RenderWindow window(VideoMode(1600, 900), "Space Shooting", Style::Default);
    window.setFramerateLimit(60);


    Clock clock ;
    float dt = 0.f ;
    float dtmultiplier = 62.5f;
    








    Image icon;
    icon.loadFromFile("Assets/space_shooting/space_ship.ico");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Explosion effect
    Texture explosionTex;
    explosionTex.loadFromFile("Assets/space_shooting/Explotion.png");
    vector<Explosion> explosions;

    // Background
   
    
    

    // Text
    Font font;
    font.loadFromFile("Fonts/ClassicCassette.ttf");

    // Player
    Texture playerTex;
    playerTex.loadFromFile("Assets/space_shooting/space_ship.png");
    Player player(&playerTex);
    int shootTimer = 20;
    bool noShootTimer = false;
    Clock shootClock;

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setPosition(window.getSize().x / 2 - scoreText.getGlobalBounds().width, 25.f);
    scoreText.setFillColor(Color::White);


    Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(30);
    hpText.setPosition(window.getSize().x / 2 - hpText.getGlobalBounds().width + 10.f , 0.f);

    
    Text loseText;
    loseText.setFont(font);
    loseText.setCharacterSize(400);
    loseText.setString("YOU LOSE");
    loseText.setFillColor(Color::Red);
    FloatRect textRect = loseText.getLocalBounds();
    loseText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    loseText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    Text winText;
    winText.setFont(font);
    winText.setCharacterSize(500);
    winText.setString("YOU WIN");
    winText.setFillColor(Color::Green);
    textRect = winText.getLocalBounds();
    winText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    winText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    Text pausetxt ;
    pausetxt.setFont(font);
    pausetxt.setCharacterSize(500);
    pausetxt.setString("PAUSED");
    pausetxt.setFillColor(Color::Yellow);
    textRect = pausetxt.getLocalBounds();
    pausetxt.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    pausetxt.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    // Enemies
    Texture enemyTex;
    enemyTex.loadFromFile("Assets/space_shooting/meteor.png");
    int spawnCount = 0;
    vector<Enemy> enemies;

    // Projectiles
    Texture bulletTex;
    bulletTex.loadFromFile("Assets/space_shooting/misslle.png");

    // Closing window controls
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.KeyPressed && event.key.code == Keyboard::Escape) {
                window.close();
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                isPaused = !isPaused; 
                }
        }
    if (!isPaused) {
       dt =  clock.restart().asSeconds();
        // Player controls and movements
        movePlayer(player , dt , dtmultiplier);
        WinCol(player, window);

        if (shootTimer < 20) {
            shootTimer++;
        }

        // Right-click to remove shootTimer for 3 seconds
        if (Mouse::isButtonPressed(Mouse::Right) && !noShootTimer) {
            noShootTimer = true;
            scoreText.setFillColor(Color::Yellow);
            shootClock.restart();
        }

        if (noShootTimer && shootClock.getElapsedTime().asSeconds() > 3.f) {
            noShootTimer = false;
            scoreText.setFillColor(Color::White);
        }

        if (Mouse::isButtonPressed(Mouse::Left) && (shootTimer >= 20 || noShootTimer) || Keyboard::isKeyPressed(Keyboard::Enter) && (shootTimer >= 20 || noShootTimer)) {
            player.bullets.push_back(Bullet(&bulletTex, player.shape.getPosition()));
            if (!noShootTimer) {
                shootTimer = 0;
            }
        }

        if (spawnCount < 50) {
            spawnCount++;
        }
        if (spawnCount >= 50) {
            enemies.push_back(Enemy(&enemyTex, window.getSize()));
            spawnCount = 0;
        }

        for (size_t i = 0; i < player.bullets.size(); i++) {
            player.bullets[i].shape.move(25.f, 0.f);

            if (player.bullets[i].shape.getPosition().x > window.getSize().x) {
                player.bullets.erase(player.bullets.begin() + i);
                break;
            }
        }

        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].shape.move(-6.f, 0.f);

            if (enemies[i].shape.getPosition().x < 0) {
                enemies.erase(enemies.begin() + i);
                score--;
                break;
            }

            if (enemies[i].shape.getGlobalBounds().intersects(player.shape.getGlobalBounds())) {
                enemies.erase(enemies.begin() + i);
                score--;
                player.HP--;

                if (player.HP == 0) {
                    player.isAlive = false;
                }
                break;
            }

            for (size_t j = 0; j < player.bullets.size(); j++) {
                if (enemies[i].shape.getGlobalBounds().intersects(player.bullets[j].shape.getGlobalBounds())) {
                    Explosion explosion;
                    explosion.sprite.setTexture(explosionTex);
                    explosion.sprite.setScale(0.4f, 0.4f);
                    player.bullets.erase(player.bullets.begin() + j);
                    enemies[i].HP--;

                    if (enemies[i].HP == 0) {
                        explosion.sprite.setPosition(enemies[i].shape.getPosition());
                        explosions.push_back(explosion);
                        enemies.erase(enemies.begin() + i);
                        score++;
                        break;
                    }
                    break;
                }
            }
        }

        // Remove explosions after 1 second
        for (size_t i = 0; i < explosions.size(); i++) {
            if (explosions[i].clock.getElapsedTime().asSeconds() > 0.3f) {
                explosions.erase(explosions.begin() + i);
                i--;
            }
        }

        hpText.setString("HP: " + to_string(player.HP));
        scoreText.setString("Score: " + to_string(score));
        if (player.HP >= 5) {
            hpText.setFillColor(Color::Green);
        } else {
            hpText.setFillColor(Color::Red);
        }
    }
    
        // Printing in window
        window.clear();
        
        backgrounddraw(window);
        


        if (player.isAlive && score > -5 && score < 100) {
            window.draw(hpText);
            window.draw(player.shape);
            window.draw(scoreText);
            for (size_t i = 0; i < player.bullets.size(); i++) {
                window.draw(player.bullets[i].shape);
            }
            for (size_t i = 0; i < enemies.size(); i++) {
                window.draw(enemies[i].shape);
            }
            for (size_t i = 0; i < explosions.size(); i++) {
                window.draw(explosions[i].sprite);
            }
        }
        if (player.isAlive && score >= 100) {
            score = 1000;
            player.HP = 1000;
            window.draw(winText);
        }
        if (!player.isAlive || score <= -5) {
            window.draw(loseText);
        }
        if (isPaused){
           window.draw(pausetxt);
       }
        window.display();
    }
    return 0;
}



void backgrounddraw(RenderWindow &window){
    Texture backgroundTex ;
    backgroundTex.loadFromFile("Assets/space_shooting/back.jpg");
    Sprite background(backgroundTex) , background1(backgroundTex), background2(backgroundTex), 
           background3(backgroundTex), background4(backgroundTex), background5(backgroundTex), 
           background6(backgroundTex), background7(backgroundTex), background8(backgroundTex),
           background9(backgroundTex), background10(backgroundTex), background11(backgroundTex), 
           background12(backgroundTex), background13(backgroundTex), background14(backgroundTex),
           background15(backgroundTex), background16(backgroundTex) , background17(backgroundTex),
           background18(backgroundTex) , background19(backgroundTex) , background20(backgroundTex),
           background21(backgroundTex) , background22(backgroundTex) , background23(backgroundTex),
           background24(backgroundTex) , background25(backgroundTex) , background26(backgroundTex) ,
           background27(backgroundTex) , background28(backgroundTex) , background29(backgroundTex),
           background30(backgroundTex) , background31(backgroundTex) , background32(backgroundTex) ,
           background33(backgroundTex) , background34(backgroundTex) , background35(backgroundTex),
           background36(backgroundTex) ;
    background.setPosition(0.f,0.f);
    background1.setPosition(background.getGlobalBounds().width,0.f);
    background2.setPosition(background.getGlobalBounds().width *2,0.f);
    background3.setPosition(background.getGlobalBounds().width*3,0.f);
    background4.setPosition(background.getGlobalBounds().width*4,0.f);
    background5.setPosition(background.getGlobalBounds().width,background.getGlobalBounds().height);
    background6.setPosition(background.getGlobalBounds().width*2,background.getGlobalBounds().height);
    background7.setPosition(background.getGlobalBounds().width*3,background.getGlobalBounds().height);
    background8.setPosition(background.getGlobalBounds().width*4,background.getGlobalBounds().height);
    background9.setPosition(background.getGlobalBounds().width,background.getGlobalBounds().height*2);
    background10.setPosition(background.getGlobalBounds().width*2,background.getGlobalBounds().height*2);
    background11.setPosition(background.getGlobalBounds().width*3,background.getGlobalBounds().height*2);
    background12.setPosition(background.getGlobalBounds().width*4,background.getGlobalBounds().height*2);
    background13.setPosition(background.getGlobalBounds().width,background.getGlobalBounds().height*3);
    background14.setPosition(background.getGlobalBounds().width*2,background.getGlobalBounds().height*3);
    background15.setPosition(background.getGlobalBounds().width*3,background.getGlobalBounds().height*3);
    background16.setPosition(background.getGlobalBounds().width*4,background.getGlobalBounds().height*3);
    background17.setPosition(0.f,background.getGlobalBounds().height);
    background18.setPosition(0.f,background.getGlobalBounds().height*2);
    background19.setPosition(0.f,background.getGlobalBounds().height*3);
    background20.setPosition(0.f,background.getGlobalBounds().height*4);
    background21.setPosition(background.getGlobalBounds().width,background.getGlobalBounds().height*4);
    background22.setPosition(background.getGlobalBounds().width*2,background.getGlobalBounds().height*4);
    background23.setPosition(background.getGlobalBounds().width*3,background.getGlobalBounds().height*4);
    background24.setPosition(background.getGlobalBounds().width*4,background.getGlobalBounds().height*4);
    background25.setPosition(background.getGlobalBounds().width*5,background.getGlobalBounds().height*4);
    background26.setPosition(background.getGlobalBounds().width*5,background.getGlobalBounds().height);
    background27.setPosition(background.getGlobalBounds().width*5,background.getGlobalBounds().height*2);
    background28.setPosition(background.getGlobalBounds().width*5,background.getGlobalBounds().height*3);
    background29.setPosition(background.getGlobalBounds().width*5,background.getGlobalBounds().height*4);
    background30.setPosition(background.getGlobalBounds().width*5,background.getGlobalBounds().height*5);
    background31.setPosition(background.getGlobalBounds().width*5,0.f);
    background32.setPosition(0.f,background.getGlobalBounds().height*5);
    background33.setPosition(background.getGlobalBounds().width,background.getGlobalBounds().height*5);
    background34.setPosition(background.getGlobalBounds().width*2,background.getGlobalBounds().height*5);
    background35.setPosition(background.getGlobalBounds().width*3,background.getGlobalBounds().height*5);
    background36.setPosition(background.getGlobalBounds().width*4,background.getGlobalBounds().height*5);


    

    //Draw 
    window.draw(background);
        window.draw(background1);
        window.draw(background2);
        window.draw(background3);
        window.draw(background4);
        window.draw(background5);
        window.draw(background6);
        window.draw(background7);
        window.draw(background8);
        window.draw(background9);
        window.draw(background10);
        window.draw(background11);
        window.draw(background12);
        window.draw(background13);
        window.draw(background14);
        window.draw(background15);
        window.draw(background16);
        window.draw(background17);
        window.draw(background18);
        window.draw(background19);
        window.draw(background20);
        window.draw(background21);
        window.draw(background22);
        window.draw(background23);
        window.draw(background24);
        window.draw(background25);
        window.draw(background26);
        window.draw(background27);
        window.draw(background28);
        window.draw(background29);
        window.draw(background30);
        window.draw(background31);
        window.draw(background32);
        window.draw(background33);
        window.draw(background34);
        window.draw(background35);
        window.draw(background36);
}

    void movePlayer(Player& player  , float dt , float dtmultiplier) {
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        player.shape.move(-10.f * dt * dtmultiplier, 0.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        player.shape.move(0.f, -10.f* dt * dtmultiplier);
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        player.shape.move(10.f* dt * dtmultiplier, 0.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        player.shape.move(0.f, 10.f* dt * dtmultiplier);
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        player.shape.move(-10.f* dt * dtmultiplier, 0.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        player.shape.move(0.f, -10.f* dt * dtmultiplier);
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        player.shape.move(10.f* dt * dtmultiplier, 0.f);
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        player.shape.move(0.f, 10.f* dt * dtmultiplier);
    }
}

void WinCol(Player& player, RenderWindow& window) {
    if (player.shape.getPosition().x <= 0) {
        player.shape.setPosition(0.f, player.shape.getPosition().y);
    }
    if (player.shape.getPosition().x >= window.getSize().x - player.shape.getGlobalBounds().width) {
        player.shape.setPosition(window.getSize().x - player.shape.getGlobalBounds().width, player.shape.getPosition().y);
    }
    if (player.shape.getPosition().y <= 0) {
        player.shape.setPosition(player.shape.getPosition().x, 0.f);
    }
    if (player.shape.getPosition().y >= window.getSize().y - player.shape.getGlobalBounds().height) {
        player.shape.setPosition(player.shape.getPosition().x, window.getSize().y - player.shape.getGlobalBounds().height);
    }
}