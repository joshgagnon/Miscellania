
var box;
var remote ="";
var pending = false;
var diff = new diff_match_patch();

window.onload = function() {
    box = document.getElementById('scratch');
    box.onkeyup = function(event){ pending = true; };
}

function push() {
    if(pending){
	var xmlHttpReq = false;
	var self = this;
	// Mozilla/Safari
	if (window.XMLHttpRequest) {
	    self.xmlHttpReq = new XMLHttpRequest();
	}
	// IE
	else if (window.ActiveXObject) {
	    self.xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
	}
	self.xmlHttpReq.open('POST', "scratch_wall.py/push", true);
	self.xmlHttpReq.setRequestHeader('Content-Type',
					 'application/x-www-form-urlencoded');
	self.xmlHttpReq.onreadystatechange = function() {
	    if (self.xmlHttpReq.readyState == 4) {
	    }
	}
	var to_send = encodeURIComponent(diff.patch_toText(diff.patch_make(remote,box.value)));
	self.xmlHttpReq.send("delta="+to_send);
	remote=box.value;
	pending = false;
    }
    setTimeout("push()",1000);
}

function pull(){
    var xmlHttpReq = false;
    var self = this;
    // Mozilla/Safari
    if (window.XMLHttpRequest) {
	self.xmlHttpReq = new XMLHttpRequest();
    }
    // IE
    else if (window.ActiveXObject) {
	self.xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
    }
    self.xmlHttpReq.open('GET', "scratch_wall.py/pull", true);
    self.xmlHttpReq.onreadystatechange = function() {
	if (self.xmlHttpReq.readyState == 4) {
	    var patch = self.xmlHttpReq.responseText;    
	    remote=diff.patch_apply(diff.patch_fromText(patch),remote)[0];
	    box.value=diff.patch_apply(diff.patch_make(box.value,remote),box.value)[0];
	    box.value = remote;
	}
    }
    self.xmlHttpReq.send();
    setTimeout("pull()",1000);
    
}

push();
pull();
