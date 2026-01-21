This is a modified version of the tigr library for WebAssembly.

Compilation

emcc main.c tigr.c tigr_emscripten.c -DTIGR_HEADLESS -DTIGR_NO_OPENGL -s USE_WEBGL2=0 -s FULL_ES2=1 -s ALLOW_MEMORY_GROWTH=1 -o index.html

python3 -m http.server
