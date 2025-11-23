#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
        // TODO
        // return 1 if cmd is one of the allowed commands
        // return 0 otherwise

        for (int i = 0; i < N; i++) {
                if (strcmp(allowed[i], cmd) == 0) {
                        return 1;
                }
        }
        return 0;
}

int getAllowedIndex(const char *cmd) {
        for (int i = 0; i < N; i++) {
                if (strcmp(allowed[i], cmd) == 0) {
                        return i;
                }
        }
        return -1;
}

int main() {

        // TODO
        // Add variables as needed
        char line[256];

        //create argument array for command line args
        char* argv[64];

        //for posix_spawnp()
        pid_t pid;
        posix_spawnattr_t attr;
        posix_spawnattr_init(&attr);

        while (1) {

                fprintf(stderr,"rsh>");

                if (fgets(line,256,stdin)==NULL) continue;

                if (strcmp(line,"\n")==0) continue;

                line[strlen(line)-1]='\0';

                // TODO
                // Add code to spawn processes for the first 9 commands
                // And add code to execute cd, exit, help commands
                // Use the example provided in myspawn.c



                //tokenize arguments and separate by space
                int argc = 0;
                char* tokens = strtok(line, " ");
                while (tokens != NULL && argc < 63) {
                        argv[argc++] = tokens;
                        tokens = strtok(NULL, " ");
                }	

                argv[argc] = NULL;

                if (argc == 0)
                        continue;

                // error handling if argv[0] is not a valid command
                if (!isAllowed(argv[0])) {
                        printf("NOT ALLOWED!\n");
                        continue;
                }

                int index = getAllowedIndex(argv[0]);

                if (index >= 0 && index <= 8) {
                        // spawn these processes as separate processes using the posix_spawnp() system call.
                        if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
                                perror("posix_spawnp");
                                continue;
                        }
                        waitpid(pid, NULL, 0);
                        continue;
                }

                if (index == 9) {
                        if (argc > 2) {
                                printf("-rsh: cd: too many arguments\n");
                        } else if (chdir(argv[1]) != 0) {
                                perror("cd");
                        }
                        continue;
                }

                if (index == 10) {
                        break;
                }

                if (index == 11) {
			printf("The allowed commands are:\n");
                        for (int i = 0; i < N; i++)
                                printf("%d: %s\n", i+1, allowed[i]);
                        continue;
                }
        }
        return 0;
}
