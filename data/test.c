#include "string.c"
#include "number.c"
#include "socket.c"

int main() {
	printf("HTTP Server 1.0");
	int fd = new_socket();
	if(socket_bind(fd, "0.0.0.0", 8012) == 1) {
		printf("socket bind success");
		while(1) {
			int client = socket_accept(fd);
			string data = socket_read(client, 8192);
			printf(data);

			int a = 0;
			int b = 1;
			int c = 0;
			string r = "";
			while(a < 100000000) {
				string tmp = r + itos(a) + "\n";
				r = tmp;
				c = a + b;
				a = b;
				b = c;
			}

			string len = itos(strlen(r));
			string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: " + len;
			string response2 = "\r\n\r\n" + r;
			string response3 = response + response2;
			int s = socket_write(client, response3);
		}
	}
}