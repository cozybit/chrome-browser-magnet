(function(){

    var messages = document.getElementById('messages');
    var result = chrome.magnet.initialize("Magnet");

    if ( result ) {
        messages.innerText = "success";
    } else {
        messages.innerText = "failure";
    }

})();
