#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/wait.h>
//ws bakers

//Process related system calls
void forkExample()
{
    pid_t id = fork();
    if(id == 0)
    {
        printf("\nChild process running with pid:%d\n",getpid());
        exit(0);//Terminating the child process once work is done
    }
    else 
    {
        printf("\nParent process running with pid:%d\n",getpid());
        wait(NULL);
    }
}

void exitExample()
{
    printf("\nExiting this process....\n");
    exit(0);
}

void waitExample()
{
    pid_t id = fork();
    if(id == 0)
    {
        printf("\nChild process running with pid:%d\n",getpid());
        sleep(2);//Simulating child is performing some work
        exit(0);
    }
    else 
    {
        printf("\nParent process running with pid:%d\n",getpid());
        wait(NULL);
        printf("\nWaiting done for child process, child process completed it's execution\n");
    }
}

void killExample()
{
    pid_t id = fork();
    if(id == 0)
    {
        printf("\nChild process running with pid:%d\n",getpid());
        while(1);
    }
    else 
    {
        printf("\nParent process running with pid:%d\n",getpid());
        kill(id,SIGKILL);
        printf("\nChild process killed successfully\n");
    }
}

//File Related System Call
void openWriteReadClose()
{
    char buffer[100];
    int fd = open("Demo.txt",O_CREAT | O_RDWR,0644);
    printf("\nFile Created(if doesn't exists)and opened successfully\n");
    write(fd,"Hello World",12);
    printf("\nWriting done successfully on the file\n");
    lseek(fd,0,SEEK_SET);//Seet the pointer of the file at the start
    read(fd,buffer,sizeof(buffer));
    printf("\nReading done successfully from file.File Contents:%s\n",buffer);
    close(fd);
    printf("\nFile closed Successfully\n");
}

void linkUnlink()
{
    link("Demo.txt","TestFilelink.txt");
    printf("\nLink Created Successfully\n");
    unlink("TestFilelink.txt");
    printf("\nLink Removed Successfully\n");
}

//Communication Related System Calls.Unidirectional pipe.It means only one child or parent at a time can write.The other one will read.
void pipeExample()
{
    int fd[2];//fd[0] -> read, fd[1] -> write
    pipe(fd);
    char buffer[100];
    pid_t id = fork();
    if(id == 0)
    {
        close(fd[0]);
        write(fd[1],"Hello From child\n",17);
        close(fd[1]);
        exit(0);
    }
    else 
    {
        close(fd[1]);
        int totalNumberOfBytes = read(fd[0],buffer,sizeof(buffer));
        buffer[totalNumberOfBytes] = '\0';
        printf("\nMessage received to parent:%s\n",buffer);
        close(fd[0]);
        wait(NULL);
    }
}

//Information related sytem calls
void processId()
{
    printf("\nThe process id of currently running process is %d",getpid());//get process id
}

void parentProcessId()
{
    printf("\nThe process id of the parent of the currently running process is %d\n",getppid());//get parent process id
}

void main()
{
    int choice;
    while(1)
    {
        printf("\n1.Process related system calls\n2.File Related System Calls\n3.Communication Related System Calls\n4.Information Related System Calls\n5.Exit\n");
        printf("\nPlease enter choice:");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1://Process related system calls
                printf("\n1.Fork\n2.Exit\n3.Wait\n4.Kill\nEnter choice:");
                scanf("%d",&choice);
                if(choice == 1)
                    forkExample();
                else if(choice == 2)
                    exitExample();
                else if(choice == 3)
                    waitExample();
                else if(choice == 4)
                    killExample();
                else 
                    printf("\nWrong choice entered\n");
                break;
            case 2://File Related system calls
                printf("\n1.OpenReadWriteClose\n2.LinkUnlink\nEnter choice:");
                scanf("%d",&choice);
                if(choice == 1)
                    openWriteReadClose();
                else if(choice == 2)
                    linkUnlink();
                else 
                    printf("\nYou hava entered wrong choice\n");
                break;
            case 3:
                printf("\n1.Pipe\nEnter choice:\n");
                scanf("%d",&choice);
                if(choice == 1)
                    pipeExample();
                else 
                    printf("\nWrong choice entered\n");
                break;
            case 4:
                printf("\n1.Get process Id\n2.Get parent process id\nEnter choice:\n");
                scanf("%d",&choice);
                if(choice == 1)
                    processId();
                else if(choice  == 2)
                    parentProcessId();
                else
                    printf("\nWrong choice entered\n");
                break;
            case 5:
                printf("\nExiting code\n");
                exit(0);
                break;
            default:
                printf("\nWrong choice entered\n");
        }
    }
}