modded class SCR_HitZone
{
	const float FREEZING_MULTIPLIER = 0.001;
	
	//------------------------------------------------------------------------------------------------
	override float ComputeEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT)
	{
		float effectiveDamage = super.ComputeEffectiveDamage(damageContext, isDOT);
		if (damageContext.damageType == EDamageType.FRZN)
			return FREEZING_MULTIPLIER;
		
		return effectiveDamage;
	}
}
