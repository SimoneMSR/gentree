/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either  version 3 of the  License, or (at your  option) any later
 * version.
 *
 * This program is  distributed in the hope that it will  be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR  A  PARTICULAR PURPOSE.  See  the GNU  General  Public  License for  more
 * details.
 *
 * You should have received a copy  of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__
#include "widget.h"
#include "font.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class TextBox> TextBoxPtr;
#else
    typedef class TextBox *                  TextBoxPtr;
#endif


/**
 * TextBox rendering widget.
 *
 * TextBox widget is used to display lines of formated text.
 *
 */
class TextBox : public Widget {

public:

    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    TextBox (void);

    /**
     * Destructor
     */
    virtual ~TextBox (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Rendering
     */
    /**
     * Render
     */
    virtual void render (void);
    //@}


    // _________________________________________________________________________
    
    /**
     * @name TextBox buffer
     */
    /**
     * Get text buffer
     *
     * @return text buffer
     */
    virtual std::string get_buffer (void);

    /**
     * Set text buffer
     *
     * @param buffer new text buffer
     */
    virtual void set_buffer (std::string buffer);
    //@}


    // _________________________________________________________________________

    /**
     * @name Autosize
     */
    /**
     * Set autosize
     *
     * @param autosize whether to autosiae textbox or not
     */
    virtual void set_autosize (bool autosize);

    /**
     * Get autosize mode
     *
     * @return autosize mode
     */
    virtual bool get_autosize (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Font size
     */
    /**
     * Set font size
     *
     * @param size (from 1 to 4)
     */
    virtual void set_fontsize (int size);

    /**
     * Get font size
     *
     * @return font size
     */
    virtual int get_fontsize();
    //@}


    // _________________________________________________________________________

    /**
     * @name Justification
     */
    /**
     * Set justification
     *
     * @param justification -1:left, 0:center, +1:right
     */
    virtual void set_justification (int justification);

    /**
     * Get justification
     *
     * @return justification (-1:left, 0:center, +1:right)
     */
    virtual bool get_justification (void);
    //@}


    // _________________________________________________________________________

    /**
     * @name Size
     */
    /**
     * Set size in window space.
     *
     * It is possible to use relative size using coordinates between 0 and 1.
     * Negative sizes relates to the complement of the related dimension. For
     * example, if x is -10, then x size will be window_width-10. If x is -.25,
     * then x size will be (1-.25)*window_width.
     *
     * @param size size
     */
    virtual void set_size (Size size);

    /**
     * Set size in window space.
     *
     * It is possible to use relative size using coordinates between 0 and 1.
     * Negative sizes relates to the complement of the related dimension. For
     * example, if x is -10, then x size will be window_width-10. If x is -.25,
     * then x size will be (1-.25)*window_width. 
     *
     * @param x position along x axis
     * @param y position along y axis
     * @param z -1 for back, +1 for front
     */
    virtual void set_size (float x=1, float y=1, float z=1);
    //@}


protected:

    // _________________________________________________________________________

    /**
     * TextBox buffer
     */
    std::string buffer_;

    /**
     * Justification (-1:left, 0:center, +1:right)
     */
    int justification_;

    /**
     * Font size (1 to 4)
     */
    int fontsize_;

    /**
     * Whether to autosize or not
     */
    bool autosize_;

    /**
     * Font to be usef
     */
    FontPtr font_;
};

#endif
