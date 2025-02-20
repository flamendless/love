/**
 * Created by bio1712 for love2d
 * Modified and updated by flamendless
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

// LOVE
#include "common/Module.h"
#include "common/runtime.h"
#include "wrap_Ads.h"
#include "sdl/Ads.h"
#include <android/log.h>

//lua
extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

namespace admob
{
	namespace ads
	{
		#define instance() (love::Module::getInstance<Ads>(love::Module::M_ADMOB))

		int w_test(lua_State *L)
		{
			//const char *text = luaL_checkstring(L, 1);
			instance()->test();
			love::luax_pushboolean(L, true);
			return 1;
		}

		int w_createBanner(lua_State *L)
		{
			const char *adID = luaL_checkstring(L, 1);
			const char *position = luaL_checkstring(L, 2);
			instance()->createBanner(adID,position);
			return 0;
		}

		int w_hideBanner(lua_State *L)
		{
			instance()->hideBanner();
			return 0;
		}

		int w_showBanner(lua_State *L)
		{
			instance()->showBanner();
			return 0;
		}

		int w_showInterstitial(lua_State *L)
		{
			instance()->showInterstitial();
			return 0;
		}

		int w_isInterstitialLoaded(lua_State *L)
		{
			bool ret = instance()->isInterstitialLoaded();
			love::luax_pushboolean(L, ret);
			printf("%s", ret ? "true" : "false");
			return 1;
		}

		int w_requestInterstitial(lua_State *L)
		{
			const char *adID = luaL_checkstring(L, 1);
			instance()->requestInterstitial(adID);
			return 0;
		}

		int w_requestRewardedAd(lua_State *L)
		{
			const char *adID = luaL_checkstring(L, 1);
			instance()->requestRewardedAd(adID);
			return 0;
		}

		int w_showRewardedAd(lua_State *L)
		{
			instance()->showRewardedAd();
			return 0;
		}

		int w_isRewardedAdLoaded(lua_State *L)
		{
			bool ret = instance()->isRewardedAdLoaded();
			love::luax_pushboolean(L, ret);
			return 1;
		}

		int w_changeEUConsent(lua_State *L)
		{
			__android_log_print(ANDROID_LOG_VERBOSE, "ADMOBLOG", "w_changeEUConsent");
			instance()->changeEUConsent();
			return 0;
		}

		int w_getDeviceLanguage(lua_State *L)
		{
			__android_log_print(ANDROID_LOG_VERBOSE, "ADMOBLOG", "w_getDeviceLanguage");
			std::string ret = instance()->getDeviceLanguage();
			love::luax_pushstring(L, ret);
			return 1;
		}

		//Private functions for callbacks
		int w_coreInterstitialError(lua_State *L)
		{
			bool ret = instance()->coreInterstitialError();
			love::luax_pushboolean(L, ret);
			return 1;
		}

		int w_coreInterstitialClosed(lua_State *L)
		{
			bool ret = instance()->coreInterstitialClosed();
			love::luax_pushboolean(L, ret);
			return 1;
		}

		int w_coreRewardedAdError(lua_State *L)
		{
			bool ret = instance()->coreRewardedAdError();
			love::luax_pushboolean(L, ret);
			return 1;
		}

		int w_coreRewardedAdDidFinish(lua_State *L)
		{
			bool ret = instance()->coreRewardedAdDidFinish();
			love::luax_pushboolean(L, ret);
			return 1;
		}

		int w_coreGetRewardType(lua_State *L)
		{
			std::string ret = instance()->coreGetRewardType();
			love::luax_pushstring(L, ret);
			return 1;
		}

		int w_coreGetRewardQuantity(lua_State *L)
		{
			int ret = instance()->coreGetRewardQuantity();
			lua_pushnumber(L, ret);
			return 1;
		}

		int w_coreRewardedAdDidStop(lua_State *L)
		{
			bool ret = instance()->coreRewardedAdDidStop();
			love::luax_pushboolean(L, ret);
			return 1;
		}

		static const luaL_Reg functions[] =
		{
			{"test", w_test},
			{"createBanner", w_createBanner},
			{"hideBanner",w_hideBanner},
			{"showBanner",w_showBanner},
			{"requestInterstitial",w_requestInterstitial},
			{"showInterstitial",w_showInterstitial},
			{"isInterstitialLoaded",w_isInterstitialLoaded},
			{"requestRewardedAd", w_requestRewardedAd},
			{"showRewardedAd", w_showRewardedAd},
			{"isRewardedAdLoaded", w_isRewardedAdLoaded},
			{"changeEUConsent", w_changeEUConsent},
			{"getDeviceLanguage", w_getDeviceLanguage},

			//Private functions for callbacks
			{"coreInterstitialError", w_coreInterstitialError},
			{"coreInterstitialClosed", w_coreInterstitialClosed},
			{"coreRewardedAdError", w_coreRewardedAdError},
			{"coreRewardedAdDidFinish", w_coreRewardedAdDidFinish},
			{"coreGetRewardType", w_coreGetRewardType},
			{"coreGetRewardQuantity", w_coreGetRewardQuantity},
			{"coreRewardedAdDidStop", w_coreRewardedAdDidStop},
			{ 0, 0 }
		};

		extern "C" int luaopen_admob(lua_State *L)
		{
			Ads *instance = instance();
			if (instance == nullptr)
				love::luax_catchexcept(L, [&](){ instance = new admob::ads::sdl::Ads(); });
			else
				instance->retain();

			love::WrappedModule w;
			w.module = instance;
			w.name = "ads";
			w.type = &love::Module::type;
			w.functions = functions;
			w.types = nullptr;

			return love::luax_register_module(L, w);
		}
	}
}
