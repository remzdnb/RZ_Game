// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RZ_ItemActorPlugin.h"

#define LOCTEXT_NAMESPACE "FRZ_ItemPluginModule"

void FRZ_ItemActorPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRZ_ItemActorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRZ_ItemActorPluginModule, RZ_ItemActorPlugin)