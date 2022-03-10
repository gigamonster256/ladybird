/*
 * Copyright (c) 2018-2022, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibGfx/StylePainter.h>
#include <LibWeb/HTML/HTMLImageElement.h>
#include <LibWeb/Layout/CheckBox.h>
#include <LibWeb/Painting/CheckBoxPaintable.h>

namespace Web::Painting {

NonnullOwnPtr<CheckBoxPaintable> CheckBoxPaintable::create(Layout::CheckBox const& layout_box)
{
    return adopt_own(*new CheckBoxPaintable(layout_box));
}

CheckBoxPaintable::CheckBoxPaintable(Layout::CheckBox const& layout_box)
    : Paintable(layout_box)
{
}

Layout::CheckBox const& CheckBoxPaintable::layout_box() const
{
    return static_cast<Layout::CheckBox const&>(m_layout_box);
}

void CheckBoxPaintable::paint(PaintContext& context, PaintPhase phase) const
{
    if (!is_visible())
        return;

    Paintable::paint(context, phase);

    if (phase == PaintPhase::Foreground)
        Gfx::StylePainter::paint_check_box(context.painter(), enclosing_int_rect(absolute_rect()), context.palette(), layout_box().dom_node().enabled(), layout_box().dom_node().checked(), layout_box().being_pressed());
}

}
