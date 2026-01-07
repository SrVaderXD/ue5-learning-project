using UnrealBuildTool;

public class learning_project : ModuleRules
{
    public learning_project(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
    
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"});

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Blutility" });
        }

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
        
        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
