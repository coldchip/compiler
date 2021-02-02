int new_socket() {
	return __callinternal__new_socket();
}

int socket_bind(int fd, string ip, int port) {
	return __callinternal__socket_bind(fd, ip, port);
}

int socket_accept(int fd) {
	return __callinternal__socket_accept(fd);
}

string socket_read(int fd, int size) {
	return __callinternal__socket_read(fd, size);
}

int socket_write(int fd, string data) {
	return __callinternal__socket_write(fd, data);
}

void socket_close(int fd) {
	__callinternal__socket_close(fd);
}