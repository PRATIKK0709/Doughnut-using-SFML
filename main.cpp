#include <SFML/Graphics.hpp>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 600;
const sf::Color BACKGROUND_COLOR(0, 0, 0);
const sf::Color DONUT_COLOR(255, 255, 255);

const double R1 = 1.0;
const double R2 = 2.0;
const double K2 = 5.0;
const double K1 = WIDTH / 2;

double A = 0.0;
double B = 0.0;
double rotationSpeedA = 0.0005; // Slower rotation
double rotationSpeedB = 0.001; // Slower rotation

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Rotating Donut");

void plot(sf::Image& image, int x, int y, const sf::Color& color = DONUT_COLOR) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        image.setPixel(x, y, color);
    }
}

void render_frame(sf::Image& image, double A, double B) {
    double cos_A = std::cos(A);
    double sin_A = std::sin(A);
    double cos_B = std::cos(B);
    double sin_B = std::sin(B);

    double zbuffer[WIDTH][HEIGHT] = {0.0};

    for (int theta = 0; theta < 628; theta += 10) {
        double costheta = std::cos(theta / 100.0);
        double sintheta = std::sin(theta / 100.0);

        for (int phi = 0; phi < 628; phi += 1) {
            double cosphi = std::cos(phi / 100.0);
            double sinphi = std::sin(phi / 100.0);

            double x = R2 + R1 * costheta;
            double y = R1 * sintheta;

            double x1 = x * (cos_B * cosphi + sin_A * sin_B * sinphi) - y * cos_A * sin_B;
            double y1 = x * (sin_B * cosphi - sin_A * cos_B * sinphi) + y * cos_A * cos_B;
            double z1 = K2 + x * cos_A * sinphi + y * sin_A;
            double ooz = 1 / z1;

            int xp = static_cast<int>(WIDTH / 2 + K1 * ooz * x1);
            int yp = static_cast<int>(HEIGHT / 2 - K1 * ooz * y1);

            if (xp >= 0 && xp < WIDTH && yp >= 0 && yp < HEIGHT) {
                if (ooz > zbuffer[xp][yp]) {
                    zbuffer[xp][yp] = ooz;
                    plot(image, xp, yp);
                }
            }
        }
    }
}

int main() {
    sf::Image frame;
    frame.create(WIDTH, HEIGHT, BACKGROUND_COLOR);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        frame.create(WIDTH, HEIGHT, BACKGROUND_COLOR);
        render_frame(frame, A, B);

        sf::Texture texture;
        texture.loadFromImage(frame);
        sf::Sprite sprite(texture);

        window.clear();
        window.draw(sprite);
        window.display();

        A += rotationSpeedA; // Slower rotation
        B += rotationSpeedB; // Slower rotation
    }

    return 0;
}
