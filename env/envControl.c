
#include "env.h"

void getShellEnv()
{
    char *shell = getenv("SHELL");
    if (shell != NULL)
    {
        printf("SHELL : %s\n", shell);
    }
}

int setEnv(void)
{
    char *val;
    val = getenv("SHELL");
    printf("SHELL : %s\n", val);
    if (val != NULL)
    {
        printf("SHELL : %s\n", val);
    }
    else
    {
        printf("SHELL not found\n");
    }

    setenv("SHELL", "/bin/bash", 0);
    printf("SHELL : %s\n", getenv("SHELL"));
    setenv("SHELL", val, 0);
    printf("SHELL : %s\n", getenv("SHELL"));

    return -1;
}

int deletEnv(void)
{
    char *val;
    val = getenv("SHELL");
    printf("SHELL : %s\n", val);
    if (val != NULL)
    {
        printf("SHELL : %s\n", val);
    }
    else
    {
        printf("SHELL not found\n");
    }

    unsetenv("SHELL");
    printf("SHELL : %s\n", getenv("SHELL"));
    setenv("SHELL", val, 0);
    printf("SHELL : %s\n", getenv("SHELL"));

    return -1;
}