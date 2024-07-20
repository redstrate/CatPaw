/*
    SPDX-FileCopyrightText: 2024 Joshua Goins <josh@redstrate.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "catpaw.h"
#include "catpawconfig.h"

#include "core/rendertarget.h"
#include "core/renderviewport.h"
#include "effect/effecthandler.h"

#include <cmath>
#include <opengl/openglcontext.h>

using namespace KWin;

CatPawEffect::CatPawEffect()
{
    effects->hideCursor();

    reconfigure(ReconfigureAll);

    connect(effects, &EffectsHandler::mouseChanged, this, &CatPawEffect::slotMouseChanged);
}

CatPawEffect::~CatPawEffect()
{
    effects->showCursor();
}

void CatPawEffect::reconfigure(ReconfigureFlags)
{
    CatPawConfig::self()->read();
    m_kittyColor = CatPawConfig::kittyColor();
}

void CatPawEffect::paintScreen(const RenderTarget &renderTarget, const RenderViewport &viewport, int mask, const QRegion &region, Output *screen)
{
    effects->paintScreen(renderTarget, viewport, mask, region, screen);

    drawKitty(renderTarget, viewport);
}

void CatPawEffect::postPaintScreen()
{
    effects->postPaintScreen();
    repaint();
}

void CatPawEffect::slotMouseChanged(const QPointF &pos,
                                    const QPointF &,
                                    Qt::MouseButtons buttons,
                                    Qt::MouseButtons oldButtons,
                                    Qt::KeyboardModifiers,
                                    Qt::KeyboardModifiers)
{
    Q_UNUSED(oldButtons)
    m_cursorPos = pos;
    m_pressed = buttons.testFlag(Qt::LeftButton);
    repaint();
}

void CatPawEffect::repaint()
{
    effects->addRepaintFull(); // TODO: use selective repaints
}

void CatPawEffect::drawKitty(const RenderTarget &renderTarget, const RenderViewport &viewport)
{
    // cat paw is unsupported on non-opengl systems
    if (!effects->isOpenGLCompositing())
        return;

    if (const auto context = effects->openglContext()) {
        if (!context->isOpenGLES()) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        }
        glLineWidth(1.0f);
        GLVertexBuffer *vbo = GLVertexBuffer::streamingBuffer();
        vbo->reset();
        const auto scale = viewport.scale();
        ShaderBinder binder(ShaderTrait::UniformColor | ShaderTrait::TransformColorspace);
        binder.shader()->setUniform(GLShader::Mat4Uniform::ModelViewProjectionMatrix, viewport.projectionMatrix());
        binder.shader()->setColorspaceUniforms(ColorDescription::sRGB, renderTarget.colorDescription());
        binder.shader()->setUniform(GLShader::ColorUniform::Color, kittyColor());

        QList<QPointF> marks;
        QList<QVector2D> verts;

        verts.clear();
        verts.reserve(1.0f);

        QPointF arrow_tail;
        arrow_tail.setX(m_cursorPos.x() + 200.0f);
        arrow_tail.setY(viewport.renderRect().bottomRight().y() + 50);
        QPointF arrow_head = m_cursorPos;

        double angle = atan2((double)(arrow_tail.y() - arrow_head.y()), (double)(arrow_tail.x() - arrow_head.x()));

        int width = 70;
        int innerWidth = 45;
        int innerSomething = 4;

        // triangle A
        marks += arrow_head;
        marks += arrow_head + QPoint(innerWidth * cos(angle + M_PI / innerSomething),
                                     innerWidth * sin(angle + M_PI / innerSomething)); // right one
        marks += arrow_head + QPoint(innerWidth * cos(angle - M_PI / innerSomething),
                                     innerWidth * sin(angle - M_PI / innerSomething)); // left one

        // triangle B
        marks += arrow_head + QPoint(innerWidth * cos(angle + M_PI / innerSomething),
                                     innerWidth * sin(angle + M_PI / innerSomething)); // right one
        marks += arrow_tail + QPoint(width * cos(angle + M_PI / 6),
                                     width * sin(angle + M_PI / 6)); // right one
        marks += arrow_tail + QPoint(width * cos(angle - M_PI / 6),
                                     width * sin(angle - M_PI / 6)); // left one

        // triangle C
        marks += arrow_head + QPoint(innerWidth * cos(angle - M_PI / innerSomething),
                                     innerWidth * sin(angle - M_PI / innerSomething)); // right one
        marks += arrow_head + QPoint(innerWidth * cos(angle + M_PI / innerSomething),
                                     innerWidth * sin(angle + M_PI / innerSomething)); // right one
        marks += arrow_tail + QPoint(width * cos(angle - M_PI / 6),
                                     width * sin(angle - M_PI / 6)); // left one

        auto drawCircle = [&](const QPointF position, float size) {
            QList<QVector2D> circleVerts;

            int i;
            int triangleAmount = 20; // # of triangles used to draw circle

            // GLfloat radius = 0.8f; //radius
            GLfloat twicePi = 2.0f * M_PI;

            circleVerts += QVector2D(position * scale); // center of circle
            for (i = 0; i <= triangleAmount; i++) {
                circleVerts +=
                    QVector2D(position.x() + (size * cos(i * twicePi / triangleAmount)), position.y() + (size * sin(i * twicePi / triangleAmount))) * scale;
            }

            vbo->setVertices(circleVerts);
            vbo->render(GL_TRIANGLE_FAN);
        };

        for (const auto &p : std::as_const(marks)) {
            verts.push_back(QVector2D(p.x() * scale, p.y() * scale));
        }

        vbo->setVertices(verts);
        vbo->render(GL_TRIANGLES);

        // draw paw
        drawCircle(m_cursorPos, m_pressed ? 48.0f : 45.0f);

        float beanRadius = 20.0f;
        int beanOut = 50;
        if (m_pressed) {
            beanRadius = 18.0f;
            beanOut = 48;
        }

        drawCircle(m_cursorPos + QPointF(beanOut * cos(angle + M_PI / 1.12), beanOut * sin(angle + M_PI / 1.12)), beanRadius);
        drawCircle(m_cursorPos + QPointF(beanOut * cos(angle - M_PI / 1.12), beanOut * sin(angle - M_PI / 1.12)), beanRadius);

        drawCircle(m_cursorPos + QPointF(beanOut * cos(angle + M_PI / 1.5), beanOut * sin(angle + M_PI / 1.5)), beanRadius);
        drawCircle(m_cursorPos + QPointF(beanOut * cos(angle - M_PI / 1.5), beanOut * sin(angle - M_PI / 1.5)), beanRadius);

        glLineWidth(1.0);
        if (!context->isOpenGLES()) {
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_BLEND);
        }
    }
}

QColor CatPawEffect::kittyColor() const
{
    return m_kittyColor;
}

#include "moc_catpaw.cpp"
