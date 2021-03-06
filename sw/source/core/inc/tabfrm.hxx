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
#ifndef INCLUDED_SW_SOURCE_CORE_INC_TABFRM_HXX
#define INCLUDED_SW_SOURCE_CORE_INC_TABFRM_HXX

#include <tools/mempool.hxx>
#include "layfrm.hxx"
#include "flowfrm.hxx"

class SwTable;
class SwBorderAttrs;
class SwAttrSetChg;

/// SwTabFrm is one table in the document layout, containing rows (which contain cells).
class SwTabFrm: public SwLayoutFrm, public SwFlowFrm
{
    friend void CalcCntnt( SwLayoutFrm *pLay, bool bNoColl, bool bNoCalcFollow );

    // does the special treatment for _Get[Next|Prev]Leaf()
    using SwFrm::GetLeaf;
    SwLayoutFrm *GetLeaf( MakePageType eMakePage, bool bFwd );

    SwTable* pTable;

    bool bComplete          :1; //Eintrage als Repaint ohne das CompletePaint
                                    //der Basisklasse gesetzt werden muss. Damit
                                    //sollen unertraegliche Tabellen-Repaints
                                    //vermieden werden.
    bool bCalcLowers        :1; //Im MakeAll auf jedenfall auch fuer Stabilitaet
    //des Inhaltes sorgen.
    bool bLowersFormatted   :1;//Kommunikation zwischen MakeAll und Layact
    bool bLockBackMove      :1; //BackMove-Test hat der Master erledigt.
    bool bResizeHTMLTable   :1; //Resize des HTMLTableLayout rufen im MakeAll
    //Zur Optimierung, damit dies nicht im
    //CntntFrm::Grow gerufen werden muss, denn dann
    //wird es ggf. fuer jede Zelle gerufen #47483#
    bool bONECalcLowers     :1; //Primaer fuer die StarONE-SS. Beim MakeAll werden
    //die Cntnts auf jedenfall per Calc() formatiert.
    //es finden keine zusaetzlichen Invalidierungen
    //statt und dieser Weg kann auch kaum garantien
    //geben.

    bool bHasFollowFlowLine :1; // Means that the first line in the follow
                                // is indented to contain content from a broken
                                // cell
    bool bIsRebuildLastLine :1; // Means that currently the last line of the
                                // TabFrame is rebuilded. In this case we
                                // do not want any notification to the master
                                // table
    bool bRestrictTableGrowth :1;       // Usually, the table may grow infinite,
                                        // because the table can be split in
                                        // SwTabFrm::MakeAll. In MakeAll, this
                                        // flag is set to indicate that the table
                                        // may only grow inside its upper. This
                                        // is necessary, in order to let the text
                                        // flow into the FollowFlowLine
    bool bRemoveFollowFlowLinePending :1;
    // #i26945#
    bool bConsiderObjsForMinCellHeight :1; // Usually, the floating screen objects
                                           // are considered on the calculation
                                           // for the minimal cell height.
                                           // For splitting table rows algorithm
                                           // it's needed not to consider floating
                                           // screen object for the preparation
                                           // of the re-calculation of the
                                           // last table row.
    // #i26945#
    bool bObjsDoesFit :1; // For splitting table rows algorithm, this boolean
                          // indicates, if the floating screen objects fits

    bool mbInRecalcLowerRow : 1;

    //Split() spaltet den Frm an der angegebenen Stelle, es wird ein
    //Follow erzeugt und aufgebaut und direkt hinter this gepastet.
    //Join() Holt sich den Inhalt aus dem Follow und vernichtet diesen.
    bool Split( const SwTwips nCutPos, bool bTryToSplit, bool bTableRowKeep );
    bool Join();

    void _UpdateAttr(
        const SfxPoolItem*,
        const SfxPoolItem*, sal_uInt8 &,
        SwAttrSetChg *pa = 0,
        SwAttrSetChg *pb = 0 );

    virtual bool ShouldBwdMoved( SwLayoutFrm *pNewUpper, bool bHead, bool &rReformat ) SAL_OVERRIDE;

protected:
    virtual void MakeAll() SAL_OVERRIDE;
    virtual void Format( const SwBorderAttrs *pAttrs = 0 ) SAL_OVERRIDE;
    virtual void Modify( const SfxPoolItem*, const SfxPoolItem* ) SAL_OVERRIDE;
        // only changes the Framesize, not the PrtArea size
    virtual SwTwips GrowFrm  ( SwTwips, bool bTst = false, bool bInfo = false ) SAL_OVERRIDE;

public:
    SwTabFrm( SwTable &, SwFrm* );  // calling Regist Flys always after creation _and_pasting!
    SwTabFrm( SwTabFrm & ); // _only_ for the creation of follows
    virtual ~SwTabFrm();

