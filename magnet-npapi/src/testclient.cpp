#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Magnet.h"

void magnet_join_fn (stMagnetHeader *header) {
    printf("Joined: %s\n", MagnetHeaderGetNodeName(header));
}

int test_join_public_channel () {

	const char *tmp_path = "/tmp/";

	stMagnetListener *listener = MagnetListenerInit();

	MagnetListenerSetOnJoinCB(listener, magnet_join_fn);

	MagnetInit (tmp_path);

	MagnetSetListener (listener);

	MagnetStart ();
	bool ret = MagnetJoinChannel("Magnet");

    getchar();

	MagnetRelease ();

	if (!ret)
		return 0;

	return -1;
}

main()
{
    test_join_public_channel();
}
