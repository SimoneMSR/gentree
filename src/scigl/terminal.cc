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
#include <sstream>
#include "terminal.h"

// ____________________________________________________________________ Terminal
Terminal::Terminal (void) : Widget ()
{
    set_prompt ("> ");
    input_ = "";
    font_ = Font::Font12();
    set_margin    (5,5,5,5);
    set_radius    (0);
    set_size      (Size (1, 1));
    set_position  (0,0,-1);
    set_gravity   (1,1);
    set_fg_color  (0,0,0,1);
    set_bg_color  (1,1,.9,1);
    set_br_color  (0,0,0,0);
    set_alpha     (1.0f);
    cursor_ = 0;
    handlers_[__SIGNAL_ACTIVATE__] = 0;
    handlers_[__SIGNAL_COMPLETE__] = 0;
    handlers_[__SIGNAL_HISTORY_NEXT__] = 0;
    handlers_[__SIGNAL_HISTORY_PREV__] = 0;

    std::ostringstream oss;
    oss << "Terminal_" << id_;
    set_name (oss.str());
}

// ___________________________________________________________________ ~Terminal
Terminal::~Terminal (void)
{}

// _____________________________________________________________________ connect
void
Terminal::connect (std::string signal, void (*func)(Terminal *, std::string))
{
    if (signal == "activate")
        handlers_[__SIGNAL_ACTIVATE__] = func;
    else if (signal == "complete")
        handlers_[__SIGNAL_COMPLETE__] = func;
    else if (signal == "history-next")
        handlers_[__SIGNAL_HISTORY_NEXT__] = func;
    else if (signal == "history-prev")
        handlers_[__SIGNAL_HISTORY_PREV__] = func;
    else
        throw std::runtime_error("Unknown signal: " + signal);
}

// _______________________________________________________________________ split
std::vector<std::string>
split (const std::string &text)
{
    std::string::size_type cur_pos  = 0;
    std::string::size_type next_pos = text.find ("\n", cur_pos);
    std::vector<std::string> items;
    do {
        std::string item;
        if (next_pos < text.size()) {
            items.push_back (text.substr(cur_pos, next_pos-cur_pos+1));
        } else {
            items.push_back (text.substr(cur_pos));
            break;
        }
        cur_pos = next_pos+1;
        next_pos = text.find ("\n", cur_pos);
    } while (cur_pos < text.size());
    return items;
}

// _______________________________________________________________________ print
void
Terminal::print (const std::string &text)
{
    if (not text.size())
        return;

    std::vector<std::string> parts = split(text);
    if (not parts.size())
        return;

    // Is there at least one line ?
    if (buffer_.size() > 0) {
        int index = buffer_.size()-1;
        int length = buffer_[index].size();
        // Is the last line not empty ?
        if (length > 0) {
            // Does the last character of the last line is not '\n' ?
            if (buffer_[index][length-1] != '\n') {
                buffer_[index].append (parts[0]);
                for (unsigned int i=1; i<parts.size(); i++)
                    buffer_.push_back (parts[i]);
                return;
            }
        }
    }
    // In all other cases, we append text to the buffer
    for (unsigned int i=0; i<parts.size(); i++)
        buffer_.push_back (parts[i]);
}

// ______________________________________________________________________ render
void
Terminal::render (void)
{
    render_start ();
    if (not get_visible()) {
        render_finish();
        return;
    }

    
    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    float height = viewport[3];

    glPushMatrix();

    Widget::render();

    glEnable (GL_SCISSOR_TEST);
    glScissor (int(get_position().x + get_margin().left),
               int(height-1-get_position().y - get_size().y + get_margin().down),
               int(get_size().x - get_margin().right - get_margin().left),
               int(get_size().y - get_margin().up    - get_margin().down));
    glTranslatef (get_position().x + get_margin().left,
                  height-1-get_position().y - get_margin().up,
                  1);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glEnable (GL_TEXTURE_2D);
    glDepthMask (GL_TRUE);
    glEnable (GL_BLEND);

    // How many lines to render ?
    int l = 0;
    Size size = font_->get_glyph_size();
    int h = 0;
    if (prompt_.size())
        h = size.y; // For prompt line
    int dy = 0;
    for (int i=(buffer_.size()-1); i>= 0; i--) {
        if (h > (get_size().y - get_margin().up - get_margin().down))
            break;
        l++;
        h += size.y;
    }

    // dy to be aligned on terminal bottom line
    if (h > (get_size().y - get_margin().up - get_margin().down))
        dy = h - (get_size().y - get_margin().up - get_margin().down);
    glTranslatef (0, dy, 0);

    // buffer
    glPushMatrix();
    glColor4f (fg_color_.r,fg_color_.g, fg_color_.b, fg_color_.a*alpha_);
    glListBase (font_->get_base());
    for (int i=int(buffer_.size()-l); i<int(buffer_.size()); i++) {
        //glPushMatrix();
        font_->render_ansi_string (buffer_[i], alpha_);
        //glPopMatrix();
    }

    // prompt & cursor
    //glColor4f (fg_color_.r,fg_color_.g, fg_color_.b, fg_color_.a*alpha_);
    //glPushMatrix();
    font_->render_ansi_string (prompt_, alpha_);
    //glTranslatef (s.x*size.x,0,0);
    glDisable (GL_TEXTURE_2D);
    glBegin (GL_LINES);
    glVertex3f (cursor_*size.x +0.375, 0, 0);
    glVertex3f (cursor_*size.x +0.375, -size.y, 0);
    glEnd();
    glEnable (GL_TEXTURE_2D);
    font_->render_ansi_string (input_, alpha_);
    //glPopMatrix();

    glPopMatrix();


    glPopMatrix();

    render_finish();
}

