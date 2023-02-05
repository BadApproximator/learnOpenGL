// Be sure to include GLAD before GLFW. 
// The include file for GLAD includes 
// the required OpenGL headers behind the scenes 
// (like GL/gl.h) so be sure to include GLAD
// before other header files that require OpenGL (like GLFW).

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Shader.h"

struct ModelTransform
{
    glm::vec3 position;
    glm::vec3 rotation; // Euler's angles
    glm::vec3 scale;

    void setUniformScale(float s)
    {
        scale.x = s;
        scale.y = s;
        scale.z = s;
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
#pragma region WINDOW INITIALIZATION
    /* GLFW initialization */
    glfwInit();
    /* GLFW configuration */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* create window */
    GLFWwindow* window = glfwCreateWindow(800, 800, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    /* called each time the window is resized */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
#pragma endregion

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    const int verts = 6;
    GLfloat vertices[verts * (3 + 3)] = {
         0.0f,  0.5f,  0.0f,     1.0f, 0.0f, 1.0f,
         0.5f,  0.0f,   0.0f,     1.0f, 0.0f, 0.0f,
         -0.5f, 0.0f,   0.0f,     0.0f, 1.0f, 0.0f,
         0.0f,  -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
    };
    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    ModelTransform polygonTrans1 = {
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
    };
    
    ModelTransform polygonTrans2 = {
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
    };
    ModelTransform polygonTrans3 = {
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 1.f),
    };

#pragma region BUFFERS INITIALIZATION
    /* Vertex Buffer Object */
    /* Vertex Array Object */
    /* Element Buffer Object */
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    /* bind the Vertex Array Object first,
       then bind and set vertex buffer(s),
       and then configure vertex attributes(s).
    */ 
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts * (3+3), vertices, GL_STATIC_DRAW);
    // GL_STATIC_DRAW
    // GL_STREAM_DRAW (very rare)
    // GL_DYNAMIC_DRAW
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, indices, GL_STATIC_DRAW);

    // position attribute   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion

    Shader* polygonShader = new Shader("shaders/basic.vert", "shaders/basic.frag");

    // Для режима wireframe (только ребра)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    polygonTrans1.position.x = 0.5f;
    polygonTrans1.setUniformScale(0.5f);

    /* simple render loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process some keys
        processInput(window);

        polygonTrans1.rotation.z = glfwGetTime() * 60.0;
        polygonTrans1.position.x = 0.8f * cos(glfwGetTime());
        polygonTrans1.position.y = 0.8f * sin(glfwGetTime());
        polygonTrans1.setUniformScale(0.7);

        polygonTrans2.rotation.z = glfwGetTime() * 30.0;
        polygonTrans2.position.x = 0.8f * cos(glfwGetTime() + 3.14f);
        polygonTrans2.position.y = 0.8f * sin(glfwGetTime() + 3.14f);
        polygonTrans2.setUniformScale(1.2f);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        polygonShader->use();

        // 1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, polygonTrans1.position);
        model = glm::rotate(model, glm::radians(polygonTrans1.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans1.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans1.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, polygonTrans1.scale);

        polygonShader->setMatrix4f("model", model);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, polygonTrans2.position);
        model = glm::rotate(model, glm::radians(polygonTrans2.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans2.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans2.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, polygonTrans2.scale);

        polygonShader->setMatrix4f("model", model);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, polygonTrans3.position);
        model = glm::rotate(model, glm::radians(polygonTrans3.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans3.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans3.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, polygonTrans3.scale);

        polygonShader->setMatrix4f("model", model);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* see info about Double Buffer concept */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete polygonShader;

    /* As soon as we exit the render loop
       we would like to properly clean/delete
       all of GLFW's resources that were allocated. */
    glfwTerminate();

    return 0;
}