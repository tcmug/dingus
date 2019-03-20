/////
// TW_Component header properties
/////

struct component_t *parent; // TW_Component parent component.

struct component_t *kids;
struct component_t *prev;
struct component_t *next;

int resized;
TW_Rectangle rect;          // Target TW_Rectangle.
struct TW_Texture_t *cache; // Cache texture
struct TW_Shader_t *shader;
int rerender;

// mount_callback mount;
render_callback render; // Render callback.
click_callback click;   // Click callback.
int lua_click;

// click
// doubleclick
// focus
// blur
// ...