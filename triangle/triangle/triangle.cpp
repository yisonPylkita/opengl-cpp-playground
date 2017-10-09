#include <iostream>
#include <string>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>


// TODO: replace it with spdlog
class Log {
public:
    static void debug(const std::string &msg) {
        std::cout << "[DEBUG]: " << msg << std::endl;
    }

    static void error(const std::string &msg) {
        std::cout << "[ERROR]: " << msg << std::endl;
    }
};

int main_impl()
{
    sf::ContextSettings opengl_context;
    opengl_context.majorVersion = 4;
    opengl_context.minorVersion = 5;
    opengl_context.depthBits = 24;
    sf::Window window(sf::VideoMode(800, 600), "Triangle", sf::Style::Default, opengl_context);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    bool running = true;
    while (running) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                // end the program
                running = false;
            } else if (event.type == sf::Event::Resized) {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    return EXIT_SUCCESS;
}

int main()
{
    int ret_code = EXIT_SUCCESS;
    try {
        ret_code = main_impl();
    } catch (const std::exception &ex) {
        using namespace std::string_literals;
        Log::error("Unhandled std exception -> "s + ex.what());
    } catch (...) {
        Log::error("Unrecognized unhandled exception");
    }

    Log::debug("Application exited with code: " + std::to_string(ret_code));
    return ret_code;
}
