/*
 * This file is part of QZeitgeist.
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
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


#ifndef QZEITGEIST_H_
#define QZEITGEIST_H_

#include <QtGlobal>
#include "qzeitgeist_version.h"
#include "qzeitgeist_defines.h"

#define QZEITGEIST_EXPORT __attribute__ ((visibility ("default")))

#ifndef QZEITGEIST_DEPRECATED
#define QZEITGEIST_DEPRECATED Q_DECL_DEPRECATED
#endif

namespace QZeitgeist
{

typedef void *HANDLE;

enum RelevantResultType {
    RecentRelevantResult = 0,
    RelatedRelevantResult = 1
};

enum StorageState {
    NotAvailableState = 0,
    AvailableState = 1,
    AnyState = 2
};

enum DataModelError {
    InvalidSignatureError = 0,
    NullEventError = 1,
    TooManyResultsError = 2
};

enum ResultType {
    MostRecentEventsResult = 0,
    LeastRecentEventsResult = 1,
    MostRecentSubjectsResult = 2,
    LeastRecentSubjectsResult = 3,
    MostPopularSubjectsResult = 4,
    LeastPopularSubjectsResult = 5,
    MostPopularActorResult = 6,
    LeastPopularActorResult = 7,
    MostRecentActorResult = 8,
    LeastRecentActorResult = 9,
    MostRecentOriginResult = 10,
    LeastRecentOriginResult = 11,
    MostPopularOriginResult = 12,
    LeastPopularOriginResult = 13,
    OldestActorResult = 14,
    MostRecentSubjectInterpretationResult = 15,
    LeastRecentSubjectInterpretationResult = 16,
    MostPopularSubjectInterpretationResult = 17,
    LeastPopularSubjectInterpretationResult = 18,
    MostRecentMimetypeResult = 19,
    LeastRecentMimetypeResult = 20,
    MostPopularMimetypeResult = 21,
    LeastPopularMimetypeResult = 22,
    MostRecentCurrentUrlResult = 23,
    LeastRecentCurrentUrlResult = 24,
    MostPopularCurrentUrlResult = 25,
    LeastPopularCurrentUrlResult = 26,
    MostRecentEventOriginResult = 27,
    LeastRecentEventOriginResult = 28,
    MostPopularEventOriginResult = 29,
    LeastPopularEventOriginResult = 30,
    MostRecentCurrentOriginResult = 31,
    LeastRecentCurrentOriginResult = 32,
    MostPopularCurrentOriginResult = 33,
    LeastPopularCurrentOriginResult = 34,
    RelevancyResult = 100
};

} // namespce QZeitgeist

#endif // QZEITGEIST_H_

