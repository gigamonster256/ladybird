/*
 * Copyright (c) 2021, Tim Flynn <trflynn89@serenityos.org>
 * Copyright (c) 2022, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibGUI/Event.h>
#include <LibGfx/Painter.h>
#include <LibGfx/StylePainter.h>
#include <LibWeb/DOM/Document.h>
#include <LibWeb/HTML/BrowsingContext.h>
#include <LibWeb/Layout/Label.h>
#include <LibWeb/Layout/RadioButton.h>
#include <LibWeb/Painting/RadioButtonPaintable.h>

namespace Web::Layout {

RadioButton::RadioButton(DOM::Document& document, HTML::HTMLInputElement& element, NonnullRefPtr<CSS::StyleProperties> style)
    : LabelableNode(document, element, move(style))
{
    set_intrinsic_width(12);
    set_intrinsic_height(12);
}

RadioButton::~RadioButton()
{
}

void RadioButton::handle_mousedown(Badge<EventHandler>, const Gfx::IntPoint&, unsigned button, unsigned)
{
    if (button != GUI::MouseButton::Primary || !dom_node().enabled())
        return;

    m_being_pressed = true;
    set_needs_display();

    m_tracking_mouse = true;
    browsing_context().event_handler().set_mouse_event_tracking_layout_node(this);
}

void RadioButton::handle_mouseup(Badge<EventHandler>, const Gfx::IntPoint& position, unsigned button, unsigned)
{
    if (!m_tracking_mouse || button != GUI::MouseButton::Primary || !dom_node().enabled())
        return;

    // NOTE: Changing the checked state of the DOM node may run arbitrary JS, which could disappear this node.
    NonnullRefPtr protect = *this;

    bool is_inside_node_or_label = enclosing_int_rect(m_paint_box->absolute_rect()).contains(position);
    if (!is_inside_node_or_label)
        is_inside_node_or_label = Label::is_inside_associated_label(*this, position);

    if (is_inside_node_or_label)
        set_checked_within_group();

    m_being_pressed = false;
    m_tracking_mouse = false;
    browsing_context().event_handler().set_mouse_event_tracking_layout_node(nullptr);
}

void RadioButton::handle_mousemove(Badge<EventHandler>, const Gfx::IntPoint& position, unsigned, unsigned)
{
    if (!m_tracking_mouse || !dom_node().enabled())
        return;

    bool is_inside_node_or_label = enclosing_int_rect(m_paint_box->absolute_rect()).contains(position);
    if (!is_inside_node_or_label)
        is_inside_node_or_label = Label::is_inside_associated_label(*this, position);

    if (m_being_pressed == is_inside_node_or_label)
        return;

    m_being_pressed = is_inside_node_or_label;
    set_needs_display();
}

void RadioButton::handle_associated_label_mousedown(Badge<Label>)
{
    m_being_pressed = true;
    set_needs_display();
}

void RadioButton::handle_associated_label_mouseup(Badge<Label>)
{
    // NOTE: Changing the checked state of the DOM node may run arbitrary JS, which could disappear this node.
    NonnullRefPtr protect = *this;

    set_checked_within_group();
    m_being_pressed = false;
}

void RadioButton::handle_associated_label_mousemove(Badge<Label>, bool is_inside_node_or_label)
{
    if (m_being_pressed == is_inside_node_or_label)
        return;

    m_being_pressed = is_inside_node_or_label;
    set_needs_display();
}

void RadioButton::set_checked_within_group()
{
    if (dom_node().checked())
        return;

    dom_node().set_checked(true, HTML::HTMLInputElement::ChangeSource::User);
    String name = dom_node().name();

    document().for_each_in_inclusive_subtree_of_type<HTML::HTMLInputElement>([&](auto& element) {
        if (element.checked() && (element.layout_node() != this) && (element.name() == name))
            element.set_checked(false, HTML::HTMLInputElement::ChangeSource::User);
        return IterationDecision::Continue;
    });
}

OwnPtr<Painting::Paintable> RadioButton::create_paintable() const
{
    return Painting::RadioButtonPaintable::create(*this);
}

}
