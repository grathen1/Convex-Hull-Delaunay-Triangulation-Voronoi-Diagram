#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>

int state = 0;

std::vector<sf::Vector2f> points;

void draw_menu(sf::RenderWindow& window, bool is_main, sf::Text& inputText, sf::RectangleShape& inputBox, sf::Text& confirmText, sf::RectangleShape& confirmButton) {
    sf::Font font;
    if (!font.loadFromFile("C:/Users/ACER/downloads/BrownieStencil.ttf")) {
        std::cerr << "Error loading font\n";
    }

    if (is_main) {
        sf::Text name;
        name.setFont(font);
        name.setString("convex hull");
        name.setCharacterSize(50);
        name.setFillColor(sf::Color::White);
        name.setPosition(10 + (870 - name.getLocalBounds().width) / 2,
                         10 + (500 - name.getLocalBounds().height) / 2 - 10);
        window.draw(name);

        sf::Text author;
        author.setFont(font);
        author.setString("made by Rita Kulesh");
        author.setCharacterSize(15);
        author.setFillColor(sf::Color::White);
        author.setPosition(10 + (170 - author.getLocalBounds().width) / 2,
                           780 + (10 - author.getLocalBounds().height) / 2 - 10);
        window.draw(author);

        sf::RectangleShape randButt(sf::Vector2f(200, 50));
        randButt.setFillColor(sf::Color::Magenta);
        randButt.setPosition(30, 300);
        window.draw(randButt);

        sf::Text randText;
        randText.setFont(font);
        randText.setString("Random");
        randText.setCharacterSize(25);
        randText.setFillColor(sf::Color::White);
        randText.setPosition(30 + (200 - randText.getLocalBounds().width) / 2,
                             300 + (60 - randText.getLocalBounds().height) / 2 - 10);
        window.draw(randText);

        sf::RectangleShape manualButt(sf::Vector2f(200, 50));
        manualButt.setFillColor(sf::Color::Magenta);
        manualButt.setPosition(250, 300);
        window.draw(manualButt);

        sf::Text manualText;
        manualText.setFont(font);
        manualText.setString("Manual");
        manualText.setCharacterSize(25);
        manualText.setFillColor(sf::Color::White);
        manualText.setPosition(250 + (200 - manualText.getLocalBounds().width) / 2,
                               300 + (60 - manualText.getLocalBounds().height) / 2 - 10);
        window.draw(manualText);

        sf::RectangleShape fileButt(sf::Vector2f(200, 50));
        fileButt.setFillColor(sf::Color::Magenta);
        fileButt.setPosition(470, 300);
        window.draw(fileButt);

        sf::Text fileText;
        fileText.setFont(font);
        fileText.setString("file");
        fileText.setCharacterSize(25);
        fileText.setFillColor(sf::Color::White);
        fileText.setPosition(470 + (200 - fileText.getLocalBounds().width) / 2,
                             300 + (60 - fileText.getLocalBounds().height) / 2 - 10);
        window.draw(fileText);

        sf::RectangleShape exitButt(sf::Vector2f(200, 50));
        exitButt.setFillColor(sf::Color::Red);
        exitButt.setPosition(690, 300);
        window.draw(exitButt);

        sf::Text exitText;
        exitText.setFont(font);
        exitText.setString("Exit");
        exitText.setCharacterSize(25);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(690 + (200 - exitText.getLocalBounds().width) / 2,
                             300 + (60 - exitText.getLocalBounds().height) / 2 - 10);
        window.draw(exitText);
    } else {
        sf::RectangleShape startButt(sf::Vector2f(200, 50));
        startButt.setFillColor(sf::Color::Green);
        startButt.setPosition(10, 10);
        window.draw(startButt);

        sf::Text startText;
        startText.setFont(font);
        startText.setString("Start");
        startText.setCharacterSize(25);
        startText.setFillColor(sf::Color::Black);
        startText.setPosition(10 + (200 - startText.getLocalBounds().width) / 2,
                              15 + (50 - startText.getLocalBounds().height) / 2 - 10);
        window.draw(startText);

        sf::RectangleShape menuButt(sf::Vector2f(200, 50));
        menuButt.setFillColor(sf::Color::Red);
        menuButt.setPosition(10, 70);
        window.draw(menuButt);

        sf::Text menuText;
        menuText.setFont(font);
        menuText.setString("Menu");
        menuText.setCharacterSize(25);
        menuText.setFillColor(sf::Color::Black);
        menuText.setPosition(10 + (200 - menuText.getLocalBounds().width) / 2,
                             75 + (50 - menuText.getLocalBounds().height) / 2 - 10);
        window.draw(menuText);

        sf::RectangleShape xButt(sf::Vector2f(20, 20));
        xButt.setFillColor(sf::Color::Red);
        xButt.setPosition(890, 10);
        window.draw(xButt);

        sf::Text xText;
        xText.setFont(font);
        xText.setString("x");
        xText.setCharacterSize(20);
        xText.setFillColor(sf::Color::White);
        xText.setPosition(890 + (20 - xText.getLocalBounds().width) / 2,
                          15 + (23 - xText.getLocalBounds().height) / 2 - 10);
        window.draw(xText);


    }if (!is_main && state == 2) {
        sf::RectangleShape clearButt(sf::Vector2f(200, 50));
        clearButt.setFillColor(sf::Color::Yellow);
        clearButt.setPosition(10, 130);
        window.draw(clearButt);

        sf::Text clearText;
        clearText.setFont(font);
        clearText.setString("Clear");
        clearText.setCharacterSize(25);
        clearText.setFillColor(sf::Color::Black);
        clearText.setPosition(10 + (200 - clearText.getLocalBounds().width) / 2,
                              135 + (50 - clearText.getLocalBounds().height) / 2 - 10);
        window.draw(clearText);
    }

    if (!is_main && state == 1) {
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(confirmButton);
        window.draw(confirmText);
    }
}

