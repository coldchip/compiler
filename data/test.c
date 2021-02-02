#include "string.c"
#include "number.c"
#include "socket.c"
#include "system.c"
#include "fs.c"

int main() {
	printf("ChipCode HTTP Server 1.0");

	int port = 8012;

	int fd = new_socket();
	if(socket_bind(fd, "0.0.0.0", port) == 1) {
		printf("socket bind success");
		while(1) {
			int client = socket_accept(fd);
			string data = socket_read(client, 8192);

			string head = explode(data, "\r\n", 0);

			string method = explode(head, " ", 0);
			string path = explode(head, " ", 1);
			string version = explode(head, " ", 2);

			if(strcmp(path, "/") == 0) {
				string index = path + "index.html";
				path = index;
			}

			string prefix = "." + path;
			path = prefix;

			printf(path);

			int fs = fopen(path, "rb");
			if(fs == 1) {
				string data = fread(fs, 1000000);
				fclose(fs);

				string result = "<!DOCTYPE html><html><head><style>body, html { margin: 0; padding: 0; width: 100%; height: 100%; } * { font-family: monospace; } .box { font-family: monospace; width: 100%; height: 100%; }</style></head><body><textarea class='box'>" + data + "</textarea></body>";

				string len = itos(strlen(result));
				string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: " + len + "\r\n\r\n" + result;
				int s = socket_write(client, response);
				socket_close(client);
			} else {
				string list = exec("ls .");
			
				int g = 0;
				string res = "";
				string elem = "0";
				while(strcmp(elem, "") == 1) {
					string tmp = explode(list, "\n", g);
					elem = tmp;

					printf(elem);

					string tmp2 = res + "<a href='" + elem + "'>" + elem + "</a><br>";
					res = tmp2;

					g = g + 1;
				}
				string result = "<!DOCTYPE html><html><head><style>* { font-family: monospace; } .box { font-family: monospace; }</style></head><body><div class='box'>" + res + "</div></body>";

				string len = itos(strlen(result));
				string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: " + len + "\r\n\r\n" + result;
				int s = socket_write(client, response);
				socket_close(client);
			}

			
		}
	} else {
		printf("Unable to bind to port " + port);
	}
}