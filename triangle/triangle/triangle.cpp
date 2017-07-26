#include <iostream>
#include <string>
#include <codecvt>
#include <functional>
#include <thread>
#include <chrono>
#include <GLFW/glfw3.h>

using namespace std::chrono_literals;


namespace str {
    std::string ws2s(const std::wstring &wout)
    {
        typedef std::codecvt_utf8<wchar_t> convert_type;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes(wout);
    }

    std::wstring s2ws(const std::string &str)
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(str);
    }
}

struct Log {
    static void debug(const std::wstring &msg)
    {
#ifdef _DEBUG
        std::wcout << msg << std::endl;
#endif
    }

    static void info(const std::wstring &msg)
    {
        std::wcout << msg << std::endl;
    }

    static void error(const std::wstring &msg)
    {
        std::wcout << msg << std::endl;
    }
};

/// <summary>Execute some code on 
///
class on_scope_leave_class
{
public:
    using callback = std::function<void()>;

    on_scope_leave_class(const callback &on_destroy)
    {
        _on_destroy = on_destroy;
    }

    // Disallow object move or copy
    on_scope_leave_class(const on_scope_leave_class &) = delete;
    on_scope_leave_class operator=(const on_scope_leave_class &) = delete;
    on_scope_leave_class(on_scope_leave_class &&) = delete;
    on_scope_leave_class operator=(on_scope_leave_class &&) = delete;

    ~on_scope_leave_class()
    {
        _on_destroy();
    }

private:
    callback _on_destroy;
};

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE TOKENPASTE2(Unique_, __LINE__)

#define on_scope_leave(callback) on_scope_leave_class UNIQUE(callback)

int wmain_impl()
{
    if (!glfwInit()) {
        Log::error(L"Could not initialize glfw library");
        return EXIT_FAILURE;
    }

    on_scope_leave([] {
        glfwTerminate();
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL triangle", nullptr, nullptr);
    if (!window) {
        Log::error(L"Could not initialize window");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
        // draw
        glClear(GL_COLOR_CLEAR_VALUE | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);

        // handle events
        glfwPollEvents();

        // most basic frames/second management
        std::this_thread::sleep_for(10ms);
    }

    return EXIT_SUCCESS;
}

int wmain()
{
    int ret_code = EXIT_SUCCESS;
    try {
        ret_code = wmain_impl();
    } catch (const std::exception &ex) {
        Log::error(L"Unhandled std exception -> " + str::s2ws(ex.what()));
    } catch (...) {
        Log::error(L"Unrecognized unhandled examption");
    }

    Log::debug(L"Application exited with code: " + std::to_wstring(ret_code));
    return ret_code;
}
