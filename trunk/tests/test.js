#!/usr/bin/rhino
/* Has to be run in Rhino as SpiderMonkey does not provide
   any means of file I/O :/ (eg. readFile()) */

load ("../firefox/content/sha.js");
load ("../firefox/content/pwmonkey.js");

String.prototype.trim = function () {
    return this.replace(/^\s*/, "").replace(/\s*$/, "");
}


function loadCsv (fileName)
{
	var contents = readFile (fileName);
	var lines = contents.trim ().split ('\n');
	for (var i in lines) {
		lines[i] = lines[i].split (',');
	}
	return lines;
}


function hex2array (hex)
{
		var expectedHash = [];

		/* Hash to array */
		for (var j = 0; j < hex.length/2; j++) {
			var num = parseInt (hex.substr (2*j, 2), 16);
			expectedHash[j] = num;
		}
		return expectedHash;

}


function testIdent ()
{
	var data = loadCsv ("data/ident.csv");
	var ok = 0;
	var total = 0;

	for (var i  in data) {
		total++;
		var domain = data[i][0];
		var password = data[i][1];
		var expectedIdent = data[i][2];

		var ident = PasswordMonkey.ident (domain, password);
		if (ident == expectedIdent)
			ok++;
	}

	print ("IDENT:    " + ok + " of " + total + " assertions ok");
}


function testHash ()
{
	var data = loadCsv ("data/hash.csv");
	var ok = 0;
	var total = 0;

	for (var i  in data) {
		total++;
		var str = data[i][0];
		var hashHex = data[i][1];
		var expectedHash = hex2array (hashHex);

		var hash = PasswordMonkey.hash (str);

		var equal = true;
		if (expectedHash.length == hash.length) {
			for (var j = 0; j < hash.length; j++) {
				if (hash[j] != expectedHash[j])
					equal = false;
			}
		} else {
			equal = false;
		}

		if (equal)
			ok++;
	}

	print ("HASH:     " + ok + " of " + total + " assertions ok");
}

function testEncode ()
{
	var data = loadCsv ("data/encode.csv");
	var ok = 0;
	var total = 0;

	for (var i  in data) {
		total++;
		var hashHex = data[i][0];
		var expectedB64 = data[i][1];

		var hash = hex2array (hashHex);

		var b64 = PasswordMonkey.encode (hash);

		if (expectedB64 == b64) {
			ok++;
		}else {
			print ("expected '" + expectedB64 + "' got '" + b64 + "'");
		}
	}

	print ("ENCODE:   " + ok + " of " + total + " assertions ok");
}


function testTruncate ()
{
	var data = loadCsv ("data/truncate.csv");
	var ok = 0;
	var total = 0;

	for (var i  in data) {
		total++;
		var str = data[i][0];
		var num = data[i][1];
		var expectedRes = data[i][2];


		var res = PasswordMonkey.truncate (str, num);

		if (expectedRes == res)
			ok++;
	}

	print ("TRUNCATE: " + ok + " of " + total + " assertions ok");
}


try {
testIdent ();
testHash ();
testEncode ();
testTruncate ();
} catch (e) {
	for (var i in e) {
		print (i + ": " + e[i]);
	}
}
