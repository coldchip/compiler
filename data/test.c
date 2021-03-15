void printf(int a) {
    syscall(0, &a, 4);
}

int main() {
    printf(444);
}
