# Penguino

This AzerothCore module contains Penguino's C++ gossip script and level 80 gear
presets. It does not require ALE.

## Level 80 boost

Talk to Penguino and choose **Boost me to level 80 with best-in-slot gear**.
Select **PvE raid gear** or **PvP arena gear**, choose a specialization for your
class, and accept the in-game confirmation. All ten playable classes are
supported, with 31 specialization presets for each gear mode, including separate
cat and bear Druid sets.

The boost raises characters below level 80 to level 80 and grants the selected
equipment in their bags. Characters already at level 80 can collect either or
both gear sets. Existing equipment and talent selections are preserved. The
boost learns available class trainer spells and the armor/weapon proficiencies
needed for the selected gear, maximizes combat skills, and provides the reputation
required to equip Ashen Verdict rings. Talents, gems, enchants, and equipping the
reward are left to the player.

PvE presets contain heroic ICC/Ruby Sanctum gear, tier 10 pieces, and applicable
legendary weapons. PvP presets contain Wrathful Gladiator gear and appropriate
raid offsets. Faction-specific items are matched to the player; Humans receive
a second throughput trinket instead of a PvP medallion. Item lists, references,
and preset assumptions are documented in [GEAR.md](GEAR.md). Best-in-slot choices
can vary with talent builds, gems, enchants, and group composition.

The reward only adds missing pieces, counting equipped items, bags, and the bank;
dual-wield presets still provide two copies of a weapon when needed. Make room
for the entire missing set before using the boost (up to 17 free bag slots).
If another module changes the inventory while the boost runs, any overflow is
delivered by mail. Boosting requires being alive, out of combat, and not flying
or trading, and a realm level cap of at least 80.

