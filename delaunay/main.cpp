#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>
#include <cmath>

int state = 0;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    Point(const sf::Vector2f& v) : x(v.x), y(v.y) {}

    bool operator <(const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Triangle {
    Point a, b, c;
    Triangle(const Point& a, const Point& b, const Point& c) : a(a), b(b), c(c) {}

    bool containsVertex(const Point& p) const {
        return a == p || b == p || c == p;
    }

    bool operator==(const Triangle& other) const {
        std::vector<Point> thisPoints = {a, b, c};
        std::vector<Point> otherPoints = {other.a, other.b, other.c};
        std::sort(thisPoints.begin(), thisPoints.end(), [](const Point& p1, const Point& p2){ return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y); });
        std::sort(otherPoints.begin(), otherPoints.end(), [](const Point& p1, const Point& p2){ return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y); });
        return thisPoints == otherPoints;
    }


    void draw(sf::RenderWindow& window) const {
        sf::ConvexShape triangle;
        triangle.setPointCount(3);
        triangle.setPoint(0, sf::Vector2f(a.x, a.y));
        triangle.setPoint(1, sf::Vector2f(b.x, b.y));
        triangle.setPoint(2, sf::Vector2f(c.x, c.y));
        triangle.setOutlineColor(sf::Color::White);
        triangle.setOutlineThickness(1);
        triangle.setFillColor(sf::Color::Transparent);
        window.draw(triangle);
    }

};

std::vector<Point> points;
std::vector<Triangle> triangles;

double determinant(const Point& A, const Point& B, const Point& C, const Point& P) {
    double ax = A.x - P.x;
    double ay = A.y - P.y;
    double bx = B.x - P.x;
    double by = B.y - P.y;
    double cx = C.x - P.x;
    double cy = C.y - P.y;

    return ax * (by * (cx * cx + cy * cy) - (bx * bx + by * by) * cy) -
           ay * (bx * (cx * cx + cy * cy) - (bx * bx + by * by) * cx) +
           (ax * ax + ay * ay) * (bx * cy - by * cx);
}

// isInsideCircumcircle function checks if point P lies inside the circumcircle of the triangle formed by points A, B, and C
bool isInsideCircumcircle(const Point& A, const Point& B, const Point& C, const Point& P) {
    double det = determinant(A, B, C, P);
    return det > 0;
}

bool validTriangle(const Triangle& candidate, const std::vector<Point>& allPoints) {
    for (const auto& point : allPoints) {
        if (isInsideCircumcircle(candidate.a, candidate.b, candidate.c, point)) {
            return false; // If any point is inside the circumcircle, it's not a valid triangle
        }
    }
    return true;
}

bool edgeCompare(const std::pair<Point, Point>& a, const std::pair<Point, Point>& b) {
    Point a1 = a.first < a.second ? a.first : a.second;
    Point a2 = a.first < a.second ? a.second : a.first;
    Point b1 = b.first < b.second ? b.first : b.second;
    Point b2 = b.first < b.second ? b.second : b.first;
    return a1 < b1 || (a1 == b1 && a2 < b2);
}

// removeDuplicateEdges function removes duplicate edges from the vector
void removeDuplicateEdges(std::vector<std::pair<Point, Point>>& edges) {
    std::sort(edges.begin(), edges.end(), edgeCompare);

    // Unique requires a sorted vector and removes consecutive duplicate edges.
    // Since the edgeCompare sorts the edges regardless of their direction (i.e., (a, b) == (b, a)),
    // consecutive edges that are the same will be next to each other.
    auto it = std::unique(edges.begin(), edges.end(),
                          [](const std::pair<Point, Point>& a, const std::pair<Point, Point>& b) {
                              return a.first == b.first && a.second == b.second;
                          });
    edges.erase(it, edges.end());

    // Edge pairs that consist of the same points but in reversed order are still considered duplicates.
    // The following loop removes these by finding non-unique pairs (a, b) and (b, a).
    it = std::adjacent_find(edges.begin(), edges.end(),
                            [](const std::pair<Point, Point>& a, const std::pair<Point, Point>& b) {
                                return (a.first == b.second && a.second == b.first);
                            });
    while (it != edges.end()) {
        // Erase the current and next edge since they are duplicates.
        it = edges.erase(it);
        it = edges.erase(it);

        // Find the next pair of duplicate edges.
        it = std::adjacent_find(it, edges.end(),
                                [](const std::pair<Point, Point>& a, const std::pair<Point, Point>& b) {
                                    return (a.first == b.second && a.second == b.first);
                                });
    }
}

