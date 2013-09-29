
/*
 Copyright (C) 2010-2013 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Color.h"

Color::Color() :
Vec<float, 4>(0.0f, 0.0f, 0.0f, 0.0f) {}

Color::Color(const float r, const float g, const float b, const float a) :
Vec<float, 4>(r, g, b, a) {}

Color::Color(const Color& color, const float a) :
Vec<float, 4>(color.r(), color.g(), color.b(), a) {}

Color::Color(const std::string& str) :
Vec<float, 4>(str) {}

float Color::r() const {
    return x();
}

float Color::g() const {
    return y();
}

float Color::b() const {
    return z();
}

float Color::a() const {
    return w();
}