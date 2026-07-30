modded class FDE
{
	override EDamageType GetDefaultDamageType()
	{
		return FRZN_DMG;
	}
	
    private const float FREEZE_FUDGE = 1.37;
	
    override bool ExecuteSynchronizedSoundPlayback(notnull SCR_ExtendedDamageManagerComponent dmgManager)
    {
        /*SCR_CharacterDamageManagerComponent characterDamageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
        if (!characterDamageMgr)
            return false;

        characterDamageMgr.SoundHit(true, EDamageType.INCENDIARY);
        return true;*/
		SCR_CharacterDamageManagerComponent characterDamageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
		if (!characterDamageMgr)
			return false;

		float damageValue = m_fLocalDPSValue;
		bool decaying = m_bIsDecaying;
		SCR_BatchedPoisonDamageEffects batchedData = SCR_BatchedPoisonDamageEffects.Cast(SCR_DamageSufferingSystem.GetInstance().GetBatchedDataOfType(dmgManager, Type()));
		if (batchedData)
		{
			damageValue = batchedData.m_fDamageValue;
			decaying = decaying && batchedData.m_iNumberOfEffects < 2;
		}

		characterDamageMgr.SoundHit(damageValue > CRITICAL_DMG_THRESHOLD && !decaying, EDamageType.INCENDIARY);
		return true;
    }

    override void RecalculateDPS(float timeSlice, notnull SCR_ExtendedDamageManagerComponent dmgManager)
    {
        /*const SCR_CharacterDamageManagerComponent characterDmgManager = SCR_CharacterDamageManagerComponent.Cast(dmgManager);

        int decayFactor = 1;
        if (m_bIsDecaying)
            decayFactor = -1;

        m_fLocalDPSValue = Math.Clamp(
            GetDPS() * FREEZE_FUDGE * (1 + decayFactor * characterDmgManager.GetPoisonBuildupFactor() * timeSlice),
            0,
            MAX_DMG_VALUE
        );
        SetDPS(m_fLocalDPSValue);*/
		const SCR_CharacterDamageManagerComponent characterDmgManager = SCR_CharacterDamageManagerComponent.Cast(dmgManager);

		int decayFactor = 1;
		if (m_bIsDecaying)
			decayFactor = -1;//if it shouldnt expire then we are constantly exposed to the source of the poison

		m_fLocalDPSValue = Math.Clamp(GetDPS() * (1 + decayFactor * characterDmgManager.GetPoisonBuildupFactor() * timeSlice), 0, MAX_DMG_VALUE);
		SetDPS(m_fLocalDPSValue); // poison buildup
    }

    override bool HijackDamageEffect(SCR_ExtendedDamageManagerComponent dmgManager)
	{
	    /*array<ref SCR_PersistentDamageEffect> damageEffects = {};
	    dmgManager.FindAllDamageEffectsOfTypeOnHitZone(FDE, GetAffectedHitZone(), damageEffects);
	
	    foreach (SCR_PersistentDamageEffect dmgEffect : damageEffects)
	    {
	        FDE existing = FDE.Cast(dmgEffect);
	        if (!existing || existing == this)
	            continue;
	
	        existing.SetDPS(existing.GetDPS() + GetDPS());
	        existing.ApplyEffect(dmgManager);
	        return true;
	    }
	
	    return false; */
		// same as with bleeding, this should prevent us from regenerating
		dmgManager.TerminateDamageEffectsOfType(SCR_PhysicalHitZonesRegenDamageEffect);

		array<ref SCR_PersistentDamageEffect> damageEffects = {};
		dmgManager.FindAllDamageEffectsOfTypeOnHitZone(SCR_PoisonDamageEffect, GetAffectedHitZone(), damageEffects);

		SCR_PoisonDamageEffect poisonEffect;
		IEntity instigatorEnt = GetInstigator().GetInstigatorEntity();

		int otherMaxDuration;
		const int thisMaxDuration = GetMaxDuration();
		foreach (SCR_PersistentDamageEffect dmgEffect : damageEffects)
		{
			poisonEffect = SCR_PoisonDamageEffect.Cast(dmgEffect);
			if (!poisonEffect || poisonEffect == this)
				continue;

			if (poisonEffect.GetInstigator().GetInstigatorEntity() != instigatorEnt)
				continue;

			// combine same effects from the same entity
			poisonEffect.SetDPS(poisonEffect.GetDPS() + GetDPS());

			// is that effect supposed to end at some point?
			otherMaxDuration = poisonEffect.GetMaxDuration();
			if (otherMaxDuration > 0)
			{
				if (thisMaxDuration > 0)
					poisonEffect.SetMaxDuration(otherMaxDuration + thisMaxDuration); // if both of us are meant to expire, then lets combine that time
				else
					poisonEffect.SetMaxDuration(0); // if this new effect is not supposed to expire, then we make that old one not expire

				poisonEffect.ApplyEffect(dmgManager);
			}

			return true;
		}

		return false;
	}
	
    override void OnEffectRemoved(SCR_ExtendedDamageManagerComponent dmgManager)
    {
        /*ExecuteSynchronizedSoundPlayback(dmgManager);
        SCR_DamageSufferingSystem.GetInstance().UnregisterEffect(dmgManager, this);

        SCR_CharacterDamageManagerComponent characterDamageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
        if (!characterDamageMgr)
            return;

		characterDamageMgr.TryThaw();
        characterDamageMgr.RegenPhysicalHitZones();*/
		ExecuteSynchronizedSoundPlayback(dmgManager);
		SCR_DamageSufferingSystem.GetInstance().UnregisterEffect(dmgManager, this);
		SCR_PoisonScreenEffect poisonScreenEffect = SCR_PoisonScreenEffect.Cast(GetScreenEffect(dmgManager, SCR_PoisonScreenEffect));
		if (!poisonScreenEffect)
			return;

		poisonScreenEffect.OnDamageEffectRemoved(this);
		
		SCR_CharacterDamageManagerComponent characterDamageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
		if (!characterDamageMgr)
			return;
		
		characterDamageMgr.RegenPhysicalHitZones();
    }
	
	override void OnEffectAdded(SCR_ExtendedDamageManagerComponent dmgManager)
	{
	    /*if (GetDPS() <= 0)
	        SetDPS(3.5);          // guarantee a baseline so it's visible
	
	    m_fLocalDPSValue = GetDPS();
	    super.OnEffectAdded(dmgManager);   // keep poison registration/screen effect
		*/
		m_fLocalDPSValue = GetDPS();
		ExecuteSynchronizedSoundPlayback(dmgManager);
		SCR_DamageSufferingSystem.GetInstance().RegisterEffect(dmgManager, this);
		SCR_PoisonScreenEffect poisonScreenEffect = SCR_PoisonScreenEffect.Cast(GetScreenEffect(dmgManager, SCR_PoisonScreenEffect));
		if (!poisonScreenEffect)
			return;

		poisonScreenEffect.OnDamageEffectAdded(this);
	}
};

/*
modded class DamageEffectEvaluator : ScriptAndConfig
{
	void HandleEffectConsequences(FDE effect, ExtendedDamageManagerComponent dmgManager);
}
*/