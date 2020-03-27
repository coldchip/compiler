void main() {
	//int a = 1 + 2 + 3;
	string a = "aa";
	string b = "aa";	
	printf(a);
}

/*

.text1
	"hello wtf"

pushstack
	load .text1
	call printf
	pushag af
	exec
popstack

*/