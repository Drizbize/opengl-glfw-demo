#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <fstream>
#include <sstream>

std::string LoadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

u_int CompileShader(u_int type, const std::string& source)
{
    u_int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader!\n" << message;
        
        delete[] message;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

u_int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    u_int program = glCreateProgram();
    u_int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    u_int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glewInit();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,

        -0.5f,  0.5f,
    };

    u_int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    u_int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positions, GL_DYNAMIC_DRAW);

    u_int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(u_int), indices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexPath = "Shaders/BasicVertex.glsl";
    std::string fragmentPath = "Shaders/BasicFragment.glsl";

    u_int shader = CreateShader(LoadShaderSource(vertexPath), LoadShaderSource(fragmentPath));
    glUseProgram(shader);

    int timeUniform = glGetUniformLocation(shader, "time");
    glUniform1f(timeUniform, 0.f);

    float time = 0.f;
    float prevTime = glfwGetTime();
    float dt = 0.f;

    int prevState = GLFW_RELEASE;
    while (!glfwWindowShouldClose(window)) {
        dt = prevTime - glfwGetTime();
        prevTime = glfwGetTime();

        time += dt;

        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS && prevState == GLFW_RELEASE) {
            double xPos, yPos;
            int width, height;

            glfwGetCursorPos(window, &xPos, &yPos);
            glfwGetWindowSize(window, &width, &height);
            
            float ndcX = (float)((xPos / width)  * 2.0 - 1.0);
            float ndcY = (float)(1.0 - (yPos / height) * 2.0);

            float* ptr = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            ptr[2] = ndcX;
            ptr[3] = ndcY;

            glUnmapBuffer(GL_ARRAY_BUFFER);
        }
        prevState = state;

        glClearColor(0.0f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(timeUniform, time);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    
    glDeleteProgram(shader);

    return 0;
}
