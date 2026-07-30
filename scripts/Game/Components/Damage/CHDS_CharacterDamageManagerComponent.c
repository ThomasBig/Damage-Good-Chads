modded class FDE : SCR_PoisonDamageEffect
{
	override EDamageType GetDefaultDamageType()
	{
		return FRZN_DMG;
	}
	
}

modded class SCR_CharacterControllerComponent
{
    protected FDE m_pFrozenEffectRef;

    // ------------------------------------------------------------------------
    override void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState, bool isJIP)
    {
		
       /* if (isJIP)
            return;

        if (newLifeState == FRZN_ST)
        {
            SCR_ChimeraCharacter c = SCR_ChimeraCharacter.Cast(GetCharacter());
            SCR_CharacterDamageManagerComponent dmg = SCR_CharacterDamageManagerComponent.Cast(c.FindComponent(SCR_CharacterDamageManagerComponent));

            c.SetSpeedLimit(this, 0.1);

            m_pFrozenEffectRef = dmg.m_pFrozenEffect;
        }

        else if (newLifeState == ECharacterLifeState.INCAPACITATED)
        {
            SCR_ChimeraCharacter c = SCR_ChimeraCharacter.Cast(GetCharacter());
            SCR_CharacterDamageManagerComponent dmg = SCR_CharacterDamageManagerComponent.Cast(c.FindComponent(SCR_CharacterDamageManagerComponent));
            dmg.SoundKnockout();
        }
        else if (newLifeState == ECharacterLifeState.DEAD)
        {
            SCR_ChimeraCharacter c = SCR_ChimeraCharacter.Cast(GetCharacter());
            SCR_CharacterDamageManagerComponent dmg = SCR_CharacterDamageManagerComponent.Cast(c.FindComponent(SCR_CharacterDamageManagerComponent));
            dmg.SoundDeath(previousLifeState);
            dmg.RemoveAllBleedingParticlesAfterDeath();
        }*/
    }
}
