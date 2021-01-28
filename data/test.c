#include "string.c"
#include "number.c"
#include "socket.c"

int main() {
	printf("HTTP Server 1.0");

	//char arr[100];

	int fd = new_socket();
	if(socket_bind(fd, "0.0.0.0", 8012) == 1) {
		printf("socket bind success");
		while(1 == 1) {
			int client = socket_accept(fd);
			string data = socket_read(client, 8192);
			printf(data);

			int t = 0;
			string r = "";
			while(t < 500) {
				int a = 0;
				int b = 1;
				int c = 0;
				while(a < 1000000000) {
					string tmp = r + itos(a) + "\n";
					r = tmp;
					c = a + b;
					a = b;
					b = c;
				}
				t = t + 1;
			}

			string len = itos(strlen(r));
			string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: application/json\r\nContent-Length: " + len + "\r\n\r\n" + r;
			int s = socket_write(client, response);
		}
	}
}