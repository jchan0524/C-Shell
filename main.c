#include "sh.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void sig_handler(int signal);

int main(int argc, char **argv, char **envp)
{
  

  signal(SIGINT, sig_handler); // Register signal handler for SIGINT (Ctrl+C)
  sigignore(SIGTSTP); // Ignore SIGTSTP (Ctrl+Z)
  sigignore(SIGTERM); // Ignore SIGTERM (Terminate signal)

  return sh(argc, argv, envp); // Call the sh function to start the shell
  return 1;
}

void sig_handler(int signal)
{
  pid_t k = getpid();
  kill(k, SIGTERM);

  /* define your signal handler */
}
