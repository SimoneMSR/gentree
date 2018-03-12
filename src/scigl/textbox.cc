/*
 * Copyright (C) 2008 Nicolas P. Rougier
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.

 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "font.h"
#include "textbox.h"


// _____________________________________________________________________________
TextBox::TextBox (void) : Widget ()
{
    set_fg_color (0,0,0,1);
    set_bg_color (1,1,1,0);
    set_br_color (0,0,0,0);
    set_margin (0,0,0,0);
    set_autosize (true);
    set_justification (-1);
    set_fontsize (0);

    std::ostringstream oss;
    oss << "TextBox_" << id_;
    set_name (oss.str());
}


// _____________________________________________________________________________
TextBox::~TextBox (void)
{}


// _____________________________________________________________________________
void
TextBox::render (void)
{
    render_start ();
    if (not get_visible()) {
        render_finish();
        return;
    }

    GLint viewport[4]; 
    glGetIntegerv (GL_VIEWPORT, viewport);
    GLint scissor[4]; 
    glGetIntegerv (GL_SCISSOR_BOX, scissor);
    GLint scissor_active;
    glGetIntegerv (GL_SCISSOR_TEST, &scissor_active);
    float height = viewport[3];

    glPushMatrix();
    Widget::render();

    int border = 2;
    if ((get_br_color().alpha * alpha_) == 0) {
        border = 0;
    }

    // Set scissor test
    int x = viewport[0] + int(get_position().x + border/2);
    int y = viewport[1] + int(height-get_position().y-get_size().y + border/2);
    int w = int(get_size().x) - border;
    int h = int(get_size().y) - border;
    if (scissor_active) {
        if (x < scissor[0])
            x = scissor[0];
        if (y < scissor[1])
            y = scissor[1];
        if ((x+w) > (viewport[0]+scissor[2]))
            w += (viewport[0]+scissor[2]) - (x+w);
        if ((y+h) > (viewport[1]+scissor[3]))
            h += (viewport[1]+scissor[3]) - (y+h);
    }
    glEnable (GL_SCISSOR_TEST);
    glScissor (x,y,w,h);

    glTranslatef (get_position().x          + get_margin().left+1,
                  height-1-get_position().y - get_margin().up+1,
                  1);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    
    glColor4f (fg_color_.r,fg_color_.g, fg_color_.b, fg_color_.a*alpha_);
    glEnable (GL_TEXTURE_2D);
    glDepthMask (GL_TRUE);
    glEnable (GL_BLEND);
    glListBase (font_->get_base());

    glPushMatrix();
    if (justification_ == -1) {
        //glTranslatef (int(get_size().x/2), 0, 0);
    } else if (justification_ == 0) {
        glTranslatef (int((get_size().x-get_margin().right-get_margin().left)/2), 0, 0);
    } else if (justification_ == +1) {
        glTranslatef (int(get_size().x -get_margin().right-get_margin().left-1),0,0);
    }
    Size s = font_->render_ansi_string (buffer_, alpha_, justification_);
    glPopMatrix();

    // FIX ME:
    //  Autosize is computed once the test has been rendered at least once,
    //  consequently, first time text is displayed, autosize does not work.
    if (autosize_) {
        size_request_.x = s.x * font_->get_glyph_size().x
            + get_margin().right + get_margin().left;
        size_request_.y = s.y * font_->get_glyph_size().y
            + get_margin().up + get_margin().down;
    }

    glPopMatrix();
    render_finish();
}


// __________________________________________________________________ get_buffer
std::string
TextBox::get_buffer (void)
{
    return buffer_;
}


// __________________________________________________________________ set_buffer
void
TextBox::set_buffer (std::string buffer)
{
    buffer_ = buffer;
}


// ________________________________________________________________ set_fontsize
void
TextBox::set_fontsize (int size)
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
TextBox::get_fontsize()
{
    return fontsize_;
}


// ___________________________________________________________ set_justification
void
TextBox::set_justification (int justification)
{
    switch (justification) {
    case 0:
        justification_ = 0;
        break;
    case +1:
        justification_ = +1;
        break;
    default:
        justification_ = -1;
        break;
    }
}


// ___________________________________________________________ get_justification
bool
TextBox::get_justification (void)
{
    return justification_;
}


// ________________________________________________________________ set_autosize
void
TextBox::set_autosize (bool autosize)
{
    autosize_ = autosize;
}


// ________________________________________________________________ get_autosize
bool
TextBox::get_autosize (void)
{
    return autosize_;
}


// ____________________________________________________________________ set_size
void
TextBox::set_size (Size size)
{
    if (autosize_)
        size_.z = size.z;
    else
        Widget::set_size (size);
}


// ____________________________________________________________________ set_size
void
TextBox::set_size (float x, float y, float z)
{
    if (autosize_)
        size_.z = z;
    else
        Widget::set_size (x,y,z);
}
