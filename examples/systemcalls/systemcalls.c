#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
printf("do_system()++\n");
printf("cmd %s\n", cmd);
/*  TODO
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    if (system(cmd) == 0){
        return true;
    }
    else{ 
        return false;
    }
	printf("do_system()--\n");
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    printf("do_exec()++\n");
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    pid_t pid = fork();
    if (pid == -1){
        printf("fork failed\n");
		return false;
    } else if (pid == 0){
        //Child process
        if(execv(command[0], command) == -1){
            printf("execv failed\n");
            exit(1);
        }
    } else {
        //Parent process
		int status;
        waitpid(pid, &status, 0); //wait for child process to complete
        printf("Child process finished\n");
	    //Check the child's exit status
		if (WIFEXITED(status)) {
			int exit_status = WEXITSTATUS(status);
			if (exit_status == 0) {
				printf("Command succeeded.\n");
			} else {
				printf("Command failed with exit code %d.\n", exit_status);
				return false; // Return false to indicate failure
			}
		} else {
			printf("Child did not exit normally.\n");
			return false; // Return false to indicate failure
		}
    }

    va_end(args);
    printf("do_exec()--\n");
    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
	printf("do_exec_redirect()++\n");
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    int kidpid;
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
    if (fd < 0) { printf("open error"); return false; }
    switch (kidpid = fork()) {
      case -1: printf("fork error"); return false;
      case 0:
	//Redirection happens here
        if (dup2(fd, 1) < 0) { printf("dup2 error"); return false; }
        close(fd);
        execvp(command[0], command); printf("execv error"); exit(1);
      default:
    /* do whatever the parent wants to do. */
	        //Parent process
		int status;
        waitpid(kidpid, &status, 0); //wait for child process to complete
        printf("Child process finished\n");
	    //Check the child's exit status
		if (WIFEXITED(status)) {
			int exit_status = WEXITSTATUS(status);
			if (exit_status == 0) {
				printf("Command succeeded.\n");
			} else {
				printf("Command failed with exit code %d.\n", exit_status);
				return false; // Return false to indicate failure
			}
		} else {
			printf("Child did not exit normally.\n");
			return false; // Return false to indicate failure
		}
}

    va_end(args);
	printf("do_exec_redirect()--\n");
    return true;
}
