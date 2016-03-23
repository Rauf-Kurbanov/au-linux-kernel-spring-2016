#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vsd_ioctl.h"

void man(char * run_name) {
    printf("NAME\n"
           "        %s\n" 
           "DESCRIPTION\n"
           "        Gets and sets size of files\n\n"
           "        -getsize\n"
           "           gets size of file\n"
           "        -setsize size\n"
           "           sets size of file in bytes\n", run_name);
}

int main(int argc, char **argv) {
    int vsd;
    int res = EXIT_FAILURE;

    if (argc > 3 || argc < 2) {
        man(argv[0]);
        return EXIT_FAILURE;
    }

    vsd = open("/dev/vsd", O_RDONLY);
    if (vsd == -1) {
        perror("open");
        printf("No device found.\n");
        return EXIT_FAILURE;
    }

    if (!strcmp("-getsize", argv[1])) {
        vsd_ioctl_get_size_arg_t arg;
        if (ioctl(vsd, VSD_IOCTL_GET_SIZE, &arg)) {
            perror("ioctl");
            res = EXIT_FAILURE;
        }
        printf("VSD size: %lu\n", arg.size);
        res = EXIT_SUCCESS;
    } else if (!strcmp("-setsize", argv[1])) {
        if (argc != 3) {
            man(argv[0]);
            goto err;
        }

        int size = atoi(argv[2]);
        if (size <= 0) {
            man(argv[0]);
            goto err;
        }
        vsd_ioctl_set_size_arg_t arg = {
                .size = size
        };
        if (ioctl(vsd, VSD_IOCTL_SET_SIZE, &arg)) {
            perror("ioctl");
            res =  EXIT_FAILURE;
        }
        res =  EXIT_SUCCESS;
    } else {
        man(argv[0]);
    }

err:
    close(vsd);
    return res;
}