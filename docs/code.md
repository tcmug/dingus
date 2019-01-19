# Code

## Component

Look at [src/main.c](../src/main.c) to get a grip on how components are used. Use cases of components are provided there.

Few alignment related components as well as text renderers can be found in [src/components](../src/components).

In its simplest form a component is a struct which consists of [basic properties](../src/core/component_props.inc) and component specifies ones. Naturally properties can be any type of C variable.

Below is a short summary of basic props.

### .rect

The area the component occupies as SDL_Rect.

### .children

A list of components children. Initialized with a **LIST(...)** macro. Default is NULL.

### .update

Used for updating component, prepping textures etc.

### .render

Called every frame. Default is node_render_children.
Built in functions:

- #### node_render_children

  - Has no content.
  - Renders children.

- #### texture_render
  - Render texture.
  - Renders children.
  - Useful for caching.

## Examples

```c
PARENT(
    <other props>,
    .children = LIST(
        TEXT(
            .window = &props,
            .state = "Hello",
            .rect = {0, 0, 200, 200}
        ),
        TEXT(
            .window = &props,
            .state = "World",
            .rect = {200, 200, 200, 200}
        )
    )
);
```

## Custom components

To extend a component:

```c

typedef struct s_my_component {

#include "../core/component_props.inc"

  int my_value;

} my_component;

#define MY_COMPONENT_DEFAULTS .update = &my_update, .render = &my_render

#define MY_COMPONENT(...) ((component *)COMPONENT(my_component, MY_COMPONENT_DEFAULTS, __VA_ARGS__))

```

### Usage

```c
PARENT(
    <other props>,
    .children =
        LIST(
            MY_COMPONENT(.my_value = 1),
            MY_COMPONENT(.my_value = 2)
        )
    );
```
