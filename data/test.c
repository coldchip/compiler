void printf(a) {
	//__call__(a);
}

void main() {
	var cc = "aa";
	var aa = cc;
	var ss = aa;
	{
		var dd = "d";
	}
	printf(aa);
}
