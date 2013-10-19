#include "g_local.h"
 
void Mark_Dummy_Spawn_Point(edict_t *ent, int type)
{
ent->s.origin[2] += 60;
ent->dummypointset=1;
ent->dummytype = type; // set what object the dummy is
VectorCopy(ent->s.origin,ent->dummypoint);
}
 
void dummydie(edict_t *ent, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int n;
	 
	if(strcmp(ent->classname,"bowl")==0)
		Add_Ammo (attacker, FindItem("Grenades"), 1);
	if(strcmp(ent->classname,"oven")==0)
		Add_Ammo (attacker, FindItem("Cakes"), 100);

	ent->think=G_FreeEdict;
	ent->nextthink=level.time+0.1;
	 
	for (n= 0; n < 4; n++)
        ThrowGib (ent, "models/objects/gibs/sm_meat/tris.md2", damage, GIB_ORGANIC);
        ThrowGib (ent, "models/objects/gibs/chest/tris.md2", damage, GIB_ORGANIC);
        ThrowHead (ent, "models/objects/gibs/head2/tris.md2", damage, GIB_ORGANIC);
}
 
 
void dummypain (edict_t *ent, edict_t *other, float kick, int damage)
{
	//if (ent->health < (ent->max_health))
		//ent->health = ent->max_health;
	if (level.time < ent->pain_debounce_time)
		return;
	ent->pain_debounce_time = level.time + 1;
	ent->s.frame+=1;
	if (ent->s.frame>5)
        ent->s.frame=0;

}
 
 
void Spawn_Dummy(edict_t *ent)
{
vec3_t  mins = {-16, -16, -24};
vec3_t  maxs = {16, 16, 32};
edict_t *dummy;
 
if (!(ent->dummypointset))
        return;
//gi.cprintf(ent,PRINT_HIGH,"dummy being spawned");
 
 
dummy=G_Spawn();

dummy->s.skinnum=0;
dummy->s.frame=0;
dummy->solid=SOLID_BBOX;
dummy->takedamage=DAMAGE_AIM;
dummy->movetype = MOVETYPE_STEP;
dummy->enemy=dummy;
if(ent->dummytype == 2){
	dummy->classname="bowl";
	dummy->s.modelindex=gi.modelindex ("models/items/invulner/tris.md2");
}
else if(ent->dummytype == 1){
	dummy->classname="oven";
	dummy->s.modelindex=gi.modelindex ("models/objects/barrels/tris.md2");
}else{
	dummy->classname="dummy";
	dummy->s.modelindex=gi.modelindex ("models/monsters/insane/tris.md2");
}
dummy->die=dummydie;
dummy->pain=dummypain;
VectorCopy (mins, dummy->mins);
VectorCopy (maxs, dummy->maxs);
VectorClear (dummy->velocity);
dummy->s.effects = 0;
VectorCopy(ent->dummypoint,dummy->s.origin);
dummy->health=5;
dummy->max_health=ent->max_health;
dummy->mass = 200;
dummy->flags = 0;
dummy->svflags &= ~SVF_DEADMONSTER;
gi.linkentity (dummy);
 
}
/*
void Spawn_Ammo(edict_t *ent)
{
	edict_t *ammo;
	 
	ammo=G_Spawn();


	VectorCopy(ent->s.origin,ammo->s.origin);
	ammo->solid = SOLID_BBOX;
	ammo->model = "models/items/healing/medium/tris.md2";
	ammo->count = 10;
	SpawnItem (ammo, FindItem ("Health"));
	gi.soundindex ("items/n_health.wav");
}*/