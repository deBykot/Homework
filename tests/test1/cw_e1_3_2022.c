#include <stdio.h>
#include <stdlib.h>

struct s1 {
    short v1;
    double v2;
    char v3;
};

struct s2 {
    double v2;
    short v1;
    char v3;
};

size_t compactify(void * ptr, size_t size)
{
    struct s1 * ps1 = (struct s1*)ptr;
    struct s2 * ps2 = (struct s2*)ptr;
    size_t num = size/sizeof(*ps1);
    size_t i;
    for (i = 0; i < num; i++) {
        struct s2 s2;
        s2.v1 = ps1->v1;
        s2.v2 = ps1->v2;
        s2.v3 = ps1->v3;
        *ps2 = s2;
        ps1++; ps2++;
    }
    return num * sizeof (struct s2);
}

int main()
{
    //printf("%d-%d\n", sizeof (struct s1), sizeof (struct s2));
    struct s1 arr[10];
    size_t s1 = sizeof (arr);
    size_t s2 = compactify(arr, s1);
    printf("old size=%d, new size=%d\n", s1, s2);
    return 0;
}

