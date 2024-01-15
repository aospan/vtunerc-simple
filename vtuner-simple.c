#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

#define VTUNER_MAJOR        226
#define VTUNER_GET_MESSAGE  _IOR(VTUNER_MAJOR, 1, struct vtuner_message *)
#define VTUNER_SET_RESPONSE     _IOW(VTUNER_MAJOR, 2, struct vtuner_message *)
#define VTUNER_SET_NAME     _IOW(VTUNER_MAJOR, 3, char *)
#define VTUNER_SET_TYPE     _IOW(VTUNER_MAJOR, 4, char *)
#define VTUNER_SET_FE_INFO  _IOW(VTUNER_MAJOR, 6, struct dvb_frontend_info *)
#define VTUNER_SET_NUM_MODES    _IOW(VTUNER_MAJOR, 7, int)
#define VTUNER_SET_MODES    _IOW(VTUNER_MAJOR, 8, char *)

#define READ_SIZE 7*188
#define UDELAY 5

int main(int argc, char **argv){
    int fd, vtuner_control;
    char buf[READ_SIZE];
    int64_t bw = 0;
    int ret = 0;
    time_t start = time(0);
    time_t last = time(0);
    int count = 100000;
    
    vtuner_control = open("/dev/vtunerc0", O_RDWR );
    if (vtuner_control < 0)
    {
        perror("/dev/vtunerc0");
        return 1;
    }

    if (ioctl(vtuner_control, VTUNER_SET_TYPE, "DVB-S")) {
        perror("VTUNER_SET_TYPE failed\n");
        return 1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        perror("infile");
        return 1;
    }

    last = time(0);
    while (count--)
    {
        while ( (ret = read (fd, &buf[0], READ_SIZE)) > 0 ){
            bw += write(vtuner_control, &buf[0], READ_SIZE);
            usleep(UDELAY);
            if (time(0) > last+3){
                printf("%" PRId64 " bytes written, speed = %.2f Mbit/sec \n", bw, (8*(double)bw/(time(0) - start))/1048576 );
                last = time(0);
            }
        }

        lseek(fd, 0, SEEK_SET);
    }
}
