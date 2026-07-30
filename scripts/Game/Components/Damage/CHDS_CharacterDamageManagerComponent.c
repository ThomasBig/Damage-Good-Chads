modded class SCR_CharacterControllerComponent
{
    // ------------------------------------------------------------------------
    override void OnLifeStateChanged(ECharacterLifeState previousLifeState, ECharacterLifeState newLifeState, bool isJIP)
    {
		m_OnLifeStateChanged.Invoke(previousLifeState, newLifeState, isJIP);
				
		IEntity vehicle = CompartmentAccessComponent.GetVehicleIn(GetCharacter());
		if (!vehicle)
			return;
		
		SCR_VehicleFactionAffiliationComponent vehicleFactionAff = SCR_VehicleFactionAffiliationComponent.Cast(vehicle.FindComponent(SCR_VehicleFactionAffiliationComponent));
		if (!vehicleFactionAff)
			return;
		
		vehicleFactionAff.UpdateOccupantsCount();	
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

modded class SCR_CharacterDamageManagerComponent
{
//-----------------------------------------------------------------------------------------------------------
	protected override void OnDamage(notnull BaseDamageContext damageContext)
	{		
		if (!s_aDamageManagerData.IsIndexValid(m_iDamageManagerDataIndex))
			return;

		Print(damageContext.damageEffect.GetAffectedHitZone().ToString());
		
		ScriptInvoker invoker = s_aDamageManagerData[m_iDamageManagerDataIndex].GetOnDamage(false);
		if (invoker)
			invoker.Invoke(damageContext);

		if (damageContext.damageValue > 0)
			RegenPhysicalHitZones();

		KnockOffTheHelmet(damageContext);
		#ifdef ENABLE_DIAG	
		if (DiagMenu.GetBool(SCR_DebugMenuID.DEBUGUI_CHARACTER_LOG_PLAYER_DAMAGE))
		{
			SCR_HitZone scriptedHz = SCR_HitZone.Cast(damageContext.struckHitZone);
			if (!scriptedHz)
				return;

			IEntity hzOwner = scriptedHz.GetOwner();
			if (!hzOwner)
				return;
			
			string instigatorName;
			int instigatorID = damageContext.instigator.GetInstigatorPlayerID();
			IEntity instigatorEntity = damageContext.instigator.GetInstigatorEntity();
			
			if (instigatorID > 0)
			{
				instigatorName = GetGame().GetPlayerManager().GetPlayerName(instigatorID);
			}
			else
			{
				ResourceName prefabName;
				if (instigatorEntity)
				{
					EntityPrefabData prefabData = instigatorEntity.GetPrefabData();
					if (prefabData)
						prefabName = prefabData.GetPrefabName();
				}
				
				if (prefabName.IsEmpty())
				{
					if (instigatorEntity)
					{
						instigatorName = ((instigatorEntity.GetID()).ToString());
					}
					else
					{
						instigatorName = instigatorEntity.ToString();
					}
				}
				else
				{
					TStringArray strs = new TStringArray;
					prefabName.Split("/", strs, true);
					instigatorName = ((instigatorEntity.GetID()).ToString()) + strs[strs.Count() - 1];
				}
			}
			
			string hzOwnerName;
			int hzOwnerID = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(hzOwner);
			if (hzOwnerID > 0)
			{
				hzOwnerName = GetGame().GetPlayerManager().GetPlayerName(hzOwnerID);
			}
			else
			{
				EntityPrefabData prefabData = hzOwner.GetPrefabData();
				ResourceName prefabName = prefabData.GetPrefabName();
				
				if (prefabName.IsEmpty())
				{
					hzOwnerName = ((hzOwner.GetID()).ToString());
				}
				else
				{
					TStringArray strs = new TStringArray;
					prefabName.Split("/", strs, true);
					hzOwnerName = ((hzOwner.GetID()).ToString()) + strs[strs.Count() - 1];
				}
			}
			
			if (EntityUtils.IsPlayer(instigatorEntity) || EntityUtils.IsPlayer(hzOwner))
				PrintFormat("HIT LOG: (%1) damaged (%2) - [Damage = %3, Speed = %4]", instigatorName, hzOwnerName, damageContext.damageValue, damageContext.impactVelocity);
		}
		#endif
		if (damageContext.damageType == 69)
        {
            SCR_CharacterHitZone hz = SCR_CharacterHitZone.Cast(damageContext.struckHitZone);
			if (hz)
            	ApplyFreeze(hz, 3.5);
			else
				Print("Nope");
        }
	}
}