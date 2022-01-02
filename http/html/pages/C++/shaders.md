## Shaders in C++

A shader is a computer program that is used to do image processing such as special effects, color effects, lighting, and, well, shading. \
The position, brightness, contrast, hue, and other effects on all pixels, vertices, or textures used to produce the final image on the screen can be altered during runtime, using algorithms constructed in the shader program(s). \
These days, most shader programs are built to run directly on the Graphical Processing Unit (GPU). \
In this article, we are going to get acquainted with shaders and implement our own shader infrastructure for the example engine.

Shader programs are executed in parallel. \
This means, for example, that a shader might be executed once per pixel, with each of the executions running simultaneously on different threads on the GPU. \
The amount of simultaneous threads depends on the graphics card specific GPU, with modern cards sporting processors in the thousands. \
This all means that shader programs can be very performant and provide developers with lots of creative flexibility.
