#include "env.h"

int main(int argc, char *argv[], char **envp)
{

    char **env = envp;
    while (*env)
    {
        printf("%s\n", *env);
        env++;
    }
    getShellEnv();
    deletEnv();
    setEnv();
}