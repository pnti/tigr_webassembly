#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES2/gl2.h>
#include "tigr.h"

static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx;
static GLuint tex;
static GLuint prog;
static GLuint vbo;

static const char* vs =
    "precision mediump float;\n"
    "attribute vec2 p;\n"
    "varying vec2 uv;\n"
    "void main(){\n"
    "  uv = vec2(\n"
    "    (p.x + 1.0) * 0.5,\n"
    "    1.0 - (p.y + 1.0) * 0.5\n"
    "  );\n"
    "  gl_Position = vec4(p, 0.0, 1.0);\n"
    "}\n";

static const char* fs =
    "precision mediump float;"
    "varying vec2 uv;"
    "uniform sampler2D t;"
    "void main(){"
    "gl_FragColor = texture2D(t, uv);"
    "}";

static GLuint compile(GLenum type, const char* src)
{
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, 0);
    glCompileShader(s);
    return s;
}

void tigrPlatformInit(Tigr* bmp)
{
    EmscriptenWebGLContextAttributes a;
    emscripten_webgl_init_context_attributes(&a);
a.alpha = EM_TRUE;
a.premultipliedAlpha = EM_TRUE;
    a.majorVersion = 1;

    ctx = emscripten_webgl_create_context("#canvas", &a);
    emscripten_webgl_make_context_current(ctx);

    emscripten_set_canvas_element_size("#canvas", bmp->w, bmp->h);
    glViewport(0, 0, bmp->w, bmp->h);

    GLuint vsid = compile(GL_VERTEX_SHADER, vs);
    GLuint fsid = compile(GL_FRAGMENT_SHADER, fs);

    prog = glCreateProgram();
    glAttachShader(prog, vsid);
    glAttachShader(prog, fsid);
    glLinkProgram(prog);
    glUseProgram(prog);

GLint samp = glGetUniformLocation(prog, "t");
glUniform1i(samp, 0);


    static const float quad[] = {
        -1,-1, 1,-1, -1,1, 1,1
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    GLint loc = glGetAttribLocation(prog, "p");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, 0, 0, 0);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        bmp->w, bmp->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        0
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

glEnable(GL_BLEND);
glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

}

void tigrPlatformPresent(Tigr* bmp)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    glTexSubImage2D(
        GL_TEXTURE_2D, 0,
        0, 0,
        bmp->w, bmp->h,
        GL_RGBA, GL_UNSIGNED_BYTE,
        bmp->pix
    );

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glFlush();
}

static void loop(void* fn)
{
    ((void(*)(void))fn)();
}

void tigrSetMainLoop(void (*fn)(void))
{
    emscripten_set_main_loop_arg(loop, fn, 0, 1);
}

