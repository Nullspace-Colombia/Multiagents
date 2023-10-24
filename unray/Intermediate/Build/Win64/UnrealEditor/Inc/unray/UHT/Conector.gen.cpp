// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "unray/Classes/Conector.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeConector() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_APawn();
	UNRAY_API UClass* Z_Construct_UClass_AConector();
	UNRAY_API UClass* Z_Construct_UClass_AConector_NoRegister();
	UPackage* Z_Construct_UPackage__Script_unray();
// End Cross Module References
	void AConector::StaticRegisterNativesAConector()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AConector);
	UClass* Z_Construct_UClass_AConector_NoRegister()
	{
		return AConector::StaticClass();
	}
	struct Z_Construct_UClass_AConector_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AConector_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_APawn,
		(UObject* (*)())Z_Construct_UPackage__Script_unray,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AConector_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Conector.h" },
		{ "ModuleRelativePath", "Classes/Conector.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AConector_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AConector>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AConector_Statics::ClassParams = {
		&AConector::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AConector_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AConector_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AConector()
	{
		if (!Z_Registration_Info_UClass_AConector.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AConector.OuterSingleton, Z_Construct_UClass_AConector_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AConector.OuterSingleton;
	}
	template<> UNRAY_API UClass* StaticClass<AConector>()
	{
		return AConector::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AConector);
	AConector::~AConector() {}
	struct Z_CompiledInDeferFile_FID_Users_Valentina_Documents_1_Universidad_AI_Multiagents_unray_HostProject_Plugins_unray_Source_unray_Classes_Conector_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Valentina_Documents_1_Universidad_AI_Multiagents_unray_HostProject_Plugins_unray_Source_unray_Classes_Conector_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AConector, AConector::StaticClass, TEXT("AConector"), &Z_Registration_Info_UClass_AConector, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AConector), 1358821732U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_Valentina_Documents_1_Universidad_AI_Multiagents_unray_HostProject_Plugins_unray_Source_unray_Classes_Conector_h_2026662235(TEXT("/Script/unray"),
		Z_CompiledInDeferFile_FID_Users_Valentina_Documents_1_Universidad_AI_Multiagents_unray_HostProject_Plugins_unray_Source_unray_Classes_Conector_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_Valentina_Documents_1_Universidad_AI_Multiagents_unray_HostProject_Plugins_unray_Source_unray_Classes_Conector_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
