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
#ifndef __TERMINAL_H__
#define __TERMINAL_H__
#include <string>
#include <vector>
#include "widget.h"
#include "font.h"

#if defined(HAVE_BOOST)
#   include <boost/shared_ptr.hpp>
    typedef boost::shared_ptr<class Terminal> TerminalPtr;
#else
    typedef class Terminal *                  TerminalPtr;
#endif

const int __SIGNAL_ACTIVATE__ = 0;
const int __SIGNAL_COMPLETE__ = 1;
const int __SIGNAL_HISTORY_NEXT__ = 2;
const int __SIGNAL_HISTORY_PREV__ = 3;

/**
 * Terminal widget for user interactions.
 *
 * The terminal widget allows to print message and to enter commands.
 */
class Terminal : public Widget {
public:

    // _________________________________________________________________________

    /**
     * @name Creation/Destruction
     */
    /**
     * Default constructor
     */
    Terminal (void);

    /**
     * Destructor
     */
    virtual ~Terminal (void);
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
     * @name Event processing
     */
    /**
     * Keyboard action
     *
     * @param action   action to be performed
     * @param keyname  key name
     */
    virtual bool keyboard_action (std::string action,
                                  std::string keyname);
    //@}


    // _________________________________________________________________________

    /**
     * @name Input/Output
     */
    /**
     * Printing text on terminal
     *
     * @param text text to be printed
     */
    virtual void print (const std::string &text);

    /**
     * Get input
     *
     * @return current input
     */
    virtual std::string get_input  (void);

    /**
     * Set input
     *
     * @param input new input
     */
    virtual void set_input (std::string input);
    //@}


    // _________________________________________________________________________

    /**
     * Connect a callback function 
     *
     * @param signal signal name
     * @param func   function to be called on command
     */    
    virtual void connect (std::string signal, void (*func)(Terminal *,std::string));
    //@}


    // _________________________________________________________________________

    /**
     * @name Prompt
     */
    /**
     * Set prompt
     *
     * @param prompt new prompt
     */
    virtual void set_prompt (std::string prompt);

    /**
     * Get prompt
     *
     * @return prompt
     */
    virtual std::string get_prompt (void);
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
     * @name Character geometry
     */
    /**
     * Get rows
     *
     * @return rows
     */
    virtual int get_rows (void);

    /**
     * Get columns
     *
     * @return columns
     */
    virtual int get_columns (void);
    //@}


protected:

    // _________________________________________________________________________

    /**
     * Font
     */
    FontPtr font_;

    /**
     * Text buffer
     */
    std::vector<std::string> buffer_;

    /**
     * Prompt
     */
    std::string prompt_;

    /**
     * Current line input
     */
    std::string input_;

    /**
     * Kill buffer
     */
    std::string kill_buffer_;

    /**
     * Cursor position
     */
    unsigned int cursor_;

    /**
     * Font size (1 to 4)
     */
    int fontsize_;

    /**
     * Event handlers
     */
    void (*handlers_[4])(Terminal *, std::string);
};

#endif
