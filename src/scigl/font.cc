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
#include <vector>
#include <sstream>
#include "font.h"


// ________________________________________________________________________ Font
Font::Font (const unsigned char *data,
            const unsigned int data_width,
            const unsigned int data_height) : data_ (data),
                                              data_width_ (data_width), 
                                              data_height_ (data_height) 
{
    base_ = 0;
    texture_ = 0;

    // Setup 256 colors
    colors_[ 0] = Color ( 46/256.0f,  52/256.0f,  54/256.0f, 1.0f);
    colors_[ 1] = Color (204/256.0f,   0/256.0f,   0/256.0f, 1.0f);
    colors_[ 2] = Color ( 78/256.0f, 154/256.0f,   6/256.0f, 1.0f);
    colors_[ 3] = Color (196/256.0f, 160/256.0f,   0/256.0f, 1.0f);
    colors_[ 4] = Color ( 52/256.0f, 101/256.0f, 164/256.0f, 1.0f);
    colors_[ 5] = Color (117/256.0f,  80/256.0f, 123/256.0f, 1.0f);
    colors_[ 6] = Color (  6/256.0f, 152/256.0f, 154/256.0f, 1.0f);
    colors_[ 7] = Color (211/256.0f, 215/256.0f, 207/256.0f, 1.0f);
    colors_[ 8] = Color ( 85/256.0f,  87/256.0f,  83/256.0f, 1.0f);
    colors_[ 9] = Color (239/256.0f,  41/256.0f,  41/256.0f, 1.0f);
    colors_[10] = Color (138/256.0f, 226/256.0f,  52/256.0f, 1.0f);
    colors_[11] = Color (252/256.0f, 233/256.0f,  79/256.0f, 1.0f);
    colors_[12] = Color (114/256.0f, 159/256.0f, 207/256.0f, 1.0f);
    colors_[13] = Color (173/256.0f, 127/256.0f, 168/256.0f, 1.0f);
    colors_[14] = Color ( 52/256.0f, 226/256.0f, 226/256.0f, 1.0f);
    colors_[15] = Color (238/256.0f, 238/256.0f, 236/256.0f, 1.0f);
    for (int r=0; r< 6; r++) {
        for (int g=0; g< 6; g++) {
            for (int b=0; b< 6; b++) {
                int i = 16+r*6*6 + g*6 +b;
                colors_[i] = Color (r/5.0f, g/5.0f, b/5.0f, 1.0f);
            }
        }
    }
    for (int i=0; i<24; i++)
        colors_[232+i] = Color (i/24.0f, i/24.0f, i/24.0f, 1.0f);
}


// _______________________________________________________________________ ~Font
Font::~Font (void)
{}


// _______________________________________________________________________ setup
void
Font::setup (void)
{
    glGenTextures (1, &texture_);
    glBindTexture (GL_TEXTURE_2D, texture_);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_ALPHA, data_width_, data_height_,
                  0, GL_ALPHA, GL_UNSIGNED_BYTE, data_);
    base_ = glGenLists (2*128);
    float cw = data_width_/16.0;
    float ch = data_height_/12.0;
    glyph_size_ = Size (cw,ch);
    float dx = cw / float (data_width_);
    float dy = ch / float (data_height_);
    for (int t=0; t<2; t++) {
        for (int c=0; c<128; c++) {
            glNewList (base_+t*128+c, GL_COMPILE);
            if (c == '\n') {
                glPopMatrix ();
                glTranslatef (0, -ch, 0);
                glPushMatrix ();
            } else if (c >= 32) {
                int y = (c-32) / 16 + t*6;
                int x = (c-32) % 16;
                glBegin(GL_QUADS);
                glTexCoord2f((x  )*dx, (y+1)*dy); glVertex2f(0, -ch);
                glTexCoord2f((x  )*dx, (y  )*dy); glVertex2f(0,   0);
                glTexCoord2f((x+1)*dx, (y  )*dy); glVertex2f(cw,  0);
                glTexCoord2f((x+1)*dx, (y+1)*dy); glVertex2f(cw,-ch);
                glEnd();
            }
            // FIXME: is that right ?
            if (c > '\n') {
                glTranslatef (cw, 0, 0);
            }
            glEndList();
        }
    }
}


// ________________________________________________________________ string_split
std::vector<std::string>
string_split (std::string text, std::string sep)
{
    std::vector<std::string> items;
    int cut;
    while ((cut = text.find(sep)) != int(text.npos)) {
        if(cut >= 0) {
            items.push_back(text.substr(0,cut+1));
        }
        text = text.substr(cut+1);
    }
    if (text.length() > 0) {
        items.push_back(text);
    }
    return items;
}


