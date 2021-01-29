#include "string.c"
#include "number.c"
#include "socket.c"

int main() {
	printf("HTTP Server 1.0");

	int fd = new_socket();
	if(socket_bind(fd, "0.0.0.0", 8012) == 1) {
		printf("socket bind success");
		while(1 == 1) {
			int client = socket_accept(fd);
			string data = socket_read(client, 8192);
			printf(data);

			int t = 0;
			string result = "";

			int arr_len = 100;

			char arr[arr_len];
			int a = 0;
			int b = 1;
			int c = 0;

			while(t < arr_len) {
				arr[t] = a;
				c = a + b;
				a = b;
				b = c;
				t = t + 1;
			}

			int o = 0;
			while(o < arr_len) { 
				string tmp = result + itos(arr[o]) + "\r";
				result = tmp;
				o = o + 1;
			}
			

			string len = itos(strlen(result));
			string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: application/json\r\nContent-Length: " + len + "\r\n\r\n" + result;
			int s = socket_write(client, response);
		}
	}
}