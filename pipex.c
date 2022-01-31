#include <unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<sys/wait.h>

int main(int argc,char *argv[])
{
    int fd[2];
    int file;
    int pid;
    if (pipe(fd) == -1)
        return 1;
    pid = fork();
    if(pid == 0)
    {
        file = open(argv[1],O_RDONLY);
        if (file == -1)
            return (-1);
        close(fd[0]);
        dup2(file,0);
        dup2(fd[1],1);
        char *arg[] = { "bash", "-c", argv[2], NULL };
        execve("/usr/bin/bash", arg,NULL);
        perror("execve");
        close(file);
        close(fd[1]);
        // char *s;
        // s = get_next_line(file);
    }
    else
    {
        wait(NULL);
        int file2 = open(argv[4],O_WRONLY | O_CREAT );
        if (file2 == -1)
            return (-1);
        close(fd[1]);
        dup2(fd[0],0);
        dup2(file2,1);
        char *arg[] = { "bash", "-c", argv[3], NULL };
        execve("/usr/bin/bash", arg,NULL);
        perror("execve");
        close(fd[0]);
        
        close(file2);
    }
    close(fd[0]);
    close(fd[1]);
}