void draw_circle(sf::RenderWindow& window, float x, float y, float radius, sf::Color color) {
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(x - radius, y - radius);
    window.draw(circle);
}

void drawCircumcircles(sf::RenderWindow& window, const std::vector<Triangle>& triangles) {
    for (const auto& triangle : triangles) {
        // Calculate the circumcenter and radius of the circumcircle
        double xa = triangle.a.x;
        double ya = triangle.a.y;
        double xb = triangle.b.x;
        double yb = triangle.b.y;
        double xc = triangle.c.x;
        double yc = triangle.c.y;

        double D = 2 * (xa * (yb - yc) + xb * (yc - ya) + xc * (ya - yb));
        double xCircumcenter = ((xa * xa + ya * ya) * (yb - yc) + (xb * xb + yb * yb) * (yc - ya) +
                                (xc * xc + yc * yc) * (ya - yb)) / D;
        double yCircumcenter = ((xa * xa + ya * ya) * (xc - xb) + (xb * xb + yb * yb) * (xa - xc) +
                                (xc * xc + yc * yc) * (xb - xa)) / D;
        double circumradius = sqrt((xa - xCircumcenter) * (xa - xCircumcenter) +
                                   (ya - yCircumcenter) * (ya - yCircumcenter));

        // Draw the circumcircle with outline only
        sf::CircleShape circumcircle(circumradius);
        circumcircle.setOutlineThickness(1); // Set the thickness of the outline
        circumcircle.setOutlineColor(sf::Color::Blue); // Set the color of the outline
        circumcircle.setPosition(xCircumcenter - circumradius, yCircumcenter - circumradius); // Set position
        circumcircle.setFillColor(sf::Color::Transparent); // No fill color
        window.draw(circumcircle);
    }
}

void drawEverything(sf::RenderWindow& window) {
    window.clear();

    for (const auto& point : points) {
        sf::CircleShape shape(3);
        shape.setPosition(point.x - 1.5f, point.y - 1.5f);
        shape.setFillColor(sf::Color::Red);
        window.draw(shape);
    }

    for (const auto& triangle : triangles) {
        triangle.draw(window);
    }

    drawCircumcircles(window, triangles);

    window.display();
}

void performDelaunayTriangulation(std::vector<Point>& points, std::vector<Triangle>& triangles, sf::RenderWindow& window) {
    triangles.clear();
    if (points.size() < 3) return;


    double minX = points[0].x;
    double minY = points[0].y;
    double maxX = minX;
    double maxY = minY;
    for (const auto &point: points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
    }

    double dx = maxX - minX;
    double dy = maxY - minY;
    double deltaMax = std::max(dx, dy);
    double midx = (minX + maxX) / 2;
    double midy = (minY + maxY) / 2;

    Point p1(midx - 20 * deltaMax, midy - deltaMax);
    Point p2(midx + 20 * deltaMax, midy - deltaMax);
    Point p3(midx, midy + 20 * deltaMax);
    triangles.push_back(Triangle(p1, p2, p3));

    for (const auto& point : points) {
        std::vector<Triangle> badTriangles;
        std::vector<std::pair<Point, Point>> polygonEdges;

        drawEverything(window); // Show the current state before adding the new point
        draw_circle(window, point.x, point.y, 3, sf::Color::Yellow); // Highlight the new point
        window.display();
        sf::sleep(sf::milliseconds(500));

        // Find all triangles that are no longer valid due to the insertion
        for (const auto& triangle : triangles) {
            if (isInsideCircumcircle(triangle.a, triangle.b, triangle.c, point)) {
                badTriangles.push_back(triangle);

                // Push the edges onto the polygonEdges array
                polygonEdges.push_back(std::make_pair(triangle.a, triangle.b));
                polygonEdges.push_back(std::make_pair(triangle.b, triangle.c));
                polygonEdges.push_back(std::make_pair(triangle.c, triangle.a));
            }
        }

        // Remove them from the triangles array
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
                                       [&](const Triangle& t) {
                                           return std::find(badTriangles.begin(), badTriangles.end(), t) != badTriangles.end();
                                       }),
                        triangles.end());

        drawEverything(window); // Update visualization to show removal of bad triangles
        sf::sleep(sf::milliseconds(500));

        // Remove duplicate edges
        removeDuplicateEdges(polygonEdges);

        // Re-triangulate the polygonal hole
        for (const auto& edge : polygonEdges) {
            Triangle newTriangle(edge.first, edge.second, point);
            if (std::find(triangles.begin(), triangles.end(), newTriangle) == triangles.end()) {
                triangles.push_back(newTriangle);
            }
        }
        drawEverything(window); // Show the updated triangulation with the new triangles
        window.display();
        sf::sleep(sf::milliseconds(500));
    }

    // Remove triangles with vertices of the super-triangle
    triangles.erase(std::remove_if(triangles.begin(), triangles.end(),
                                   [&](const Triangle& t) {
                                       return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
                                   }),
                    triangles.end());
    drawEverything(window); // Visualize the final triangulation without the super-triangle
    window.display();
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

