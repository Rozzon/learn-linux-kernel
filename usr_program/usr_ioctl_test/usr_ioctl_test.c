#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE_FILE "/dev/my_ioctl_device"
#define MY_IOCTL_CMD _IOW('k', 1, int)

int main() {
    int fd, value;

    // Open the device file
    fd = open(DEVICE_FILE, O_RDWR);
    if (fd == -1) {
        perror("Failed to open device file");
        return 1;
    }

    // Send an integer value to the driver via ioctl
    value = 42;
    if (ioctl(fd, MY_IOCTL_CMD, &value) == -1) {
        perror("IOCTL failed");
        close(fd);
        return 1;
    }

    printf("Sent value to driver: %d\n", value);

    // Close the device file
    close(fd);

    return 0;
}

