/////
// TW_Component header properties
/////

struct window_t *window;       // TW_Window specs.
struct component_t **children; // TW_Component children.
struct component_t *parent;    // TW_Component parent component.

struct component_t *kids;
struct component_t *prev;
struct component_t *next;

int resized;
TW_Rectangle rect;       // Target TW_Rectangle.
struct texture_t *cache; // SDL Texture
int rerender;

// mount_callback mount;
render_callback render; // Render callback.
click_callback click;   // Click callback.

// click
// doubleclick
// focus
// blur
// ...