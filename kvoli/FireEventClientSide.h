//#include "KillSounds.h"
//
//bool __fastcall hkFireEventClientSide(void* pThis, void*, IGameEvent* event) {
//	if (!strcmp(event->GetName(), "player_death")) {
//		int iVictimUserID = event->GetInt("userid", 0);
//		int iAttackerUserID = event->GetInt("attacker", 0);
//		bool bWasHeadshot = event->GetBool("headshot", false);
//		int iVictimEntIndex = Interfaces->Engine->GetPlayerForUserID(iVictimUserID);
//		int iAttackerEntIndex = Interfaces->Engine->GetPlayerForUserID(iAttackerUserID);
//
//		if (iVictimUserID != iAttackerUserID) {
//			if (iAttackerEntIndex == Interfaces->Engine->GetLocalPlayer()) {
//				Resolver::ResolverPlayer_s& sPlayer = Resolver::ResolverPlayers[iVictimEntIndex];
//				if (!sPlayer.MissedShotsVector.empty())
//					sPlayer.MissedShotsVector.clear();
//			}
//		}
//	}
//	else if (!strcmp(event->GetName(), "round_freeze_end"))
//		//PlaySoundA("C:\\CSGOSound\\you_da_pimp.wav", 0, SND_ASYNC)
//		;
//	else if (!strcmp(event->GetName(), "bomb_planted"))
//		//PlaySoundA("C:\\CSGOSound\\last_chance.wav", 0, SND_ASYNC)
//		;
//	else if (!strcmp(event->GetName(), "round_end")) {
//		CBaseEntity* pLocal = Interfaces->EntList->GetClientEntity(Interfaces->Engine->GetLocalPlayer());
//
//		if (pLocal->m_iTeamNum() == 3 || pLocal->m_iTeamNum() == 2) {
//			int winner = event->GetInt("winner", 0);
//
//			if (winner == pLocal->m_iTeamNum()) {
//				static const char* pSounds[] = {
//					"C:\\CSGOSound\\no_one_better.wav",
//					"C:\\CSGOSound\\made_them_your_bitch.wav",
//					"C:\\CSGOSound\\187_baby.wav",
//					"C:\\CSGOSound\\way_to_go.wav",
//					"C:\\CSGOSound\\youre_on_top_baby.wav",
//					"C:\\CSGOSound\\no_one_touches_the_pimp.wav",
//					"C:\\CSGOSound\\someones_been_busy.wav",
//				};
//
//				//PlaySoundA(pSounds[rand() % (sizeof(pSounds) / 4)], 0, SND_ASYNC);
//			}
//			else {
//				static const char* pSounds[] = {
//					"C:\\CSGOSound\\try_again.wav",
//					"C:\\CSGOSound\\dont_let_those_bitches_own_you.wav",
//					"C:\\CSGOSound\\better_luck_next_time.wav",
//					"C:\\CSGOSound\\all_hype.wav",
//					"C:\\CSGOSound\\you_show_those_bitches_next_time.wav",
//					"C:\\CSGOSound\\keep_up.wav",
//				};
//
//				//PlaySoundA(pSounds[rand() % (sizeof(pSounds) / 4)], 0, SND_ASYNC);
//			}
//		}
//
//		
//	}
//
//	return GameEventManagerVmt->getOrig<bool(__thiscall*)(void*, IGameEvent*)>(8)(pThis, event);
//}
//
////bomb_planted
