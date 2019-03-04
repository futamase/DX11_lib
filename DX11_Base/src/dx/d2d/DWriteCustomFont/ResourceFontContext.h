// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//
//----------------------------------------------------------------------------

#pragma once

#include "Common.h"

class ResourceFontContext
{
public:
    ResourceFontContext();
    ~ResourceFontContext();

	void SetDwriteFactory(IDWriteFactory* factory) { m_pFactory = factory; }

    HRESULT Initialize();

    HRESULT CreateFontCollection(
        void const* fontCollectionKey,  // [keySize] in bytes
        UINT32 keySize,
        OUT IDWriteFontCollection** result
        );

private:
    // Not copyable or assignable.
    ResourceFontContext(ResourceFontContext const&);
    void operator=(ResourceFontContext const&);

    HRESULT InitializeInternal();

	IDWriteFactory* m_pFactory;
	bool m_init = false;

    // Error code from Initialize().
    HRESULT hr_;
};