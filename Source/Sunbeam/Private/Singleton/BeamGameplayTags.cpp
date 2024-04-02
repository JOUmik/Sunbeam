// Copyright Project SunBeam. All Rights Reserved.


#include "Singleton/BeamGameplayTags.h"

namespace SunbeamGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(LightSourceBase, "LightSource", "Light Source Base Tag, used to define interactions and visibility");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(LightSource_Sun, "LightSource.Sun", "Gameplay Tag for Sun Light Source");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(LightSource_Moon, "LightSource.Moon", "Gameplay Tag for Moon Light Source");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondarySourceBase, "SecondarySource", "Secondary Light Source Base Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondarySource_Sunflower, "SecondarySource.Sunflower", "Gameplay Tag for Sunflower Secondary Light Source");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondarySource_Lamp, "SecondarySource.Lamp", "Gameplay Tag for Lamp Secondary Light Source");
}

