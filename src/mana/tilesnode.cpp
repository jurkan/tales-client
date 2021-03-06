/*
 * Mana Mobile
 * Copyright (C) 2013  Thorbjørn Lindeijer
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "tilesnode.h"

namespace Mana {

TilesNode::TilesNode(QSGTexture *texture, const QVector<TileData> &tileData)
    : mGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 0)
{
    setFlag(QSGNode::OwnedByParent);

    mMaterial.setTexture(texture);
    mOpaqueMaterial.setTexture(texture);

    mGeometry.setDrawingMode(GL_TRIANGLES);

    // TODO: Using StaticPattern would make sense since the data is never
    // being modified, however this is causing issues on the Raspberry Pi.
    //mGeometry.setVertexDataPattern(QSGGeometry::StaticPattern);

    processTileData(tileData);

    setGeometry(&mGeometry);
    setMaterial(&mMaterial);
    setOpaqueMaterial(&mOpaqueMaterial);
}

void TilesNode::processTileData(const QVector<TileData> &tileData)
{
    const QSize s = mMaterial.texture()->textureSize();
    const QRectF r = mMaterial.texture()->normalizedTextureSubRect();

    const float s_x = r.width() / s.width();
    const float s_y = r.height() / s.height();

    // TODO: By using indices the memory usage could be reduced by 25%, at the
    // cost of an additional indirection:
    //
    // Currently each tile takes:   6 * 16         = 96 bytes
    // With indices it would take:  4 * 16 + 4 * 2 = 72 bytes

    // Two triangles to draw each tile
    mGeometry.allocate(tileData.size() * 6);
    QSGGeometry::TexturedPoint2D *v = mGeometry.vertexDataAsTexturedPoint2D();

    foreach (const TileData &data, tileData) {
        // Taking into account the normalized texture subrectancle
        const float s_width = data.width * s_x;
        const float s_height = data.height * s_y;
        const float s_tx = r.x() + data.tx * s_x;
        const float s_ty = r.y() + data.ty * s_y;

        // TopLeft                      // TopRight
        v[0].x = data.x;                v[2].x = data.x + data.width;
        v[0].y = data.y;                v[2].y = data.y;
        v[0].tx = s_tx;                 v[2].tx = s_tx + s_width;
        v[0].ty = s_ty;                 v[2].ty = s_ty;

        // BottomLeft
        v[1].x = data.x;
        v[1].y = data.y + data.height;
        v[1].tx = s_tx;
        v[1].ty = s_ty + s_height;


                                        // TopRight
                                        v[5].x = data.x + data.width;
                                        v[5].y = data.y;
                                        v[5].tx = s_tx + s_width;
                                        v[5].ty = s_ty;

        // BottomLeft                   // BottomRight
        v[3].x = data.x;                v[4].x = data.x + data.width;
        v[3].y = data.y + data.height;  v[4].y = data.y + data.height;
        v[3].tx = s_tx;                 v[4].tx = s_tx + s_width;
        v[3].ty = s_ty + s_height;      v[4].ty = s_ty + s_height;

        v += 6;
    }

    markDirty(DirtyGeometry);
}

} // namespace Mana
