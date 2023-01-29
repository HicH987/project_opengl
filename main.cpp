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
//Camera camera(SCR_WIDTH, SCR_HEIGHT, vec3(0.55, 7.7, 11.8));
Camera camera(SCR_WIDTH, SCR_HEIGHT, vec3(0.55, 7.7, 20));
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
bool showElectronOrbit = true;

float animationSpeed = 0.0001f;


float electronOrbitRadius = 100.0f;
vec3 atomPos = vec3(0.0f, -1.0f, 0.0f);
vec3 electronPos =
        atomPos + vec3(sin(frameToggled) * electronOrbitRadius, 0.0f, cos(frameToggled) * electronOrbitRadius);

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

    // init skybox
    // ------------------------------------
    Skybox skyBox("resources/textures/kurt/top.png", "resources/textures/kurt/bottom.png",
                  "resources/textures/kurt/left.png", "resources/textures/kurt/right.png",
                  "resources/textures/kurt/back.png", "resources/textures/kurt/front.png");

    // init atom object
    // ------------------------------------
    LightObj atomObj(mat4(1.0f), atomPos);
    atomObj.loadObj("resources/models/a.obj");
    atomObj.bind_VAO_VBO();
    atomObj.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    atomObj.setObjectColor(0.0f, 0.0f, 1.0f);
    atomObj.setLightColor(0.92f, 1.0f, 1.0f);


    Circle electronOrbitCircle1(atomPos, electronOrbitRadius, vec3(0.92f, 0.35f, 0.0f), 3000);
    Object electronObj1(mat4(1.0f));
    electronObj1.loadObj("resources/models/electron.obj");
    electronObj1.bind_VAO_VBO();
    electronObj1.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    electronObj1.setObjectColor(0.92f, 0.35f, 0.0f);

    Circle electronOrbitCircle2(atomPos, electronOrbitRadius, vec3(0.92f, 0.35f, 0.0f), 3000);
    Object electronObj2(mat4(1.0f));
    electronObj2.loadObj("resources/models/electron.obj");
    electronObj2.bind_VAO_VBO();
    electronObj2.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    electronObj2.setObjectColor(0.92f, 0.35f, 0.0f);

    Circle electronOrbitCircle3(atomPos, electronOrbitRadius, vec3(0.92f, 0.35f, 0.0f), 3000);
    Object electronObj3(mat4(1.0f));
    electronObj3.loadObj("resources/models/electron.obj");
    electronObj3.bind_VAO_VBO();
    electronObj3.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    electronObj3.setObjectColor(0.92f, 0.35f, 0.0f);

    Circle electronOrbitCircle4(atomPos, electronOrbitRadius, vec3(0.92f, 0.35f, 0.0f), 3000);
    Object electronObj4(mat4(1.0f));
    electronObj4.loadObj("resources/models/electron.obj");
    electronObj4.bind_VAO_VBO();
    electronObj4.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    electronObj4.setObjectColor(0.92f, 0.35f, 0.0f);

    Circle electronOrbitCircle5(atomPos, electronOrbitRadius, vec3(0.92f, 0.35f, 0.0f), 3000);
    Object electronObj5(mat4(1.0f));
    electronObj5.loadObj("resources/models/electron.obj");
    electronObj5.bind_VAO_VBO();
    electronObj5.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    electronObj5.setObjectColor(0.92f, 0.35f, 0.0f);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    const float angleStep = 2 * 3.14159f / 5;
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
            // calculate atom position
            // --------------------
            mat4 model = mat4(1.0f);
            model = translate(model, atomPos);
            atomObj.Model = model;
            atomObj.updateLightPos(electronPos);
            atomObj.updateViewPos(camera);

            // calculate electron scale, position, orbit rotation & Rotate around itself
            // ----------------------------------------------------------------------
            model = mat4(1.0f);
            model = scale(model, vec3(0.1f, 0.1f, 0.1f));

            electronPos = atomPos +
                          vec3(sin(frameToggled) * electronOrbitRadius, 0.0f, cos(frameToggled) * electronOrbitRadius);
            model = translate(model, electronPos);
            electronObj1.Model = model;



            // calculate electron scale, position, orbit rotation & Rotate around itself
            // ----------------------------------------------------------------------
            model = mat4(1.0f);
            model = scale(model, vec3(0.1f, 0.1f, 0.1f));

            electronPos = atomPos +
                          vec3(0.0f, sin(frameToggled) * electronOrbitRadius, cos(frameToggled) * electronOrbitRadius);
            model = translate(model, electronPos);
            electronObj2.Model = model;



            // calculate electron scale, position, orbit rotation & Rotate around itself
            // ----------------------------------------------------------------------
            model = mat4(1.0f);
            model = scale(model, vec3(0.1f, 0.1f, 0.1f));

            electronPos = atomPos + vec3(cos(frameToggled) * electronOrbitRadius, sin(frameToggled) * electronOrbitRadius, 0.0f);
            model = translate(model, electronPos);
            electronObj3.Model = model;


            // calculate electron scale, position, orbit rotation & Rotate around itself
            // ----------------------------------------------------------------------
            model = mat4(1.0f);
            model = scale(model, vec3(0.1f, 0.1f, 0.1f));

            float diagonalOrbitRadius = sqrt(pow(electronOrbitRadius, 2) / 2);
            float diagonalOrbitAngle = 45.0f;

            vec3 electronPos = atomPos + vec3(sin(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius,
                                              sin(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius,
                                              cos(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius);

            model = translate(model, electronPos);
            electronObj4.Model = model;



            // calculate electron scale, position, orbit rotation & Rotate around itself
            // ----------------------------------------------------------------------
            model = mat4(1.0f);
            model = scale(model, vec3(0.1f, 0.1f, 0.1f));


            electronPos = atomPos + vec3(
                    -sin(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius,
                    sin(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius,
                    cos(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius);

            model = translate(model, electronPos);
            electronObj5.Model = model;

        }

        // draw atom, electron
        // ------------------------
        atomObj.drawObject(camera);

        electronObj1.updateViewPos(camera);
        electronObj1.updateLightPos(atomPos);
        electronObj1.drawObject(camera);

        electronObj2.updateViewPos(camera);
        electronObj2.updateLightPos(electronPos);
        electronObj2.drawObject(camera);

        electronObj3.updateViewPos(camera);
        electronObj3.updateLightPos(electronPos);
        electronObj3.drawObject(camera);

        electronObj4.updateViewPos(camera);
        electronObj4.updateLightPos(electronPos);
        electronObj4.drawObject(camera);

        electronObj5.updateViewPos(camera);
        electronObj5.updateLightPos(electronPos);
        electronObj5.drawObject(camera);


        // Draw a circle showing the electron's orbit around the atom
        if (showElectronOrbit) {
            electronOrbitCircle1.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            electronOrbitCircle1.scale(vec3(0.1f, 0.1f, 0.1f));
            electronOrbitCircle1.rotate(radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
            electronOrbitCircle1.Draw();

            electronOrbitCircle2.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            electronOrbitCircle2.scale(vec3(0.1f, 0.1f, 0.1f));
            electronOrbitCircle2.rotate(radians(90.0f), vec3(0.0f, 1.0f, 0.0f));
            electronOrbitCircle2.Draw();

            electronOrbitCircle3.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            electronOrbitCircle3.scale(vec3(0.1f, 0.1f, 0.1f));
            electronOrbitCircle3.rotate(radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
            electronOrbitCircle3.Draw();

            electronOrbitCircle4.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            electronOrbitCircle4.scale(vec3(0.1f, 0.1f, 0.1f));
            electronOrbitCircle4.rotate(radians(90.0f), vec3(1.0f, 1.0f, 0.0f));
            electronOrbitCircle4.Draw();

            electronOrbitCircle5.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            electronOrbitCircle5.scale(vec3(0.1f, 0.1f, 0.1f));
            electronOrbitCircle5.rotate(radians(90.0f), vec3(1.0f, -1.0f, 0.0f));
            electronOrbitCircle5.Draw();

        }


        // from above
        if (isCam1) {
            isCam1 = false;
            camera.Position = vec3(-0.15, 13.54, 0.74);
            camera.Front = vec3(0.017, -1, -0.033);
            camera.Pitch = -88.0f;
        }
        // from the atom
        if (isCam2) {
            camera.Position = vec3(-0.24, 3.0, 0.13);
            camera.Front = electronPos;
        }
        // from electron's orbit
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
    electronObj1.delete_VAO_Buffers();
    electronObj2.delete_VAO_Buffers();

    atomObj.delete_VAO_Buffers();

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
        showElectronOrbit = !showElectronOrbit;


}


void allCallbacks() {
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
}

