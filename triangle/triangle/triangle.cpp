#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>


using namespace std::string_literals;

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

namespace {
    // An array of 3 vectors which represents 3 vertices
    const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f,  -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
}

int main_impl()
{
    sf::ContextSettings opengl_context;
    opengl_context.majorVersion = 4;
    opengl_context.minorVersion = 5;
    opengl_context.depthBits = 24;
    sf::Window window(sf::VideoMode(800, 600), "Triangle", sf::Style::Default, opengl_context);
    window.setVerticalSyncEnabled(true);
    window.setActive(true);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw std::runtime_error("Could not initialize glew - "s +
                                 reinterpret_cast<const char *>(glewGetErrorString(err)));

    // triangle setup
    // This will identify our vertex buffer
    GLuint vertexbuffer{};
    GLuint VertexArrayID{};
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

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
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

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
        Log::error("Unhandled std exception -> "s + ex.what());
    } catch (...) {
        Log::error("Unrecognized unhandled exception");
    }

    Log::debug("Application exited with code: " + std::to_string(ret_code));
    return ret_code;
}
