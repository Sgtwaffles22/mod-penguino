#include "Chat.h"
#include "Config.h"
#include "CreatureScript.h"
#include "DBCStores.h"
#include "Item.h"
#include "Log.h"
#include "Mail.h"
#include "ObjectMgr.h"
#include "PenguinoGear.h"
#include "Player.h"
#include "ReputationMgr.h"
#include "ScriptedGossip.h"
#include "StringFormat.h"
#include "Trainer.h"
#include "World.h"
#include <memory>
#include <unordered_map>
#include <vector>

enum PenguinoGossipActions : uint32
{
    PENGUINO_ACTION_GOLD = 2,
    PENGUINO_ACTION_HONOR = 3,
    PENGUINO_ACTION_ARENA_POINTS = 4,
    PENGUINO_ACTION_BOOST = 5,
    PENGUINO_ACTION_BOOST_PVE = 6,
    PENGUINO_ACTION_BOOST_PVP = 7,
    PENGUINO_ACTION_BACK = 8,
    PENGUINO_ACTION_PRESET_START = 100
};

enum PenguinoGossipSenders : uint32
{
    PENGUINO_SENDER_PVE = GOSSIP_SENDER_MAIN + 1,
    PENGUINO_SENDER_PVP = GOSSIP_SENDER_MAIN + 2
};

constexpr uint32 PENGUINO_TEXT_ID = 1;
constexpr uint8 BOOST_LEVEL = 80;
constexpr int32 MONEY_AMOUNT = 999999999;
constexpr int32 ARENA_POINTS_AMOUNT = 10000;
constexpr int32 HONOR_POINTS_AMOUNT = 750000;

namespace
{
    bool IsLevel80BoostEnabled()
    {
        return sConfigMgr->GetOption<bool>("Penguino.EnableLevel80Boost", true);
    }

    uint32 GetFactionItem(Player const* player, uint32 entry)
    {
        for (auto const& [allianceItem, hordeItem] : sObjectMgr->FactionChangeItems)
            if (entry == allianceItem || entry == hordeItem)
                return player->GetTeamId(true) == TEAM_HORDE ? hordeItem : allianceItem;

        return entry;
    }

    void LearnGearProficiency(Player* player, ItemTemplate const* itemTemplate)
    {
        uint32 spell = 0;
        switch (itemTemplate->GetSkill())
        {
            case SKILL_AXES:         spell = 196;   break;
            case SKILL_2H_AXES:      spell = 197;   break;
            case SKILL_MACES:        spell = 198;   break;
            case SKILL_2H_MACES:     spell = 199;   break;
            case SKILL_POLEARMS:     spell = 200;   break;
            case SKILL_SWORDS:       spell = 201;   break;
            case SKILL_2H_SWORDS:    spell = 202;   break;
            case SKILL_STAVES:       spell = 227;   break;
            case SKILL_BOWS:         spell = 264;   break;
            case SKILL_GUNS:         spell = 266;   break;
            case SKILL_DAGGERS:      spell = 1180;  break;
            case SKILL_THROWN:       spell = 2567;  break;
            case SKILL_CROSSBOWS:    spell = 5011;  break;
            case SKILL_WANDS:        spell = 5009;  break;
            case SKILL_FIST_WEAPONS: spell = 15590; break;
            case SKILL_PLATE_MAIL:   spell = 750;   break;
            case SKILL_MAIL:         spell = 8737;  break;
            case SKILL_LEATHER:      spell = 9077;  break;
            case SKILL_CLOTH:        spell = 9078;  break;
            case SKILL_SHIELD:       spell = 9116;  break;
            default:                              break;
        }

        if (spell && !player->HasSpell(spell))
            player->learnSpell(spell, false);
    }

