/*
 * gui.h
 *
 * Written by Hampus Fridholm
 *
 * Last updated: 2025-02-26
 *
 *
 * Events
 * mouse-down-left
 * mouse-down-right
 * mouse-up-left
 * mouse-up-right
 * key-down
 * key-up
 * resize
 * quit
 */

#ifndef GUI_H
#define GUI_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*
 * This is the exact same structure as SDL_Color,
 * but this way, the user doesn't need SDL.h
 */
typedef struct gui_color_t
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} gui_color_t;

/*
 *
 */
typedef struct gui_border_t
{
  int         thickness;
  int         opacity;
  gui_color_t color;
} gui_border_t;

/*
 *
 */
typedef struct gui_text_t
{
  char*       text;
  char*       font;
  gui_color_t color;
} gui_text_t;

/*
 *
 */
typedef struct gui_asset_t
{
  char* name;     // Name of asset
  char* filepath; // Path to asset
} gui_asset_t;

/*
 *
 */
typedef enum gui_pos_t
{
  GUI_POS_NONE,
  GUI_POS_LEFT,
  GUI_POS_RIGHT,
  GUI_POS_TOP,
  GUI_POS_BOTTOM,
  GUI_POS_CENTER
} gui_pos_t;

/*
 *
 */
typedef enum gui_size_type_t
{
  GUI_SIZE_NONE, // Size not specified
  GUI_SIZE_REL,  // Relative size to parent
  GUI_SIZE_ABS,  // Absolute size in pixels
  GUI_SIZE_MAX   // Max available size
} gui_size_type_t;

/*
 *
 */
typedef struct gui_size_t
{
  gui_size_type_t type;
  union
  {
    float rel;
    int   abs;
  } value;
} gui_size_t;

/*
 *
 */
typedef struct gui_rect_t
{
  gui_size_t width;
  gui_size_t height;

  float aspect_ratio; // Backup aspect ratio

  gui_size_t left;   // Margin to left
  gui_size_t right;  // Margin to right
  gui_size_t top;    // Margin on top
  gui_size_t bottom; // Margin on bottom

  gui_pos_t  xpos;   // Horizontal alignment
  gui_pos_t  ypos;   // Vertical   alignment
} gui_rect_t;

/*
 *
 */

typedef struct gui_t gui_t;

typedef struct gui_window_t gui_window_t;

typedef struct gui_menu_t   gui_menu_t;

typedef struct gui_assets_t gui_assets_t;

/*
 * Event
 */

/*
 *
 */
typedef enum gui_event_handler_type_t
{
  GUI_EVENT_HANDLER_GUI,
  GUI_EVENT_HANDLER_MOUSE,  // Get position of mouse
  GUI_EVENT_HANDLER_KEY,    // Get pressed key
  GUI_EVENT_HANDLER_RESIZE, // Get new size of screen
  GUI_EVENT_HANDLER_WINDOW  // Get window
} gui_event_handler_type_t;

/*
 *
 */
typedef struct gui_event_handler_t
{
  gui_event_handler_type_t type;
  union
  {
    void* (*gui)   (gui_t* gui);
    void* (*mouse) (gui_t* gui, int x, int y);
    void* (*key)   (gui_t* gui, int key);
    void* (*resize)(gui_t* gui, int width, int height);
    void* (*window)(gui_t* gui, gui_window_t* window);
  } handler;
} gui_event_handler_t;

/*
 * GUI
 */

extern int    gui_init(void);

extern void   gui_quit(void);

extern gui_t* gui_create(int width, int height, char* title);

extern void   gui_start(gui_t* gui, int fps);

extern void   gui_stop(gui_t* gui);

extern void   gui_destroy(gui_t** gui);

extern int    gui_clear(gui_t* gui);

extern int    gui_texture_render(gui_t* gui, char* menu_name, char** window_names, char* texture_name, gui_rect_t rect);

extern int    gui_chunk_play(gui_t* gui, char* name);

extern int    gui_text_render(gui_t* gui, char* menu_name, char** window_names, gui_text_t text, gui_rect_t rect);

extern int    gui_event_create(gui_t* gui, char* name, gui_event_handler_t handler);

extern void   gui_user_event_trigger(gui_t* gui, char* name);

/*
 * Menu
 */

extern void          gui_active_menu_set(gui_t* gui, char* name);

extern gui_menu_t*   gui_menu_create(gui_t* gui, char* name);

extern int           gui_menu_destroy(gui_t* gui, const char* name);

extern gui_window_t* gui_menu_window_create(gui_menu_t* menu, char* name, gui_rect_t gui_rect, gui_border_t border);

extern int           gui_menu_window_destroy(gui_menu_t* menu, char* name);

/*
 * Window
 */

extern bool          gui_window_name_check(gui_window_t* window, char* name);

extern bool          gui_curr_window_name_check(gui_t* gui, char* name);

extern int           gui_curr_window_border_render(gui_t* gui, gui_border_t border);

extern gui_window_t* gui_window_child_create(gui_window_t* window, char* name, gui_rect_t gui_rect);

extern int           gui_window_child_destroy(gui_window_t* window, char* name);

/*
 * Assets
 */

extern int gui_textures_load(gui_t* gui, gui_asset_t* assets, size_t count);

extern int gui_fonts_load(gui_t* gui, gui_asset_t* assets, size_t count);

extern int gui_chunks_load(gui_t* gui, gui_asset_t* assets, size_t count);

#endif // GUI_H

/*
 * This header library file uses _IMPLEMENT guards
 *
 * If GUI_IMPLEMENT is defined, the definitions will be included
 */

#ifdef GUI_IMPLEMENT

#include <stdbool.h>
#include <errno.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

/*
 * Initialize SDL drivers
 */
int gui_init(void)
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());

    return 1;
  }

  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
  {
    fprintf(stderr, "IMG_Init: %s\n", IMG_GetError());

    return 2;
  }

  if (TTF_Init() == -1)
  {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());

    return 3;
  }

  if (Mix_Init(0) != 0)
  {
    fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());

    return 4;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
  {
    fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());

    return 5;
  }

  return 0;
}

/*
 * Quit SDL drivers
 */
void gui_quit(void)
{
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

/*
 * Create SDL Window
 */
static inline SDL_Window* sdl_window_create(int width, int height, char* title)
{
  SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

  if (!window)
  {
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());

    return NULL;
  }

  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

  return window;
}

/*
 * Destroy SDL Window
 */
static inline void sdl_window_destroy(SDL_Window** window)
{
  if (!window || !(*window)) return;

  SDL_DestroyWindow(*window);

  *window = NULL;
}

/*
 * Create SDL Renderer
 */
static inline SDL_Renderer* sdl_renderer_create(SDL_Window* window)
{
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

  if (!renderer)
  {
    fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());

    return NULL;
  }

  return renderer;
}

/*
 * Destroy SDL Renderer
 */
static inline void sdl_renderer_destroy(SDL_Renderer** renderer)
{
  if (!renderer || !(*renderer)) return;

  SDL_DestroyRenderer(*renderer);

  *renderer = NULL;
}

/*
 * Destroy SDL Texture
 */
static inline void sdl_texture_destroy(SDL_Texture** texture)
{
  if (!texture || !(*texture)) return;

  SDL_DestroyTexture(*texture);

  *texture = NULL;
}

/*
 * Create SDL Texture
 */
static inline SDL_Texture* sdl_texture_create(SDL_Renderer* renderer, int width, int height)
{
  SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

  if (!texture)
  {
    fprintf(stderr, "SDL_CreateTexture: %s\n", SDL_GetError());

    return NULL;
  }

  if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) != 0)
  {
    fprintf(stderr, "SDL_SetTextureBlendMode: %s", SDL_GetError());

    SDL_DestroyTexture(texture);

    return NULL;
  }

  return texture;
}

/*
 * Load SDL Texture
 */
static inline SDL_Texture* sdl_texture_load(SDL_Renderer* renderer, const char* filepath)
{
  SDL_Surface* surface = IMG_Load(filepath);
  
  if (!surface)
  {
    fprintf(stderr, "IMG_Load: %s\n", IMG_GetError());

    return NULL;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  if (!texture)
  {
    fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n", SDL_GetError());

    return NULL;
  }

  return texture;
}

/*
 * Load TTF Font
 */
static inline TTF_Font* ttf_font_load(const char* filepath, int size)
{
  TTF_Font* font = TTF_OpenFont(filepath, size);

  if (!font)
  {
    fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());

    return NULL;
  }

  return font;
}

