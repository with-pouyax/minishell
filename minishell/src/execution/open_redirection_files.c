#include "../minishell.h"


int open_input_file(t_redirection *redir, int fd_in)
{
    int new_fd;

    if (fd_in != -2)
		close(fd_in);
    new_fd = open(redir->filename, O_RDONLY, 0);
    if (new_fd == -1)
    {
        perror("open file failed");
        return (EXIT_FAILURE);
    }
    dup2(new_fd, STDIN_FILENO);
    close(new_fd); 
    return (new_fd);
}

int open_output_file(t_redirection *redir, int fd_out)
{
  int new_fd;

  if (fd_out != -2)
  close(fd_out);
  new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (new_fd == -1)
  {
      perror("open file failed");
      return (EXIT_FAILURE);
  }
  dup2(new_fd, STDOUT_FILENO);
  close(new_fd); 
  return (new_fd);
}

int open_append_file(t_redirection *redir, int fd_out)
{
  int new_fd;

  if (fd_out != -2)
  close(fd_out);
  new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
  if (new_fd == -1)
  {
      perror("open file failed");
      return (EXIT_FAILURE);
  }
  dup2(new_fd, STDOUT_FILENO);    //redirect output
  close(new_fd); 
  return (new_fd);
}
