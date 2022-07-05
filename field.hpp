//
// Created by PlatinumSamurai on 05.07.2022.
//

#ifndef WIREWORLD_FIELD_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>


class Field {
private:
    std::vector<std::vector<sf::RectangleShape>> squares;

public:
    Field(const int &width, const int &height, const int &cellSize, const sf::Color &fillColor);
    void updateColors(const sf::Color &activeColor, const sf::RenderWindow &window);
    void resetField(const sf::Color &color);
    void updateCells(const sf::Color &dead, const sf::Color &alive);
    int mooreNeighbourhood(std::pair<int, int> position, const sf::Color &keyColor);
    void highlightMousePosition(const sf::Color &dead, const sf::Color &alive, const sf::RenderWindow &window);

    std::vector<std::vector<sf::RectangleShape>> getField() const;

};


Field::Field(const int &width, const int &height, const int &cellSize, const sf::Color &fillColor) {
    squares = std::vector<std::vector<sf::RectangleShape>>(width, std::vector<sf::RectangleShape>(height,
    sf::RectangleShape(sf::Vector2f(cellSize, cellSize))));

    for(int i = 0; i < squares.size(); ++i) {
        for(int j = 0; j < squares.at(i).size(); ++j) {
            squares.at(i).at(j).setPosition((cellSize + 1) * i, (cellSize + 1) * j);
            squares.at(i).at(j).setFillColor(fillColor);
        }
    }
}


void Field::updateColors(const sf::Color &activeColor, const sf::RenderWindow &window) {
    for(auto &item : squares) {
        for(auto &elem : item) {
            if(elem.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                               sf::Mouse::getPosition(window).y)) {
                elem.setFillColor(activeColor);
                break;
            }
        }
    }
}


void Field::resetField(const sf::Color &color) {
    for(auto &item : squares) {
        for(auto &elem : item) {
            elem.setFillColor(color);
        }
    }
}


void Field::updateCells(const sf::Color &dead, const sf::Color &alive) {
    std::vector<std::pair<int, int>> shouldBeBlack;
    std::vector<std::pair<int, int>> shouldBeWhite;
    for (int i = 0; i < squares.size(); ++i) {
        for (int j = 0; j < squares.at(i).size(); ++j) {
                sf::Color cellColor = squares.at(i).at(j).getFillColor();
                int counter = mooreNeighbourhood(std::make_pair(i, j), alive);
                if(cellColor == alive) {
                    counter--;
                }
                if(counter == 3 and cellColor != alive) {
                    shouldBeBlack.emplace_back(std::make_pair(i, j));
                } else if(cellColor == alive and (counter < 2 or counter > 3)) {
                    shouldBeWhite.emplace_back(std::make_pair(i, j));
                }
        }
    }

    for(const std::pair<int, int> &item : shouldBeBlack) {
        squares.at(item.first).at(item.second).setFillColor(alive);
    }
    for(const std::pair<int, int> &item : shouldBeWhite) {
        squares.at(item.first).at(item.second).setFillColor(dead);
    }
}


int Field::mooreNeighbourhood(std::pair<int, int> position, const sf::Color &keyColor) {
    bool overflowX = false;
    bool overflowY = false;
    int counter = 0;

    for(int u = -1; u < 2; ++u) {
        for(int k = -1; k < 2; ++k) {
            if(position.first + u > static_cast<int>(squares.size()) - 1 or position.first + u < 0) {
                overflowX = true;
                position.first = (position.first + u > static_cast<int>(squares.size()) - 1) ? -1 : squares.size();
            }
            if(position.second + k > static_cast<int>(squares.begin()->size()) - 1 or position.second + k < 0) {
                overflowY = true;
                position.second = (position.second + k > static_cast<int>(squares.begin()->size()) - 1) ? -1 : squares.begin()->size();
            }
            if(squares.at(position.first + u).at(position.second + k).getFillColor() == keyColor) {
                counter++;
            }

            if(overflowX) {
                overflowX = false;
                position.first = (position.first == -1) ? squares.size() - 1 : 0;
            }
            if(overflowY) {
                overflowY = false;
                position.second = (position.second == -1) ? squares.begin()->size() - 1 : 0;
            }
        }
    }

    return counter;
}

void Field::highlightMousePosition(const sf::Color &dead, const sf::Color &alive, const sf::RenderWindow &window) {
    for(auto &item : squares) {
        for(auto &elem : item) {
            sf::Color cellColor = elem.getFillColor();
            if(cellColor != alive) {
                if (elem.getGlobalBounds().contains(sf::Mouse::getPosition(window).x,
                                                    sf::Mouse::getPosition(window).y)) {
                    elem.setFillColor(sf::Color(200, 200, 200));
                } else {
                    elem.setFillColor(dead);
                }
            }
        }
    }
}


std::vector<std::vector<sf::RectangleShape>> Field::getField() const {
    return squares;
}


#define WIREWORLD_FIELD_HPP

#endif //WIREWORLD_FIELD_HPP