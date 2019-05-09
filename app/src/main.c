#define APP_ENTRY __attribute__ ((section (".app_entry")))

APP_ENTRY int main() {
    return 101;
}
