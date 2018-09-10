#include <stdio.h>
#include <string.h>
#ifdef DECLARE_SNPRINTF
#ifdef __cplusplus
extern "C"
#endif /*__cplusplus*/
int snprintf(char *, int, const char *, ...);
#endif /*DECLARE_SNPRINTF*/
int main() {
    char buf[32];
    snprintf(buf, 8, "%s", "1234567890");
    if (strlen(buf)!=7) return 1;
    return 0;
}

