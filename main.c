#include "stdio.h"
#include "regExpress.h"

void main()
{
    bool res = isMatch("mississippi","mis*is*p*.");
    printf("%d", res);
}
