# What is waitpid?

- we are going to talk about waitpid() function.
- What waitpid() does is, instead of waiting for any child process to finish execution, it is going to wait for a specific child process to finish its execution, given we have the process ID of it.
- we are going to create two child processes here, thats gonna be very straight forward. 
- now the result of the code will be :
```bash
Finished executing (29093)
waited for 29093
Finished executing (29092)
waited for 29092
```

```c
int main(int argc, char *argv[])
{
    int pid1 = fork(); 
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        sleep(4); // we sleep for 4 seconds, so the first child process will finish after second child process.
        printf("Finished executing (%d)\n", getpid());
        return 0;
    }

    int pid2 = fork();
    if (pid2 == -1)
    {
        
        perror("fork");
        return 2;
    }
    if (pid2 == 0)
    {
        sleep(1); // we sleep for 1 second, so the second child process will finish first before the first child process.
        printf("Finished executing (%d)\n", getpid());
        return 0;
    }

    int pid1_result = wait(NULL);
    printf("waited for %d\n", pid1_result);
    int pid2_result = wait(NULL);
    printf("waited for %d\n", pid2_result);
        
    return 0;
}
```

- what if we wanted it to be the other way around ? 
- what if we want first wait for the longest running process to finish, how would we do that ?
- that could be done using waitpid() function.
- instea of calling wait() function, we call waitpid() function and it's return the same thing, but it's gonna take different arguments.
- the first argument is the process ID of the child process that we want to wait for.
- the second argument is the same as the second argument for wait().
- the third argument is the options that we can pass to the waitpid() function, telling it how to wait for the child process to finish, we are not going into that so we pass 0.
- now if we try to run the code, we will get :

```bash
Finished executing (2324)
Finished executing (29320)
waited for 29320
waited for 29324
```
- as we can see this time, we can notice that :
- the finished executing process does print out, but no longer do we wait for it, so only after the second child process finished its execution, we actually print messages for both of those waitpid() calls, so this a bit more diffrent.
- Now if you really do care about the order of waiting for those child processes, then this is the function (waitpid()) that you should use.
- the other nice thing about waitpid() function is that the pid value could be also -1.
- if it is -1, instead of waiting for that specific process ID, it is gonna wait for that specific process group ID and usually most processes have the same group ID, unless we create it with that fork(), in that case they would have all the same process group ID.
- So that is 1 way to use waitpid() function.
- If you pass -1, it is the same as using wait() function (waitpid(-1, NULL, 0)), it is gonna wait for any child process to finish.
- if we pass 0, we are gonna wait for any child process with the same process group ID.
- if we pass the number greater than 0, it is gonna wait for that specific process ID as we saw before.


```c
int main(int argc, char *argv[])
{
    int pid1 = fork(); 
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        sleep(4); // we sleep for 4 seconds, so the first child process will finish after second child process.
        printf("Finished executing (%d)\n", getpid());
        return 0;
    }

    int pid2 = fork();
    if (pid2 == -1)
    {
        
        perror("fork");
        return 2;
    }
    if (pid2 == 0)
    {
        sleep(1); // we sleep for 1 second, so the second child process will finish first before the first child process.
        printf("Finished executing (%d)\n", getpid());
        return 0;
    }

    int pid1_result = waitpid(pid1, NULL, 0);
    printf("waited for %d\n", pid1_result);
    int pid2_result = waitpid(pid2, NULL, 0);
    printf("waited for %d\n", pid2_result);
        
    return 0;
}
```

- the more interesting thing that we can do with waitpid() function is that we can pass here as the third argument a certain flag, for example WNOHANG.
- WNOHANG means, its not gonna wait for the process to finish it's execution, it is just gonna check if it finished its execution, then it is gonna be fine, we are gonna return like before, but if you didn't, at the time of calling this waitpid() , then it's gonna return straight ahead and its gonna save the status in inside the second argument that we passed in telling us oh this process didn't actually finish execution, we just call in with WNOHANG.
- so now if I try to run the code, we will get :

```bash
waited for 0
waited for 0
```

- We recieved zero here because it didn't wait for anythig, it just checked, it saw that no process really finished execution yet and it  was like ok well, let me continue and there is that, so this is one way we can use WNOHANG or like waitpid().
- so if we do waitpid( -1, NULL, WNOHANG), it is the same as wait(NULL), but instead of waiting, you are actually returning straight ahead if there is no process that has terminated in the meantime.
- with waitpid() function, it is kind of niche, we dont usually need to use it.   


```c
int main(int argc, char *argv[])
{
    int pid1 = fork(); 
    if (pid1 == -1)
    {
        perror("fork");
        return 1;
    }

    if (pid1 == 0)
    {
        sleep(4); // we sleep for 4 seconds, so the first child process will finish after second child process.
        printf("Finished executing (%d)\n", getpid());
        return 0;
    }

    int pid2 = fork();
    if (pid2 == -1)
    {
        
        perror("fork");
        return 2;
    }
    if (pid2 == 0)
    {
        sleep(1); // we sleep for 1 second, so the second child process will finish first before the first child process.
        printf("Finished executing (%d)\n", getpid());
        return 0;
    }

    int pid1_result = waitpid(pid1, NULL, WNOHANG);
    printf("waited for %d\n", pid1_result);
    int pid2_result = waitpid(pid2, NULL, WNOHANG);
    printf("waited for %d\n", pid2_result);
        
    return 0;
}
```