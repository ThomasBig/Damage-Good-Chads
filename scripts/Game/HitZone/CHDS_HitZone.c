modded class SCR_HitZone
{
	//------------------------------------------------------------------------------------------------
	override float ComputeEffectiveDamage(notnull BaseDamageContext damageContext, bool isDOT)
	{
		float effectiveDamage = super.ComputeEffectiveDamage(damageContext, isDOT);
		if (damageContext.damageType == EDamageType.FRZN)
			return 5;
		
		return effectiveDamage;
		
		
		/*if (damageContext.damageValue == 0)
			return 0;

		if (damageContext.damageType == EDamageType.TRUE || damageContext.damageType == EDamageType.PROCESSED_FRAGMENTATION)
			return damageContext.damageValue;

		//apply base multiplier
		float effectiveDamage = damageContext.damageValue * GetBaseDamageMultiplier();

		//apply damage multiplier for this specific damage type
		effectiveDamage *= GetDamageMultiplier(damageContext.damageType);

		//DOT doesn't get affected by damage reduction/thresholds, and neither does healing.
		if (isDOT || effectiveDamage < 0)
			return effectiveDamage;

		//apply flat damage reduction
		effectiveDamage -= GetDamageReduction();

		//if its less than the damage threshold we discard the damage.
		//if the damage to take becomes negative (healing) because of the flat damage reduction, this should reset it back to 0 dmg to take.
		if (effectiveDamage < GetDamageThreshold())
			effectiveDamage = 0;

		return effectiveDamage;*/
	}
}
