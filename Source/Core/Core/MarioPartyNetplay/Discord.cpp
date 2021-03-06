#include "Discord.h"
#include "Core/NetPlayClient.h"
#include "UICommon/DiscordPresence.h"
#include "UICommon/NetPlayIndex.h"


bool mpn_update_discord()
{
  NetPlaySession session;
  DiscordRichPresence RichPresence = {};
  if (!Memory::IsInitialized())
    RichPresence.state = "In a party";
  RichPresence.largeImageKey = CurrentState.Image ? CurrentState.Image : "default";
  RichPresence.largeImageText = CurrentState.Title ? CurrentState.Title : "In-Game";
  RichPresence.partyMax = 4;

  if (CurrentState.Scenes != NULL && CurrentState.Scene != NULL)
    RichPresence.state = CurrentState.Scene->Name.c_str();

  if (CurrentState.Addresses != NULL)
  {
    char Details[128] = "";

    if (CurrentState.Boards && CurrentState.Board)
    {
      snprintf(Details, sizeof(Details), "Players: %d/%d Turn: %d/%d",
          session.player_count,
          RichPresence.partyMax,
          mpn_read_value(CurrentState.Addresses->CurrentTurn, 1),
          mpn_read_value(CurrentState.Addresses->TotalTurns, 1));
      RichPresence.smallImageKey = CurrentState.Board->Icon.c_str();
      RichPresence.smallImageText = CurrentState.Board->Name.c_str();
    }
    else
    {
      RichPresence.smallImageKey = "";
      RichPresence.smallImageText = "";
    }
    RichPresence.details = Details;
  }
  RichPresence.startTimestamp = time(0);
  Discord_UpdatePresence(&RichPresence);
  return true;
}
