#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "sh.h"
#include <glob.h>

int flag = -1;
/*
 int sh
 The main shell of the program
 int argc
 char **argv - argument array
 char envp - enviroment variable
 */
int sh(int argc, char **argv, char **envp)
{
    // Variable declarations
    char *prompt = calloc(PROMPTMAX, sizeof(char));
    char *pwd = calloc(PROMPTMAX, sizeof(char *));
    char *commandpath, *owd, *tmp;
    char **args = calloc(MAXARGS, sizeof(char *));
    int uid, go = 1;
    struct passwd *password_entry;
    char *homedir;
    struct pathelement *pathlist;

    // Get user ID and password entry
    uid = getuid();
    password_entry = getpwuid(uid);   /* get passwd info */
    homedir = password_entry->pw_dir; /* Home directory to start
                    out with*/

    // Get the current working directory
    if ((pwd = getcwd(NULL, PATH_MAX + 1)) == NULL)
    {
        perror("getcwd");
        exit(2);
    }
    owd = calloc(strlen(pwd) + 1, sizeof(char));
    memcpy(owd, pwd, strlen(pwd));
    prompt[0] = ' ';
    prompt[1] = '\0';

    // Put PATH into a linked list
    pathlist = get_path();
    pwd = getcwd(NULL, 0);
    tmp = getcwd(NULL, 0);

    while (go)
    {

        printf(">> ");
        pwd = getcwd(NULL, 0);
        printf("Current working dir: %s\n", pwd);
        printf(">> ");

        // Read the user's input
        fgets(prompt, PROMPTMAX, stdin);

        if (prompt != NULL)
        {
            if (prompt[strlen(prompt) - 1] == '\n')
            {
                prompt[strlen(prompt) - 1] = 0;
            }

            char *token = strtok(prompt, " ");
            for (int j = 0; j < PROMPTMAX; j++)
            {
                argv[j] = "";
            }

            int i = 0;
            while (token != NULL)
            {
                argv[i] = token;
                i++;
                token = strtok(NULL, " ");
            }

            // Check the user's input for specific commands
            if (strcmp(argv[0], "exit") == 0)
            {
                // Free allocated memory and exit the program
                for (int e = 0; e < MAXARGS; e++)
                {
                    free(args[e]);
                }
                free(prompt);
                free(pwd);
                free(args);
                free(owd);

                break;
            }
            else if (strcmp(argv[0], "which") == 0)
            {
                which(argv[1], pathlist);
            }
            else if (strcmp(argv[0], "where") == 0)
            {

                where(argv[1], pathlist);
            }
            else if (strcmp(argv[0], "cd") == 0)
            {
                // Change the current working directory
                homedir = getenv("HOME");
                if ((strcmp(argv[1], "") == 0))
                {
                    if (homedir != NULL)
                    {
                        printf("Home directory:");
                        printf("%s\n", homedir);
                        chdir(homedir);
                        tmp = pwd;
                    }
                }
                else if (strcmp(argv[1], "-") == 0)
                {
                    chdir(tmp);
                    pwd = tmp;
                }
                else if ((strcmp(argv[1], "-") != 0) && (strcmp(argv[1], "") != 0))
                {
                    chdir(argv[1]);
                    tmp = pwd;
                    pwd = argv[1];
                }
            }
            else if (strcmp(argv[0], "pwd") == 0)
            {
                // Print the current working directory
                printf("CWD = [%s]\n", pwd);
            }
            else if (strcmp(argv[0], "list") == 0)
            {
                // List files in the directory
                if (strcmp(argv[1], "") == 0)
                {
                    list(pwd);
                }
                else if (strcmp(argv[1], "") != 0)
                {
                    list(argv[1]);
                }
            }
            else if (strcmp(argv[0], "pid") == 0)
            {
                // Get the process ID
                int pid = getpid();
                printf("%d\n", pid);
            }
            else if (strcmp(argv[0], "kill") == 0)
            {
                // Kill a process
                if ((strcmp(argv[1], "") != 0) && (strcmp(argv[2], "") != 0))
                {
                    int new_int = atoi(argv[2]);
                    int new_int2 = atoi(argv[1]);
                    kill(new_int, new_int2);
                }
                else if ((strcmp(argv[1], "") != 0))
                {
                    int new_int = atoi(argv[1]);
                    kill(new_int, SIGTERM);
                }
            }
            else if (strcmp(argv[0], "prompt") == 0)
            {

                // Set the command prompt
                if (strcmp(argv[1], "") == 0)
                {
                    char *p = calloc(PROMPTMAX, sizeof(char));
                    printf("\ninput prompt prefix: ");
                    fgets(p, PROMPTMAX, stdin);
                    if (p != NULL)
                    {
                        if (p[strlen(p) - 1] == '\n')
                        {
                            p[strlen(p) - 1] = 0;
                        }
                    }

                    char *c = malloc(strlen(p) + strlen(pwd) + 1);
                    strcpy(c, p);
                    strcat(c, pwd);
                    printf("%s \n", c);
                    free(c);
                }
                else if (strcmp(argv[1], "") != 0)
                {
                    char *c = malloc(strlen(pwd) + strlen(argv[1]) + 1);
                    strcpy(c, argv[1]);
                    strcat(c, " ");
                    strcat(c, pwd);
                    pwd = c;
                    printf("%s\n", c);
                    free(c);
                }
            }
            else if (strcmp(argv[0], "printenv") == 0)
            {
                // Print environment variables
                if (strcmp(argv[1], "") == 0)
                {
                    for (int i = 0; envp[i] != NULL; i++)
                    {
                        printf("%s", envp[i]);
                    }
                    getchar();
                    printf("\n");
                }
                else if (strcmp(argv[1], "") != 0)
                {
                    printf("%s\n", getenv(argv[1]));
                }
                else if (strcmp(argv[2], "") != 0)
                {
                }
            }
            else if (strcmp(argv[0], "setenv") == 0)
            {
                // Set environment variables
                if (strcmp(argv[1], "") == 0)
                {
                    for (int i = 0; envp[i] != NULL; i++)
                    {
                        printf("\n%s", envp[i]);
                    }
                }
                else if (strcmp(argv[1], "") != 0 && strcmp(argv[2], "") == 0)
                {
                    setenv(*envp, NULL, 1);
                }
                else if (strcmp(argv[1], "") != 0 && strcmp(argv[2], "") != 0)
                {
                    setenv(*envp, argv[2], 1);
                }
                else if (strcmp(argv[3], "") != 0)
                {
                    printf("setenv: Syntax Error\n");
                }
            }
            else if (('/' == argv[0][0]) || (('.' == argv[0][0]) && ('/' == argv[0][1])) || (('.' == argv[0][0]) && ('.' == argv[0][1]) && ('/' == argv[0][2])))
            {
                // Execute a command with a path
                char *token = strtok(argv[0], "/");
                int k = 0;
                if (('/' == argv[0][0]))
                {
                    if (access(argv[0], X_OK) == 0)
                    {
                        system(argv[0]);
                    }
                }
                while (token != NULL)
                {
                    argv[k] = token;
                    k++;
                    token = strtok(NULL, "/");
                }
                if (access(argv[1], X_OK) == 0)
                {
                    system(argv[1]);
                }
            }
            else if (strcmp("", argv[0]) != 0)
            {
                // Execute a command from the system
                commandpath = which(argv[0], pathlist);
                int j;
                for (j = 0; j < PROMPTMAX; j++)
                {
                    if (strcmp("", argv[j]) == 0)
                    {
                        break;
                    }
                }
                argv[j] = NULL;
                int tmp = -1;
                tmp = find_char(argv, j);

                if (flag == 1)
                {
                    // Handle wildcard character '*'
                    glob_t paths;
                    int retval = glob(argv[tmp], GLOB_NOCHECK | GLOB_NOSORT, NULL, &paths);
                    if (retval == 0)
                    {
                        int idx;
                        for (idx = 0; idx < paths.gl_pathc; idx++)
                        {
                            printf("%s\n", paths.gl_pathv[idx]);
                        }
                        globfree(&paths);
                    }
                    else
                    {
                        puts("glob() failed");
                    }
                }
                else if (flag == 2)
                {
                    // Handle wildcard character '?'
                    int v;
                    for (v = 0; v < PATH_MAX; v++)
                    {
                        if (argv[tmp][v] == '?')
                        {

                            break;
                        }
                    }
                    glob_t paths;
                    int retval = glob(&argv[tmp][v], GLOB_NOCHECK | GLOB_NOSORT, NULL, &paths);
                    if (retval == 0)
                    {
                        int idx;
                        for (idx = 0; idx < paths.gl_pathc; idx++)
                        {
                            printf("%s\n", paths.gl_pathv[idx]);
                        }
                        globfree(&paths);
                    }
                    else
                    {
                        puts("glob() failed");
                    }
                }
                else
                {
                    // Fork and execute the command
                    int pid1 = fork();
                    if (pid1)
                    {
                        waitpid(pid1, NULL, 0);
                    }
                    else
                    {
                        execve(commandpath, argv, NULL);

                        exit(-1);
                    }
                }
            }
        }
        else
        {
            perror("getcwd() error");
            fprintf(stderr, "%s: Command not found. \n", args[0]);
            exit(0);
        }
    }
    return 0;
} /* sh() */
/*
 which
 this function finds the path of an executable
 command - the executable the function looks for
 pathlist - the path in a linked list of the current directory
 */
