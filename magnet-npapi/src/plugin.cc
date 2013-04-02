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

#define LOG_PRINT_DEBUG(level, application, fmt, ...) fprintf (stderr, "[libmagnet] "fmt"\n", ##__VA_ARGS__)

const char* METHOD_SAY_HELLO = "sayHello";
const char* METHOD_IS_INITIALIZED = "isInitialized";
const char* METHOD_GET_ERROR = "getError";
const char* METHOD_IS_LISTENING = "isListening";
const char* METHOD_IS_JOINED = "isJoined";
const char* METHOD_GET_MESSAGES = "getMessages";

const char* MAGNET_CHANNEL_NAME = "DemoChannel";

const char *TEMP_PATH= "/tmp/";

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

class ScriptablePluginObjectPrivate
{
public:
    MagnetPlugin* pPlugin;
    NPP npp;

    ScriptablePluginObjectPrivate(NPP instance)
        : pPlugin(0)
        , npp (instance)
    {
    }
};

class MagnetPluginPrivate
{
public:
    static bool listening;
    static bool joined;
    static stMagnetListener* listener;
    static stMagnetHeader* header;
    static std::string messages;

    NPP pNPInstance;
    NPWindow * Window;
    NPBool bInitialized;
    ScriptablePluginObject *pScriptableObject;
    bool magnetInitialized;
    std::string error;

#ifdef _WINDOWS
    HWND m_hWnd; 
#endif

    MagnetPluginPrivate(NPP pNPInstanceIn)
        : pNPInstance(pNPInstanceIn)
        , bInitialized(false)
        , pScriptableObject(NULL)
    {
    }
};

stMagnetHeader* MagnetPluginPrivate::header = 0;
stMagnetListener* MagnetPluginPrivate::listener = 0;

bool MagnetPluginPrivate::listening = false;
bool MagnetPluginPrivate::joined = false;

ScriptablePluginObject::ScriptablePluginObject(NPP instance)
    : d(new ScriptablePluginObjectPrivate(instance))
{
}

ScriptablePluginObject::~ScriptablePluginObject()
{
    delete d;
}

void
ScriptablePluginObject::SetPlugin(MagnetPlugin* p)
{
    d->pPlugin = p;
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

bool StringReturnHelper(const char* outString, NPVariant* result)
{
        char* npOutString = (char *)npnfuncs->memalloc(strlen(outString) + 1);

        if (!npOutString)
            return false;

        strcpy(npOutString, outString);
        STRINGZ_TO_NPVARIANT(npOutString, *result);

        return true;
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

        std::string error = thisObj->d->pPlugin->error();
        if ( ! StringReturnHelper(error.c_str(), result) )
            return false;

        ret_val = true;

    } else if (!strcmp(name, METHOD_GET_MESSAGES)) {

        std::string messages = thisObj->d->pPlugin->getMessages();
        if ( ! StringReturnHelper(messages.c_str(), result) )
            return false;

        ret_val = true;

    } else if ( !strcmp(name, METHOD_IS_LISTENING) ) {

        ret_val = true;
        BOOLEAN_TO_NPVARIANT(thisObj->d->pPlugin->isListening(), *result);

    } else if ( !strcmp(name, METHOD_IS_JOINED) ) {

        ret_val = true;
        BOOLEAN_TO_NPVARIANT(thisObj->d->pPlugin->isJoined(), *result);

    } else if ( !strcmp(name, METHOD_IS_INITIALIZED) ) {

        ret_val = true;
        BOOLEAN_TO_NPVARIANT(thisObj->d->pPlugin->isInitialized(), *result);

    } else if ( !strcmp(name, METHOD_SAY_HELLO) ) {

        stMagnetPayload *payload = MagnetPayloadInit ();

        const char* HELLO_MESSAGE = "<+Hello_From_Chrome+>";
        MagnetPayloadAppendBlob (payload, (unsigned char *) HELLO_MESSAGE, strlen (HELLO_MESSAGE) + 1);

        MagnetHeaderSetType (MagnetPluginPrivate::header, "text/plain");
        MagnetSendData (MagnetPluginPrivate::header, &payload);

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
    : d(new MagnetPluginPrivate(pNPInstance))
{
#ifdef _WINDOWS
     d->hWnd = NULL;
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

    delete d;
}

std::string
MagnetPlugin::error()
{
    return d->error;
}

bool
MagnetPlugin::isListening()
{
    return MagnetPluginPrivate::listening;
}

std::string
MagnetPlugin::getMessages()
{
    return MagnetPluginPrivate::messages;
}

bool
MagnetPlugin::isJoined()
{
    return MagnetPluginPrivate::joined;
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

    bool success = true;

    d->listener = MagnetListenerInit();

    auto listen_cb = [] (const char* local_name) {
        MagnetPluginPrivate::listening = true;
    };

	MagnetListenerSetOnListeningCB(d->listener, listen_cb);

    auto join_cb  = [] (stMagnetHeader *header) {
        MagnetPluginPrivate::header = header;
        MagnetPluginPrivate::joined = true;
    };

	MagnetListenerSetOnJoinCB(d->listener, join_cb);
    
    auto rcv_data_fn = [] (stMagnetHeader *header, stMagnetPayload *payload) {
	    char* result = (char *) MagnetDataGetContents (MagnetPayloadFirst(payload));
        MagnetPluginPrivate::messages += std::string(result);
    };

	MagnetListenerSetOnDataReceivedCB(d->listener, rcv_data_fn);

    if ( ! MagnetInit(TEMP_PATH) )
    {
        success = false;
        d->error = "MagnetInit() failed";
    }

	MagnetSetListener(d->listener);
    MagnetJoinChannel(MAGNET_CHANNEL_NAME);

    if ( success && ! MagnetStart() )
    {
        success = false;
        d->error = "MagnetStart() failed";
    }

    d->bInitialized = success;

    return true;
}

NPBool MagnetPlugin::isInitialized()
{
     return d->bInitialized;
}

ScriptablePluginObject* MagnetPlugin::GetScriptableObject()
{
     if (!d->pScriptableObject) {

          d->pScriptableObject = 
              (ScriptablePluginObject*)npnfuncs->createobject(d->pNPInstance, &plugin_ref_obj);

          d->pScriptableObject->SetPlugin(this);

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
