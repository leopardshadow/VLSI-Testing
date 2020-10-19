#include<sys/types.h>
#include<unistd.h>
#include <stdio.h> 
#include<stdlib.h>


int main() {

    system("ps aux | grep mem");

    return 0;
}