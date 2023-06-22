
#include "get_path.h"

struct pathelement *get_path()
{
  /* path is a copy of the PATH and p is a temp pointer */
  char *path, *p;

  /* tmp is a temp point used to create a linked list and pathlist is a
     pointer to the head of the list */
  struct pathelement *tmp, *pathlist = NULL;

  p = getenv("PATH"); /* get a pointer to the PATH env var.
         make a copy of it, since strtok modifies the
         string that it is working with... */

  // Allocate memory for the copy of PATH
  path = malloc((strlen(p) + 1) * sizeof(char));
  strncpy(path, p, strlen(p));
  path[strlen(p)] = '\0';
  p = strtok(path, ":"); // PATH is : delimited

  // loop through the PATH
  do                     
  {
    // to build a linked list of dirs
    if (!pathlist) // create head of list 
    {
      tmp = calloc(1, sizeof(struct pathelement));
      pathlist = tmp;
    }
    else // add on next element 
    {
      tmp->next = calloc(1, sizeof(struct pathelement));
      tmp = tmp->next;
    }

     // Store the current directory element in the linked list
    tmp->element = p;
    tmp->next = NULL;

  } while (p = strtok(NULL, ":"));

  return pathlist;
} 
