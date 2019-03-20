#ifndef RESOURCES_H
#define RESOURCES_H

#include "shader.h"

typedef struct TW_Resource_t {
#include "resource_props.h"
} TW_Resource;

TW_Resource *TW_ResourceAdd(TW_Resource *root, const char *name,
                            TW_Resource *res);
TW_Resource *TW_ResourceFind(TW_Resource *root, const char *name);

#endif