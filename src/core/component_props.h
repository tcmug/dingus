
struct s_component **children; // Component children.
struct s_window *window;       // Window specs.
SDL_Rect rect;                 // Target rectangle.
int resized;                   // Area resized?
SDL_Texture *texture;          // SDL Texture

update_callback update; // Update callback.
render_callback render; // Render callback.
click_callback click;   // Click callback.

// click
// doubleclick
// focus
// blur
// ...