    void LearnClassTrainerSpells(Player* player)
    {
        bool learnedSpell;
        do
        {
            learnedSpell = false;
            for (Trainer::Trainer const* trainer : sObjectMgr->GetClassTrainers(player->getClass()))
            {
                if (!trainer->IsTrainerValidForPlayer(player))
                    continue;

                for (Trainer::Spell const& spell : trainer->GetSpells())
                {
                    if (!trainer->CanTeachSpell(player, &spell))
                        continue;

                    if (spell.IsCastable())
                        player->CastSpell(player, spell.SpellId, true);
                    else
                        player->learnSpell(spell.SpellId, false);

                    if (!trainer->CanTeachSpell(player, &spell))
                        learnedSpell = true;
                }
            }
        } while (learnedSpell);
    }

    void BoostPlayer(Player* player, Creature* creature, Penguino::GearPreset const& preset, bool pvp)
    {
        ChatHandler handler(player->GetSession());
        if (!player->IsAlive() || player->IsInCombat() || player->IsInFlight() || player->GetTradeData())
        {
            handler.SendSysMessage("You must be alive, out of combat, and not flying or trading to use the boost.");
            return;
        }

        if (sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL) < BOOST_LEVEL)
        {
            handler.SendSysMessage("Level 80 boosts are unavailable because this realm's level cap is below 80.");
            return;
        }

        Penguino::GearItems gear = pvp ? preset.PvP : preset.PvE;
        if (pvp && player->getRace(true) == RACE_HUMAN)
            gear[12] = preset.HumanPvPTrinket;

        // Unstored Item objects must leave the player's update queue before being deleted.
        auto deleteItem = [player](Item* item)
        {
            if (item)
            {
                item->RemoveFromUpdateQueueOf(player);
                delete item;
            }
        };
        using PendingItem = std::unique_ptr<Item, decltype(deleteItem)>;
        std::vector<PendingItem> pendingItems;
        std::vector<Item*> inventoryCheck;
        std::unordered_map<uint32, uint32> requiredCounts;
        std::vector<ItemTemplate const*> templates;

        // Create and validate the entire missing set before changing the player's level.
        for (uint32 entry : gear)
        {
            if (!entry)
                continue;

            entry = GetFactionItem(player, entry);
            ItemTemplate const* itemTemplate = sObjectMgr->GetItemTemplate(entry);
            if (!itemTemplate || !(itemTemplate->AllowableClass & player->getClassMask())
                || !(itemTemplate->AllowableRace & player->getRaceMask())
                || itemTemplate->RequiredLevel > BOOST_LEVEL
                || (itemTemplate->HasFlag2(ITEM_FLAG2_FACTION_HORDE) && player->GetTeamId(true) != TEAM_HORDE)
                || (itemTemplate->HasFlag2(ITEM_FLAG2_FACTION_ALLIANCE) && player->GetTeamId(true) != TEAM_ALLIANCE))
            {
                LOG_ERROR("module.penguino", "Invalid level 80 gear item {} for class {}.", entry, preset.Class);
                handler.SendSysMessage("This gear preset is unavailable. Please contact a game master.");
                return;
            }

            templates.push_back(itemTemplate);
            if (player->GetItemCount(entry, true) >= ++requiredCounts[entry])
                continue;

            PendingItem item(Item::CreateItem(entry, 1, player), deleteItem);
            if (!item)
            {
                LOG_ERROR("module.penguino", "Could not create level 80 gear item {}.", entry);
                handler.SendSysMessage("The gear could not be created. Please contact a game master.");
                return;
            }

            inventoryCheck.push_back(item.get());
            pendingItems.push_back(std::move(item));
        }

        uint32 limitedCategory = 0;
        InventoryResult result = player->CanStoreItems(inventoryCheck.data(),
            static_cast<int>(inventoryCheck.size()), &limitedCategory);
        if (result != EQUIP_ERR_OK)
        {
            player->SendEquipError(result, nullptr, nullptr);
            handler.SendSysMessage(Acore::StringFormat(
                "Make room for all {} missing gear items before using the boost.", pendingItems.size()));
            return;
        }