// ------------------------------------------------------------------- key_press
bool
Terminal::keyboard_action (std::string action, std::string keyname)
{
    if (not get_visible())
        return false;
    if ((action == "type")  and
        (keyname.size() == 1) and ((keyname[0] > 31) && (keyname[0] < 127))) {
        std::string::iterator i = input_.begin() + cursor_;
        input_.insert(i, keyname[0]);
        cursor_++;
        return true;
    } else {
        if (action == "home") {
            cursor_ = 0;
            return true;
        } else if (action == "left") {
            if (cursor_ > 0)
                cursor_--;
            return true;
        } else if (action == "delete") {
            if (cursor_ < input_.size()) {
                std::string::iterator i = input_.begin() + cursor_;
                input_.erase (i);
            }
            return true;
        } else if (action == "end") {
            cursor_ = input_.size();
            return true;
        } else if (action == "right") {
            if (cursor_ < input_.size())
                cursor_++;
            return true;
        } else if (action == "backspace") {
            if (cursor_ > 0) {
                std::string::iterator i = input_.begin() + cursor_ - 1;
                input_.erase (i);
                cursor_--;
            }
            return true;     
        } else if ((action == "history-prev") and  (handlers_[__SIGNAL_HISTORY_PREV__])) {
            (*handlers_[__SIGNAL_HISTORY_PREV__])(this, input_);
            return true;
        } else if ((action == "history-next") and (handlers_[__SIGNAL_HISTORY_NEXT__])) {
            (*handlers_[__SIGNAL_HISTORY_NEXT__])(this, input_);
            return true;
        } else if ((action == "complete") and (handlers_[__SIGNAL_COMPLETE__])) {
            (*handlers_[__SIGNAL_COMPLETE__])(this, input_);
            return true;
        } else if (action == "kill") {   // Control-k : kill from cursor
            if (cursor_ < input_.size()) {
                kill_buffer_ = input_.substr (cursor_);
                input_ = input_.substr (0, cursor_);
            }
            return true;
        } else if (action == "clear") {   // Control-l : clear
            buffer_.clear();
            buffer_.push_back ("");
            return true;
        } else if ((action == "return") or (action == "enter")) {
            print (prompt_+input_+std::string("\n"));
            if (handlers_[__SIGNAL_ACTIVATE__])
                (*handlers_[__SIGNAL_ACTIVATE__])(this, input_);
            input_ = "";
            cursor_ = 0;
            return true;
        } else if (action == "yank") {   // Control-y : yank
            input_.insert (cursor_, kill_buffer_);
            cursor_ += kill_buffer_.size();
            return true;
        }
    }
    return false;
}

// __________________________________________________________________ get_prompt
std::string
Terminal::get_prompt (void)
{
    return prompt_;
}

// __________________________________________________________________ set_prompt
void
Terminal::set_prompt (std::string prompt)
{
    prompt_ = prompt;
}

// ___________________________________________________________________ get_input
std::string
Terminal::get_input (void)
{
    return input_;
}

// ___________________________________________________________________ set_input
void
Terminal::set_input (std::string input)
{
    input_ = input;
    cursor_ = input_.size();
}

// ________________________________________________________________ set_fontsize
void
Terminal::set_fontsize (int size)
{
    fontsize_ = size;
    switch (fontsize_) {
    case 1:
        font_ = Font::Font16();
        break;
    case 2:
        font_ = Font::Font24();
        break;
    case 3:
        font_ = Font::Font32();
        break;
    default:
        font_ = Font::Font12();
        break;
    }
}


// ________________________________________________________________ get_fontsize
int
Terminal::get_fontsize()
{
    return fontsize_;
}


// ____________________________________________________________________ get_rows
int
Terminal::get_rows (void)
{
    return int(get_size().y) / int(font_->get_glyph_size().y);
}


// _________________________________________________________________ get_columns
int
Terminal::get_columns (void)
{
    return int(get_size().x) / int(font_->get_glyph_size().x);
}
