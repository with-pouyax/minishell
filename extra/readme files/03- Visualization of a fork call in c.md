# Visualization of a fork call in c

- Here, the author is trying to visualize the `fork` call in C, which is very helpful.
- I can't make notes out of this, but come back to this video whenever you get confused about the `fork()` function.

```c
int main(int argc, char *argv[])
{
    int id = fork(); // It will create a child process from the main process. This child process is very important because it has the exact same memory as the main process.
    int n;
    if (id == 0)
    {
        n = 1;
    }
    else
    {
        n = 6;
    }
    if (id != 0)
    {
        wait();
    }
    int i;
    for (i = n; i < n + 5; i++)
    {
        printf("%d\n", i);
        fflush(stdout);
    }
    if (id == 0){
        printf("\n");
    }
    return 0;
}
```
    