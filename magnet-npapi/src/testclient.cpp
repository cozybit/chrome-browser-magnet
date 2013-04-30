#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Magnet.h"

void magnet_join_fn (stMagnetHeader *header) {
    printf("Joined: %s\n", MagnetHeaderGetNodeName(header));
}

void rcv_data_fn(stMagnetHeader *header, stMagnetPayload *payload) {
    char* result = (char *) MagnetDataGetContents (MagnetPayloadFirst(payload));
    printf("Message: %s\n", result);
}

int test_join_public_channel () {

	const char *tmp_path = "/tmp/";

	stMagnetListener *listener = MagnetListenerInit();

	MagnetListenerSetOnJoinCB(listener, magnet_join_fn);
	MagnetListenerSetOnDataReceivedCB(listener, rcv_data_fn);

	MagnetInit (tmp_path);

	MagnetSetListener (listener);

	MagnetStart ();
	bool ret = MagnetJoinChannel("MESH_MEDIA_CHANNEL");

    	getchar();

	MagnetRelease ();

	if (!ret)
		return 0;

	return -1;
}

int main()
{
    test_join_public_channel();
    return 0;
}
