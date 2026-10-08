# OpenGL GLFW demo

A small C++ exercise in modern OpenGL (3.3 core profile): a quad drawn with a vertex buffer and an index buffer, GLSL shaders loaded from files, a `time` uniform that changes the colour, and a mouse click that moves one corner of the quad by rewriting the GPU buffer.

> Personal learning project.

## What it shows
- Window and OpenGL context with **GLFW**, extension loading with **GLEW**
- Vertex buffer (VBO) and index buffer (IBO), vertex attribute layout
- Loading, compiling and linking **GLSL shaders** from `Shaders/`, with the compile error log printed
- Passing data to the shader with a **uniform** (`time`)
- Updating a GPU buffer from the CPU on a mouse click (`glMapBuffer`)

## Build and run (Linux)
Requirements: CMake 3.10+, a C++17 compiler, GLFW 3, GLEW and OpenGL.

```bash
# Arch:           sudo pacman -S cmake glfw glew
# Debian/Ubuntu:  sudo apt install cmake libglfw3-dev libglew-dev
cmake -S . -B build
cmake --build build
cd build && ./app
```

The shaders are copied next to the executable at configure time. Left click inside the window to move one corner of the quad.
The code uses `u_int` from glibc, so it targets Linux.
