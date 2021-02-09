#include "string.c"
#include "number.c"
#include "socket.c"
#include "system.c"
#include "fs.c"
#include "io.c"

int main() {
	printf("ChipCode HTTP Server 1.0\n");

	int port = 8012;

	// __bc_inject__("0x00, 0x00");

	/*

	char[] stringggggg = "sidasddaopkdopkad"; // please implement this

	*/

	int fd = new_socket();
	if(socket_bind(fd, "0.0.0.0", port) == 1) {
		printf("socket bind success\n");
		while(1) {
			int client = socket_accept(fd);
			char[] header = socket_read(client, 8192);



			
			int fs = fopen("./lol.txt", "rb");
			if(fs == 1) {
				char[] data = fread(fs, 999999);
				int len = sizeof(data);
				char[] response = strjoin(strjoin("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: ", itos(len)), "\r\n\r\n");

				int s = socket_write(client, response, strlen(response));
				int f = socket_write(client, data, len);
				socket_close(client);
				fclose(fs);
			}

			
		}
	} else {
		printf("Unable to bind to port " + itos(port) + "\n");
	}
}