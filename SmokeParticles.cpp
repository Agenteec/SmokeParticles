
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

const int WindowWidth = 1024;
const int WindowHeight = 728;

struct Particle
{
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float lifetime;
    float alpha;
    float animatiotTime;
    float danimatiotTime;
    bool isAnimated;
    int xm, ym;
    int x, y;
    bool operator==(const Particle& other) const
    {
        return (velocity == other.velocity) &&
            (lifetime == other.lifetime);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Smoke Effect");
    sf::Vector2i offset(WindowWidth, WindowHeight);
    sf::Texture smokeTexture;
    if (!smokeTexture.loadFromFile("Resources\\png\\effects\\particles\\smoke\\Smoke30Frames.png")) {

    }
    std::vector<Particle> particles;


    std::default_random_engine randomEngine;
    std::uniform_real_distribution<float> velocityDist(-50.0f, 50.0f);
    std::uniform_real_distribution<float> lifetimeDist(1.0f, 6.0f);
    std::uniform_int_distribution<int> spriteDistX(0, 5);
    std::uniform_int_distribution<int> spriteDistY(0, 4);
    sf::Font font;
    if (!font.loadFromFile("Resources\\fonts\\arial.ttf")) {

    }
    bool isMouseBLP = 0;
    bool isMouseBRP = 0;
    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(10, 10);
    float elapsedTime = 0.f;
    unsigned int frames = 0;
    sf::Vector2i MousePos;
    sf::Clock clock;
    float particle_regen_time = 0.01f;
    float dparticle_regen_time = 0.f;
    MousePos = sf::Mouse::getPosition(window);
    while (window.isOpen()) {
        sf::Event event;

        float dts = clock.restart().asSeconds();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved)
            {
                MousePos = sf::Mouse::getPosition(window);
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    isMouseBLP = 1;
                }
                if (event.key.code == sf::Mouse::Right)
                {
                    isMouseBRP = 1;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    isMouseBLP = 0;
                }
                if (event.key.code == sf::Mouse::Right)
                {
                    isMouseBRP = 0;
                }
            }
        }
        dparticle_regen_time += dts;
        if (dparticle_regen_time >= particle_regen_time)
        {
            // Создание новой частицы дыма
            if (isMouseBLP) {
                Particle particle;
                particle.sprite.setTexture(smokeTexture);
                
                particle.sprite.setTextureRect(sf::IntRect(spriteDistX(randomEngine) * 256, spriteDistY(randomEngine) * 256, 256, 256));
                particle.sprite.setOrigin(128, 128);
                particle.sprite.setPosition(MousePos.x, MousePos.y);
                particle.velocity.x = velocityDist(randomEngine);
                particle.velocity.y = velocityDist(randomEngine) - 100.0f;
                particle.lifetime = lifetimeDist(randomEngine);
                particle.alpha = particle.lifetime;
                particles.push_back(particle);
                dparticle_regen_time = 0.f;
            }
            dparticle_regen_time += dts;
        }

        // Обновление положения и времени жизни частиц
        for (auto& particle : particles) {
            particle.sprite.move(particle.velocity * dts);
            particle.lifetime -= dts;
            particle.sprite.setColor(sf::Color(255, 255, 255, 255 * (particle.lifetime / particle.alpha)));

            // Удаление частиц, у которых истекло время жизни
            if (particle.lifetime <= 0.0f) {
                particles.erase(std::remove(particles.begin(), particles.end(), particle), particles.end());
            }
        }

        window.clear();
        // Расчет FPS
        elapsedTime += dts;
        frames++;
        if (elapsedTime >= 1.0f) {
            float fps = frames / elapsedTime;
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
            frames = 0;
            elapsedTime = 0;
        }


        // Отрисовка частиц дыма
        for (const auto& particle : particles) {
            window.draw(particle.sprite);
        }
        window.draw(fpsText);
        window.display();
    }
}
