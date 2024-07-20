/*
    SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include <effect/effect.h>
#include <opengl/glutils.h>

#include <KLocalizedString>

namespace KWin
{

class CatPawEffect : public Effect
{
    Q_OBJECT
public:
    CatPawEffect();
    ~CatPawEffect() override;
    void reconfigure(ReconfigureFlags) override;
    void paintScreen(const RenderTarget &renderTarget, const RenderViewport &viewport, int mask, const QRegion &region, Output *screen) override;
    void postPaintScreen() override;

    // for properties
    QColor kittyColor() const;

private Q_SLOTS:
    void slotMouseChanged(const QPointF &pos,
                          const QPointF &old,
                          Qt::MouseButtons buttons,
                          Qt::MouseButtons oldbuttons,
                          Qt::KeyboardModifiers modifiers,
                          Qt::KeyboardModifiers oldmodifiers);

private:
    void repaint();
    void drawKitty(const RenderTarget &renderTarget, const RenderViewport &viewport);

    QColor m_kittyColor;
    QPointF m_cursorPos;
    bool m_pressed = false;
};

} // namespace
