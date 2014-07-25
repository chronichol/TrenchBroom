/*
 Copyright (C) 2010-2014 Kristian Duske
 
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

#ifndef __TrenchBroom__FontGlyph__
#define __TrenchBroom__FontGlyph__

#include "VecMath.h"

#include <vector>

namespace TrenchBroom {
    namespace Renderer {
        class FontGlyph {
        public:
            typedef std::vector<FontGlyph> List;
        private:
            float m_x;
            float m_y;
            float m_w;
            float m_h;
            int m_a;
        public:
            FontGlyph(float x, float y, float w, float h, int a);
            
            void appendVertices(Vec2f::List& vertices, float xOffset, float yOffset, float textureSize, bool clockwise) const;
            
            int advance() const;
        private:
            float sMin(float textureSize) const;
            float sMax(float textureSize) const;
            float tMin(float textureSize) const;
            float tMax(float textureSize) const;
        };
    }
}

#endif /* defined(__TrenchBroom__FontGlyph__) */