// _________________________________________________________ parse_ansi_sequence
std::vector<int>
parse_ansi_sequence (const std::string &text)
{
    std::vector<int> codes;
    std::string seq = text.substr(2, text.size()-2);
    std::istringstream iss(seq);
    std::string tmp;
    while (std::getline(iss, tmp, ';')) {
        std::istringstream iss2(tmp);
        int v;
        iss2 >> v;
        codes.push_back (v);
    }
    return codes;
}


// ___________________________________________________________ parse_ansi_string
std::vector <std::pair <std::vector<int>, std::string> >
parse_ansi_string (const std::string &text)
{
    std::vector <std::pair <std::vector<int>, std::string> > fragments;
    std::string sep = "\033[";
    if (not text.size())
        return fragments;
    std::string::size_type start = 0;
    std::string::size_type end =  text.find (sep.c_str(), start+1);
    if (end == std::string::npos) {
        end = text.size()-1;
    }
    do {
        std::string fragment = text.substr(start, end-start);
        std::vector<int> codes;
        if (text[start] == '\033') {
            codes = parse_ansi_sequence (fragment.substr(0, fragment.find("m")));
            fragment = fragment.substr(fragment.find("m")+1);
        }
        std::vector<std::string> s = string_split (fragment, "\n");
        for (unsigned int i=0; i< s.size(); i++) {
            std::pair <std::vector<int>, std::string> item (codes,s[i]);
            fragments.push_back (item);
        }
        //std::pair <std::vector<int>, std::string> item (codes,fragment);
        //fragments.push_back (item);
        start = end;
        end = text.find (sep.c_str(), start+1);
        if (end == std::string::npos) {
            end = text.size();
        }
    } while(end != start);
    return fragments;
};


// ______________________________________________________________________ render
void
Font::render (const std::string &text)
{
    if ((not texture_) or (not base_))
        setup();
    glBindTexture (GL_TEXTURE_2D, texture_);
    glListBase (base_);
    glCallLists (text.size(), GL_UNSIGNED_BYTE, text.c_str());
}

