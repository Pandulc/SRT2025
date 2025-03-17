#include <stdio.h>
#include <time.h>
#include <unistd.h> // Para sleep() en sistemas Unix/Linux
#ifdef _WIN32
    #include <windows.h>
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

int main() {
    while (1) {
        time_t t;
        struct tm *tm_info;
        char buffer[9];

        time(&t);
        tm_info = localtime(&t);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);

        printf("\r%s", buffer);
        fflush(stdout);

        sleep(1);
    }
    return 0;
}
