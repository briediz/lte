#include <stdio.h>
#include <fcntl.h>

int main()
{
  int fd, interrupts=0;
  
  fd = open("/dev/gpioir17", O_RDONLY);
  printf("fd=%d\n", fd);
  if(fd<0){
    perror("/dev/gpioir17");
    return -1;
  }
  
  while(1){
    read(fd, &interrupts, sizeof(interrupts));
    printf("interrupts: %d\n", interrupts);
  }
  return 0;
}