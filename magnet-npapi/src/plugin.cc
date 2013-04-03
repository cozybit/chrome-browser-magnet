/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
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
 *
 * ***** END LICENSE BLOCK ***** */

//////////////////////////////////////////////////
//
// MagnetPlugin class implementation
//

#include <signal.h>
#include <unistd.h>

#include <string.h>

#include <stdio.h>

#include <string>
#include <map>

#include "plugin.h"

#include "Magnet.h"

#define LOG_PRINT_DEBUG(level, application, fmt, ...) \
    fprintf (stderr, "[libmagnet] "fmt"\n", ##__VA_ARGS__)

const char* TEMP_PATH = "/tmp";

const char* METHOD_SAY_HELLO = "sayHello";
const char* METHOD_IS_INITIALIZED = "isInitialized";
const char* METHOD_GET_ERROR = "getError";
const char* METHOD_IS_LISTENING = "isListening";
const char* METHOD_IS_JOINED = "isJoined";
const char* METHOD_GET_MESSAGES = "getMessages";

const char* MAGNET_CHANNEL_NAME = "DemoChannel";

static NPClass plugin_ref_obj = {
     NP_CLASS_STRUCT_VERSION,
     ScriptablePluginObject::Allocate,
     ScriptablePluginObject::Deallocate,
     NULL,
     ScriptablePluginObject::HasMethod,
     ScriptablePluginObject::Invoke,
     ScriptablePluginObject::InvokeDefault,
     ScriptablePluginObject::HasProperty,
     ScriptablePluginObject::GetProperty,
     NULL,
     NULL,
};

bool StringReturnHelper(const char* outString, NPVariant* result)
{
        char* npOutString = (char *)npnfuncs->memalloc(strlen(outString) + 1);

        if (!npOutString)
            return false;

        strcpy(npOutString, outString);
        STRINGZ_TO_NPVARIANT(npOutString, *result);

        return true;
}

class ScriptablePluginObjectPrivate
{
public:
    NPP npp; 
    //MagnetPluginPrivate* pPlugin;

    ScriptablePluginObjectPrivate(NPP instance)
        : npp(instance)
    {
    }
};

class MagnetPluginPrivate
{
public:

  NPP pNPInstance;
  NPWindow * Window;
  NPBool bInitialized;
  ScriptablePluginObject *pScriptableObject;

  std::string error;
  static std::string messages;

#ifdef _WINDOWS
  HWND hWnd; 
#endif

    MagnetPluginPrivate(NPP instance)
        : pNPInstance(instance)
        , bInitialized(false)
        , pScriptableObject(NULL)
    {
    }
};

ScriptablePluginObject::ScriptablePluginObject(NPP instance)
     : d(new ScriptablePluginObjectPrivate(instance))
{
}

ScriptablePluginObject::~ScriptablePluginObject()
{
    delete d;
}

NPObject* ScriptablePluginObject::Allocate(NPP instance, NPClass* npclass)
{
     return (NPObject*)(new ScriptablePluginObject(instance));
}

void ScriptablePluginObject::Deallocate(NPObject* obj)
{
     delete (ScriptablePluginObject*)obj;
}

bool ScriptablePluginObject::HasMethod(NPObject* obj, NPIdentifier methodName)
{
     return true;
}

bool ScriptablePluginObject::InvokeDefault(NPObject* obj, const NPVariant* args,
          uint32_t argCount, NPVariant* result)
{
     return true ;
}

bool ScriptablePluginObject::Invoke(NPObject* obj, NPIdentifier methodName,
                                    const NPVariant* args, uint32_t argCount,
                                    NPVariant* result)
{
    ScriptablePluginObject *thisObj = (ScriptablePluginObject*)obj;

    char* name = npnfuncs->utf8fromidentifier(methodName);
    bool ret_val = false;

    if (!name) {
        return ret_val;
    }
    if (!strcmp(name, METHOD_GET_ERROR)) {
        std::string error = "";

        //error = thisObj->d->pPlugin->error;
        if ( ! StringReturnHelper(error.c_str(), result) )
            return false;

        ret_val = true;
    } else if (!strcmp(name, METHOD_GET_MESSAGES)) {

        std::string messages = "";
#ifdef NOT_YET
        messages = thisObj->d->pPlugin->getMessages();
#endif
        if ( ! StringReturnHelper(messages.c_str(), result) )
            return false;

        ret_val = true;

    } else if ( !strcmp(name, METHOD_IS_LISTENING) ) {

        ret_val = true;
        bool b = false;

#ifdef NOT_YET
        b = thisObj->d->pPlugin->isListening();
#endif

        BOOLEAN_TO_NPVARIANT(b, *result);

    } else if ( !strcmp(name, METHOD_IS_JOINED) ) {

        ret_val = true;
        bool b = false;

#ifdef NOT_YET
        b = thisObj->d->pPlugin->isJoined();
#endif
        BOOLEAN_TO_NPVARIANT(b, *result);

    } else if ( !strcmp(name, METHOD_IS_INITIALIZED) ) {

        ret_val = true;
        bool b = false;

#ifdef NOT_YET
        b = thisObj->d->pPlugin->isInitialized();
#endif
        BOOLEAN_TO_NPVARIANT(b, *result);

    } else if ( !strcmp(name, METHOD_SAY_HELLO) ) {

#ifdef NOT_YET
        stMagnetPayload *payload = MagnetPayloadInit ();

        const char* HELLO_MESSAGE = "<+Hello_From_Chrome+>";
        MagnetPayloadAppendBlob (payload, (unsigned char *) HELLO_MESSAGE, strlen (HELLO_MESSAGE) + 1);

        MagnetHeaderSetType (MagnetPluginPrivate::header, "text/plain");
        MagnetSendData (MagnetPluginPrivate::header, &payload);
#endif

        ret_val = true;
        VOID_TO_NPVARIANT(*result);

    } else {
        // Exception handling.
        npnfuncs->setexception(obj, "Unknown method");
    }

     npnfuncs->memfree(name);

      return ret_val;
}

bool ScriptablePluginObject::HasProperty(NPObject* obj, NPIdentifier propertyName)
{
     return false;
}

bool ScriptablePluginObject::GetProperty(NPObject* obj, NPIdentifier propertyName,
          NPVariant* result)
{
     return false;
}

MagnetPlugin::MagnetPlugin(NPP pNPInstance)
    : d (new MagnetPluginPrivate(pNPInstance))
{
#ifdef _WINDOWS
     m_hWnd = NULL;
#endif

}

MagnetPlugin::~MagnetPlugin()
{
     if (d->pScriptableObject)
          npnfuncs->releaseobject((NPObject*)d->pScriptableObject);
#ifdef _WINDOWS
     d->hWnd = NULL;
#endif
     d->bInitialized = false;

    bool success = true;

	if ( ! MagnetStop() ) 
	{
		success = false;
        LOG_PRINT_DEBUG(MAGNET_LOG_DEBUG, "magnet-npapi", "Failed to stop Magnet");
	}

	if ( success && ! MagnetRelease() )
	{
		success = false;
        LOG_PRINT_DEBUG(MAGNET_LOG_DEBUG, "magnet-npapi", "Failed to release Magnet");
	}
}

NPBool MagnetPlugin::init(NPWindow* pNPWindow)
{
     if(pNPWindow == NULL)
          return false;

#ifdef _WINDOWS
     d->hWnd = (HWND)pNPWindow->window;
     if(d->hWnd == NULL)
          return false;
#endif
    d->Window = pNPWindow;
    d->bInitialized = true;

    bool success = true;

    static stMagnetListener* listener = MagnetListenerInit();

    auto listen_cb = [] (const char* local_name) {
#if 0
        MagnetPluginPrivate::listening = true;
#endif
    };

	MagnetListenerSetOnListeningCB(listener, listen_cb);

    auto join_cb  = [] (stMagnetHeader *header) {
#if 0
        MagnetPluginPrivate::header = header;
        MagnetPluginPrivate::joined = true;
#endif
    };

	MagnetListenerSetOnJoinCB(listener, join_cb);
    
    auto rcv_data_fn = [] (stMagnetHeader *header, stMagnetPayload *payload) {

	    char* result = (char *) MagnetDataGetContents (MagnetPayloadFirst(payload));
        MagnetPluginPrivate::messages += std::string(result);

    };

	MagnetListenerSetOnDataReceivedCB(listener, rcv_data_fn);

    if ( ! MagnetInit(TEMP_PATH) )
    {
        success = false;
        //d->error = "MagnetInit() failed";

        LOG_PRINT_DEBUG(MAGNET_LOG_DEBUG, "magnet-npapi", "failed to init magnet");
    }

	MagnetSetListener(listener);
    MagnetJoinChannel(MAGNET_CHANNEL_NAME);

    if ( success && ! MagnetStart() )
    {
        success = false;
        //d->error = "MagnetStart() failed";

        LOG_PRINT_DEBUG(MAGNET_LOG_DEBUG, "magnet-npapi", "failed to start magnet");
    }

    //d->bInitialized = success;

    return true;
}

NPBool MagnetPlugin::isInitialized()
{
     return d->bInitialized;
}

ScriptablePluginObject * MagnetPlugin::GetScriptableObject()
{
     if (!d->pScriptableObject) {
          d->pScriptableObject = (ScriptablePluginObject*)npnfuncs->createobject(d->pNPInstance, &plugin_ref_obj);
          //d->pScriptableObject->d->pPlugin = d;

          // Retain the object since we keep it in plugin code
          // so that it won't be freed by browser.
          npnfuncs->retainobject((NPObject*)d->pScriptableObject);
     }

     return d->pScriptableObject;
}

#ifdef _WINDOWS
HWND MagnetPlugin::GetHWnd()
{
     return d->hWnd;
}
#endif
