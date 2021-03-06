/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */
#include "Context.hxx"

#define AnyApplicationName "any"
#define AnyContextName "any"

namespace sfx2 { namespace sidebar {

const sal_Int32 Context::NoMatch = 4;
const sal_Int32 Context::ApplicationWildcardMatch = 1;
const sal_Int32 Context::ContextWildcardMatch = 2;
const sal_Int32 Context::OptimalMatch = 0;  // Neither application nor context name is "any".

Context::Context (void)
    : msApplication(AnyApplicationName),
      msContext(AnyContextName)
{
}




Context::Context (
    const ::rtl::OUString& rsApplication,
    const ::rtl::OUString& rsContext)
    : msApplication(rsApplication),
      msContext(rsContext)
{
}




sal_Int32 Context::EvaluateMatch (
    const Context& rOther) const
{
    const bool bApplicationNameIsAny (rOther.msApplication.equalsAscii(AnyApplicationName));
    if (rOther.msApplication.equals(msApplication) || bApplicationNameIsAny)
    {
        // Application name matches.
        const bool bContextNameIsAny (rOther.msContext.equalsAscii(AnyContextName));
        if (rOther.msContext.equals(msContext) || bContextNameIsAny)
        {
            // Context name matches.
            return (bApplicationNameIsAny ? ApplicationWildcardMatch : 0)
                + (bContextNameIsAny ? ContextWildcardMatch : 0);
        }
    }
    return NoMatch;
}







bool Context::operator== (const Context& rOther) const
{
    return msApplication.equals(rOther.msApplication)
        && msContext.equals(rOther.msContext);
}




bool Context::operator!= (const Context& rOther) const
{
    return ( ! msApplication.equals(rOther.msApplication))
        || ( ! msContext.equals(rOther.msContext));
}


} } // end of namespace sfx2::sidebar

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
