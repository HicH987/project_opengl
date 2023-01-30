#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
//.............................
#include "extra_tools/Window.h"
#include "extra_tools/camera.h"
#include "extra_tools/3d_object.h"
//.............................
#include "external_lib/skybox.h"
#include "external_lib/circle.h"

using namespace glm;
using namespace std;
using Skybox = Learus_Skybox::Skybox;
using Circle = Learus_Circle::Circle;

// prototype of all used functions
// ------------------------------------
void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void allCallbacks();

// windows
// -------
const unsigned int SCR_WIDTH = 950;
const unsigned int SCR_HEIGHT = 400;
GLFWwindow *window;

// camera
// -------
Camera camera(SCR_WIDTH, SCR_HEIGHT, vec3(-0.107400, 2, 20));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
// -------
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float frameToggled = 0.0f;

// stoping Animation
// -------
bool diableCursor = true;
bool isCam1 = false;
bool isCam2 = false;
bool isCam3 = false;
bool isFreeCam = false;
bool showElectronOrbit = true;
float animationSpeed = 1.0f;


float electronOrbitRadius = 100.0f;
vec3 atomPos = vec3(0.0f, -1.0f, 0.0f);
vec3 electronPos = atomPos + vec3(
        sin(frameToggled) * electronOrbitRadius,
        0.0f,
        cos(frameToggled) * electronOrbitRadius
);
vec3 lightElectronPos = electronPos;

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
    Object atomUDObj(mat4(1.0f));
    atomUDObj.loadObj("resources/models/atom_ud.obj");
    atomUDObj.bind_VAO_VBO();
    atomUDObj.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    atomUDObj.setObjectColor(0.66,0.05,0.06);
    atomUDObj.setLightColor(0.92, 1.0, 1.0);

    Object atomLRObj(mat4(1.0f));
    atomLRObj.loadObj("resources/models/atom_lr.obj");
    atomLRObj.bind_VAO_VBO();
    atomLRObj.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    atomLRObj.setObjectColor(0.15,0.05,0.66);
    atomLRObj.setLightColor(0.92, 1.0, 1.0);

    Object atomRLObj(mat4(1.0f));
    atomRLObj.loadObj("resources/models/atom_rl.obj");
    atomRLObj.bind_VAO_VBO();
    atomRLObj.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    atomRLObj.setObjectColor(1.0f, 1.0f, 1.0f);
    atomRLObj.setLightColor(0.92, 1.0, 1.0);


    Circle electronOrbitCircle1(atomPos, electronOrbitRadius, vec3(0.94, 0.39, 0.39), 3000);
    Object electronObj1(mat4(1.0f));
    electronObj1.loadObj("resources/models/electron.obj");
    electronObj1.bind_VAO_VBO();
    electronObj1.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    electronObj1.setObjectColor(0.94, 0.39, 0.39);
    electronObj1.setLightColor(0.92, 1.0, 1.0);

    Circle electronOrbitCircle2(atomPos, electronOrbitRadius, vec3(0.39, 0.44, 0.98), 3000);
    Object electronObj2(mat4(1.0f));
    electronObj2.loadObj("resources/models/electron.obj");
    electronObj2.bind_VAO_VBO();
    electronObj2.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    electronObj2.setObjectColor(0.39, 0.44, 0.94);
    electronObj2.setLightColor(0.92, 1.0, 1.0);

    Circle electronOrbitCircle3(atomPos, electronOrbitRadius, vec3(0.39, 0.94, 0.53), 3000);
    Object electronObj3(mat4(1.0f));
    electronObj3.loadObj("resources/models/electron.obj");
    electronObj3.bind_VAO_VBO();
    electronObj3.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    electronObj3.setObjectColor(0.39, 0.94, 0.53);
    electronObj3.setLightColor(0.92, 1.0, 1.0);

    Circle electronOrbitCircle4(atomPos, electronOrbitRadius, vec3(0.62, 0.39, 0.94), 3000);
    Object electronObj4(mat4(1.0f));
    electronObj4.loadObj("resources/models/electron.obj");
    electronObj4.bind_VAO_VBO();
    electronObj4.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    electronObj4.setObjectColor(0.62, 0.39, 0.94);
    electronObj4.setLightColor(0.92, 1.0, 1.0);

    Circle electronOrbitCircle5(atomPos, electronOrbitRadius, vec3(0.94, 0.6, 0.39), 3000);
    Object electronObj5(mat4(1.0f));
    electronObj5.loadObj("resources/models/electron.obj");
    electronObj5.bind_VAO_VBO();
    electronObj5.loadSahders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    electronObj5.setObjectColor(0.94, 0.6, 0.39);
    electronObj5.setLightColor(0.92, 1.0, 1.0);


    Circle lightElectronOrbitCircle(atomPos, electronOrbitRadius - 50, vec3(0.92, 1, 0), 300);
    LightObj lightElectronObj(mat4(1.0f), lightElectronPos);
    lightElectronObj.loadObj("resources/models/electron.obj");
    lightElectronObj.bind_VAO_VBO();
    lightElectronObj.loadSahders("shaders/light_object.vs", "shaders/light_object.fs");
    lightElectronObj.setObjectColor(0.92, 1, 0);



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
        frameToggled += deltaTime * animationSpeed;

        // input
        // ----------------
        processInput(window);

        // draw skybox
        // ---------------
        skyBox.setUniforms(camera.UpdatedProjection(), mat4(mat3(camera.UpdatedView())));
        skyBox.Draw();



        // calculate atom position
        // --------------------
        mat4 model = mat4(1.0f);
        model = scale(model, vec3(0.3f, 0.3f, 0.3f));
        model = translate(model, atomPos);
        // Rotate around itself
        model = rotate(model, frameToggled * 1.5f * radians(-50.0f), vec3(0.1f, 1.0f, 0.0f));
        atomUDObj.Model = model;
        // calculate atom position
        // --------------------
        model = mat4(1.0f);        model = scale(model, vec3(0.3f, 0.3f, 0.3f));
        model = translate(model, atomPos);
        // Rotate around itself
        model = rotate(model, frameToggled * 1.5f * radians(-50.0f), vec3(0.1f, 1.0f, 0.0f));
        atomLRObj.Model = model;
        // calculate atom position
        // --------------------
        model = mat4(1.0f);        model = scale(model, vec3(0.3f, 0.3f, 0.3f));
        model = translate(model, atomPos);
        // Rotate around itself
        model = rotate(model, frameToggled * 1.5f * radians(-50.0f), vec3(0.1f, 1.0f, 0.0f));
        atomRLObj.Model = model;




        // calculate electron scale, position, orbit rotation & Rotate around itself
        // ----------------------------------------------------------------------
        model = mat4(1.0f);
        model = scale(model, vec3(0.1f, 0.1f, 0.1f));

        electronPos = atomPos + vec3(
                sin(frameToggled) * electronOrbitRadius,
                0.0f,
                cos(frameToggled) * electronOrbitRadius);
        model = translate(model, electronPos);
        electronObj1.Model = model;


        // calculate electron scale, position, orbit rotation & Rotate around itself
        // ----------------------------------------------------------------------
        model = mat4(1.0f);
        model = scale(model, vec3(0.1f, 0.1f, 0.1f));

        electronPos = atomPos + vec3(
                0.0f,
                sin(frameToggled) * electronOrbitRadius,
                cos(frameToggled) * electronOrbitRadius);
        model = translate(model, electronPos);
        electronObj2.Model = model;



        // calculate electron scale, position, orbit rotation & Rotate around itself
        // ----------------------------------------------------------------------
        model = mat4(1.0f);
        model = scale(model, vec3(0.1f, 0.1f, 0.1f));

        electronPos = atomPos + vec3(
                cos(frameToggled) * electronOrbitRadius,
                sin(frameToggled) * electronOrbitRadius,
                0.0f);
        model = translate(model, electronPos);
        electronObj3.Model = model;


        // calculate electron scale, position, orbit rotation & Rotate around itself
        // ----------------------------------------------------------------------
        model = mat4(1.0f);
        model = scale(model, vec3(0.1f, 0.1f, 0.1f));

        float diagonalOrbitRadius = sqrt(pow(electronOrbitRadius, 2) / 2);
        float diagonalOrbitAngle = 45.0f;

        electronPos = atomPos + vec3(sin(frameToggled + diagonalOrbitAngle) * diagonalOrbitRadius,
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



        // calculate electron scale, position, orbit rotation & Rotate around itself
        // ----------------------------------------------------------------------
        model = mat4(1.0f);
        model = scale(model, vec3(0.05f, 0.05f, 0.05f));

        lightElectronPos = atomPos + vec3(
                sin(frameToggled * 5) * electronOrbitRadius,
                0.0f,
                cos(frameToggled * 5) * electronOrbitRadius);
        model = translate(model, lightElectronPos);
        lightElectronObj.Model = model;




        // draw atom, electron
        // ------------------------
        atomUDObj.updateViewPos(camera);
        atomUDObj.updateLightPos(lightElectronPos);
        atomUDObj.drawObject(camera);
        atomLRObj.updateViewPos(camera);
        atomLRObj.updateLightPos(lightElectronPos);
        atomLRObj.drawObject(camera);
        atomRLObj.updateViewPos(camera);
        atomRLObj.updateLightPos(lightElectronPos);
        atomRLObj.drawObject(camera);

        electronObj1.updateViewPos(camera);
        electronObj1.updateLightPos(lightElectronPos);
        electronObj1.drawObject(camera);

        electronObj2.updateViewPos(camera);
        electronObj2.updateLightPos(lightElectronPos);
        electronObj2.drawObject(camera);

        electronObj3.updateViewPos(camera);
        electronObj3.updateLightPos(lightElectronPos);
        electronObj3.drawObject(camera);

        electronObj4.updateViewPos(camera);
        electronObj4.updateLightPos(lightElectronPos);
        electronObj4.drawObject(camera);

        electronObj5.updateViewPos(camera);
        electronObj5.updateLightPos(lightElectronPos);
        electronObj5.drawObject(camera);

        lightElectronObj.updateViewPos(camera);
        lightElectronObj.updateLightPos(lightElectronPos);
        lightElectronObj.drawObject(camera);


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

            lightElectronOrbitCircle.setUniforms(camera.UpdatedProjection(), camera.UpdatedView());
            lightElectronOrbitCircle.scale(vec3(0.1f, 0.1f, 0.1f));
            lightElectronOrbitCircle.rotate(radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
            lightElectronOrbitCircle.Draw();

        }


        // from above
        if (isCam1) {
            isCam1 = false;
            camera.Position = vec3(0.0316, 19.73, -0.81);
            camera.Front = vec3(0.0, -1, -0.0175);
            camera.Pitch = -89.0f;
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
    electronObj3.delete_VAO_Buffers();
    electronObj4.delete_VAO_Buffers();
    electronObj5.delete_VAO_Buffers();
    lightElectronObj.delete_VAO_Buffers();

    atomUDObj.delete_VAO_Buffers();
    atomLRObj.delete_VAO_Buffers();
    atomRLObj.delete_VAO_Buffers();

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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (animationSpeed != 0) animationSpeed = 0;
        else animationSpeed = 1;
    }

    if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
        animationSpeed = animationSpeed * 1.2;
    if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
        animationSpeed = animationSpeed / 1.2;
    if (key == GLFW_KEY_KP_MULTIPLY && action == GLFW_PRESS)
        animationSpeed = 1;

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

