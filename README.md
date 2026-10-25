# Penguino

This AzerothCore module contains Penguino's C++ gossip script and level 80 gear
presets. It does not require ALE or Eluna.

## Installation and missing NPC recovery

Build the module into `worldserver` and apply
`data/sql/db-world/updates/2026_10_10_00_mod_penguino.sql` to the world database.
The SQL installs creature entry `9000100`, its model, eleven world spawns, and
vendor inventory, and binds the NPC to `npc_penguino`.

With `compose.podman.yaml`, the `ac-db-import` service applies new core and module
SQL updates before `worldserver` starts. The updater skips an unchanged SQL file
that is already recorded in the world's `updates` table, even if its NPC data
has subsequently been deleted. A rebuild or restart alone will not restore it.

If Penguino is missing, check the template and spawns for entry `9000100`.
Deliberately reapply the installation SQL to restore deleted NPC data, then
restart `worldserver` to load it. Check that the eleven reserved spawn GUIDs in
the SQL are available before reapplying it. Restoring the database rows does not
require rebuilding a server that already contains `npc_penguino`.

## Missing level 80 boost menu

The boost option is always added by the current C++ script. If Penguino's menu
only shows the vendor and gold options, check whether the running `worldserver`
binary includes the boost text:

```bash
podman exec ac-worldserver grep -a -q -F \
  'Boost me to level 80 with best-in-slot gear.' /azerothcore/env/dist/bin/worldserver
```

An exit status of `1` means the running binary lacks the boost option. Mounting
the module source into a container does not update its compiled C++ scripts.
Build and recreate the worldserver service from the repository root:

```bash
podman compose -f compose.podman.yaml build ac-worldserver
podman compose -f compose.podman.yaml up -d --no-deps --force-recreate ac-worldserver
```

Run these commands as the same user that owns the live Podman stack (`hserver`
on this server), without `sudo`. Other users have separate image and container
stores. Restarting an existing container keeps its original image; recreating
the service deploys the newly built image.

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

