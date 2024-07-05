#include "stdafx.h"
#include <Spore\Simulator\cPlaceColonyToolStrategy.h>
using namespace Simulator;

void Initialize()
{

}

void Dispose()
{

}

virtual_detour(PlaceColonyStrategy_detour, cPlaceColonyToolStrategy, cToolStrategy, bool(cSpaceToolData*, bool, const char16_t**))
{
	bool detoured(cSpaceToolData * pTool, bool showErrors, const char16_t** ppFailText = nullptr)
	{
		bool canUse = Simulator::cToolStrategy::Update(pTool, showErrors, ppFailText);
		Simulator::cStarRecord* record = Simulator::GetActiveStarRecord();

		//Before detouring update, we check for the following:
		//  - Is this an active star whose owner is the player (and not another empire)? If so then return the Update detour.
		//  - If not then is this a non sentient planet? If so then return the Update detour.
		//  - If not then return false.

		if (record != nullptr && (record->mEmpireID == Simulator::GetPlayerEmpireID() || (int(record->mTechLevel) == 0 || int(record->mTechLevel) == 1)))
		{
			return canUse;
		}
		return false;
	}
};

void AttachDetours()
{
	PlaceColonyStrategy_detour::attach(GetAddress(Simulator::cPlaceColonyToolStrategy, Update));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

