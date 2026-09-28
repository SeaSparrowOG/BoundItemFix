#include "Hooks/hooks.h"

namespace Hooks {
	void BoundItemMonitor::Install()
	{
		REL::Relocation<std::uintptr_t> getBaseObjectTarget{ REL::ID(34229), 0x48 };

		if (!REL::Pattern<"E8">().match(getBaseObjectTarget.address())) {
			REX::FAIL("Failed to validate hook pattern, aborting load.");
		}

		auto& trampoline = REL::GetTrampoline();
		_getBaseObject = trampoline.write_call<5>(getBaseObjectTarget.address(), &GetBaseObject);
	}

	RE::EffectSetting* BoundItemMonitor::GetBaseObject(RE::ActiveEffect* a_effect)
	{
		if (a_effect &&
			(a_effect->flags.any(RE::ActiveEffect::Flag::kDual)
				|| a_effect->castingSource == RE::MagicSystem::CastingSource::kInstant)) {
			a_effect->castingSource = RE::MagicSystem::CastingSource::kRightHand;
		}
		return _getBaseObject(a_effect);
	}
}