/*
 * Destroy (close) TTF Font
 */
static inline void ttf_font_destroy(TTF_Font** font)
{
  if (!font || !(*font)) return;

  TTF_CloseFont(*font);

  *font = NULL;
}

/*
 * Set target texture of renderer
 */
static inline int sdl_target_set(SDL_Renderer* renderer, SDL_Texture* target)
{
  int status = SDL_SetRenderTarget(renderer, target);

  if (status != 0)
  {
    fprintf(stderr, "SDL_SetRenderTarget: %s\n", SDL_GetError());
  }

  return status;
}

/*
 * Clear the supplied texture
 */
static inline int sdl_target_clear(SDL_Renderer* renderer, SDL_Texture* target)
{
  SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

  // 1. Temporarly set the new target
  if (sdl_target_set(renderer, target) != 0)
  {
    return 1;
  }

  if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
  {
    return 2;
  }

  SDL_RenderClear(renderer);

  // 2. Change back to the old target
  if (sdl_target_set(renderer, old_target) != 0)
  {
    return 3;
  }

  return 0;
}

/*
 *
 */
static inline int sdl_rect_render(SDL_Renderer* renderer, SDL_Rect* rect)
{
  if (SDL_RenderFillRect(renderer, rect) != 0)
  {
    fprintf(stderr, "SDL_RenderFillRect: %s\n", SDL_GetError());

    return 1;
  }

  return 0;
}

/*
 * Convert gui_color_t to correct SDL_Color
 */
static inline SDL_Color sdl_color_create(gui_color_t color)
{
  return (SDL_Color) {
    .r = color.r,
    .g = color.g,
    .b = color.b,
    .a = color.a
  };
}

/*
 * Render a border around rect
 */
static inline int sdl_border_render(SDL_Renderer* renderer, gui_border_t border, SDL_Rect rect)
{
  SDL_Color color = sdl_color_create(border.color);

  if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, border.opacity) != 0)
  {
    return 1;
  }

  // Draw top border
  SDL_Rect top =
  {
    rect.x - border.thickness,
    rect.y - border.thickness,
    rect.w + border.thickness * 2,
    border.thickness
  };

  if (sdl_rect_render(renderer, &top) != 0)
  {
    return 2;
  }

  // Draw bottom border
  SDL_Rect bottom =
  {
    rect.x - border.thickness,
    rect.y + rect.h,
    rect.w + border.thickness * 2,
    border.thickness
  };
  
  if (sdl_rect_render(renderer, &bottom) != 0)
  {
    return 3;
  }

  // Draw left border
  SDL_Rect left =
  {
    rect.x - border.thickness,
    rect.y - border.thickness,
    border.thickness,
    rect.h + border.thickness * 2
  };
  
  if (sdl_rect_render(renderer, &left) != 0)
  {
    return 4;
  }

  // Draw right border
  SDL_Rect right =
  {
    rect.x + rect.w,
    rect.y - border.thickness,
    border.thickness,
    rect.h + border.thickness * 2
  };
  
  if (sdl_rect_render(renderer, &right) != 0)
  {
    return 5;
  }

  return 0;
}

/*
 * Render SDL Texture
 */
static inline int sdl_texture_render(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* rect)
{
  if (!texture) return 0;

  int status = SDL_RenderCopy(renderer, texture, NULL, rect);

  if (status != 0)
  {
    fprintf(stderr, "SDL_RenderCopy: %s\n", SDL_GetError());
  }

  return status;
}

/*
 * Render a border around rect to target texture
 */
static inline int sdl_target_border_render(SDL_Renderer* renderer, SDL_Texture* target, gui_border_t border, SDL_Rect rect)
{
  SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

  // 1. Temporarly set the new target
  if (sdl_target_set(renderer, target) != 0)
  {
    return 1;
  }

  int status = sdl_border_render(renderer, border, rect);

  // 2. Change back to the old target
  if (sdl_target_set(renderer, old_target) != 0)
  {
    return 2;
  }

  return (status == 0) ? 0 : 3;
}

/*
 * Render texture to target texture
 */
static inline int sdl_target_texture_render(SDL_Renderer* renderer, SDL_Texture* target, SDL_Texture* texture, SDL_Rect* rect)
{
  SDL_Texture* old_target = SDL_GetRenderTarget(renderer);

  // 1. Temporarly set the new target
  if (sdl_target_set(renderer, target) != 0)
  {
    return 1;
  }

  int status = sdl_texture_render(renderer, texture, rect);

  // 2. Change back to the old target
  if (sdl_target_set(renderer, old_target) != 0)
  {
    return 2;
  }

  return (status == 0) ? 0 : 3;
}

/*
 * Resize SDL Texture
 */
static inline int sdl_texture_resize(SDL_Texture** texture, SDL_Renderer* renderer, int width, int height)
{
  SDL_Texture* new_texture = sdl_texture_create(renderer, width, height);

  if (!new_texture)
  {
    return 1;
  }

  sdl_texture_destroy(texture);

  *texture = new_texture;

  return 0;
}

/*
 * Load chunk
 */
static inline Mix_Chunk* mix_chunk_load(const char* filepath)
{
  Mix_Chunk* chunk = Mix_LoadWAV(filepath);

  if (!chunk)
  {
    fprintf(stderr, "Mix_LoadWAV: %s\n", Mix_GetError());

    return NULL;
  }

  return chunk;
}

/*
 * Play chunk
 */
static inline int mix_chunk_play(Mix_Chunk* chunk)
{
  int status = Mix_PlayChannel(-1, chunk, 0);

  if (status == -1)
  {
    fprintf(stderr, "Mix_PlayChannel: %s\n", Mix_GetError());
  }

  return status;
}

/*
 * Free chunk
 */
static inline void mix_chunk_destroy(Mix_Chunk** chunk)
{
  if (!chunk || !(*chunk)) return;

  Mix_FreeChunk(*chunk);

  *chunk = NULL;
}

/*
 *
 */
typedef struct gui_texture_t
{
  char*        name;
  SDL_Texture* texture;
} gui_texture_t;

/*
 *
 */
typedef struct gui_font_t
{
  char*     name;
  TTF_Font* font;
} gui_font_t;

/*
 *
 */
typedef struct gui_chunk_t
{
  char*      name;
  Mix_Chunk* chunk;
} gui_chunk_t;

/*
 *
 */
typedef struct gui_music_t
{
  char*      name;
  Mix_Music* music;
} gui_music_t;

/*
 *
 */
typedef struct gui_assets_t
{
  gui_texture_t** textures;
  size_t          texture_count;

  gui_font_t**    fonts;
  size_t          font_count;

  gui_chunk_t**   chunks;
  size_t          chunk_count;

  gui_music_t**   musics;
  size_t          music_count;
} gui_assets_t;

/*
 *
 */
typedef struct gui_window_t
{
  char*          name;
  gui_rect_t     gui_rect;
  SDL_Rect       sdl_rect;
  SDL_Texture*   texture;
  gui_border_t   border;
  gui_window_t** children;
  size_t         child_count;
  bool           is_child;
  union
  {
    gui_window_t* window; // is_child: true
    gui_menu_t*   menu;   // is_child: false
  } parent;
  gui_t*         gui;
} gui_window_t;

/*
 *
 */
typedef struct gui_menu_t
{
  char*          name;
  SDL_Texture*   texture;
  gui_window_t** windows;
  size_t         window_count;
  gui_t*         gui;
} gui_menu_t;

/*
 *
 */
typedef struct gui_event_t
{
  char*                name;
  gui_event_handler_t* handlers;
  size_t               handler_count;
} gui_event_t;

/*
 *
 */
typedef struct gui_t
{
  SDL_Window*   window;
  SDL_Renderer* renderer;
  char*         title;
  int           width;
  int           height;
  gui_menu_t**  menus;
  size_t        menu_count;
  char*         menu_name;
  gui_assets_t* assets;
  gui_event_t** events;
  size_t        event_count;
  bool          is_running;
  gui_window_t* last_window;
  gui_window_t* curr_window;
} gui_t;

