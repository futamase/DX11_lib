// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//
//----------------------------------------------------------------------------

#include "Common.h"
//#include "CustomFont.h"
#include "ResourceFontContext.h"
#include "ResourceFontCollectionLoader.h"
#include "ResourceFontFileLoader.h"

ResourceFontContext::ResourceFontContext() : hr_(S_FALSE)
{

}

ResourceFontContext::~ResourceFontContext()
{
	if (hr_ == S_FALSE) return;
	m_pFactory->UnregisterFontCollectionLoader(ResourceFontCollectionLoader::GetLoader());
	m_pFactory->UnregisterFontFileLoader(ResourceFontFileLoader::GetLoader());
}

HRESULT ResourceFontContext::Initialize()
{
    if (hr_ == S_FALSE)
    {
        hr_ = InitializeInternal();
    }

	m_init = true;
    return hr_;
}

HRESULT ResourceFontContext::InitializeInternal()
{
    HRESULT hr = S_OK;

    if (!ResourceFontFileLoader::IsLoaderInitialized()
    ||  !ResourceFontCollectionLoader::IsLoaderInitialized())
    {
        return E_FAIL;
    }

    // Register our custom loaders with the factory object.
    //
    // Note: For this application we just use the shared DWrite factory object which is accessed via 
    //       a global variable. If we were using fonts embedded in *documents* then it might make sense 
    //       to create an isolated factory for each document. When unloading the document, one would
    //       also release the isolated factory, thus ensuring that all cached font data specific to
    //       that document would be promptly disposed of.
    //
    if (FAILED(hr = m_pFactory->RegisterFontFileLoader(ResourceFontFileLoader::GetLoader())))
        return hr;

    hr = m_pFactory->RegisterFontCollectionLoader(ResourceFontCollectionLoader::GetLoader());

    return hr;
}

HRESULT ResourceFontContext::CreateFontCollection(
    void const* fontCollectionKey,  // [keySize] in bytes
    UINT32 keySize,
    OUT IDWriteFontCollection** result
    )
{
    *result = NULL;

    HRESULT hr = S_OK;

    hr = Initialize();
    if (FAILED(hr))
        return hr;

    hr = m_pFactory->CreateCustomFontCollection(
            ResourceFontCollectionLoader::GetLoader(),
            fontCollectionKey,
            keySize,
            result
            );

    return hr;
}