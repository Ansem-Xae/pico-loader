#pragma once

class LoaderPlatform;

class LoaderPlatformFactory
{
public:
    LoaderPlatform* CreateLoaderPlatform() const;
};