        if (player->GetLevel() < BOOST_LEVEL)
        {
            player->GiveLevel(BOOST_LEVEL);
            if (player->GetLevel() != BOOST_LEVEL)
            {
                handler.SendSysMessage("The level boost was refused. No gear has been awarded.");
                return;
            }

            player->SetUInt32Value(PLAYER_XP, 0);
        }

        LearnClassTrainerSpells(player);
        for (ItemTemplate const* itemTemplate : templates)
        {
            LearnGearProficiency(player, itemTemplate);
            if (itemTemplate->RequiredReputationFaction)
            {
                FactionEntry const* faction = sFactionStore.LookupEntry(itemTemplate->RequiredReputationFaction);
                if (faction && player->GetReputationMgr().GetRank(faction) < itemTemplate->RequiredReputationRank)
                    player->GetReputationMgr().SetReputation(faction,
                        ReputationMgr::ReputationRankToStanding(ReputationRank(itemTemplate->RequiredReputationRank)));
            }
        }

        player->InitTalentForLevel();
        player->UpdateSkillsToMaxSkillsForLevel();

        CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();
        std::vector<PendingItem> mailedItems;
        uint32 awardedCount = static_cast<uint32>(pendingItems.size());
        for (PendingItem& item : pendingItems)
        {
            ItemPosCountVec positions;
            item->SetBinding(true);
            if (player->CanStoreItem(NULL_BAG, NULL_SLOT, positions, item.get()) == EQUIP_ERR_OK)
            {
                Item* storedItem = player->StoreItem(positions, item.get(), true);
                item.release();
                player->SendNewItem(storedItem, 1, true, false);
            }
            else
                mailedItems.push_back(std::move(item));
        }

        // A level/spell hook can change inventory after preflight; keep any overflow in the mailbox.
        for (std::size_t offset = 0; offset < mailedItems.size(); offset += MAX_MAIL_ITEMS)
        {
            MailDraft draft("Penguino level 80 gear", "Here is the remaining gear from your level 80 boost.");
            for (std::size_t index = offset; index < mailedItems.size() && index < offset + MAX_MAIL_ITEMS; ++index)
            {
                Item* item = mailedItems[index].release();
                item->RemoveFromUpdateQueueOf(player);
                item->SaveToDB(trans);
                draft.AddItem(item);
            }

            draft.SendMailTo(trans, MailReceiver(player), MailSender(MAIL_CREATURE, creature->GetEntry()));
        }

        player->SaveToDB(trans, false, false);
        CharacterDatabase.CommitTransaction(trans);
        handler.SendSysMessage(Acore::StringFormat(
            "Level 80 boost complete! Awarded {} items from the {} {} gear preset. Equip your gear and choose talents.",
            awardedCount, preset.Name, pvp ? "PvP" : "PvE"));
        if (!mailedItems.empty())
            handler.SendSysMessage(
                "Some gear was sent to your mailbox because your inventory changed during the boost.");
    }
}

