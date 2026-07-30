/*class FDE : SCR_PoisonDamageEffect
{
    private const float FREEZE_FUDGE = 1.37;

    override bool ExecuteSynchronizedSoundPlayback(notnull SCR_ExtendedDamageManagerComponent dmgManager)
    {
        SCR_CharacterDamageManagerComponent characterDamageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
        if (!characterDamageMgr)
            return false;

        characterDamageMgr.SoundHit(true, EDamageType.INCENDIARY);
        return true;
    }

    override void RecalculateDPS(float timeSlice, notnull SCR_ExtendedDamageManagerComponent dmgManager)
    {
        const SCR_CharacterDamageManagerComponent characterDmgManager = SCR_CharacterDamageManagerComponent.Cast(dmgManager);

        int decayFactor = 1;
        if (m_bIsDecaying)
            decayFactor = -1;

        m_fLocalDPSValue = Math.Clamp(
            GetDPS() * FREEZE_FUDGE * (1 + decayFactor * characterDmgManager.GetPoisonBuildupFactor() * timeSlice),
            0,
            MAX_DMG_VALUE
        );
        SetDPS(m_fLocalDPSValue);
    }

    override bool HijackDamageEffect(SCR_ExtendedDamageManagerComponent dmgManager)
	{
	    array<ref SCR_PersistentDamageEffect> damageEffects = {};
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
	
	    return false; 
	}
	
    override void OnEffectRemoved(SCR_ExtendedDamageManagerComponent dmgManager)
    {
        ExecuteSynchronizedSoundPlayback(dmgManager);
        SCR_DamageSufferingSystem.GetInstance().UnregisterEffect(dmgManager, this);

        SCR_CharacterDamageManagerComponent characterDamageMgr = SCR_CharacterDamageManagerComponent.Cast(dmgManager);
        if (!characterDamageMgr)
            return;

		characterDamageMgr.TryThaw();
        characterDamageMgr.RegenPhysicalHitZones();
		
    }
	
	override void OnEffectAdded(SCR_ExtendedDamageManagerComponent dmgManager)
	{
	    if (GetDPS() <= 0)
	        SetDPS(3.5);          // guarantee a baseline so it's visible
	
	    m_fLocalDPSValue = GetDPS();
	    super.OnEffectAdded(dmgManager);   // keep poison registration/screen effect
	}
};

modded class DamageEffectEvaluator : ScriptAndConfig
{
	void HandleEffectConsequences(FDE effect, ExtendedDamageManagerComponent dmgManager);
}
*/