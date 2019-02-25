/////
// TW_Component header properties
/////

struct TW_Window *window;      // TW_Window specs.
struct component_t **children; // TW_Component children.
struct component_t *parent;    // TW_Component parent component.

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