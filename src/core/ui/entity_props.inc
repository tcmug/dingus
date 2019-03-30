/////
// TW_Entity header properties
/////

int id;
// struct TW_Component_t *components;

struct TW_Entity_t *parent; // TW_Entity parent component.
struct TW_Entity_t *kids;
struct TW_Entity_t *prev;
struct TW_Entity_t *next;

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