#include <fcntl.h> // open
#include <unistd.h> // close, read, write
#include <stdlib.h> // malloc

int main()
{
    int fd, size;
    char *c = malloc(sizeof(char) * 100);


    if ((fd = open("/dev/ljp_dev", O_RDWR) )== -1)
        return -1;

    if ((size = read(fd, c, 50)) == -1)
        return -1;

    c[size] = 0;

    // stdout: 1
    if (write(1, c, size) == -1)
        return -1;

    if (close(fd) < 0)
        return -1;
    
    return 0;
}
