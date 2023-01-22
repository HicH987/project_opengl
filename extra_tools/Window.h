#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>


void Resize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

class Window {
public:
    GLFWwindow *window;
    float Width;
    float Height;

    void windowHintGLFW() {
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    int initGLFW() {
        if (!glfwInit()) {
            std::cout << "Failed to initialize glfw.\n";
            return -1;
        }

        std::cout << "-- GLFW OK! \n";
        return 0;
    }

    int checkWindowGLFW(GLFWwindow *window) {
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        std::cout << "-- Windows OK! \n";
        return 0;
    }

    int initGLAD() {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
        std::cout << "-- GLAD OK! \n";
        return 0;
    }


    Window(int width, int height, const char *title, bool isResize) {
        Width = width;
        height = height;
        windowHintGLFW();

        if (initGLFW() == -1)
            window = NULL;

        window = glfwCreateWindow(
                width,
                height,
                title,
                NULL,
                NULL);

        if (checkWindowGLFW(window) == -1)
            window = NULL;

        glfwMakeContextCurrent(window);

        if (initGLAD() == -1)
            window = NULL;

        if (isResize)
            glfwSetFramebufferSizeCallback(window, Resize);

        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }
};