std::vector<Point> readPointsFromFile(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    float x, y;

    while (file >> x >> y) {
        points.push_back(Point{x, y});
    }

    return points;
}

void draw_menu(sf::RenderWindow& window, bool is_main, sf::Text& inputText, sf::RectangleShape& inputBox, sf::Text& confirmText, sf::RectangleShape& confirmButton) {
    sf::Font font;
    if (!font.loadFromFile("C:/Users/ACER/downloads/BrownieStencil.ttf")) {
        std::cerr << "Error loading font\n";
    }

    if (is_main) {
        sf::Text name;
        name.setFont(font);
        name.setString("Delaunay Triangulation");
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

int main() {
    sf::Font font;
    if (!font.loadFromFile("C:/Users/ACER/downloads/BrownieStencil.ttf")) {
        std::cerr << "Error loading font\n";
    }
    int window_width = 920;
    int window_height = 800;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Delaunay Triangulation");

    int numOfPoints = 10;
    std::string inputString = "10";  // default

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(20, 133);
    inputText.setString(std::to_string(numOfPoints));

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
                    if (event.text.unicode == 8 && !inputString.empty()) {
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
                    inputText.setString(inputString);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                if (confirmButton.getGlobalBounds().contains(mousePos)) {
                    numOfPoints = std::stoi(inputText.getString().toAnsiString());
                }}
            if (event.type == sf::Event::MouseButtonPressed) {
                if (state == 0) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (event.mouseButton.y >= 300 && event.mouseButton.y <= 350) {
                            if (event.mouseButton.x >= 30 && event.mouseButton.x <= 230) {
                                state = 1;
                            } else if (event.mouseButton.x >= 250 && event.mouseButton.x <= 450) {
                                state = 2;
                                points.clear();
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
                                points.clear();
                                triangles.clear();
                                std::vector<Point> randomPoints = generateRandomPoints(numOfPoints, 0, window_width, 0, window_height);
                                points = randomPoints;
                                performDelaunayTriangulation(points, triangles, window);
                                std::cout << "Number of triangles: " << triangles.size() << std::endl;
                                // Now draw everything on the screen
                                drawEverything(window);
                               // This will now draw the updated triangles as well
                                // Pause to visualize
                                sf::sleep(sf::seconds(5));
                            }
                            if (event.mouseButton.y >= 70 && event.mouseButton.y <= 120) {
                                state = 0;
                            }
                        }
                        if (event.mouseButton.x >= 890 && event.mouseButton.x <= 900) {
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
                                performDelaunayTriangulation(points, triangles, window);
                                std::cout << "Number of triangles: " << triangles.size() << std::endl;
                                // Now draw everything on the screen
                                window.clear();
                                drawEverything(window); // This will now draw the updated triangles as well
                                // Pause to visualize
                                sf::sleep(sf::seconds(5));
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
                                points.clear();
                                triangles.clear();
                                std::vector<Point> filePoints = readPointsFromFile(
                                        "C:/Users/ACER/downloads/points.txt");
                                if (!filePoints.empty()) {
                                    points = filePoints;
                                    for (const auto& point : points) {
                                        draw_circle(window, point.x, point.y, 3, sf::Color::Red);
                                    }
                                    performDelaunayTriangulation(points, triangles, window);
                                    drawEverything(window);
                                    sf::sleep(sf::seconds(5));
                                } else {
                                    std::cerr << "Failed to read points from file or file is empty.\n";
                                }

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
