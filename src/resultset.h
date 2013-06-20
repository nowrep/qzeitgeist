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

#include "qzeitgeist.h"

namespace QZeitgeist
{

class Event;

class QZEITGEIST_EXPORT ResultSet
{
public:
    ResultSet(HANDLE handle);
    ~ResultSet();

    int size() const;
    int estimatedMatches() const;

    Event nextValue();
    bool hasNext() const;

    int currentPosition() const;
    void reset();

private:
    // Disable copying
    ResultSet(const ResultSet &other);
    ResultSet &operator=(const ResultSet &other);

    HANDLE m_handle;
};

} // namespace QZeitgeist

#endif // QZEITGEIST_RESULTEST_H_
