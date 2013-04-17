/* ***** BEGIN LICENSE BLOCK *****
* Version: MPL 1.1/GPL 2.0/LGPL 2.1
* This code was based on the npsimple.c sample code in Gecko-sdk.
*
* The contents of this file are subject to the Mozilla Public License Version
* 1.1 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
* for the specific language governing rights and limitations under the
* License.
*
* Contributor(s):
*
* Alternatively, the contents of this file may be used under the terms of
* either the GNU General Public License Version 2 or later (the "GPL"), or 
* the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
* in which case the provisions of the GPL or the LGPL are applicable instead
* of those above. If you wish to allow use of your version of this file only
* under the terms of either the GPL or the LGPL, and not to allow others to
* use your version of this file under the terms of the NPL, indicate your
* decision by deleting the provisions above and replace them with the notice
* and other provisions required by the GPL or the LGPL. If you do not delete
* the provisions above, a recipient may use your version of this file under
* the terms of any one of the NPL, the GPL or the LGPL.
* ***** END LICENSE BLOCK ***** */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "plugin.h"

bool _getEnvDebugFlag()
{
    char* e = getenv("MAGNET_NPAPI_DEBUG");
    return e != 0 && strlen(e) > 0 && strcmp(e, "0") != 0;
}

#if !defined(NDEBUG) || defined(_DEBUG)

    const int g_WAIT_FOR_SIGCONT_TIMEOUT = 60;

    volatile int caught_signal = 0;

    void sigcont_handler(int sig)
    {
        caught_signal = sig; 
    }

    void pause_jam()
    {
        caught_signal = 0;
        int s = g_WAIT_FOR_SIGCONT_TIMEOUT;

        while(s-- > 0 && caught_signal == 0)
            sleep(1);
    }

    class SigHandleInit
    {
    public:
        SigHandleInit() {
            signal(SIGCONT, sigcont_handler);
        }
    };

    static SigHandleInit sigHandleInit;

#endif

const char* MAGNET_PLUGIN_NAME = "MagnetPlugin";
const char* MAGNET_PLUGIN_DESC = "MagnetPlugin for doing magnet communications in the browser";

NPError NPP_GetValue(NPP instance, NPPVariable variable, void* value) {
  switch(variable) {
  default:
    return NPERR_GENERIC_ERROR;
  case NPPVpluginNameString:
    *((char **)value) = const_cast<char*>(MAGNET_PLUGIN_NAME);
    break;
  case NPPVpluginDescriptionString:
    *((char **)value) = const_cast<char*>(MAGNET_PLUGIN_DESC);
    break;
  case NPPVpluginScriptableNPObject: {
      if(instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;
      MagnetPlugin* pPlugin = (MagnetPlugin*)instance->pdata;
      if(pPlugin == NULL)
        return NPERR_GENERIC_ERROR;
      *(NPObject **)value = (NPObject*)pPlugin->GetScriptableObject();
    }
    break;
  case NPPVpluginNeedsXEmbed:
    *((char *)value) = 1;
    break;
  }
  return NPERR_NO_ERROR;
}

NPError NPP_New(NPMIMEType pluginType, NPP instance,
                uint16_t mode, int16_t argc, char* argn[],
                char* argv[], NPSavedData* saved) {

    static bool g_PAUSED_ONCE = false;

    if ( ! g_PAUSED_ONCE )
    {
        g_PAUSED_ONCE = true;

        if ( _getEnvDebugFlag() )
            pause_jam();
    }

    if(instance == NULL)
        return NPERR_INVALID_INSTANCE_ERROR;

#ifdef _WINDOWS
    int bWindowed = 1;
#else
    int bWindowed = 0;
#endif

    npnfuncs->setvalue(instance, NPPVpluginWindowBool, (void *)(size_t)bWindowed);

    MagnetPlugin * pPlugin = new MagnetPlugin(instance);
    if(pPlugin == NULL)
        return NPERR_OUT_OF_MEMORY_ERROR;

    // window just created
    if(!pPlugin->isInitialized()) 
    { 
        if(!pPlugin->init(nullptr)) {
            delete pPlugin;
            pPlugin = NULL;
            return NPERR_MODULE_LOAD_FAILED_ERROR;
        }
    }

    instance->pdata = (void *)pPlugin;

    return NPERR_NO_ERROR;
}

NPError NPP_Destroy(NPP instance, NPSavedData** save) {
  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  MagnetPlugin * pPlugin = (MagnetPlugin *)instance->pdata;
  if(pPlugin != NULL)
    delete pPlugin;
  return NPERR_NO_ERROR;
}

NPError NPP_SetWindow(NPP instance, NPWindow* window) {
  if(instance == NULL)
    return NPERR_INVALID_INSTANCE_ERROR;

  if(window == NULL)
    return NPERR_GENERIC_ERROR;

  MagnetPlugin * pPlugin = (MagnetPlugin *)instance->pdata;
  if(pPlugin == NULL) 
    return NPERR_GENERIC_ERROR;

  return NPERR_NO_ERROR;
}

NPError NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream,
                      NPBool seekable, uint16_t* stype) {

  return NPERR_GENERIC_ERROR;
}

NPError NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason) {
  return NPERR_GENERIC_ERROR;
}

int16_t NPP_HandleEvent(NPP instance, void* event) {
  return 0;
}

