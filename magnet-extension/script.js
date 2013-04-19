(function(){

    var messages = document.getElementById('messages');

    chrome.magnet.initialize("Magnet", function(result, msg) {
        if ( result ) {
            messages.innerText = "Success initializing Magnet\n";
        } else {
            messages.innerText = "Failure initializing Magnet\n";
        }
    });

    chrome.magnet.onListening.addListener(function(){
    	messages.innerText += "Listening!\n";
    });

    chrome.magnet.onReceiveData.addListener(function(data) {
    	messages.innerText += "Received some data: " + data + "\n";
    });

    chrome.magnet.onJoin.addListener(function(){
    	messages.innerText += "Someone joined!  Sending them a greeting...\n";
    	chrome.magnet.sendData("Hello from Chrome!", function(){});
    });

})();
