void printf(int a) {
    syscall(a);
}

int main() {
    int a = 0;
    while(a < 999999) {
        printf(a);
        a = a + 1;
    }
}
