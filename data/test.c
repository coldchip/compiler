void main() {
	//int a = 1 + 2 + 3;
	var a = 65535;
	var b = "Hello World!";	
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