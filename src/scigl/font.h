/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __FONT_H__
#define __FONT_H__

#include <string>
#include "object.h"
#include "vec4f.h"
#include "font_12.h"
#include "font_16.h"
#include "font_24.h"
#include "font_32.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Font> FontPtr;
#else
    typedef class Font *                  FontPtr;
#endif


/**
 * Font rendering.
 *
 * The font object allows to display ascii text in monotype font at different
 * size (12, 16, 24 or 32). It works by building glyphs as texture mapped quads
 * and caching them in display lists. Displaying a line of text is then
 * straightforward.
 *
 */
class Font {
public:


    // _________________________________________________________________________
    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Font (const unsigned char *data,
          const unsigned int data_width,
          const unsigned int data_height);

    /**
     * Destructor
     */
    virtual ~Font (void);
    //@}


    // _________________________________________________________________________
    /**
     * @name Rendering
     */

    /**
     * Build the texture for the font.
     */
    void setup (void);

    /**
     * Render text
     *
     * @param text tet to be rendered
     */
    virtual void render (const std::string &text);

    /**
     * Render string with ansi codes
     *
     * @param text           ansi string to be rendered
     * @param alpha          alpha transparency level
     * @param justification  -1 left, 0 center, +1 right
     *
     * @return size of text in terms of columns x lines
     */
    virtual Size render_ansi_string (const std::string &text,
                                     float alpha = 1,
                                     int justification=-1);
    //@}

    
    // _________________________________________________________________________
    /**
     *  @name Available fonts.
     */
    /**
     * Bitstream Vera Sans Mono 12
     *
     * @return Bitstream Vera Sans Mono 12 font
     */
    static FontPtr Font12 (void);

    /**
     * Bitstream Vera Sans Mono 16
     *
     * @return Bitstream Vera Sans Mono 16 font
     */
    static FontPtr Font16 (void);

    /**
     * Bitstream Vera Sans Mono 24
     *
     * @return Bitstream Vera Sans Mono 24 font
     */
    static FontPtr Font24 (void);

    /**
     * Bitstream Vera Sans Mono 32
     *
     * @return Bitstream Vera Sans Mono 32 font
     */
    static FontPtr Font32 (void);
    //@}


    // _________________________________________________________________________
    /**
     * Get glyph size
     *
     * @return glyph size
     */
    Size get_glyph_size (void);


    // _________________________________________________________________________
    /**
     * Get list base
     *
     * @return display list base
     */
    GLuint get_base (void);


    // _________________________________________________________________________
    /**
     * Get texture id
     *
     * @return texture id
     */
    GLuint get_texture (void);



protected:
    /**
     * Display list base
     */
    GLuint base_;

    /**
     * Font texture (all glyphs at once)
     */
    GLuint texture_;

    /**
     * Font data
     */
    const unsigned char *data_;

    /**
     * Font data width
     */
    const unsigned int data_width_;

    /**
     * Font data height
     */
    const unsigned int data_height_;

    /**
     * Glyph size
     * 
     * Fonts are fixed width, all glyphs have the same size.
     */
    Size glyph_size_;


public:
    /**
     * Default colors
     * 
     * 256 colors for ansi escape sequences
     */
    Color colors_[256];
};

#endif
