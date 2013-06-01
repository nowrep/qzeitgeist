/*
 * This file is part of QZeitgeist.
 *
 * Copyright (C) 2013 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
extern "C" {
#include <zeitgeist.h>
}

#include "symbol.h"
#include <QtCore/QUrl>

static QList<QUrl> convertGList(GList *list)
{
    QList<QUrl> urls;
    GList *tmp = list;

    while (tmp) {
        gchar *uri = (gchar *) tmp->data;
        urls.append(QUrl(uri));
        tmp = tmp->next;
    }

    g_list_free_full(list, g_free);
    return urls;
}

namespace QZeitgeist
{

class SymbolPrivate
{
public:
    explicit SymbolPrivate(const QUrl &url);

    bool compare(const SymbolPrivate &other) const;
    bool isA(const QUrl &url) const;

    QUrl symbolUrl;
    QString displayName;
    QString description;
    QList<QUrl> parents;
    QList<QUrl> children;
};

SymbolPrivate::SymbolPrivate(const QUrl &url)
    : symbolUrl(url)
{
    QByteArray urlData = url.toString().toUtf8();

    displayName = zeitgeist_symbol_get_display_name(urlData.constData());
    description = zeitgeist_symbol_get_description(urlData.constData());
    parents = convertGList(zeitgeist_symbol_get_all_parents(urlData.constData()));
    children = convertGList(zeitgeist_symbol_get_all_children(urlData.constData()));
}

bool SymbolPrivate::compare(const SymbolPrivate &other) const
{
    return symbolUrl == other.symbolUrl && displayName == other.displayName &&
           description == other.description && parents == other.parents &&
           children == other.children;
}

bool SymbolPrivate::isA(const QUrl &url) const
{
    QByteArray urlData = url.toString().toUtf8();
    QByteArray symbolData = symbolUrl.toString().toUtf8();

    return zeitgeist_symbol_is_a(symbolData.constData(), urlData.constData());
}

// class Symbol
Symbol::Symbol(const QUrl &url)
    : d(new SymbolPrivate(url))
{
}

Symbol::~Symbol()
{
}

Symbol &Symbol::operator=(const Symbol &other)
{
    if (this != &other) {
        d = other.d;
    }

    return *this;
}

bool Symbol::operator==(const Symbol &other) const
{
    return d->compare(*other.d);
}

QUrl Symbol::url() const
{
    return d->symbolUrl;
}

QString Symbol::displayName() const
{
    return d->displayName;
}

QString Symbol::description() const
{
    return d->description;
}

QList<QUrl> Symbol::parents() const
{
    return d->parents;
}

QList<QUrl> Symbol::children() const
{
    return d->children;
}

bool Symbol::isA(const QUrl &url) const
{
    return d->isA(url);
}

}; // namespace QZeitgeist

