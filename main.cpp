#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include<windows.h>

using namespace glm;
using namespace std;

#include "extra_tools/Window.h"
#include "extra_tools/camera.h"
#include "extra_tools/3d_object.h"

#include "external_lib/skybox.h"
#include "external_lib/circle.h"

using Skybox = Learus_Skybox::Skybox;
using Circle = Learus_Circle::Circle;

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void allCallbacks();

// windows
const unsigned int SCR_WIDTH = 950;
const unsigned int SCR_HEIGHT = 400;
GLFWwindow *window;

// camera
Camera camera(SCR_WIDTH, SCR_HEIGHT, vec3(0.55, 7.7, 11.8));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float frameToggled = 0.0f;

// stoping Animation
bool isAnimated = true;
bool diableCursor = true;
bool isCam1 = false;
bool isCam2 = false;
bool isCam3 = false;
bool isFreeCam = false;

bool showEarthOrbit = true;
bool showMoonOrbit = true;

float animationSpeed = 0.0001f;

float earthOrbitRadius = 100.0f;
float moonOrbitRadius = 20.0f;
vec3 sunPos = vec3(0.0f, -1.0f, 0.0f);
vec3 earthPos =
        sunPos + vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);


int main() {
    // create glfw window
    // ------------------------------------
    Window win(SCR_WIDTH, SCR_HEIGHT, "OpenGL", true);
    window = win.window;
    if (window == NULL) {
        std::cout << "Failed to initWindow \n";
        return -1;
    }
    allCallbacks();


    // init orbits circles and skybox
    // ------------------------------------
    Circle EarthOrbitCircle(sunPos, earthOrbitRadius, vec3(0.0f, 1.0f, 1.0f), 3000);
    Circle MoonOrbitCircle(earthPos, moonOrbitRadius, vec3(1.0f, 1.0f, 0.0f), 3000);
    Skybox skyBox("resources/textures/skybox/top.png", "resources/textures/skybox/bottom.png",
                  "resources/textures/skybox/left.png", "resources/textures/skybox/right.png",
                  "resources/textures/skybox/front.png", "resources/textures/skybox/back.png");

    // init earth object
    // ------------------------------------
    Object earthObj(mat4(1.0f));
    earthObj.loadObj("resources/models/earth.obj");
    earthObj.bind_VAO_VBO();
    earthObj.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    earthObj.setObjectColor(0.0f, 0.0f, 1.0f);
    earthObj.setLightColor(0.92f, 1.0f, 1.0f);
//    25, 27, 97
    // init moon object
    // ------------------------------------
    Object moonObj(mat4(1.0f));
    moonObj.loadObj("resources/models/moon.obj");
    moonObj.bind_VAO_VBO();
    moonObj.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    moonObj.setObjectColor(1.0f, 0.5f, 0.31f);
    moonObj.setLightColor(0.92f, 01.0f, 0.0f);

    // init sun object
    // ------------------------------------
    LightObj sunObj(mat4(1.0f), sunPos);
    sunObj.loadObj("resources/models/sun.obj");
    sunObj.bind_VAO_VBO();
    sunObj.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    sunObj.setObjectColor(0.92f, 0.35f, 0.0f);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);



    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frameToggled += deltaTime;

        // input
        // ----------------
        processInput(window);

        // draw skybox
        // ---------------
        skyBox.setUniforms(camera.UpdatedProjection(), mat4(mat3(camera.UpdatedView())));
        skyBox.Draw();


        if (isAnimated) {

            // calculate sun position
            // --------------------
            mat4 model = mat4(1.0f);
            model = translate(model, sunPos);
            sunObj.Model = model;


            // calculate earth scale, position, orbit rotation & Rotate around itself
            // ----------------------------------------------------------------------
            model = mat4(1.0f);
            model = scale(model, vec3(0.1f, 0.1f, 0.1f));

            earthPos = sunPos + vec3(sin(frameToggled) * earthOrbitRadius, 0.0f, cos(frameToggled) * earthOrbitRadius);
            model = translate(model, earthPos);
            // Rotate around itself
            model = rotate(model, frameToggled * 1.5f * radians(-50.0f), vec3(0.1f, 1.0f, 0.0f));
            earthObj.Model = model;


            // calculate moon position, orbit rotation
            // -----------------------------------------
            vec3 moonPos = vec3(sin(frameToggled) * moonOrbitRadius, 0.0f, cos(frameToggled) * moonOrbitRadius);
            model = translate(model, moonPos);
            moonObj.Model = model;
        }

        // draw sun, earth and moon
        // ------------------------
        sunObj.drawObject(camera);

        earthObj.updateViewPos(camera);
        earthObj.updateLightPos(sunPos);
        earthObj.drawObject(camera);

        moonObj.updateLightPos(sunPos);
        moonObj.updateViewPos(camera);
        moonObj.drawObject(camera);


        // Draw a circle showing the earth's orbit around the sun
        if (showEarthOrbit) {
            EarthOrbitCircle.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            EarthOrbitCircle.scale(vec3(0.1f, 0.1f, 0.1f));
            EarthOrbitCircle.rotate(radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
            EarthOrbitCircle.Draw();
        }
        // Draw a circle showing the moon's orbit around the earth
        if (showMoonOrbit) {
            MoonOrbitCircle.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            MoonOrbitCircle.scale(vec3(0.1f, 0.1f, 0.1f));
            MoonOrbitCircle.translate(earthPos);
            MoonOrbitCircle.rotate(radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
            MoonOrbitCircle.Draw();
        }

        // from above
        if (isCam1) {
            isCam1 = false;
            camera.Position = vec3(-0.15, 13.54, 0.74);
            camera.Front = vec3(0.017, -1, -0.033);
            camera.Pitch = -88.0f;
        }
        // from the sun
        if (isCam2) {
            camera.Position = vec3(-0.24, 3.0, 0.13);
            camera.Front = earthPos;
        }
        // from earth's orbit
        if (isCam3) {
            isCam3 = false;
            camera.Position = vec3(-10.20, 1.17, -3.45);
            camera.Front = vec3(0.93, -0.24, 0.33);
            camera.Pitch = -14.0f;
        }

        // disable/enable mouse cursor
        if (diableCursor)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    earthObj.delete_VAO_Buffers();
    moonObj.delete_VAO_Buffers();
    sunObj.delete_VAO_Buffers();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (isFreeCam) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {

    if (isFreeCam) {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);

    }

    std::cout << "position " + to_string(camera.Position) << std::endl;
    std::cout << "front " + to_string(camera.Front) << std::endl;
    printf("pitch %f", camera.Pitch);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        isAnimated = !isAnimated;

    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
        animationSpeed = animationSpeed * 4;
    if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
        animationSpeed = animationSpeed / 4;

    if (key == GLFW_KEY_CAPS_LOCK && action == GLFW_PRESS)
        diableCursor = !diableCursor;

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        isFreeCam = !isFreeCam;

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        isCam1 = !isCam1;

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        isCam2 = !isCam2;

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        isCam3 = !isCam3;

    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        showEarthOrbit = !showEarthOrbit;

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        showMoonOrbit = !showMoonOrbit;


}


void allCallbacks() {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
}

