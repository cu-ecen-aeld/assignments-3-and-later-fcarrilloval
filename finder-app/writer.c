#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Open the syslog
    openlog("writter", LOG_PID | LOG_CONS, LOG_USER);

    // Check if both arguments are provided
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid arguments. Usage: ./writter <writefile> <writestr>");
        fprintf(stderr, "Error: Invalid arguments. Usage: ./writter <writefile> <writestr>\n");
        closelog();
        exit(1);
    }

    const char *writefile = argv[1];
    const char *writestr = argv[2];

    // Open the file for writing
    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Failed to open file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Failed to open file %s: %s\n", writefile, strerror(errno));
        closelog();
        exit(1);
    }

    // Write the string to the file
    if (fprintf(file, "%s", writestr) < 0) {
        syslog(LOG_ERR, "Failed to write to file %s: %s", writefile, strerror(errno));
        fprintf(stderr, "Error: Failed to write to file %s: %s\n", writefile, strerror(errno));
        fclose(file);
        closelog();
        exit(1);
    }

    // Log the successful write operation
    syslog(LOG_DEBUG, "Writing '%s' to '%s'", writestr, writefile);

    // Close the file
    fclose(file);

    // Close the syslog
    closelog();

    return 0;
}

