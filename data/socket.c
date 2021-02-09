int new_socket() {
	return __callinternal__new_socket();
}

int socket_bind(int fd, char ip, int port) {
	return __callinternal__socket_bind(fd, ip, port);
}

int socket_accept(int fd) {
	return __callinternal__socket_accept(fd);
}

char socket_read(int fd, int size) {
	return __callinternal__socket_read(fd, size);
}

int socket_write(int fd, char data, int length) {

	return __callinternal__socket_write(fd, data, length);
}

void socket_close(int fd) {
	__callinternal__socket_close(fd);
}