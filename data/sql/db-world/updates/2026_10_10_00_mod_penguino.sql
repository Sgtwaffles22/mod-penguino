DELETE FROM `creature_template` WHERE (`entry` = 9000100);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `CreatureImmunitiesId`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(9000100, 0, 0, 0, 0, 0, 'Penguino', '', 'Speak', 0, 100, 200, 0, 35, 129, 1, 1.14286, 1, 1, 1, 3, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 'npc_penguino', 0);

DELETE FROM `creature_template_model` WHERE (`CreatureID` = 9000100);
INSERT INTO `creature_template_model` (`CreatureID`, `Idx`, `CreatureDisplayID`, `DisplayScale`, `Probability`, `VerifiedBuild`) VALUES
(9000100, 0, 28786, 1, 1, 0);

DELETE FROM `creature` WHERE `id` = 9000100 AND `guid` IN (3110424,3110425,3110426,3110427,3110428,3110430,3110431,3110432,3110433,3110434,3110435);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110424,9000100,0,0,0,1,1,0,-8811.6,654.023,96.1942,4.49598,300,0,0,651,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110425,9000100,0,0,0,1,1,0,-4952.48,-924.052,504.261,6.09813,300,0,0,1395,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110426,9000100,1,0,0,1,1,0,9866.29,2329.59,1321.59,3.34733,300,0,0,1110,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110427,9000100,530,0,0,1,1,0,-4025.61,-11726.2,-151.858,6.1371,300,0,0,1110,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110428,9000100,0,0,0,1,1,0,-14413.7,521.134,7.75161,3.45457,300,0,0,1163,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110430,9000100,1,0,0,1,1,0,-7237.6,-3808.9,-1.12609,0.433358,300,0,0,1163,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110431,9000100,1,0,0,1,1,0,1660.36,-4432.35,19.2903,2.02866,300,0,0,4781,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110432,9000100,1,0,0,1,1,0,-1206.72,103.845,134.486,3.20035,300,0,0,3237,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110433,9000100,0,0,0,1,1,0,1556.33,240.659,-43.1018,6.27156,300,0,0,1524,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110434,9000100,530,0,0,1,1,0,9652.44,-7139.75,16.774,5.5718,300,0,0,521,0,0,0,0,0,'',NULL,0,NULL);
INSERT INTO `creature` (guid,id,map,zoneId,areaId,spawnMask,phaseMask,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,wander_distance,currentwaypoint,curhealth,curmana,MovementType,npcflag,unit_flags,dynamicflags,ScriptName,VerifiedBuild,CreateObject,Comment) VALUES (3110435,9000100,530,0,0,1,1,0,9663.31,-7519.63,15.7344,0.728212,300,0,0,605,0,0,0,0,0,'',NULL,0,NULL);

DELETE FROM `npc_vendor` WHERE (`entry` = 9000100);
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(9000100, 0, 20371, 0, 0, 0, 0),
(9000100, 0, 20651, 0, 0, 0, 0),
(9000100, 0, 22114, 0, 0, 0, 0),
(9000100, 0, 22780, 0, 0, 0, 0),
(9000100, 0, 33079, 0, 0, 0, 0),
(9000100, 0, 37012, 0, 0, 0, 0),
(9000100, 0, 39656, 0, 0, 0, 0),
(9000100, 0, 43599, 0, 0, 0, 0),
(9000100, 0, 51809, 0, 0, 0, 0),
(9000100, 0, 54811, 0, 0, 0, 0);
