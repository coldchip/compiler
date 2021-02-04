#include "string.c"
#include "number.c"
#include "socket.c"
#include "system.c"
#include "fs.c"
#include "io.c"

int main() {
	printf("ChipCode HTTP Server 1.0\n");

	int port = 8012;

	int fd = new_socket();
	if(socket_bind(fd, "0.0.0.0", port) == 1) {
		printf("socket bind success");
		while(1) {
			int client = socket_accept(fd);
			char header_buf = socket_read(client, 8192);
			string header = char_to_string(header_buf);

			string head = explode(header, "\r\n", 0);

			string method = explode(head, " ", 0);
			string path = explode(head, " ", 1);
			string version = explode(head, " ", 2);

			if(strcmp(path, "/") == 0) {
				string index = path + "index.html";
				path = index;
			}

			string prefix = "." + path;
			path = prefix;

			int fs = fopen(path, "rb");
			if(fs == 1) {
				char data = fread(fs, 99999);
				fclose(fs);

				string result = "<!DOCTYPE html><html><head><style>body, html { margin: 0; padding: 0; width: 100%; height: 100%; } * { font-family: monospace; } .box { font-family: monospace; width: 100%; height: 100%; }</style></head><body><textarea class='box'>" + char_to_string(data) + "</textarea></body>";

				string len = itos(strlen(result));
				string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: " + len + "\r\n\r\n" + result;
				int s = socket_write(client, to_array(response));
				socket_close(client);
			} else {
				string list = exec("ls .");
			
				int g = 0;
				string res = "";
				string elem = "0";
				while(strcmp(elem, "") == 1) {
					string tmp = explode(list, "\n", g);
					elem = tmp;

					string tmp2 = res + "<a href='" + elem + "'>" + elem + "</a><br>";
					res = tmp2;

					g = g + 1;
				}
				string result = "<!DOCTYPE html><html><head><style>* { font-family: monospace; } .box { font-family: monospace; }</style></head><body><div class='box'>" + res + "</div></body>";

				string len = itos(strlen(result));
				string response = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: " + len + "\r\n\r\n" + result;
				int s = socket_write(client, to_array(response));
				socket_close(client);
			}

			
		}
	} else {
		printf("Unable to bind to port " + itos(port) + "\n");
	}
}