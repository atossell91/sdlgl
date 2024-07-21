#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <SDL3/SDL.h>

#include "glad/glad.h"

std::string loadTextFile(const std::string& path);

void initOpenGL();
GLuint handleShaderStuff();
void bufferOpenGlData();

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow("Jaime", 1920, 1080, SDL_WINDOW_OPENGL);

    SDL_GLContext glCon = SDL_GL_CreateContext(win);

    initOpenGL();
    handleShaderStuff();

    SDL_GL_MakeCurrent(win, glCon);

    bufferOpenGlData();
    
    glViewport(0, 0, 1920, 1080);

    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_INT, NULL);

    SDL_GL_SwapWindow(win);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    SDL_Quit();
    return 0;
}

std::string loadTextFile(const std::string& path) {
    std::ifstream stream(path.c_str());
    std::stringstream sstream;
    sstream << stream.rdbuf();
    std::string content = sstream.str();
    if(content.length() < 1) {
        std::cout << "File '" << path << "' was empty, or doesn't exist" << std::endl;
    }
    return content;
}

void initOpenGL() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "GLAD init failire." << std::endl;
    }
}
 
GLuint handleShaderStuff() {

    std::string vsContentStr = loadTextFile("../vertex.glsl");
    std::string fsContentStr = loadTextFile("../fragment.glsl");
    const char * vsContent = vsContentStr.c_str();
    const char * fsContent = fsContentStr.c_str();

    GLuint vShader, fShader;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    char infoLog[512];

    glShaderSource(vShader, 1, &vsContent, NULL);
    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vShader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    glShaderSource(fShader, 1, &fsContent, NULL);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fShader, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    glUseProgram(shaderProgram);

    return shaderProgram;
}

void bufferOpenGlData() {
    float vertices[] = {
        -0.33f, -0.33, 0.0f,
        0.33f, -0.33f, 0.0f,
        0.33f, 0.33f, 0.0f,
        -0.33f, 0.33f, 0.0f
    };

    int indices[] = { 0, 1, 2, 3, 0 };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