/*
 * Assets
 */

/*
 * Create assets struct
 */
static inline gui_assets_t* gui_assets_create(void)
{
  gui_assets_t* assets = malloc(sizeof(gui_assets_t));

  if (!assets)
  {
    return NULL;
  }

  memset(assets, 0, sizeof(gui_assets_t));

  return assets;
}

/*
 * Destroy gui texture
 */
static inline void gui_texture_destroy(gui_texture_t** texture)
{
  if (!texture || !(*texture)) return;

  sdl_texture_destroy(&(*texture)->texture);

  free(*texture);

  *texture = NULL;
}

/*
 * Destroy gui font
 */
static inline void gui_font_destroy(gui_font_t** font)
{
  if (!font || !(*font)) return;

  ttf_font_destroy(&(*font)->font);

  free(*font);

  *font = NULL;
}

/*
 * Destroy gui chunk
 */
static inline void gui_chunk_destroy(gui_chunk_t** chunk)
{
  if (!chunk || !(*chunk)) return;

  mix_chunk_destroy(&(*chunk)->chunk);

  free(*chunk);

  *chunk = NULL;
}

/*
 * Destroy gui music
 */
static inline void gui_music_destroy(gui_music_t** music)
{
  if (!music || !(*music)) return;

  // mix_music_destroy(&(*music)->music);

  free(*music);

  *music = NULL;
}

/*
 * Destroy assets struct
 */
static inline void gui_assets_destroy(gui_assets_t** assets)
{
  if (!assets || !(*assets)) return;

  for (size_t index = 0; index < (*assets)->texture_count; index++)
  {
    gui_texture_destroy(&(*assets)->textures[index]);
  }

  free((*assets)->textures);


  for (size_t index = 0; index < (*assets)->font_count; index++)
  {
    gui_font_destroy(&(*assets)->fonts[index]);
  }

  free((*assets)->fonts);


  for (size_t index = 0; index < (*assets)->chunk_count; index++)
  {
    gui_chunk_destroy(&(*assets)->chunks[index]);
  }

  free((*assets)->chunks);


  for (size_t index = 0; index < (*assets)->music_count; index++)
  {
    gui_music_destroy(&(*assets)->musics[index]);
  }

  free((*assets)->musics);


  free(*assets);

  *assets = NULL;
}

/*
 * Create gui_texture (This is an internal function)
 */
static inline gui_texture_t* gui_texture_create(char* name, SDL_Texture* texture)
{
  gui_texture_t* gui_texture = malloc(sizeof(gui_texture_t));

  if (!gui_texture)
  {
    return NULL;
  }

  gui_texture->name    = name;
  gui_texture->texture = texture;

  return gui_texture;
}

/*
 * Load texture and add it to gui assets
 */
int gui_texture_load(gui_t* gui, gui_asset_t asset)
{
  char* name     = asset.name;
  char* filepath = asset.filepath;

  if (!gui || !name || !filepath)
  {
    return 1;
  }

  SDL_Renderer* renderer = gui->renderer;

  SDL_Texture* texture = sdl_texture_load(renderer, filepath);

  if (!texture)
  {
    return 2;
  }

  gui_assets_t* assets = gui->assets;

  if (!assets)
  {
    sdl_texture_destroy(&texture);

    return 3;
  }

  gui_texture_t* gui_texture = gui_texture_create(name, texture);

  if (!gui_texture)
  {
    sdl_texture_destroy(&texture);

    return 4;
  }

  gui_texture_t** temp_textures = realloc(assets->textures, sizeof(gui_texture_t*) * (assets->texture_count + 1));

  if (!temp_textures)
  {
    sdl_texture_destroy(&texture);

    free(gui_texture);

    return 4;
  }

  assets->textures = temp_textures;

  assets->textures[assets->texture_count++] = gui_texture;

  return 0;
}

/*
 * Load textures and add them to gui assets
 */
int gui_textures_load(gui_t* gui, gui_asset_t* assets, size_t count)
{
  if (!gui || !assets)
  {
    return 1;
  }

  for (size_t index = 0; index < count; index++)
  {
    if (gui_texture_load(gui, assets[index]) != 0)
    {
      return 2;
    }
  }

  return 0;
}

/*
 * Create gui_chunk (This is an internal function)
 */
static inline gui_chunk_t* gui_chunk_create(char* name, Mix_Chunk* chunk)
{
  gui_chunk_t* gui_chunk = malloc(sizeof(gui_chunk_t));

  if (!gui_chunk)
  {
    return NULL;
  }

  gui_chunk->name  = name;
  gui_chunk->chunk = chunk;

  return gui_chunk;
}

/*
 * Load chunk and add it to gui assets
 */
int gui_chunk_load(gui_t* gui, gui_asset_t asset)
{
  char* name     = asset.name;
  char* filepath = asset.filepath;

  if (!gui || !name || !filepath)
  {
    return 1;
  }

  Mix_Chunk* chunk = mix_chunk_load(filepath);

  if (!chunk)
  {
    return 2;
  }

  gui_assets_t* assets = gui->assets;

  if (!assets)
  {
    mix_chunk_destroy(&chunk);

    return 3;
  }

  gui_chunk_t* gui_chunk = gui_chunk_create(name, chunk);

  if (!gui_chunk)
  {
    mix_chunk_destroy(&chunk);

    return 4;
  }

  gui_chunk_t** temp_chunks = realloc(assets->chunks, sizeof(gui_chunk_t*) * (assets->chunk_count + 1));

  if (!temp_chunks)
  {
    mix_chunk_destroy(&chunk);

    free(gui_chunk);

    return 4;
  }

  assets->chunks = temp_chunks;

  assets->chunks[assets->chunk_count++] = gui_chunk;

  return 0;
}

/*
 * Load chunks and add them to gui assets
 */
int gui_chunks_load(gui_t* gui, gui_asset_t* assets, size_t count)
{
  if (!gui || !assets)
  {
    return 1;
  }

  for (size_t index = 0; index < count; index++)
  {
    if (gui_chunk_load(gui, assets[index]) != 0)
    {
      return 2;
    }
  }

  return 0;
}

/*
 * Create gui_font (This is an internal function)
 */
static inline gui_font_t* gui_font_create(char* name, TTF_Font* font)
{
  gui_font_t* gui_font = malloc(sizeof(gui_font_t));

  if (!gui_font)
  {
    return NULL;
  }

  gui_font->name = name;
  gui_font->font = font;

  return gui_font;
}

/*
 * Load font and add it to gui assets
 */
int gui_font_load(gui_t* gui, gui_asset_t asset)
{
  char* name     = asset.name;
  char* filepath = asset.filepath;

  if (!gui || !name || !filepath)
  {
    return 1;
  }

  TTF_Font* font = ttf_font_load(filepath, 24);

  if (!font)
  {
    return 2;
  }

  gui_assets_t* assets = gui->assets;

  if (!assets)
  {
    ttf_font_destroy(&font);

    return 3;
  }

  gui_font_t* gui_font = gui_font_create(name, font);

  if (!gui_font)
  {
    ttf_font_destroy(&font);

    return 4;
  }

  gui_font_t** temp_fonts = realloc(assets->fonts, sizeof(gui_font_t*) * (assets->font_count + 1));

  if (!temp_fonts)
  {
    ttf_font_destroy(&font);

    free(gui_font);

    return 4;
  }

  assets->fonts = temp_fonts;

  assets->fonts[assets->font_count++] = gui_font;

  return 0;
}

/*
 * Load fonts and add them to gui assets
 */
int gui_fonts_load(gui_t* gui, gui_asset_t* assets, size_t count)
{
  if (!gui || !assets)
  {
    return 1;
  }

  for (size_t index = 0; index < count; index++)
  {
    if (gui_font_load(gui, assets[index]) != 0)
    {
      return 2;
    }
  }

  return 0;
}

/*
 * Get loaded texture by name
 */
static inline gui_texture_t* gui_texture_get(gui_t* gui, char* name)
{
  gui_assets_t* assets = gui->assets;

  for (size_t index = 0; index < assets->texture_count; index++)
  {
    gui_texture_t* gui_texture = assets->textures[index];

    if (strcmp(gui_texture->name, name) == 0)
    {
      return gui_texture;
    }
  }

  return NULL;
}

