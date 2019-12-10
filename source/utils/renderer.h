#pragma once

#include <stdbool.h>
#include <utils/string.h>

typedef enum
{
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_ORANGE,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_BRIGHT_BLACK = 60,
    COLOR_BRIGHT_RED,
    COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_ORANGE,
    COLOR_BRIGHT_BLUE,
    COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_CYAN,
    COLOR_BRIGHT_WHITE,
} Color;

typedef enum
{
    TEXT_CENTER,
    TEXT_LEFT,
    TEXT_RIGHT,
} TextAlign;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} Region;

typedef struct
{
    bool bold;
    bool underline;

    Color foreground;
    Color background;

    TextAlign align;
} Style;

#define DEFAULT_STYLE \
    (Style) { false, false, COLOR_WHITE, COLOR_BLACK, TEXT_LEFT }

#define DISABLED_DEFAULT_STYLE \
    (Style) { false, false, COLOR_BRIGHT_BLACK, COLOR_BLACK, TEXT_LEFT }

#define INVERTED_STYLE \
    (Style) { false, false, COLOR_BLACK, COLOR_WHITE, TEXT_LEFT }

#define DISABLED_INVERTED_STYLE \
    (Style) { false, false, COLOR_BLACK, COLOR_BRIGHT_BLACK, TEXT_LEFT }

#define ALTERNATIVE_STYLE \
    (Style) { false, false, COLOR_WHITE, COLOR_BRIGHT_BLACK, TEXT_LEFT }

#define RED_STYLE \
    (Style) { false, false, COLOR_RED, COLOR_BLACK, TEXT_LEFT }

#define BLUE_STYLE \
    (Style) { false, false, COLOR_BLUE, COLOR_BLACK, TEXT_LEFT }

#define WHITE_STYLE \
    (Style) { false, false, COLOR_WHITE, COLOR_BLACK, TEXT_LEFT }

#define BOLD_STYLE \
    (Style) { true, false, COLOR_WHITE, COLOR_BLACK, TEXT_LEFT }

#define UNDERLINE_STYLE \
    (Style) { false, true, COLOR_WHITE, COLOR_BLACK, TEXT_LEFT }

Style style_regular(Style style);

Style style_bold(Style style);

Style style_with_background(Style style, Color background);

Style style_with_foreground(Style style, Color foreground);

Style style_centered(Style style);

Style style_inverted(Style style);

typedef struct
{
    Codepoint codepoint;
    Style style;
} Cell;

typedef struct
{
    int width;
    int height;

    Region clipstack[16];
    int clipstack_top;

    Cell *cells;
    int cells_allocated;
} Surface;

Surface *surface_create(void);

void surface_destroy(Surface *this);

void surface_update(Surface *this);

void surface_render(Surface *this);

Region surface_region(Surface *this);

int surface_width(Surface *this);

int surface_height(Surface *this);

void surface_push_clip(Surface *this, Region clip);

void surface_pop_clip(Surface *this);

void surface_clear(Surface *this, Style style);

void surface_plot(Surface *this, Codepoint codepoint, int x, int y, Style style);

void surface_fill(Surface *this, Codepoint codepoint, Region region, Style style);

void surface_text(Surface *this, const char *text, int x, int y, int width, Style style);

void surface_plot_line(Surface *this, Codepoint codepoint, int x0, int y0, int x1, int y1, Style style);
