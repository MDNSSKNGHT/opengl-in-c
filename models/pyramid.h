#ifndef __PYRAMID_H__
#define __PYRAMID_H__

#include "common.h"

const static struct vertex_data PYRAMID_MODEL_VERTICES[] = {
    {.position = {-0.5f, 0.0f, 0.5f},
     .normal = {0.0f, -1.0f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {0.0f, 0.0f}},
    {.position = {-0.5f, 0.0f, -0.5f},
     .normal = {0.0f, -1.0f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {0.0f, 5.0f}},
    {.position = {0.5f, 0.0f, -0.5f},
     .normal = {0.0f, -1.0f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {5.0f, 5.0f}},
    {.position = {0.5f, 0.0f, 0.5f},
     .normal = {0.0f, -1.0f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {5.0f, 0.0f}},

    {.position = {-0.5f, 0.0f, 0.5f},
     .normal = {-0.8f, 0.5f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {0.0f, 0.0f}},
    {.position = {-0.5f, 0.0f, -0.5f},
     .normal = {-0.8f, 0.5f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {5.0f, 0.0f}},
    {.position = {0.0f, 0.8f, 0.0f},
     .normal = {-0.8f, 0.5f, 0.0f},
     .color = {0.92f, 0.86f, 0.76f},
     .texture_uv = {2.5f, 5.0f}},

    {.position = {-0.5f, 0.0f, -0.5f},
     .normal = {0.0f, 0.5f, -0.8f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {5.0f, 0.0f}},
    {.position = {0.5f, 0.0f, -0.5f},
     .normal = {0.0f, 0.5f, -0.8f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {0.0f, 0.0f}},
    {.position = {0.0f, 0.8f, 0.0f},
     .normal = {0.0f, 0.5f, -0.8f},
     .color = {0.92f, 0.86f, 0.76f},
     .texture_uv = {2.5f, 5.0f}},

    {.position = {0.5f, 0.0f, -0.5f},
     .normal = {0.8f, 0.5f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {0.0f, 0.0f}},
    {.position = {0.5f, 0.0f, 0.5f},
     .normal = {0.8f, 0.5f, 0.0f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {5.0f, 0.0f}},
    {.position = {0.0f, 0.8f, 0.0f},
     .normal = {0.8f, 0.5f, 0.0f},
     .color = {0.92f, 0.86f, 0.76f},
     .texture_uv = {2.5f, 5.0f}},

    {.position = {0.5f, 0.0f, 0.5f},
     .normal = {0.0f, 0.5f, 0.8f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {5.0f, 0.0f}},
    {.position = {-0.5f, 0.0f, 0.5f},
     .normal = {0.0f, 0.5f, 0.8f},
     .color = {0.83f, 0.70f, 0.44f},
     .texture_uv = {0.0f, 0.0f}},
    {.position = {0.0f, 0.8f, 0.0f},
     .normal = {0.0f, 0.5f, 0.8f},
     .color = {0.92f, 0.86f, 0.76f},
     .texture_uv = {2.5f, 5.0f}}};

const unsigned int PYRAMID_MODEL_INDICES[] = {0, 1, 2, 0,  2,  3,  4,  6,  5,
                                              7, 9, 8, 10, 12, 11, 13, 15, 14};

#endif /* __PYRAMID_H__ */
