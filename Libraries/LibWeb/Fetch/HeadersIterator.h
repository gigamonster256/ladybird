/*
 * Copyright (c) 2022, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <LibGC/Ptr.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/Fetch/Headers.h>

namespace Web::Fetch {

class HeadersIterator final : public Bindings::PlatformObject {
    WEB_PLATFORM_OBJECT(HeadersIterator, Bindings::PlatformObject);
    GC_DECLARE_ALLOCATOR(HeadersIterator);

public:
    [[nodiscard]] static GC::Ref<HeadersIterator> create(Headers const&, JS::Object::PropertyKind iteration_kind);

    virtual ~HeadersIterator() override;

    GC::Ref<JS::Object> next();

private:
    virtual void initialize(JS::Realm&) override;
    virtual void visit_edges(JS::Cell::Visitor&) override;

    HeadersIterator(Headers const&, JS::Object::PropertyKind iteration_kind);

    GC::Ref<Headers const> m_headers;
    JS::Object::PropertyKind m_iteration_kind;
    size_t m_index { 0 };
};

}
