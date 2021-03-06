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
#ifndef INCLUDED_SFX2_INC_SIDEBAR_CONTEXTCHANGEBROADCASTER_HXX
#define INCLUDED_SFX2_INC_SIDEBAR_CONTEXTCHANGEBROADCASTER_HXX

#include <com/sun/star/frame/XFrame.hpp>

namespace cssu = ::com::sun::star::uno;

namespace sfx2 { namespace sidebar {


/** This class is a helper for broadcasting context changes that are
    tied to shells being activated or deactivated.
*/
class ContextChangeBroadcaster
{
public:
    ContextChangeBroadcaster (void);
    ~ContextChangeBroadcaster (void);

    void Initialize (const ::rtl::OUString& rsContextName);

    void Activate (const cssu::Reference<css::frame::XFrame>& rxFrame);
    void Deactivate (const cssu::Reference<css::frame::XFrame>& rxFrame);

    /** Enable or disable the broadcaster.
        @param bIsEnabled
            The new value of the "enabled" state.
        @return
            The old value of the "enabled" state is returned.
    */
    bool SetBroadcasterEnabled (const bool bIsEnabled);

private:
    rtl::OUString msContextName;
    bool mbIsBroadcasterEnabled;

    void BroadcastContextChange (
        const cssu::Reference<css::frame::XFrame>& rxFrame,
        const ::rtl::OUString& rsModuleName,
        const ::rtl::OUString& rsContextName);
    ::rtl::OUString GetModuleName (
        const cssu::Reference<css::frame::XFrame>& rxFrame);
};


} } // end of namespace ::sd::sidebar

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
