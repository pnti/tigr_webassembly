# TIGR WebGL (Emscripten Backend)

This project provides a **WebGL backend for TIGR (Tiny Graphics Library)**, allowing TIGR applications to run in a web browser when compiled with **Emscripten**.

TIGR remains a **pure software renderer**: all drawing is performed on a CPU-side framebuffer.  
WebGL is used strictly as a **presentation layer**, displaying the framebuffer on an HTML canvas.

## Features

- **WebGL 1.0 backend** for browser execution
- **No changes to the TIGR public API**
- **Software rendering preserved**
- **Correct alpha blending** (premultiplied alpha)
- **Minimal, self-contained platform layer**

## What’s not included

- Input handling (keyboard, mouse, touch)
- Canvas resizing and DPI scaling

## Building

Compile the project using Emscripten:

```bash
emcc main.c tigr.c tigr_emscripten.c \
    -DTIGR_NO_OPENGL \
    -DTIGR_HEADLESS \
    -s USE_WEBGL2=0 \
    -s FULL_ES2=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -o index.html