    void JoinAndDelFollows();   // for DelFrms of the TableNodes!

    // calls thr RegistFlys of the rows
    void RegistFlys();

    inline const SwTabFrm *GetFollow() const;
    inline       SwTabFrm *GetFollow();
    SwTabFrm* FindMaster( bool bFirstMaster = false ) const;

    virtual bool GetInfo( SfxPoolItem &rHnt ) const SAL_OVERRIDE;
    virtual void Paint( SwRect const&,
                        SwPrintData const*const pPrintData = NULL ) const SAL_OVERRIDE;
    virtual void CheckDirection( bool bVert ) SAL_OVERRIDE;

    virtual void Cut() SAL_OVERRIDE;
    virtual void Paste( SwFrm* pParent, SwFrm* pSibling = 0 ) SAL_OVERRIDE;

    virtual void Prepare( const PrepareHint ePrep = PREP_CLEAR,
                          const void *pVoid = 0, bool bNotify = true ) SAL_OVERRIDE;

                 SwCntntFrm *FindLastCntnt();
    inline const SwCntntFrm *FindLastCntnt() const;

    const SwTable *GetTable() const { return pTable; }
          SwTable *GetTable()       { return pTable; }

    bool IsComplete()  { return bComplete; }
    void SetComplete() { bComplete = true; }
    void ResetComplete() { bComplete = false; }

    bool IsLowersFormatted() const      { return bLowersFormatted; }
    void SetLowersFormatted( bool b )   { bLowersFormatted = b;    }

    void SetCalcLowers()        { bCalcLowers = true;      } // use rarely
    void SetResizeHTMLTable()   { bResizeHTMLTable = true; } // same
    void SetONECalcLowers()     { bONECalcLowers = true;   }

    // Start: New stuff for breaking table rows

    bool HasFollowFlowLine() const { return bHasFollowFlowLine; }
    void SetFollowFlowLine( bool bNew ) { bHasFollowFlowLine = bNew; }
    //return the SwTabFrm (if any) that this SwTabFrm is a follow flow line for
    SwTabFrm* GetFollowFlowLineFor();

    bool IsRebuildLastLine() const { return bIsRebuildLastLine; }
    void SetRebuildLastLine( bool bNew ) { bIsRebuildLastLine = bNew; }

    bool IsRestrictTableGrowth() const { return bRestrictTableGrowth; }
    void SetRestrictTableGrowth( bool bNew ) { bRestrictTableGrowth = bNew; }

    bool IsRemoveFollowFlowLinePending() const { return bRemoveFollowFlowLinePending; }
    void SetRemoveFollowFlowLinePending( bool bNew ) { bRemoveFollowFlowLinePending = bNew; }

    bool IsInRecalcLowerRow() const
    {
        return mbInRecalcLowerRow;
    }
    void SetInRecalcLowerRow( bool bNew )
    {
        mbInRecalcLowerRow = bNew;
    }

    // #i26945#
    bool IsConsiderObjsForMinCellHeight() const
    {
        return bConsiderObjsForMinCellHeight;
    }
    void SetConsiderObjsForMinCellHeight( bool _bNewConsiderObjsForMinCellHeight )
    {
        bConsiderObjsForMinCellHeight = _bNewConsiderObjsForMinCellHeight;
    }

    // #i26945#
    bool DoesObjsFit() const
    {
        return bObjsDoesFit;
    }
    void SetDoesObjsFit( bool _bNewObjsDoesFit )
    {
        bObjsDoesFit = _bNewObjsDoesFit;
    }

    bool RemoveFollowFlowLine();

    // End: New stuff for breaking table rows

    bool CalcFlyOffsets(
        SwTwips& rUpper,
        long& rLeftOffset,
        long& rRightOffset ) const;

    SwTwips CalcHeightOfFirstContentLine() const;

    bool IsInHeadline( const SwFrm& rFrm ) const;
    SwRowFrm* GetFirstNonHeadlineRow() const;

    bool IsLayoutSplitAllowed() const;

    // #i29550#
    bool IsCollapsingBorders() const;

    sal_uInt16 GetBottomLineSize() const;

    virtual void dumpAsXmlAttributes(xmlTextWriterPtr writer) SAL_OVERRIDE;

    DECL_FIXEDMEMPOOL_NEWDEL(SwTabFrm)
};

inline const SwCntntFrm *SwTabFrm::FindLastCntnt() const
{
    return ((SwTabFrm*)this)->FindLastCntnt();
}

inline const SwTabFrm *SwTabFrm::GetFollow() const
{
    return (const SwTabFrm*)SwFlowFrm::GetFollow();
}
inline SwTabFrm *SwTabFrm::GetFollow()
{
    return (SwTabFrm*)SwFlowFrm::GetFollow();
}

#endif // INCLUDED_SW_SOURCE_CORE_INC_TABFRM_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