// __________________________________________________________ render_ansi_string
Size
Font::render_ansi_string (const std::string &text,
                          float alpha,
                          int justification)
{
    int cw = glyph_size_.x;
    int ch = glyph_size_.y;

    if ((not texture_) or (not base_))
        setup();
    glBindTexture (GL_TEXTURE_2D, texture_);
    glEnable(GL_BLEND);

    bool use_underline = false;
    bool use_background = false;
    Color foreground, fg, bg;

    glGetFloatv (GL_CURRENT_COLOR, foreground.data);
    glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
    fg = foreground;
    fg.a = 1;

    std::vector <std::pair <std::vector<int>, std::string> > fragments;
    fragments = parse_ansi_string (text);
    
    int start = 0;
    int textsize = 0;
    Size size = Size(0,1);

    for (unsigned int i=0; i<fragments.size(); i++) {
        std::string text = fragments[i].second;
        if (((text.size()) and (text[text.size()-1]) == '\n'))
            textsize += text.size()-1;
        else
            textsize += text.size();

        if (((text.size()) and (text[text.size()-1]) == '\n') or (i == (fragments.size()-1))) {
            if (justification == -1) {
                
            } else if (justification == 0) {
                glPushMatrix();
                glTranslatef (-textsize*cw/2,0,0);
            } else if (justification == +1) {
                glPushMatrix();
                glTranslatef (-textsize*cw,0,0);
            }
            for (unsigned int j=start; j<=i; j++) {
                std::vector<int> codes = fragments[j].first;
                std::string text = fragments[j].second;
                if (codes.size() == 0) {
                    //glListBase (base_);
                    glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
                    //use_underline = false;
                    //use_background = false;
                }
                for (unsigned int k=0; k<codes.size(); k++) {
                    if ((codes[k] > 30) and (codes[k] <38)) {
                        fg = colors_[codes[k]-30];
                        glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
                    }
                    else if ((codes[k] > 40) and (codes[k] <48)) {
                        bg = colors_[codes[k]-40];
                        use_background = true;
                    }
                    else {
                        switch (codes[k]) {
                        case 0:
                            foreground = colors_[0];
                            fg = foreground;
                            glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
                            use_underline = false;
                            use_background = false;
                            glListBase (base_);
                            break;
                        case 1:
                            glListBase (base_+128);
                            break;
                        case 4:
                            use_underline = true;
                            break;
                        case 30:
                            fg = foreground;
                            glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
                            break;
                        case 38:
                            if (k < (codes.size() -2) and (codes[k+1] == 5)) {
                                fg =  colors_[codes[k+2]];
                                glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
                                k += 2;
                            }
                            break;
                        case 40:
                            use_background = false;
                            break;
                        case 48:
                            if (k < (codes.size() -2) and (codes[k+1] == 5)) {
                                use_background = true;
                                bg = colors_[codes[k+2]];
                                k += 2;
                            }
                            break;
                        }
                    }
                }
                if ((use_background) and (text.size() > 0)) {
                    glDisable(GL_TEXTURE_2D);
                    glColor4f (bg.r, bg.g, bg.b, bg.a*alpha);
                    glBegin(GL_QUADS);
                    glVertex2f (cw*int(text.size()), -ch);
                    glVertex2f (                  0, -ch);
                    glVertex2f (                  0, 0);
                    glVertex2f (cw*int(text.size()), 0);
                    glEnd();
                    glEnable(GL_TEXTURE_2D);
                    glColor4f (fg.r, fg.g, fg.b, fg.a*alpha);
                    glTranslatef (0,0,1);
                }
                if ((use_underline) and (text.size() > 0)) {
                    if (text[text.size()-1] == '\n') {
                        glCallLists (text.size()-1, GL_UNSIGNED_BYTE, text.c_str());
                        glDisable(GL_TEXTURE_2D);
                        glBegin(GL_LINES);
                        glVertex2f (-cw*int(text.size()), -ch-.5);
                        glVertex2f (                   0, -ch-.5);
                        glEnd();
                        glCallLists (1, GL_UNSIGNED_BYTE, &text[text.size()-1]);
                        
                    } else {
                        glCallLists (text.size(), GL_UNSIGNED_BYTE, text.c_str());
                        glDisable(GL_TEXTURE_2D);
                        glBegin(GL_LINES);
                        glVertex2f (-cw*int(text.size()), -ch-.5);
                        glVertex2f (                   0, -ch-.5);
                        glEnd();
                    }
                    glEnable(GL_TEXTURE_2D);
                } else {
                    glCallLists (text.size(), GL_UNSIGNED_BYTE, text.c_str());
                }
            }

            if (justification == -1) {
            } else if (justification == 0) {
                glPopMatrix();
            } else if (justification == +1) {
                glPopMatrix();
                glTranslatef (-textsize*cw,0,0);
            }
            start = i+1;
            if (text[text.size()-1] == '\n') {
                if ((textsize-1) > size.x) {
                    size.x = textsize-1;
                }
                size.y++;
            } else {
                if (textsize > size.x) {
                    size.x = textsize;
                }
            }
            textsize = 0;
        }
    }
    return size;
}



// ______________________________________________________________ get_glyph_size
Size
Font::get_glyph_size (void)
{
    return glyph_size_;
}


// ____________________________________________________________________ get_base
GLuint
Font::get_base (void)
{
    return base_;
}


// _________________________________________________________________ get_texture
GLuint
Font::get_texture (void)
{
    return texture_;
}


// ______________________________________________________________________ Font12
FontPtr
Font::Font12 (void) {
#if defined(HAVE_BOOST)
    static FontPtr font = FontPtr();
#else
    static FontPtr font = 0;
#endif
    if (not font)
        font = FontPtr (new Font(font_12.data, font_12.width, font_12.height));
    return font;
}


// ______________________________________________________________________ Font16
FontPtr
Font::Font16 (void) {
#if defined(HAVE_BOOST)
    static FontPtr font = FontPtr();
#else
    static FontPtr font = 0;
#endif
    if (not font)
        font = FontPtr (new Font(font_16.data, font_16.width, font_16.height));
    return font;
}


// ______________________________________________________________________ Font24
FontPtr
Font::Font24 (void) {
#if defined(HAVE_BOOST)
    static FontPtr font = FontPtr();
#else
    static FontPtr font = 0;
#endif
    if (not font)
        font = FontPtr (new Font(font_24.data, font_24.width, font_24.height));
    return font;
}


// ______________________________________________________________________ Font32
FontPtr
Font::Font32 (void) {
#if defined(HAVE_BOOST)
    static FontPtr font = FontPtr();
#else
    static FontPtr font = 0;
#endif
    if (not font)
        font = FontPtr (new Font(font_32.data, font_32.width, font_32.height));
    return font;
}
