#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils/math.h"
#include "utils/renderer.h"
#include "utils/terminal.h"
#include "utils/assert.h"
#include "utils/string.h"

Style style_regular(Style style)
{
    style.bold = false;
    style.underline = false;

    return style;
}

Style style_bold(Style style)
{
    style.bold = true;
    style.underline = false;

    return style;
}

Style style_centered(Style style)
{
    style.align = TEXT_CENTER;

    return style;
}

Style style_with_background(Style style, Color background)
{
    style.background = background;

    return style;
}

Style style_with_foreground(Style style, Color foreground)
{
    style.foreground = foreground;

    return style;
}

Style style_inverted(Style style)
{
    Color tmp = style.foreground;
    style.foreground = style.background;
    style.background = tmp;

    return style;
}

Surface *surface_create(void)
{
    Surface *this = malloc(sizeof(Surface));

    terminal_get_size(&this->width, &this->height);

    this->cells = calloc(this->width * this->height, sizeof(Cell));
    this->cells_allocated = this->width * this->height;

    this->clipstack_top = 0;

    return this;
}

void surface_destroy(Surface *this)
{
    free(this->cells);
    free(this);
}

void surface_update(Surface *this)
{
    assert(this->clipstack_top == 0);

    terminal_get_size(&this->width, &this->height);

    if (this->width * this->height > this->cells_allocated)
    {
        this->cells = realloc(this->cells, sizeof(Cell) * this->width * this->height);
        this->cells_allocated = this->width * this->height;
    }
}

static int style_cmp(Style a, Style b)
{
    return memcmp(&a, &b, sizeof(Style));
}

void surface_render(Surface *this)
{
    terminal_set_cursor_position(0, 0);
    terminal_hide_cursor();

    Style current_style = DEFAULT_STYLE;
    printf("\e[0m");

    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            Cell c = this->cells[x + y * this->width];

            if (style_cmp(current_style, c.style) != 0)
            {
                printf("\e[0m");

                if (c.style.bold)
                {
                    printf("\e[1m");
                }

                if (c.style.underline)
                {
                    printf("\e[4m");
                }

                printf("\e[3%d;4%dm", c.style.foreground, c.style.background);

                current_style = c.style;
            }

            if (c.codepoint == 0)
            {
                printf(" ");
            }
            else
            {
                uint8_t utf8[5];
                strutf8(utf8, c.codepoint);
                printf("%s", utf8);
            }
        }
    }

    terminal_show_cursor();
}

Region surface_clip(Surface *this)
{
    if (this->clipstack_top == 0)
    {
        return (Region){0, 0, this->width, this->height};
    }
    else
    {
        return this->clipstack[this->clipstack_top - 1];
    }
}

int surface_width(Surface *this)
{
    return surface_clip(this).width;
}

int surface_height(Surface *this)
{
    return surface_clip(this).height;
}

Region surface_region(Surface *this)
{
    return (Region){0, 0, surface_width(this), surface_height(this)};
}

void surface_push_clip(Surface *this, Region clip)
{
    clip.x += surface_clip(this).x;
    clip.y += surface_clip(this).y;

    this->clipstack[this->clipstack_top] = clip;
    this->clipstack_top++;
}

void surface_pop_clip(Surface *this)
{
    assert(this->clipstack_top > 0);
    this->clipstack_top--;
}

void surface_clear(Surface *this, Style style)
{
    surface_fill(this, ' ', surface_region(this), style);
}

void surface_plot(Surface *this, Codepoint codepoint, int x, int y, Style style)
{
    Region r = surface_clip(this);

    // FIXME: this condition look sily...
    if (x >= 0 && x < r.width && x + r.x >= 0 && x + r.x < this->width &&
        y >= 0 && y < r.height && y + r.y >= 0 && y + r.y < this->height)
    {
        x += r.x;
        y += r.y;

        this->cells[x + y * this->width] = (Cell){codepoint, style};
    }
}

void surface_fill(Surface *this, Codepoint codepoint, Region region, Style style)
{
    for (int x = 0; x < region.width; x++)
    {
        for (int y = 0; y < region.height; y++)
        {
            surface_plot(this, codepoint, region.x + x, region.y + y, style);
        }
    }
}

void surface_text(Surface *this, const char *text, int x, int y, int width, Style style)
{
    int textlenght = utf8len(text);

    int offset = 0;
    int content = min(textlenght, width);
    int padding = max(0, width - textlenght);

    if (style.align == TEXT_CENTER)
    {
        padding /= 2;
    }

    if (style.align == TEXT_RIGHT || style.align == TEXT_CENTER)
    {
        for (int i = 0; i < padding; i++)
        {
            surface_plot(this, ' ', x + i, y, style_regular(style));
        }

        offset += padding;
    }

    //FIXME: iterate over code point...

    int i = 0;
    while (i < content)
    {
        Codepoint cp;
        text += utf8str((uint8_t *)text, &cp);
        surface_plot(this, cp, x + i + offset, y, style);

        i++;
    }

    if (content < textlenght)
    {
        surface_plot(this, u'…', x + content + offset - 1, y, style);
    }

    offset += content;

    if (style.align == TEXT_LEFT || style.align == TEXT_CENTER)
    {
        for (int i = offset; i < width; i++)
        {
            surface_plot(this, ' ', x + i, y, style_regular(style));
        }
    }
}

void surface_plot_line_x_aligned(Surface *this, Codepoint codepoint, int x, int start, int end, Style style)
{
    for (int i = start; i < end; i++)
    {
        surface_plot(this, codepoint, x, i, style);
    }
}

void surface_plot_line_y_aligned(Surface *this, Codepoint codepoint, int y, int start, int end, Style style)
{
    for (int i = start; i < end; i++)
    {
        surface_plot(this, codepoint, i, y, style);
    }
}

void surface_plot_line_not_aligned(Surface *this, Codepoint codepoint, int x0, int y0, int x1, int y1, Style style)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;)
    {
        surface_plot(this, codepoint, x0, y0, style);

        if (x0 == x1 && y0 == y1)
            break;

        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void surface_plot_line(Surface *this, Codepoint codepoint, int x0, int y0, int x1, int y1, Style style)
{
    if (x0 == x1)
    {
        surface_plot_line_x_aligned(this, codepoint, x0, min(y0, y1), max(y0, y1), style);
    }
    else if (y0 == y1)
    {
        surface_plot_line_y_aligned(this, codepoint, y0, min(x0, x1), max(x0, x1), style);
    }
    else
    {
        surface_plot_line_not_aligned(this, codepoint, x0, y0, x1, y1, style);
    }
}