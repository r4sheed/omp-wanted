// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_omp_wanted
    #undef _inc_omp_wanted
#endif

// custom include-guard to ensure we don't duplicate
#if defined _omp_wanted_included
	#endinput
#endif
#define _omp_wanted_included

#include <open.mp>

//   .oooooo..o                          ooooo         o8o   .o8                                    o8o                     
//  d8P'    `Y8                          `888'         `"'  "888                                    `"'                     
//  Y88bo.       .ooooo.   .ooooo.        888         oooo   888oooo.  oooo d8b  .oooo.   oooo d8b oooo   .ooooo.   .oooo.o 
//   `"Y8888o.  d88' `88b d88' `88b       888         `888   d88' `88b `888""8P `P  )88b  `888""8P `888  d88' `88b d88(  "8 
//       `"Y88b 888ooo888 888ooo888       888          888   888   888  888      .oP"888   888      888  888ooo888 `"Y88b.  
//  oo     .d8P 888    .o 888    .o       888       o  888   888   888  888     d8(  888   888      888  888    .o o.  )88b 
//  8""88888P'  `Y8bod8P' `Y8bod8P'      o888ooooood8 o888o  `Y8bod8P' d888b    `Y888""8o d888b    o888o `Y8bod8P' 8""888P' 
//                                                                                                                          
//                                                                                                                          
//    

/**
 * <library>omp-wanted</library>
 * <summary>This callback is called when a player wanted level changes.</summary>
 * <param name="playerid">The ID of the player that attained wanted level</param>
 * <param name="newlevel">The amount of wanted stars the player currently have</param>
 * <param name="oldlevel">The amount of wanted stars the player had before</param>
 * <returns>This callback does not handle returns.</returns>
*/
forward OnPlayerWantedLevelChange(playerid, newlevel, oldlevel);

/**
 * <library>omp_wanted</library>
 * <summary>Maximum number of wanted stars that a player can have</summary>
 */
#if !defined MAX_WANTED_LEVEL
	#define MAX_WANTED_LEVEL    (6)
#else
    #assert MAX_WANTED_LEVEL == (6)
#endif

static 
	INC_gsWantedLevel[MAX_PLAYERS];

stock Wanted_SetPlayerWantedLevel(playerid, level)
{
	// printf("Wanted_SetPlayerWantedLevel(%i, %i)", playerid, level);

    level = clamp(level, 0, MAX_WANTED_LEVEL);
    
    new
		ret = SetPlayerWantedLevel(playerid, level);
    if(ret)
    {
        if(INC_gsWantedLevel[playerid] != level)
        {
            // if(level < INC_gsWantedLevel[playerid])
			// {  
			// }
			// else if(level > INC_gsWantedLevel[playerid])
			// {
			// }

			CallRemoteFunction("OnPlayerWantedLevelChange", "iii", playerid, level, INC_gsWantedLevel[playerid]);

            INC_gsWantedLevel[playerid] = level;
        }
    }

    return ret;
}

#if defined _ALS_SetPlayerWantedLevel
    #undef SetPlayerWantedLevel
#else
    #define _ALS_SetPlayerWantedLevel
#endif
#define SetPlayerWantedLevel Wanted_SetPlayerWantedLevel

/**
 * <library>omp-wanted</library>
 * <summary>Gives wanted stars for a player.</summary>
 * <param name="playerid">The ID of the player to set wanted level to</param>
 * <param name="level">The amount of wanted stars to give.</param>
 * <seealso name="SetPlayerWantedLevel" />
 * <returns>
 *   <b><c>true</c></b>: The function executed successfully.<p/>
 *   <b><c>false</c></b>: The function failed to execute. This means the player is not connected.
 * </returns>
 */
stock GivePlayerWantedLevel(playerid, level)
{
    return SetPlayerWantedLevel(playerid, (GetPlayerWantedLevel(playerid) + level));
}

forward OnPlayerDisconnect(playerid, reason);
public OnPlayerDisconnect(playerid, reason)
{
    INC_gsWantedLevel[playerid] = 0;
    #if defined Wanted_OnPlayerDisconnect
        return Wanted_OnPlayerDisconnect(playerid, reason);
    #else
        return 1;
    #endif
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect Wanted_OnPlayerDisconnect
#if defined Wanted_OnPlayerDisconnect
    forward Wanted_OnPlayerDisconnect(playerid, reason);
#endif