char *which(char *command, struct pathelement *pathlist)
{

    while (pathlist)
    {
        char *test = malloc(strlen(pathlist->element) + 1 + strlen(command) + 1);
        if (strcmp("", command) != 0)
        {
            sprintf(test, "%s/%s", pathlist->element, command);
            if (access(test, X_OK) == 0)
            {

                return test;
            }
        }
        pathlist = pathlist->next;
    }
    return NULL;
}

/*
where
this function finds all the paths of an executable
command - the executable the function looks for
pathlist - the path in a linked list of the current directory
*/

char *where(char *command, struct pathelement *pathlist)
{

    while (pathlist)
    {
        char *test = malloc(strlen(pathlist->element) + 1 + strlen(command) + 1);
        if (strcmp("", command) != 0)
        {
            sprintf(test, "%s/%s", pathlist->element, command);
            if (access(test, F_OK) == 0)
            {
                printf("[%s]\n", test);
            }
        }
        pathlist = pathlist->next;
    }
    return command;
}
/*
 list
 lists the files in the directory
 char dir - the directory to get file in

 */
void list(char *dir)
{
    DIR *d;
    struct dirent *dire;
    d = opendir(dir);
    if (d)
    {
        while ((dire = readdir(d)) != NULL)
        {
            if (dire->d_type == DT_REG)
            {
                printf("%s\n", dire->d_name);
            }
        }
        closedir(d);
    }
}

/*
 find_char
 this finds the wildcard character in the argv array
 char **arr - this is the argv array to find the possible wildcard character
 int j - this is the max position the wildcard character can be before it is null
 this returns the position of the wildcard character
 */
int find_char(char **arr, int j)
{

    for (int l = 0; l < PROMPTMAX; l++)
    {
        for (int k = 0; k < PROMPTMAX; k++)
        {
            if (k < j)
            {
                if (arr[k][l] == '*')
                {

                    flag = 1;
                    return k;
                }
                else if (arr[k][l] == '?')
                {

                    flag = 2;
                    return k;
                }
            }
        }
    }
    flag = -1;
    return -1;
}