struct Point {
    float x, y;
    bool operator <(const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

float cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

void drawPointsAndHull(sf::RenderWindow& window, const std::vector<Point>& points, const std::vector<Point>& hull) {
    window.clear();
    for (const auto& point : points) {
        sf::CircleShape shape(5);
        shape.setPosition(point.x - 2.5, point.y - 2.5);
        window.draw(shape);
    }
    for (size_t i = 0; i < hull.size(); ++i) {
        sf::Vertex line[] =
                {
                        sf::Vertex(sf::Vector2f(hull[i].x, hull[i].y)),
                        sf::Vertex(sf::Vector2f(hull[(i + 1) % hull.size()].x, hull[(i + 1) % hull.size()].y))
                };
        window.draw(line, 2, sf::Lines);
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Control the speed of the animation
}

std::vector<Point> generateRandomPoints(int count, int minX, int maxX, int minY, int maxY) {
    std::vector<Point> points(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribX(minX, maxX);
    std::uniform_real_distribution<> distribY(minY, maxY);

    for (int i = 0; i < count; ++i) {
        points[i].x = distribX(gen);
        points[i].y = distribY(gen);
    }
    return points;
}

std::vector<Point> convexHull(std::vector<Point>& points, sf::RenderWindow& window) {
    int n = points.size(), k = 0;
    if (n <= 3) return points;

    std::vector<Point> H(2 * n), sortedPoints(points);
    sort(sortedPoints.begin(), sortedPoints.end());

    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k - 2], H[k - 1], sortedPoints[i]) <= 0) {
            k--;
        }
        H[k++] = sortedPoints[i];
    }

    for (int i = n - 1, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(H[k - 2], H[k - 1], sortedPoints[i]) <= 0) {
            k--;
        }
        H[k++] = sortedPoints[i];
    }

    H.resize(k - 1);
    return H;
}

void draw_circle(sf::RenderWindow& window, float x, float y, float radius, sf::Color color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(x - radius, y - radius);
    window.draw(circle);
}

std::vector<Point> readPointsFromFile(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    float x, y;

    while (file >> x >> y) {
        points.push_back(Point{x, y});
    }

    return points;
}

