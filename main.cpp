#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>


const int WIDTH = 1920;
const int HEIGHT = 1080;
const int SQUARES_HORIZONTAL = 128;
const int SQUARES_VERTICAL = 69;
const int SQUARE_SIZE = 14;
const int BUTTON_OFFSET = 600;


void mooreNeighbourhood(std::vector<std::vector<sf::RectangleShape>> &squares);


int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Game Of Life", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    std::vector<std::vector<sf::RectangleShape>> squares(SQUARES_HORIZONTAL,
            std::vector<sf::RectangleShape>(SQUARES_VERTICAL,
                    sf::RectangleShape(sf::Vector2f(SQUARE_SIZE, SQUARE_SIZE))));
    bool isPlay = false;
    sf::Clock timer;
    sf::Texture play;
    sf::Texture pause;
    sf::Texture restart;
    sf::Texture faster;
    sf::Texture slower;
    sf::Sprite playPauseSprite;
    sf::Sprite restartSprite;
    sf::Sprite fasterSprite;
    sf::Sprite slowerSprite;
    float deltaTime = 0.15;

    play.loadFromFile("src\\play.png");
    pause.loadFromFile("src\\pause.png");
    restart.loadFromFile("src\\restart.png");
    faster.loadFromFile("src\\faster.png");
    slower.loadFromFile("src\\slower.png");

    playPauseSprite.setTexture(play);
    playPauseSprite.setScale(0.35, 0.35);
    playPauseSprite.setPosition(BUTTON_OFFSET, HEIGHT - SQUARE_SIZE * 3);
    restartSprite.setTexture(restart);
    restartSprite.setScale(0.35, 0.35);
    restartSprite.setPosition(playPauseSprite.getPosition().x + playPauseSprite.getGlobalBounds().width + 20,
            HEIGHT - SQUARE_SIZE * 3);

    fasterSprite.setTexture(faster);
    fasterSprite.setScale(0.35, 0.35);
    fasterSprite.setPosition(WIDTH - BUTTON_OFFSET - fasterSprite.getGlobalBounds().width,
            HEIGHT - SQUARE_SIZE * 3);
    slowerSprite.setTexture(slower);
    slowerSprite.setScale(0.35, 0.35);
    slowerSprite.setPosition(fasterSprite.getPosition().x - fasterSprite.getGlobalBounds().width - 20,
                              HEIGHT - SQUARE_SIZE * 3);


    for(int i = 0; i < squares.size(); ++i) {
        for(int j = 0; j < squares.at(i).size(); ++j) {
            squares.at(i).at(j).setPosition((SQUARE_SIZE + 1) * i, (SQUARE_SIZE + 1) * j);
        }
    }

    while(window.isOpen()) {
        sf::Event event{};
        while(window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::KeyPressed:
                    switch(event.key.code) {
                        case sf::Keyboard::E:
                            std::cout << "I go home by button E" << std::endl;
                            window.close();

                            break;

                        case sf::Keyboard::Escape:
                            std::cout << "I go home by button Esc" << std::endl;
                            window.close();

                            break;

                        case sf::Keyboard::Space:
                            isPlay = !isPlay;
                            timer.restart();

                            break;

                        default:
                            break;
                    }

                    break;

                case sf::Event::MouseButtonPressed:
                    switch(event.mouseButton.button) {
                        case sf::Mouse::Left:
                            for(auto &item : squares) {
                                for(auto &elem : item) {
                                    if(elem.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                            sf::Mouse::getPosition(window).y)) {
                                        elem.setFillColor(sf::Color::Black);
                                        break;
                                    }
                                }
                            }

                            if(playPauseSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                    sf::Mouse::getPosition(window).y)) {
                                if(!isPlay) {
                                    isPlay = !isPlay;
                                    playPauseSprite.setTexture(pause);
                                } else {
                                    isPlay = !isPlay;
                                    playPauseSprite.setTexture(play);
                                }
                            }
                            if(restartSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                                                          sf::Mouse::getPosition(window).y)) {
                                for(auto &item : squares) {
                                    for(auto &elem : item) {
                                        if(elem.getFillColor() == sf::Color::Black) {
                                            elem.setFillColor(sf::Color::White);
                                        }
                                    }
                                }
                            }

                            if(fasterSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                                                        sf::Mouse::getPosition(window).y)) {
                                if(deltaTime >= 2) {
                                    deltaTime -= 1;
                                } else if(deltaTime >= 1) {
                                    deltaTime -= 0.5;
                                } else if(deltaTime >= 0.7) {
                                    deltaTime -= 0.1;
                                } else if(deltaTime >= 0.5) {
                                    deltaTime -= 0.05;
                                } else if( deltaTime >= 0.03) {
                                    deltaTime -= 0.01;
                                }
                            }

                            if(slowerSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                                                       sf::Mouse::getPosition(window).y)) {
                                if(deltaTime <= 0.15) {
                                    deltaTime += 0.01;
                                } else if(deltaTime <= 0.5) {
                                    deltaTime += 0.05;
                                } else if(deltaTime <= 0.7) {
                                    deltaTime += 0.1;
                                } else if(deltaTime <= 1) {
                                    deltaTime += 0.5;
                                } else {
                                    deltaTime += 1;
                                }
                            }

                            break;

                        case sf::Mouse::Right:
                            for(auto &item : squares) {
                                for(auto &elem : item) {
                                    if(elem.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                            sf::Mouse::getPosition(window).y)) {
                                        elem.setFillColor(sf::Color::White);
                                        break;
                                    }
                                }
                            }

                            break;

                        default:
                            break;
                    }

                    break;

                default:
                    break;
            }
        }

        if(isPlay and timer.getElapsedTime() >= sf::seconds(deltaTime)) {
            timer.restart();
            mooreNeighbourhood(squares);
        }

        for(auto &item : squares) {
            for(auto &elem : item) {
                if(elem.getFillColor() != sf::Color::Black) {
                    if (elem.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                            sf::Mouse::getPosition(window).y)) {
                        elem.setFillColor(sf::Color(200, 200, 200));
                    } else {
                        elem.setFillColor(sf::Color::White);
                    }
                }
            }
        }

        window.clear(sf::Color(150, 150, 255));

        for(const auto &item : squares) {
            for(const auto &elem : item) {
                window.draw(elem);
            }
        }

        window.draw(playPauseSprite);
        window.draw(restartSprite);
        window.draw(fasterSprite);
        window.draw(slowerSprite);

        window.display();
    }

    return 0;
}