/*
 * Get loaded font by name
 */
static inline gui_font_t* gui_font_get(gui_t* gui, char* name)
{
  gui_assets_t* assets = gui->assets;

  for (size_t index = 0; index < assets->font_count; index++)
  {
    gui_font_t* gui_font = assets->fonts[index];

    if (strcmp(gui_font->name, name) == 0)
    {
      return gui_font;
    }
  }

  return NULL;
}

/*
 * Get loaded chunk by name
 */
static inline gui_chunk_t* gui_chunk_get(gui_t* gui, char* name)
{
  gui_assets_t* assets = gui->assets;

  for (size_t index = 0; index < assets->chunk_count; index++)
  {
    gui_chunk_t* gui_chunk = assets->chunks[index];

    if (strcmp(gui_chunk->name, name) == 0)
    {
      return gui_chunk;
    }
  }

  return NULL;
}

/*
 * Chunk
 */

/*
 * Play loaded chunk
 */
int gui_chunk_play(gui_t* gui, char* name)
{
  if (!gui || !name)
  {
    return 1;
  }

  gui_chunk_t* chunk = gui_chunk_get(gui, name);

  if (!chunk)
  {
    return 2;
  }

  if (mix_chunk_play(chunk->chunk) != 0)
  {
    return 3;
  }

  return 0;
}

/*
 * Font
 */

/*
 * Get texture of text
 *
 * Maybe: replace with gui_text_t text 
 */
static inline SDL_Texture* sdl_text_texture_create(SDL_Renderer* renderer, const char* text, TTF_Font* font, SDL_Color color)
{
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);

  if (!surface)
  {
    return NULL;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  if (!texture)
  {
    fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n", SDL_GetError());

    return NULL;
  }

  return texture;
}

/*
 * Get width and height of text
 */
static inline int sdl_text_w_and_h_get(int* w, int* h, const char* text, TTF_Font* font)
{
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, (SDL_Color) { 0 });

  if (!surface)
  {
    fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());

    return 1;
  }

  *w = surface->w;
  *h = surface->h;

  SDL_FreeSurface(surface);

  return 0;
}

/*
 * Get width and height of texture
 */
static inline int gui_texture_w_and_h_get(int* w, int* h, SDL_Texture* texture)
{
  if (SDL_QueryTexture(texture, NULL, NULL, w, h) != 0)
  {
    fprintf(stderr, "SDL_QueryTexture: %s\n", SDL_GetError());

    return 1;
  }

  return 0;
}

/*
 * Window
 */

/*
 * Get the absolute pixel size from gui_size
 * in case of relative size, parent_size is used in relation
 */
static inline int gui_size_abs_get(int parent_size, gui_size_t gui_size)
{
  switch (gui_size.type)
  {
    case GUI_SIZE_REL:
      return parent_size * gui_size.value.rel;

    case GUI_SIZE_ABS:
      return gui_size.value.abs;

    case GUI_SIZE_MAX:
      return parent_size;

    default: case GUI_SIZE_NONE:
      return 0;
  }
}

/*
 *
 */
static inline int sdl_rect_x_get(gui_pos_t pos, int left_width, int right_width, int parent_width, int w)
{
  switch (pos)
  {
    case GUI_POS_LEFT:
      return left_width;

    case GUI_POS_RIGHT:
      return parent_width - w - right_width;

    default: case GUI_POS_CENTER:
      return (float) (parent_width - w) / 2.f;
  }
}

/*
 *
 */
static inline int sdl_rect_y_get(gui_pos_t pos, int top_height, int bottom_height, int parent_height, int h)
{
  switch (pos)
  {
    case GUI_POS_TOP:
      return top_height;

    case GUI_POS_BOTTOM:
      return parent_height - h - bottom_height;

    default: case GUI_POS_CENTER:
      return (float) (parent_height - h) / 2.f;
  }
}

/*
 *
 */
static inline int sdl_rect_w_get(int left_width, int right_width, int parent_width, gui_size_t width)
{
  int abs_width = gui_size_abs_get(parent_width, width);

  return MIN(parent_width - left_width - right_width, abs_width);
}

/*
 *
 */
static inline int sdl_rect_h_get(int top_height, int bottom_height, int parent_height, gui_size_t height)
{
  int abs_height = gui_size_abs_get(parent_height, height);

  return  MIN(parent_height - top_height - bottom_height, abs_height);
}

/*
 * Create SDL Rect from gui_rect,
 * with the help of parent width and height and aspect ratio
 */
static inline SDL_Rect sdl_rect_create(gui_rect_t gui_rect, int parent_width, int parent_height)
{
  int left_width    = gui_size_abs_get(parent_width,  gui_rect.left);
  int right_width   = gui_size_abs_get(parent_width,  gui_rect.right);
  int top_height    = gui_size_abs_get(parent_height, gui_rect.top);
  int bottom_height = gui_size_abs_get(parent_height, gui_rect.bottom);

  int w = sdl_rect_w_get(left_width, right_width, parent_width, gui_rect.width);

  int h = sdl_rect_h_get(top_height, bottom_height, parent_height, gui_rect.height);

  if (gui_rect.aspect_ratio)
  {
    if (gui_rect.width.type == GUI_SIZE_NONE)
    {
      w = (float) h * gui_rect.aspect_ratio;
    }
    else if (gui_rect.height.type == GUI_SIZE_NONE)
    {
      h = (float) w / gui_rect.aspect_ratio;
    }
  }

  int x = sdl_rect_x_get(gui_rect.xpos, left_width, right_width, parent_width, w);

  int y = sdl_rect_y_get(gui_rect.ypos, top_height, bottom_height, parent_height, h);

  return (SDL_Rect) {x, y, w, h};
}

/*
 * Only destroy window (This is an internal function)
 */
static inline void _gui_window_destroy(gui_window_t** window)
{
  if (!window || !(*window)) return;

  for (size_t index = 0; index < (*window)->child_count; index++)
  {
    _gui_window_destroy(&(*window)->children[index]);
  }

  free((*window)->children);

  sdl_texture_destroy(&(*window)->texture);

  free(*window);

  *window = NULL;
}

/*
 * Get menu window by name
 */
static inline gui_window_t* gui_menu_window_get(gui_menu_t* menu, const char* name)
{
  if (!menu || !name)
  {
    return NULL;
  }

  for (ssize_t index = 0; index < menu->window_count; index++)
  {
    gui_window_t* window = menu->windows[index];

    if (window && strcmp(window->name, name) == 0)
    {
      return window;
    }
  }

  return NULL;
}

/*
 * Get index of menu window by name
 */
static inline ssize_t gui_menu_window_index_get(gui_menu_t* menu, const char* name)
{
  if (!menu || !name)
  {
    return -1;
  }

  for (ssize_t index = 0; index < menu->window_count; index++)
  {
    gui_window_t* window = menu->windows[index];

    if (window && strcmp(window->name, name) == 0)
    {
      return index;
    }
  }

  return -1;
}

/*
 * Get index of window child by name
 */
static inline ssize_t gui_window_child_index_get(gui_window_t* window, const char* name)
{
  if (!window || !name)
  {
    return -1;
  }

  for (ssize_t index = 0; index < window->child_count; index++)
  {
    gui_window_t* child = window->children[index];

    if (child && strcmp(child->name, name) == 0)
    {
      return index;
    }
  }

  return -1;
}

/*
 * Get window child by name
 */
static inline gui_window_t* gui_window_child_get(gui_window_t* window, const char* name)
{
  if (!window || !name)
  {
    return NULL;
  }

  for (ssize_t index = 0; index < window->child_count; index++)
  {
    gui_window_t* child = window->children[index];

    if (child && strcmp(child->name, name) == 0)
    {
      return child;
    }
  }

  return NULL;
}

/*
 * Remove child from window and destroy it
 */