int main() {
    sf::Font font;
    if (!font.loadFromFile("C:/Users/ACER/downloads/BrownieStencil.ttf")) {
        std::cerr << "Error loading font\n";
    }
    sf::RenderWindow window(sf::VideoMode(920, 800), "Demo");
    int numOfPoints = 10;
    std::string inputString = "10";  // default

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20, 133);
    inputText.setString(std::to_string(numOfPoints)); // Значення за замовчуванням

    sf::RectangleShape inputBox(sf::Vector2f(100, 30));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(10, 130);

    sf::Text confirmText;
    confirmText.setFont(font);
    confirmText.setCharacterSize(18);
    confirmText.setFillColor(sf::Color::Black);
    confirmText.setPosition(125, 133);
    confirmText.setString("Generate");

    sf::RectangleShape confirmButton(sf::Vector2f(90, 30));
    confirmButton.setFillColor(sf::Color::Green);
    confirmButton.setPosition(120, 130);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }if (event.type == sf::Event::TextEntered) {
                if (state == 1 && inputBox.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    if (event.text.unicode == 8 && !inputString.empty()) { // Handle backspace
                        inputString.pop_back();
                    } else if (event.text.unicode >= '0' && event.text.unicode <= '9') { // Ensure only numbers are added
                        inputString += static_cast<char>(event.text.unicode);
                    }else if (inputString.empty() && event.text.unicode == '\b') {
                        numOfPoints = 10;
                        inputString = std::to_string(numOfPoints);
                    } else if (inputString.empty() && event.text.unicode == '\r') {
                        numOfPoints = 10;
                        inputString = std::to_string(numOfPoints);
                    }
                    inputText.setString(inputString); // Update the displayed string
                }
            }
            // Confirm button click handling
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                if (confirmButton.getGlobalBounds().contains(mousePos)) {
                    // Confirm button logic here
                    numOfPoints = std::stoi(inputText.getString().toAnsiString()); // Update numOfPoints
                }}
            if (event.type == sf::Event::MouseButtonPressed) {
                if (state == 0) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350) {
                            if (event.mouseButton.x >= 30 && event.mouseButton.x <= 230) {
                                state = 1;
                            } else if (event.mouseButton.x >= 250 && event.mouseButton.x <= 450) {
                                state = 2;
                            } else if (event.mouseButton.x >= 470 && event.mouseButton.x <= 670) {
                                state = 3;
                            } else if (event.mouseButton.x >= 690 && event.mouseButton.x <= 890) {
                                window.close();
                            }
                        }
                    }
                } else if (state == 1) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.x >= 10 && event.mouseButton.x <= 210) {
                            if (event.mouseButton.y >= 10 && event.mouseButton.y <= 60) {
                                std::vector<Point> points = generateRandomPoints(numOfPoints, 0, 920, 0, 800);
                                std::vector<Point> hull = convexHull(points, window);
                                drawPointsAndHull(window, points, hull);
                                sf::sleep(sf::seconds(2));
                            }
                            if (event.mouseButton.y >= 70 && event.mouseButton.y <= 120) {
                                state = 0;
                            }
                        }if (event.mouseButton.x >= 890 && event.mouseButton.x <= 900) {
                            if (event.mouseButton.y >= 10 && event.mouseButton.y <= 30) {
                                window.close();
                            }
                        }
                    }
                } else if (state == 2) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        points.push_back(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.x >= 10 && event.mouseButton.x <= 210) {
                            if (event.mouseButton.y >= 10 && event.mouseButton.y <= 60) {
                                std::vector<Point> pointList;
                                for (const auto &point: points) {
                                    pointList.push_back({point.x, point.y});
                                }
                                std::vector<Point> hull = convexHull(pointList, window);
                                drawPointsAndHull(window, pointList, hull);
                                sf::sleep(sf::seconds(2));
                            }
                            if (event.mouseButton.y >= 70 && event.mouseButton.y <= 120) {
                                state = 0;
                            }if (event.mouseButton.y >= 130 && event.mouseButton.y <= 180) {
                                points.clear();
                            }
                        }if (event.mouseButton.x >= 890 && event.mouseButton.x <= 900) {
                            if (event.mouseButton.y >= 10 && event.mouseButton.y <= 30) {
                                window.close();
                            }
                        }
                    }
                }else if (state == 3){

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.x >= 10 && event.mouseButton.x <= 210) {
                            if (event.mouseButton.y >= 10 && event.mouseButton.y <= 60) {
                                std::vector<Point> filePoints = readPointsFromFile(
                                        "C:/Users/ACER/downloads/points.txt");
                                if (!filePoints.empty()) {
                                    std::vector<Point> hull = convexHull(filePoints, window);
                                    drawPointsAndHull(window, filePoints, hull);
                                } else {
                                    std::cerr << "Failed to read points from file or file is empty.\n";
                                }
                                sf::sleep(sf::seconds(2));
                            }
                            if (event.mouseButton.y >= 70 && event.mouseButton.y <= 120) {
                                state = 0;
                            }
                        }if (event.mouseButton.x >= 890 && event.mouseButton.x <= 900) {
                            if (event.mouseButton.y >= 10 && event.mouseButton.y <= 30) {
                                window.close();
                            }
                        }
                    }
                }
            }
        }
        window.clear();
        if (state == 0) {
            draw_menu(window, true, inputText, inputBox, confirmText, confirmButton);
        }
        else if (state == 1) {
            draw_menu(window, false, inputText, inputBox, confirmText, confirmButton);
        }
        if (state == 2) {
            for (int i = 0; i < points.size(); ++i) {
                draw_circle(window, points[i].x, points[i].y, 3, sf::Color::Green);
            }
            draw_menu(window, false, inputText, inputBox, confirmText, confirmButton);
        }
        if (state == 3) {
            draw_menu(window, false, inputText, inputBox, confirmText, confirmButton);
        }

        window.display();
    }

    return 0;
}
