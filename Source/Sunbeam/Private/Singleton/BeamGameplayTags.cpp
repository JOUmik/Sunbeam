// Copyright Project SunBeam. All Rights Reserved.


#include "Singleton/BeamGameplayTags.h"

namespace SunbeamGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(LightSourceBase, "LightSource", "Light Source Base Tag, used to define interactions and visibility");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(LightSource_Sun, "LightSource.Sun", "Gameplay Tag for Sun Light Source");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(LightSource_Moon, "LightSource.Moon", "Gameplay Tag for Moon Light Source");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondarySourceBase, "SecondarySource", "Secondary Light Source Base Tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondarySource_DayLamp, "SecondarySource.DayLamp", "Gameplay Tag for Lamp Secondary Light Source at Day");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(SecondarySource_NightLamp, "SecondarySource.NightLamp", "Gameplay Tag for Lamp Secondary Light Source at Night");
}

