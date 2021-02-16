int main() {
	int i = 1;
    while(i < 10000) {
        if (i % 3 == 0) {
            dbg(111111111);
        }
        if (i % 5 == 0) {
            dbg(222222222);
        }
        if ((i % 3 != 0) && (i % 5 != 0)) {
            dbg(i);
        }

        i = i + 1;
    }
}