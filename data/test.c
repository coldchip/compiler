void main() {
	//int a = 1 + 2 + 3;
	string a = 3;
	string b = "Hello World!";	
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