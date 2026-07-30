modded class SCR_DamageArea
{
	//------------------------------------------------------------------------------------------------
	//! Attempts to add this effect to the entity
	//! \param[in] entity
	protected override void AddEffect(notnull IEntity entity)
	{
		const SCR_ExtendedDamageManagerComponent damageManager = SCR_ExtendedDamageManagerComponent.Cast(SCR_DamageManagerComponent.GetDamageManager(entity));
		if (!damageManager)
			return;

		HitZone hitZone = GetAffectedHitZone(damageManager, m_eHitZoneSelectionMode);
		if (!hitZone)
			return;
		
		if (m_eDamageType == EDamageType.FRZN)
			hitZone = damageManager.GetHitZoneByName("Head");

		if (m_fDamageValue == 0 || m_eDamageType == int.MAX) //int.MAX is equal to selecting in the prefab the option 'none' for m_eDamageType
		{
			damageManager.AddDamageEffect(GetDamageEffect(damageManager, hitZone));
			return;
		}

		const IEntity owner = GetParent();
		vector posDirNorm[3];
		posDirNorm[0] = entity.GetOrigin();
		SCR_DamageContext context = new SCR_DamageContext(m_eDamageType, m_fDamageValue, posDirNorm,
														entity, hitZone, Instigator.CreateInstigator(owner),
														null, 0, 0);

		context.damageEffect = GetDamageEffect(damageManager, hitZone);
		context.damageSource = owner;
		damageManager.HandleDamage(context);
	}
}