int gui_window_child_destroy(gui_window_t* window, char* name)
{
  if (!window || !name)
  {
    return 1;
  }

  ssize_t child_index = gui_window_child_index_get(window, name);

  if (child_index == -1)
  {
    return 2;
  }

  _gui_window_destroy(&window->children[child_index]);


  for (size_t index = child_index; index < (window->child_count - 1); index++)
  {
    window->children[index] = window->children[index + 1];
  }


  gui_window_t** temp_children = realloc(window->children, sizeof(gui_window_t*) * (window->child_count - 1));

  if (!temp_children)
  {
    // Here, the child is destroyed, but the children array isn't shrunk
    return 3;
  }

  window->children = temp_children;

  return 0;
}

/*
 * Render loaded texture (name) to window texture
 */
int gui_window_texture_render(gui_window_t* window, char* name, gui_rect_t gui_rect)
{
  if (!window || !name)
  {
    return 1;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  gui_texture_t* gui_texture = gui_texture_get(gui, name);
  
  if (!gui_texture)
  {
    return 4;
  }

  SDL_Rect sdl_rect = sdl_rect_create(gui_rect, window->sdl_rect.w, window->sdl_rect.h);

  if (sdl_target_texture_render(renderer, window->texture, gui_texture->texture, &sdl_rect) != 0)
  {
    return 5;
  }

  return 0;
}

/*
 *
 */
static inline int gui_window_text_render(gui_window_t* window, gui_text_t text, gui_rect_t gui_rect)
{
  if (!window || !text.text || !text.font)
  {
    return 1;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  gui_font_t* gui_font = gui_font_get(gui, text.font);
  
  if (!gui_font)
  {
    return 4;
  }

  SDL_Color sdl_color = sdl_color_create(text.color);

  SDL_Texture* texture = sdl_text_texture_create(renderer, text.text, gui_font->font, sdl_color);

  if (!texture)
  {
    return 5;
  }

  int textw;
  int texth;

  if (sdl_text_w_and_h_get(&textw, &texth, text.text, gui_font->font) != 0)
  {
    fprintf(stderr, "sdl_text_w_and_h_get: %s\n", strerror(errno));

    sdl_texture_destroy(&texture);

    return 6;
  }

  gui_rect.aspect_ratio = (float) textw / (float) texth;

  SDL_Rect sdl_rect = sdl_rect_create(gui_rect, window->sdl_rect.w, window->sdl_rect.h);
  
  if (sdl_target_texture_render(renderer, window->texture, texture, &sdl_rect) != 0)
  {
    sdl_texture_destroy(&texture);

    return 5;
  }

  sdl_texture_destroy(&texture);

  return 0;
}

/*
 *
 */
static inline int gui_menu_text_render(gui_menu_t* menu, gui_text_t text, gui_rect_t gui_rect)
{
  if (!menu || !text.text || !text.font)
  {
    return 1;
  }

  gui_t* gui = menu->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  gui_font_t* gui_font = gui_font_get(gui, text.font);
  
  if (!gui_font)
  {
    return 4;
  }

  SDL_Color sdl_color = sdl_color_create(text.color);

  SDL_Texture* texture = sdl_text_texture_create(renderer, text.text, gui_font->font, sdl_color);

  if (!texture)
  {
    return 5;
  }

  int textw;
  int texth;

  if (sdl_text_w_and_h_get(&textw, &texth, text.text, gui_font->font) != 0)
  {
    fprintf(stderr, "sdl_text_w_and_h_get: %s\n", strerror(errno));

    sdl_texture_destroy(&texture);

    return 6;
  }

  gui_rect.aspect_ratio = (float) textw / (float) texth;

  SDL_Rect sdl_rect = sdl_rect_create(gui_rect, gui->width, gui->height);
  

  if (sdl_target_texture_render(renderer, menu->texture, texture, &sdl_rect) != 0)
  {
    sdl_texture_destroy(&texture);

    return 5;
  }

  sdl_texture_destroy(&texture);

  return 0;
}

/*
 * Remove window from menu and destroy it
 */
int gui_menu_window_destroy(gui_menu_t* menu, char* name)
{
  if (!menu || !name)
  {
    return 1;
  }

  ssize_t window_index = gui_menu_window_index_get(menu, name);

  if (window_index == -1)
  {
    return 2;
  }

  _gui_window_destroy(&menu->windows[window_index]);


  for (size_t index = window_index; index < (menu->window_count - 1); index++)
  {
    menu->windows[index] = menu->windows[index + 1];
  }


  gui_window_t** temp_windows = realloc(menu->windows, sizeof(gui_window_t*) * (menu->window_count - 1));

  if (!temp_windows)
  {
    // Here, the window is destroyed, but the windows array isn't shrunk
    return 3;
  }

  menu->windows = temp_windows;

  return 0;
}

/*
 * Create window and add it to menu
 */
gui_window_t* gui_menu_window_create(gui_menu_t* menu, char* name, gui_rect_t gui_rect, gui_border_t border)
{
  if (!menu || !name)
  {
    return NULL;
  }

  gui_t* gui = menu->gui;

  if (!gui)
  {
    return NULL;
  }

  gui_window_t* window = malloc(sizeof(gui_window_t));

  if (!window)
  {
    return NULL;
  }

  memset(window, 0, sizeof(gui_window_t));

  window->name = name;
  window->gui  = gui;

  window->border = border;

  window->is_child = false;
  window->parent.menu = menu;

  window->gui_rect = gui_rect;

  window->sdl_rect = sdl_rect_create(window->gui_rect, gui->width, gui->height);

  window->texture = sdl_texture_create(gui->renderer, window->sdl_rect.w, window->sdl_rect.h);

  if (!window->texture)
  {
    free(window);

    return NULL;
  }


  gui_window_t** temp_windows = realloc(menu->windows, sizeof(gui_window_t*) * (menu->window_count + 1));

  if (!temp_windows)
  {
    _gui_window_destroy(&window);

    return NULL;
  }

  menu->windows = temp_windows;

  menu->windows[menu->window_count++] = window;

  return window;
}

/*
 * Create child window and add it to window
 */
gui_window_t* gui_window_child_create(gui_window_t* window, char* name, gui_rect_t gui_rect)
{
  if (!window || !name)
  {
    return NULL;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return NULL;
  }

  gui_window_t* child = malloc(sizeof(gui_window_t));

  if (!child)
  {
    return NULL;
  }

  memset(child, 0, sizeof(gui_window_t));

  child->name = name;
  child->gui  = gui;

  child->is_child = true;
  child->parent.window = window;

  child->gui_rect = gui_rect;

  child->sdl_rect = sdl_rect_create(child->gui_rect, window->sdl_rect.w, window->sdl_rect.h);

  child->texture = sdl_texture_create(gui->renderer, child->sdl_rect.w, child->sdl_rect.h);

  if (!child->texture)
  {
    free(child);

    return NULL;
  }


  gui_window_t** temp_children = realloc(window->children, sizeof(gui_window_t*) * (window->child_count + 1));

  if (!temp_children)
  {
    _gui_window_destroy(&child);

    return NULL;
  }

  window->children = temp_children;

  window->children[window->child_count++] = child;

  return child;
}

/*
 * Render window with all of it's child windows
 */
static inline int gui_window_render(gui_window_t* window)
{
  if (!window)
  {
    return 1;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  for (size_t index = 0; index < window->child_count; index++)
  {
    gui_window_t* child = window->children[index];

    if (gui_window_render(child) != 0)
    {
      return 4;
    }

    if (sdl_target_texture_render(renderer, window->texture, child->texture, &child->sdl_rect) != 0)
    {
      return 5;
    }
  }

  return 0;
}

/*
 * Menu
 */

/*
 * Set active menu
 */
void gui_active_menu_set(gui_t* gui, char* name)
{
  gui->menu_name = name;
}

/*
 * Render menu with all of it's windows and child windows
 *
 * Clear the textures of the windows that are rendered
 *
 * The for-loop decides which order to render the windows
 */
static inline int gui_menu_render(gui_menu_t* menu)
{
  if (!menu)
  {
    return 1;
  }

  gui_t* gui = menu->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  for (size_t index = 0; index < menu->window_count; index++)
  {
    gui_window_t* window = menu->windows[index];

    if (gui_window_render(window) != 0)
    {
      return 4;
    }

    if (sdl_target_texture_render(renderer, menu->texture, window->texture, &window->sdl_rect) != 0)
    {
      return 5;
    }

    if (sdl_target_border_render(renderer, menu->texture, window->border, window->sdl_rect) != 0)
    {
      return 6;
    }
  }

  return 0;
}

/*
 *
 */
static inline SDL_Texture* gui_window_parent_texture_get(gui_window_t* window)
{
  if (window->is_child)
  {
    if (window->parent.window)
    {
      return window->parent.window->texture;
    }
  }
  else
  {
    if (window->parent.menu)
    {
      return window->parent.menu->texture;
    }
  }

  return NULL;
}

/*
 * Render border around window
 */
int gui_window_border_render(gui_window_t* window, gui_border_t border)
{
  if (!window)
  {
    return 1;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  SDL_Texture* texture = gui_window_parent_texture_get(window);

  if (!texture)
  {
    return 4;
  }

  if (sdl_target_border_render(renderer, texture, border, window->sdl_rect) != 0)
  {
    return 5;
  }

  return 0;
}

/*
 *
 */
int gui_curr_window_border_render(gui_t* gui, gui_border_t border)
{
  return gui_window_border_render(gui->curr_window, border);
}

/*
 *
 */
bool gui_window_name_check(gui_window_t* window, char* name)
{
  if (!window || !name) return false;

  return (strcmp(window->name, name) == 0);
}
/*
 *
 */
bool gui_curr_window_name_check(gui_t* gui, char* name)
{
  if (!gui || !name) return false;

  return gui_window_name_check(gui->curr_window, name);
}

/*
 * Render loaded texture (name) to menu texture
 */
int gui_menu_texture_render(gui_menu_t* menu, char* name, gui_rect_t gui_rect)
{
  if (!menu || !name)
  {
    return 1;
  }

  gui_t* gui = menu->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 3;
  }

  gui_texture_t* gui_texture = gui_texture_get(gui, name);
  
  if (!gui_texture)
  {
    return 4;
  }

  SDL_Rect sdl_rect = sdl_rect_create(gui_rect, gui->width, gui->height);

  if (sdl_target_texture_render(renderer, menu->texture, gui_texture->texture, &sdl_rect) != 0)
  {
    return 5;
  }

  return 0;
}

/*
 * Default event handler for quit event
 */
static inline void* _gui_event_quit_handle(gui_t* gui)
{
  gui->is_running = false;
}

/*
 * Resize window texture and children
 */
static inline int gui_window_resize(gui_window_t* window, int width, int height)
{
  if (!window)
  {
    return 1;
  }

  gui_t* gui = window->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  window->sdl_rect = sdl_rect_create(window->gui_rect, width, height);

  if (sdl_texture_resize(&window->texture, renderer, window->sdl_rect.w, window->sdl_rect.h) != 0)
  {
    return 3;
  }


  for (size_t index = 0; index < window->child_count; index++)
  {
    gui_window_t* child = window->children[index];

    if (gui_window_resize(child, window->sdl_rect.w, window->sdl_rect.h) != 0)
    {
      return 4;
    }
  }

  return 0;
}

/*
 * Resize menu texture and windows within it
 */
static inline int gui_menu_resize(gui_menu_t* menu, int width, int height)
{
  if (!menu)
  {
    return 1;
  }

  gui_t* gui = menu->gui;

  if (!gui)
  {
    return 2;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (sdl_texture_resize(&menu->texture, renderer, width, height) != 0)
  {
    return 3;
  }


  for (size_t index = 0; index < menu->window_count; index++)
  {
    gui_window_t* window = menu->windows[index];

    if (gui_window_resize(window, width, height) != 0)
    {
      return 4;
    }
  }

  return 0;
}

/*
 * Resize gui screen to width and height
 */
int gui_resize(gui_t* gui, int width, int height)
{
  if (!gui)
  {
    return 1;
  }

  if (SDL_RenderSetLogicalSize(gui->renderer, width, height) != 0)
  {
    fprintf(stderr, "SDL_RenderSetLogicalSize: %s\n", SDL_GetError());

    return 2;
  }

  gui->width  = width;
  gui->height = height;

  for (size_t index = 0; index < gui->menu_count; index++)
  {
    gui_menu_t* menu = gui->menus[index];

    if (gui_menu_resize(menu, gui->width, gui->height) != 0)
    {
      return 3;
    }
  }

  return 0;
}

/*
 * Default event handler for resize event
 */
static inline void* _gui_event_resize_handle(gui_t* gui, int width, int height)
{
  gui_resize(gui, width, height);
}

/*
 * Create default event and asign handlers
 */
static inline int _gui_events_create(gui_t* gui)
{
  if (gui_event_create(gui, "quit",
    (gui_event_handler_t)
    {
      .type = GUI_EVENT_HANDLER_GUI,
      .handler.gui = &_gui_event_quit_handle
    }) != 0)
  {
    fprintf(stderr, "gui_event_create quit failed\n");

    return 1;
  }

  if (gui_event_create(gui, "resize",
    (gui_event_handler_t)
    {
      .type = GUI_EVENT_HANDLER_RESIZE,
      .handler.resize = &_gui_event_resize_handle
    }) != 0)
  {
    fprintf(stderr, "gui_event_create resize failed\n");

    return 2;
  }

  return 0;
}

/*
 * Create gui
 */
gui_t* gui_create(int width, int height, char* title)
{
  gui_t* gui = malloc(sizeof(gui_t));

  if (!gui)
  {
    return NULL;
  }

  memset(gui, 0, sizeof(gui_t));

  gui->window = sdl_window_create(width, height, title);

  if (!gui->window)
  {
    free(gui);

    return NULL;
  }

  gui->renderer = sdl_renderer_create(gui->window);

  if (!gui->renderer)
  {
    sdl_window_destroy(&gui->window);

    free(gui);

    return NULL;
  }

  if (_gui_events_create(gui) != 0)
  {
    sdl_window_destroy(&gui->window);

    free(gui);

    return NULL;
  }

  gui->title  = title;
  gui->width  = width;
  gui->height = height;

  gui->assets = gui_assets_create();

  return gui;
}

/*
 * Only create menu (This is an internal function)
 */
static inline gui_menu_t* _gui_menu_create(gui_t* gui, char* name)
{
  gui_menu_t* menu = malloc(sizeof(gui_menu_t));

  memset(menu, 0, sizeof(gui_menu_t));

  menu->name = name;
  menu->gui  = gui;

  menu->texture = sdl_texture_create(gui->renderer, gui->width, gui->height);

  if (!menu->texture)
  {
    free(menu);

    return NULL;
  }

  return menu;
}

/*
 * Only destroy menu (This is an internal function)
 */
static inline void _gui_menu_destroy(gui_menu_t** menu)
{
  if (!menu || !(*menu)) return;

  for (size_t index = 0; index < (*menu)->window_count; index++)
  {
    _gui_window_destroy(&(*menu)->windows[index]);
  }

  free((*menu)->windows);

  sdl_texture_destroy(&(*menu)->texture);

  free(*menu);

  *menu = NULL;
}

/*
 * Create menu and add it to GUI
 */
gui_menu_t* gui_menu_create(gui_t* gui, char* name)
{
  if (!gui || !name)
  {
    return NULL;
  }

  gui_menu_t* menu = _gui_menu_create(gui, name);

  if (!menu)
  {
    return NULL;
  }

  gui_menu_t** temp_menus = realloc(gui->menus, sizeof(gui_menu_t*) * (gui->menu_count + 1));

  if (!temp_menus)
  {
    _gui_menu_destroy(&menu);

    return NULL;
  }

  gui->menus = temp_menus;

  gui->menus[gui->menu_count++] = menu;

  // Set the newly created menu as the active one
  gui->menu_name = name;

  return menu;
}

/*
 * Get menu by name
 */
static inline gui_menu_t* gui_menu_get(gui_t* gui, const char* name)
{
  if (!gui || !name)
  {
    return NULL;
  }

  for (size_t index = 0; index < gui->menu_count; index++)
  {
    gui_menu_t* menu = gui->menus[index];

    if (menu && strcmp(menu->name, name) == 0)
    {
      return menu;
    }
  }

  return NULL;
}

/*
 * Get active menu
 */
static inline gui_menu_t* gui_active_menu_get(gui_t* gui)
{
  if (!gui) return NULL;

  return gui_menu_get(gui, gui->menu_name);
}

/*
 * Get index of menu by name
 */
static inline ssize_t gui_menu_index_get(gui_t* gui, const char* name)
{
  if (!gui || !name)
  {
    return -1;
  }

  for (ssize_t index = 0; index < gui->menu_count; index++)
  {
    gui_menu_t* menu = gui->menus[index];

    if (menu && strcmp(menu->name, name) == 0)
    {
      return index;
    }
  }

  return -1;
}

/*
 * Remove menu from GUI and destroy it
 */
int gui_menu_destroy(gui_t* gui, const char* name)
{
  if (!gui || !name)
  {
    return 1;
  }

  ssize_t menu_index = gui_menu_index_get(gui, name);

  if (menu_index == -1)
  {
    return 2;
  }

  _gui_menu_destroy(&gui->menus[menu_index]);


  for (size_t index = menu_index; index < (gui->menu_count - 1); index++)
  {
    gui->menus[index] = gui->menus[index + 1];
  }


  gui_menu_t** temp_menus = realloc(gui->menus, sizeof(gui_menu_t*) * (gui->menu_count - 1));

  if (!temp_menus)
  {
    // Here, the menu is destroyed, but the menus array isn't shrunk
    return 3;
  }

  gui->menus = temp_menus;

  return 0;
}

/*
 * Events
 */

/*
 *
 */
static inline gui_event_t* gui_event_get(gui_t* gui, char* name)
{
  for (size_t index = 0; index < gui->event_count; index++)
  {
    gui_event_t* event = gui->events[index];

    if (strcmp(event->name, name) == 0)
    {
      return event;
    }
  }

  return NULL;
}

/*
 * Destroy gui event and remove handlers
 */
static inline void gui_event_destroy(gui_event_t** event)
{
  if (!event || !(*event)) return;

  free((*event)->handlers);

  free(*event);

  *event = NULL;
}

/*
 * Destroy all gui events
 */
void gui_events_destroy(gui_event_t*** events, size_t count)
{
  for (size_t index = 0; index < count; index++)
  {
    gui_event_destroy(&(*events)[index]);
  }

  free(*events);

  *events = NULL;
}

/*
 *
 */
static inline gui_event_t* _gui_event_create(char* name, gui_event_handler_t handler)
{
  gui_event_t* event = malloc(sizeof(gui_event_t));

  if (!event)
  {
    return NULL;
  }

  event->name = name;

  event->handlers = malloc(sizeof(gui_event_handler_t));

  if (!event->handlers)
  {
    free(event);

    return NULL;
  }

  event->handlers[0] = handler;
  event->handler_count = 1;

  return event;
}

/*
 * Create event and assign handler
 */
int gui_event_create(gui_t* gui, char* name, gui_event_handler_t handler)
{
  if (!gui || !name)
  {
    return 1;
  }

  gui_event_t* event = gui_event_get(gui, name);

  if (event)
  {
    gui_event_handler_t* temp_handlers = realloc(event->handlers, sizeof(gui_event_handler_t) * (event->handler_count + 1));

    if (!temp_handlers)
    {
      return 2;
    }

    event->handlers = temp_handlers;

    event->handlers[event->handler_count++] = handler;
  }
  else
  {
    event = _gui_event_create(name, handler);

    if (!event)
    {
      return 3;
    }

    gui_event_t** temp_events = realloc(gui->events, sizeof(gui_event_t*) * (gui->event_count + 1));

    if (!temp_events)
    {
      free(event);

      return 4;
    }

    gui->events = temp_events;

    gui->events[gui->event_count++] = event;
  }

  return 0;
}

/*
 * Check if x and y is inside SDL Rect
 */
static inline bool x_and_y_is_inside_rect(int x, int y, SDL_Rect rect)
{
  return (x >= rect.x && x <= (rect.x + rect.w) &&
          y >= rect.y && y <= (rect.y + rect.h));
}

/*
 * Try to get the window child which x and y is pointing at
 */
static inline gui_window_t* gui_window_x_and_y_child_get(gui_window_t* window, int x, int y)
{
  for (size_t index = window->child_count; index-- > 0;)
  {
    gui_window_t* child = window->children[index];

    if (x_and_y_is_inside_rect(x, y, child->sdl_rect))
    {
      int child_x = x - child->sdl_rect.x;
      int child_y = y - child->sdl_rect.y;

      gui_window_t* grandchild = gui_window_x_and_y_child_get(child, child_x, child_y);

      return grandchild ? grandchild : child;
    }
  }

  return NULL;
}

/*
 * Try to get the window which x and y is pointing at
 */
gui_window_t* gui_x_and_y_window_get(gui_t* gui, int x, int y)
{
  gui_menu_t* menu = gui_active_menu_get(gui);

  if (!menu)
  {
    return NULL;
  }

  for (size_t index = menu->window_count; index-- > 0;)
  {
    gui_window_t* window = menu->windows[index];

    if (x_and_y_is_inside_rect(x, y, window->sdl_rect))
    {
      int window_x = x - window->sdl_rect.x;
      int window_y = y - window->sdl_rect.y;

      gui_window_t* child = gui_window_x_and_y_child_get(window, window_x, window_y);

      return child ? child : window;
    }
  }

  return NULL;
}

/*
 *
 */
static inline void gui_event_handler_call(gui_t* gui, SDL_Event* event, gui_window_t* window, gui_event_handler_t handler)
{
  int x = event->button.x;
  int y = event->button.y;

  int key = event->key.keysym.sym;

  int width  = event->window.data1;
  int height = event->window.data2;

  switch (handler.type)
  {
    case GUI_EVENT_HANDLER_GUI:
      if (handler.handler.gui)
      {
        handler.handler.gui(gui);
      }
      break;

    case GUI_EVENT_HANDLER_KEY:
      if (handler.handler.key)
      {
        handler.handler.key(gui, key);
      }
      break;

    case GUI_EVENT_HANDLER_MOUSE:
      if (handler.handler.mouse)
      {
        handler.handler.mouse(gui, x, y);
      }
      break;

    case GUI_EVENT_HANDLER_RESIZE:
      if (handler.handler.resize)
      {
        handler.handler.resize(gui, width, height);
      }
      break;

    case GUI_EVENT_HANDLER_WINDOW:
      if (handler.handler.window)
      {
        handler.handler.window(gui, window);
      }
      break;

    default:
      break;
  }
}

/*
 * Trigger event by calling handlers of type GUI_EVENT_HANDLER_GUI,
 * which does not require any input data
 */
void gui_user_event_trigger(gui_t* gui, char* name)
{
  gui_event_t* gui_event = gui_event_get(gui, name);

  if (!gui_event) return;

  for (size_t index = 0; index < gui_event->handler_count; index++)
  {
    gui_event_handler_t handler = gui_event->handlers[index];

    if (handler.type == GUI_EVENT_HANDLER_GUI)
    {
      if (handler.handler.gui)
      {
        handler.handler.gui(gui);
      }
    }
  }
}

/*
 *
 */
static inline void gui_event_handlers_call(gui_t* gui, SDL_Event* event, gui_window_t* window, gui_event_t* gui_event)
{
  for (size_t index = 0; index < gui_event->handler_count; index++)
  {
    gui_event_handler_t handler = gui_event->handlers[index];

    gui_event_handler_call(gui, event, window, handler);
  }
}

/*
 *
 */
static inline void gui_event_trigger(gui_t* gui, SDL_Event* event, gui_window_t* window, char* name)
{
  gui_event_t* gui_event = gui_event_get(gui, name);

  if (!gui_event) return;

  gui_event_handlers_call(gui, event, window, gui_event);
}

/*
 *
 */
static inline void gui_mouse_up_event_handle(gui_t* gui, SDL_Event* event)
{
  int x = event->button.x;
  int y = event->button.y;

  gui_window_t* window = gui_x_and_y_window_get(gui, x, y);

  switch (event->button.button)
  {
    case SDL_BUTTON_LEFT:
      gui_event_trigger(gui, event, window, "mouse-up-left");
      break;

    case SDL_BUTTON_RIGHT:
      gui_event_trigger(gui, event, window, "mouse-up-right");
      break;

    default:
      break;
  }
}

/*
 *
 */
static inline void gui_mouse_down_event_handle(gui_t* gui, SDL_Event* event)
{
  int x = event->button.x;
  int y = event->button.y;

  gui_window_t* window = gui_x_and_y_window_get(gui, x, y);

  switch (event->button.button)
  {
    case SDL_BUTTON_LEFT:
      gui_event_trigger(gui, event, window, "mouse-down-left");
      break;

    case SDL_BUTTON_RIGHT:
      gui_event_trigger(gui, event, window, "mouse-down-right");
      break;

    default:
      break;
  }
}

/*
 * Handle window event
 */
static inline void gui_window_event_handle(gui_t* gui, SDL_Event* event)
{
  switch (event->window.event)
  {
    case SDL_WINDOWEVENT_RESIZED: case SDL_WINDOWEVENT_SIZE_CHANGED:
      gui_event_trigger(gui, event, NULL, "resize");
      break;

    default:
      break;
  }
}

/*
 *
 */
static inline void gui_mouse_motion_event_handle(gui_t* gui, SDL_Event* event)
{
  int x = event->button.x;
  int y = event->button.y;

  gui_window_t* window = gui_x_and_y_window_get(gui, x, y);

  gui_event_trigger(gui, event, window, "mouse-motion");


  if (window != gui->curr_window)
  {
    if (gui->curr_window)
    {
      printf("window exit: %s\n", gui->curr_window->name);

      gui->last_window = gui->curr_window;

      gui->curr_window = NULL;

      gui_event_trigger(gui, event, gui->last_window, "window-exit");
    }

    if (window)
    {
      printf("window enter: %s\n", window->name);

      gui->last_window = gui->curr_window;

      gui->curr_window = window;

      gui_event_trigger(gui, event, gui->curr_window, "window-enter");
    }
  }
}

/*
 * Handle event
 */
static inline void gui_event_handle(gui_t* gui, SDL_Event* event)
{
  if (!event) return;

  switch (event->type)
  {
    case SDL_QUIT:
      gui_event_trigger(gui, event, NULL, "quit");
      break;

    case SDL_WINDOWEVENT:
      gui_window_event_handle(gui, event);
      break;

    case SDL_MOUSEBUTTONDOWN:
      gui_mouse_down_event_handle(gui, event);
      break;

    case SDL_MOUSEBUTTONUP:
      gui_mouse_up_event_handle(gui, event);
      break;

    case SDL_KEYDOWN:
      gui_event_trigger(gui, event, NULL, "key-down");
      break;

    case SDL_KEYUP:
      gui_event_trigger(gui, event, NULL, "key-up");
      break;

    case SDL_MOUSEMOTION:
      gui_mouse_motion_event_handle(gui, event);
      break;

    default:
      break;
  }
}

/*
 * GUI
 */

/*
 * Destroy gui
 */
void gui_destroy(gui_t** gui)
{
  if (!gui || !(*gui)) return;

  for (size_t index = 0; index < (*gui)->menu_count; index++)
  {
    _gui_menu_destroy(&(*gui)->menus[index]);
  }

  free((*gui)->menus);


  gui_assets_destroy(&(*gui)->assets);

  gui_events_destroy(&(*gui)->events, (*gui)->event_count);


  sdl_renderer_destroy(&(*gui)->renderer);

  sdl_window_destroy(&(*gui)->window);

  free(*gui);

  *gui = NULL;
}

/*
 * Search after window in menu
 */
static inline gui_window_t* gui_menu_window_search(gui_menu_t* menu, char** names)
{
  char* name;
  gui_window_t* window = NULL;

  for (size_t index = 0; names && (name = names[index]); index++)
  {
    if (index == 0)
    {
      window = gui_menu_window_get(menu, name);
    }
    else
    {
      window = gui_window_child_get(window, name);
    }

    if (!window)
    {
      return NULL;
    }
  }

  return window;
}

/*
 * Render texture on either window texture or menu texture
 */
int gui_texture_render(gui_t* gui, char* menu_name, char** window_names, char* texture_name, gui_rect_t rect)
{
  if (!gui || !menu_name || !texture_name)
  {
    return 1;
  }

  gui_menu_t* menu = gui_menu_get(gui, menu_name);

  if (!menu)
  {
    return 2;
  }

  gui_window_t* window = gui_menu_window_search(menu, window_names);

  if (window_names && *window_names && !window)
  {
    return 3;
  }

  if (window)
  {
    if (gui_window_texture_render(window, texture_name, rect) != 0)
    {
      return 4;
    }
  }
  else
  {
    if (gui_menu_texture_render(menu, texture_name, rect) != 0)
    {
      return 5;
    }
  }

  return 0;
}

/*
 * Render text on either window texture or menu texture
 */
int gui_text_render(gui_t* gui, char* menu_name, char** window_names, gui_text_t text, gui_rect_t rect)
{
  if (!gui || !menu_name)
  {
    return 1;
  }

  gui_menu_t* menu = gui_menu_get(gui, menu_name);

  if (!menu)
  {
    return 2;
  }

  gui_window_t* window = gui_menu_window_search(menu, window_names);

  if (window_names && *window_names && !window)
  {
    return 3;
  }

  if (window)
  {
    if (gui_window_text_render(window, text, rect) != 0)
    {
      return 4;
    }
  }
  else
  {
    if (gui_menu_text_render(menu, text, rect) != 0)
    {
      return 5;
    }
  }

  return 0;
}

/*
 *
 */
int gui_window_clear(gui_window_t* window)
{
  gui_t* gui = window->gui;

  if (!gui)
  {
    return 1;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 2;
  }

  if (sdl_target_clear(renderer, window->texture) != 0)
  {
    return 3;
  }

  for (size_t index = 0; index < window->child_count; index++)
  {
    gui_window_t* child = window->children[index];

    if (gui_window_clear(child) != 0)
    {
      return 4;
    }
  }

  return 0;
}

/*
 *
 */
int gui_menu_clear(gui_menu_t* menu)
{
  gui_t* gui = menu->gui;

  if (!gui)
  {
    return 1;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 2;
  }

  if (sdl_target_clear(renderer, menu->texture) != 0)
  {
    return 3;
  }

  for (size_t index = 0; index < menu->window_count; index++)
  {
    gui_window_t* window = menu->windows[index];

    if (gui_window_clear(window) != 0)
    {
      return 4;
    }
  }

  return 0;
}

/*
 * Clear screen
 */
int gui_clear(gui_t* gui)
{
  if (!gui)
  {
    return 1;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 2;
  }

  if (sdl_target_clear(renderer, NULL) != 0)
  {
    return 3;
  }

  gui_menu_t* menu = gui_active_menu_get(gui);

  if (gui_menu_clear(menu) != 0)
  {
    return 4;
  }

  return 0;
}

/*
 * Render screen
 */
int gui_render(gui_t* gui)
{
  if (!gui)
  {
    return 1;
  }

  SDL_Renderer* renderer = gui->renderer;

  if (!renderer)
  {
    return 2;
  }

  gui_menu_t* menu = gui_active_menu_get(gui);

  if (gui_menu_render(menu) != 0)
  {
    return 4;
  }

  // Render the menu texture to the screen
  if (sdl_target_texture_render(renderer, NULL, menu->texture, NULL) != 0)
  {
    return 5;
  }

  SDL_RenderPresent(renderer);

  return 0;
}

/*
 * Stop gui, don't render any more frames or handle events
 */
void gui_stop(gui_t* gui)
{
  if (!gui) return;

  gui->is_running = false;
}

/*
 * Start gui by, rendering frames and handle events
 */
void gui_start(gui_t* gui, int fps)
{
  if (!gui || fps <= 0) return;

  gui->is_running = true;

  Uint32 end_ticks   = 0;
  Uint32 start_ticks = 0;

  SDL_Event event;
  SDL_memset(&event, 0, sizeof(event));

  while (gui->is_running)
  {
    while (SDL_PollEvent(&event))
    {
      gui_event_handle(gui, &event);
    }

    end_ticks = SDL_GetTicks();

    if (end_ticks - start_ticks >= 1000 / fps)
    {
      gui_render(gui);

      start_ticks = end_ticks;
    }
  }
}

#endif // GUI_IMPLEMENT
