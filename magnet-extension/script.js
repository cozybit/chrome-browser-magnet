(function(){

    var messages = document.getElementById('messages');

    var CHANNEL_ID = "MESH_MEDIA_CHANNEL";

    function addToLog(str) {
        console.log(str);
        messages.innerText = messages.innerText + ">>> " + str + "\n";
    };

    chrome.magnet.initialize(function(result, msg) {
        if ( result ) {
            addToLog("Success initializing");
        } else {
            addToLog("Failure initializing");
        }
    });

    chrome.magnet.joinedChannels(function(channels) {

        for (var i = channels.length - 1; i >= 0; i--) {
            var channel = channels[i];
            console.log("Joined in channel: " + channel);
        };

        // var hasOurChannel = channels.indexOf(CHANNEL_ID) >= 0;

        // if ( ! hasOurChannel )
        // {
            chrome.magnet.join(CHANNEL_ID, function(result, msg) { 
                if ( result ) {
                    addToLog("Channel join succeeded")
                }
                else {
                    addToLog("Channel join failed")
                }
            });
        // }

    });

    chrome.magnet.onListening.addListener(function(){

    	addToLog("Got listening notification");

    });

    chrome.magnet.onReceiveData.addListener(function(node, channel, type, data) {

    	addToLog(
            "Received message from node: '" + node +
            "', channel: '" + channel +
            "', type: '" + type + "', " +
            "', data: '" + data);

    });

    chrome.magnet.onLeave.addListener(function(node, channel) {

        addToLog(
            "Node left, name: '" + node +
            "', channel: '" + channel + "'"
            );

    });

    chrome.magnet.onJoin.addListener(function(node, channel) {

        console.log("onJoin from node: " + node + ", channel: " + channel);

        if ( channel != CHANNEL_ID )
            return;

    	addToLog("Node '" + node + "' joined. Sending them a greeting...");
    	chrome.magnet.sendData(node, channel,  "text/plain", "Hello from Chrome!", function(){});

    });

})();
