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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "Shader.h"

float cam_dist = 5.0f;

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
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        cam_dist += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        cam_dist -= 0.01f;
}

typedef unsigned char byte;

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

    glEnable(GL_DEPTH_TEST); // проверка глубины
#pragma endregion

    int box_width, box_height, channels;
    byte* data = stbi_load("images/box.png", &box_width, &box_height, &channels, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    const int verts = 8;
    GLfloat cube[verts * (3 + 3 + 2)] = {
         -1.0f,  1.0f,   -1.0f,    1.0f, 0.0f, 0.0f,    0.f, 1.f,
         1.0f,   1.0f,   -1.0f,    0.5f, 0.5f, 0.0f,    1.f, 1.f,
         1.0f,   1.0f,   1.0f,     0.0f, 1.0f, 0.0f,    1.f, 0.f,
         -1.0f,  1.0f,   1.0f,     0.0f, 0.5f, 0.5f,    0.f, 0.f,
         -1.0f,  -1.0f,  -1.0f,    0.0f, 0.0f, 1.0f,    1.f, 0.f,
         1.0f,   -1.0f,  -1.0f,    0.5f, 0.0f, 0.5f,    0.f, 0.f,
         1.0f,   -1.0f,  1.0f,     0.5f, 0.5f, 0.5f,    0.f, 1.f,
         -1.0f,  -1.0f,  1.0f,     1.0f, 1.0f, 1.0f,    1.f, 1.f,
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 1,
        1, 4, 5,
        0, 3, 7,
        0, 7, 4,
        1, 6, 2,
        1, 5, 6,
        2, 7, 3,
        2, 6, 7,
        4, 7, 5,
        5, 7, 6,
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

    unsigned int box_texture;
    glGenTextures(1, &box_texture);
    glBindTexture(GL_TEXTURE_2D, box_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts * (3+3+2), cube, GL_STATIC_DRAW);
    // GL_STATIC_DRAW
    // GL_STREAM_DRAW (very rare)
    // GL_DYNAMIC_DRAW
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, indices, GL_STATIC_DRAW);

    // position attribute   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

    /* simple render loop */
    while (!glfwWindowShouldClose(window))
    {
        // Process some keys
        processInput(window);

        polygonTrans1.rotation.z = glfwGetTime() * 60.0;
        polygonTrans1.rotation.x = glfwGetTime() * 45.0;
        polygonTrans1.position.x = 3.0f * cos(glfwGetTime());
        polygonTrans1.position.y = 3.0f * sin(glfwGetTime());
        polygonTrans1.setUniformScale(0.2);

        polygonTrans2.rotation.z = glfwGetTime() * 30.0;
        polygonTrans2.rotation.y = glfwGetTime() * 45.0;
        polygonTrans2.position.x = 3.0f * cos(glfwGetTime() + 3.14f);
        polygonTrans2.position.y = 3.0f * sin(glfwGetTime() + 3.14f);
        polygonTrans2.setUniformScale(0.2f);

        polygonTrans3.rotation.x = glfwGetTime() * 45.0;
        polygonTrans3.rotation.y = glfwGetTime() * 45.0;
        polygonTrans3.setUniformScale(0.2f);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw our first triangle
        polygonShader->use();

        glm::vec3 pos_vec = glm::vec3(cam_dist * cos(glfwGetTime() * 0.3), 0.f, cam_dist * sin(glfwGetTime() * 0.3));
        glm::vec3 target_vec = glm::vec3(0.f, 0.f, 0.f);
        glm::vec3 up_vec = glm::vec3(0.f, 1.f, 0.f);

        glm::mat4 camera = glm::lookAt(pos_vec, target_vec, up_vec);
        //glm::mat4 projection = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f);
        glm::mat4 projection = glm::perspective(45.f, 1.0f, 0.01f, 100.f);

        // 1
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, polygonTrans1.position);
        model = glm::rotate(model, glm::radians(polygonTrans1.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans1.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans1.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, polygonTrans1.scale);

        glm::mat4 pvm = projection * camera * model; // projection-view-matrix
        polygonShader->setMatrix4f("pvm", pvm);

        glBindTexture(GL_TEXTURE_2D, box_texture);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, polygonTrans2.position);
        model = glm::rotate(model, glm::radians(polygonTrans2.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans2.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans2.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, polygonTrans2.scale);

        pvm = projection * camera * model; // projection-view-matrix
        polygonShader->setMatrix4f("pvm", pvm);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, polygonTrans3.position);
        model = glm::rotate(model, glm::radians(polygonTrans3.rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans3.rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(polygonTrans3.rotation.z), glm::vec3(0.f, 0.f, 1.f));
        model = glm::scale(model, polygonTrans3.scale);

        pvm = projection * camera * model; // projection-view-matrix
        polygonShader->setMatrix4f("pvm", pvm);

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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