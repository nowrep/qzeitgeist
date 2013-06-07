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
#ifndef QZEITGEIST_SYMBOL_H_
#define QZEITGEIST_SYMBOL_H_

#include "qzeitgeist.h"
#include <QtCore/QSharedPointer>
#include <QtCore/QList>

class QUrl;

namespace QZeitgeist
{

class SymbolPrivate;

class QZEITGEIST_EXPORT Symbol
{
public:
    Symbol(const QUrl &url);
    ~Symbol();

    Symbol &operator=(const Symbol &other);
    bool operator==(const Symbol &other) const;

    QUrl url() const;
    QString displayName() const;
    QString description() const;

    QList<QUrl> parents() const;
    QList<QUrl> children() const;

    bool isA(const QUrl &url) const;

    friend QZEITGEIST_EXPORT QDataStream &operator<<(QDataStream &stream, const Symbol &symbol);
    friend QZEITGEIST_EXPORT QDataStream &operator>>(QDataStream &stream, Symbol &symbol);

private:
    QSharedPointer<SymbolPrivate> d;
};

}; // namespace QZeitgeist

#endif // QZEITGEIST_SYMBOL_H_