void mooreNeighbourhood(std::vector<std::vector<sf::RectangleShape>> &squares) {
    std::vector<std::pair<int, int>> shouldBeBlack;
    std::vector<std::pair<int, int>> shouldBeWhite;
    bool overflowX = false;
    bool overflowY = false;

    for(int i = 0; i < squares.size(); ++i) {
        for(int j = 0; j < squares.at(i).size(); ++j) {
            int counter = 0;
            sf::Color color = squares.at(i).at(j).getFillColor();
            for(int u = -1; u < 2; ++u) {
                for(int k = -1; k < 2; ++k) {
                    if(i + u > SQUARES_HORIZONTAL - 1 or i + u < 0) {
                        overflowX = true;
                        i = (i + u > SQUARES_HORIZONTAL - 1) ? -1 : SQUARES_HORIZONTAL;
                    }
                    if(j + k > SQUARES_VERTICAL - 1 or j + k < 0) {
                        overflowY = true;
                        j = (j + k > SQUARES_VERTICAL - 1) ? -1 : SQUARES_VERTICAL;
                    }
                    if(squares.at(i + u).at(j + k).getFillColor() == sf::Color::Black) {
                        counter++;
                    }

                    if(overflowX) {
                        overflowX = false;
                        i = (i == -1) ? SQUARES_HORIZONTAL - 1 : 0;
                    }
                    if(overflowY) {
                        overflowY = false;
                        j = (j == -1) ? SQUARES_VERTICAL - 1 : 0;
                    }
                }
            }
            if(color == sf::Color::Black) {
                counter--;
            }

            if(color != sf::Color::Black and counter == 3) {
                shouldBeBlack.emplace_back(std::make_pair(i, j));
            } else if(color == sf::Color::Black and (counter < 2 or counter > 3)) {
                shouldBeWhite.emplace_back(std::make_pair(i, j));
            }
        }
    }

    for(const std::pair<int, int> &item : shouldBeBlack) {
        squares.at(item.first).at(item.second).setFillColor(sf::Color::Black);
    }
    for(const std::pair<int, int> &item : shouldBeWhite) {
        squares.at(item.first).at(item.second).setFillColor(sf::Color::White);
    }

    shouldBeBlack.clear();
    shouldBeWhite.clear();
}
