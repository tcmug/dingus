/////
// Component header properties
/////

struct component_t **children; // Component children.
struct window_t *window;       // Window specs.

int resized;
rectangle rect;          // Target rectangle.
struct texture_t *cache; // SDL Texture

update_callback update; // Update callback.
render_callback render; // Render callback.
click_callback click;   // Click callback.

// click
// doubleclick
// focus
// blur
// ...