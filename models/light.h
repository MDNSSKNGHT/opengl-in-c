#ifndef __LIGHT_SOURCE_MODEL_H__
#define __LIGHT_SOURCE_MODEL_H__

#include "common.h"

const static struct vertex_data LIGHT_MODEL_VERTICES[] = {
    {.position = {-0.1f, -0.1f, 0.1f}}, {.position = {-0.1f, -0.1f, -0.1f}},
    {.position = {0.1f, -0.1f, -0.1f}}, {.position = {0.1f, -0.1f, 0.1f}},
    {.position = {-0.1f, 0.1f, 0.1f}},  {.position = {-0.1f, 0.1f, -0.1f}},
    {.position = {0.1f, 0.1f, -0.1f}},  {.position = {0.1f, 0.1f, 0.1f}}};

const static unsigned int LIGHT_MODEL_INDICES[] = {
    0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
    2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

#endif /* __LIGHT_SOURCE_MODEL_H__ */
