using System.IO;
using UnrealBuildTool;

public class Opcua_Unreal_Client : ModuleRules
{
    public Opcua_Unreal_Client(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableUndefinedIdentifierWarnings = false;
        PublicIncludePaths.AddRange(
            new string[] {

				// opcua
				Path.Combine(ModuleDirectory, "../Opcua_Unreal_Client/"),

				// open62541
				Path.Combine(ModuleDirectory, "../Open62541/Include/include/"),
                Path.Combine(ModuleDirectory, "../Open62541/Include/include/open62541/"),
                Path.Combine(ModuleDirectory, "../Open62541/Include/build/src_generated/open62541/"),
                Path.Combine(ModuleDirectory, "../Open62541/Include/build/src_generated/"),
                Path.Combine(ModuleDirectory, "../Open62541/Include/plugins/include/open62541/"),
                Path.Combine(ModuleDirectory, "../Open62541/Include/plugins/include/open62541/plugin/"),
                Path.Combine(ModuleDirectory, "../Open62541/Include/include/open62541/plugin/"),
            }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
            }
        );

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Json", "JsonUtilities",
            "TextureCompressor"
        });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "UMG",
                "CommonUI",
                "Slate",
                "SlateCore",
                "HTTP",
                "Sockets",
                "Networking"
            }
        );

        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../Open62541/Lib/open62541.lib"));
        RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../Open62541/Lib/Binaries/Win64/open62541.dll"));
    }
}