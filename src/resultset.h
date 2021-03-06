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
#ifndef QZEITGEIST_RESULTEST_H_
#define QZEITGEIST_RESULTEST_H_

#include <QZeitgeist/QZeitgeist>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

namespace QZeitgeist
{

class Event;

class QZEITGEIST_EXPORT ResultSet
{
public:
    ResultSet();
    ~ResultSet();

    ResultSet &operator=(const ResultSet &other);

    int size() const;
    int estimatedMatches() const;

    Event nextValue();
    bool hasNext() const;

    int currentPosition() const;
    void reset();

    static ResultSet acquireHandle(HANDLE handle);

private:
    QSharedPointer<class ResultSetPrivate> d;
};

} // namespace QZeitgeist

Q_DECLARE_METATYPE(QZeitgeist::ResultSet)

#endif // QZEITGEIST_RESULTEST_H_