class npc_penguino : public CreatureScript
{
public:
    npc_penguino() : CreatureScript("npc_penguino") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        ClearGossipMenuFor(player);
        AddGossipItemFor(player, GOSSIP_ICON_VENDOR, "Show me what you have for sale.",
            GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
        AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Need gold? Sell your soul!",
            GOSSIP_SENDER_MAIN, PENGUINO_ACTION_GOLD);
        if (IsLevel80BoostEnabled())
        {
            AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Boost me to level 80 with best-in-slot gear.",
                GOSSIP_SENDER_MAIN, PENGUINO_ACTION_BOOST);
        }

        // Add these items to the menu to enable the corresponding rewards.
        // AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Give me Honor Points!",
        //     GOSSIP_SENDER_MAIN, PENGUINO_ACTION_HONOR);
        // AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Give me Arena Points!",
        //     GOSSIP_SENDER_MAIN, PENGUINO_ACTION_ARENA_POINTS);

        SendGossipMenuFor(player, PENGUINO_TEXT_ID, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action) override
    {
        ClearGossipMenuFor(player);

        if (!IsLevel80BoostEnabled()
            && (sender == PENGUINO_SENDER_PVE || sender == PENGUINO_SENDER_PVP
                || action == PENGUINO_ACTION_BOOST || action == PENGUINO_ACTION_BOOST_PVE
                || action == PENGUINO_ACTION_BOOST_PVP))
        {
            CloseGossipMenuFor(player);
            return true;
        }

        if (sender == PENGUINO_SENDER_PVE || sender == PENGUINO_SENDER_PVP)
        {
            if (action >= PENGUINO_ACTION_PRESET_START)
            {
                uint32 index = action - PENGUINO_ACTION_PRESET_START;
                if (index < std::size(Penguino::GearPresets)
                    && Penguino::GearPresets[index].Class == player->getClass())
                    BoostPlayer(player, creature, Penguino::GearPresets[index], sender == PENGUINO_SENDER_PVP);
            }

            CloseGossipMenuFor(player);
            return true;
        }

        if (sender == GOSSIP_SENDER_MAIN)
        {
            switch (action)
            {
                case PENGUINO_ACTION_BOOST:
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Level 80 boost with PvE raid gear.",
                        GOSSIP_SENDER_MAIN, PENGUINO_ACTION_BOOST_PVE);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Level 80 boost with PvP arena gear.",
                        GOSSIP_SENDER_MAIN, PENGUINO_ACTION_BOOST_PVP);
                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back.", GOSSIP_SENDER_MAIN, PENGUINO_ACTION_BACK);
                    SendGossipMenuFor(player, PENGUINO_TEXT_ID, creature);
                    return true;
                case PENGUINO_ACTION_BOOST_PVE:
                case PENGUINO_ACTION_BOOST_PVP:
                {
                    bool pvp = action == PENGUINO_ACTION_BOOST_PVP;
                    for (uint32 index = 0; index < std::size(Penguino::GearPresets); ++index)
                    {
                        auto const& preset = Penguino::GearPresets[index];
                        if (preset.Class != player->getClass())
                            continue;

                        AddGossipItemFor(player, GOSSIP_ICON_CHAT, preset.Name,
                            pvp ? PENGUINO_SENDER_PVP : PENGUINO_SENDER_PVE, PENGUINO_ACTION_PRESET_START + index,
                            Acore::StringFormat("Boost to level 80 and receive the {} {} gear set?",
                                preset.Name, pvp ? "PvP" : "PvE"), 0, false);
                    }

                    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Back.", GOSSIP_SENDER_MAIN, PENGUINO_ACTION_BOOST);
                    SendGossipMenuFor(player, PENGUINO_TEXT_ID, creature);
                    return true;
                }
                case PENGUINO_ACTION_BACK:
                    return OnGossipHello(player, creature);
                case GOSSIP_ACTION_TRADE:
                    player->GetSession()->SendListInventory(creature->GetGUID());
                    break;
                case PENGUINO_ACTION_GOLD:
                    player->ModifyMoney(MONEY_AMOUNT);
                    ChatHandler(player->GetSession()).SendSysMessage(
                        Acore::StringFormat("You have been awarded {} gold!", MONEY_AMOUNT));
                    break;
                case PENGUINO_ACTION_HONOR:
                    player->ModifyHonorPoints(HONOR_POINTS_AMOUNT);
                    ChatHandler(player->GetSession()).SendSysMessage(
                        Acore::StringFormat("You have been awarded {} Honor Points!", HONOR_POINTS_AMOUNT));
                    break;
                case PENGUINO_ACTION_ARENA_POINTS:
                    player->ModifyArenaPoints(ARENA_POINTS_AMOUNT);
                    ChatHandler(player->GetSession()).SendSysMessage(
                        Acore::StringFormat("You have been awarded {} Arena Points!", ARENA_POINTS_AMOUNT));
                    break;
                default:
                    break;
            }
        }

        CloseGossipMenuFor(player);
        return true;
    }
};

void AddSC_npc_penguino()
{
    new npc_penguino();
}
