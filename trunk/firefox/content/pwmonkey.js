
function PasswordMonkey () {
}


PasswordMonkey.PWMON_ID = "5dfabc489612601f0e253781fbc6f009";


PasswordMonkey.ident = function (domain, password)
{
	return "pwmon(" + domain + ";" + password + ";" + PasswordMonkey.PWMON_ID + ")";
}


PasswordMonkey.hash = function (text)
{
	var sha = new jsSHA (text, "ASCII");
	var digestHex = sha.getHash ("SHA-256", "HEX");

	var digest = [];
	for (var j = 0; j < digestHex.length/2; j++) {
		digest[j] = parseInt (digestHex.substr (2*j, 2), 16);
	}
	return digest;
}


PasswordMonkey.encode = function (data)
{
	var base64chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'.split("");

	var p = "";
	var b64 = "";
	var c = data.length % 3;
	if (c > 0) {
		while (c++ < 3) {
			p += "=";
			data.push (0);
		}
	}

	for (var i = 0; i < data.length; i += 3) {
		var num = (data[i] << 16) + (data[i+1] << 8) + data[i+2];
		var d = [(num >>> 18) & 63, (num >>> 12) & 63, (num >>> 6) & 63, num & 63];
		
		for (var j = 0; j < 4; j++) {
			b64 += base64chars[d[j]];
		}
	}
	return b64.substr(0, b64.length - p.length) + p;
}


PasswordMonkey.truncate = function (subject, length)
{
	return subject.substr (0, length);
}


PasswordMonkey.generatePass = function (domain, password, len)
{
	if (len == undefined) len = 20;

	var id = PasswordMonkey.ident (domain, password);
	var hash = PasswordMonkey.hash (id);
	var encoded = PasswordMonkey.encode (hash);
	return PasswordMonkey.truncate (encoded, len);

}

PasswordMonkey._hex2b64 = function (hexStr)
{
	var base64chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'.split("");
	var b64 = "";
	var p = "";

	var c = (hexStr.length % 6)/2;
	while (c-- > 0) {
		p += "=";
		hexStr += "00";
	}

//	var d = [];
//	for (var i = 0; i < hexStr.length / 2; i++) {
//		d[i] = parseInt (hexStr.substr (2*i, 2), 16);		
//	}
	for (var i = 0; i < hexStr.length/6; i++) {
		var num = parseInt (hexStr.substr (6*i,6), 16);

		var d = [(num >>> 18) & 63, (num >>> 12) & 63, (num >>> 6) & 63, num & 63];

		for (var j = 0; j < 4; j++) {
			b64 += base64chars[d[j]];
		}
	}

	return b64 + p;
}
