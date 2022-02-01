/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlakhssa <mlakhssa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 14:56:43 by mlakhssa          #+#    #+#             */
/*   Updated: 2022/02/01 15:43:58 by mlakhssa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include<sys/wait.h>

char	**free_problem(char **arg, int position)
{
	int	i;

	i = 0;
	while (i < position)
	{
		free(arg[i]);
		i++;
	}
	free(arg);
	return (0);
}

char	**alloc(char **arg, char *argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	arg = (char **)malloc(sizeof(char *) * 3);
	if (arg == 0)
		return (0);
	arg[0] = (char *)malloc(sizeof(char) * 5);
	if (arg[0] == 0)
		return (free_problem(arg, 0));
	arg[1] = (char *)malloc(sizeof(char) * 3);
	if (arg[1] == 0)
		return (free_problem(arg, 1));
	arg[2] = (char *)malloc(sizeof(char) * (i + 1));
	if (arg[2] == 0)
		return (free_problem(arg, 2));
	arg[0] = "bash";
	arg[1] = "-c";
	arg[2] = argv;
	return (arg);
}

int	part_1(char **argv, int fd[2])
{
	char	**arg;
	int		file;

	arg = NULL;
	arg = alloc(arg, argv[2]);
	file = open(argv[1], O_RDONLY);
	if (file == -1)
	{
		perror(argv[1]);
		return (-1);
	}
	close(fd[0]);
	dup2(file, 0);
	dup2(fd[1], 1);
	execve("/bin/bash", arg, NULL);
	perror("execve");
	free(arg[0]);
	free(arg[1]);
	free(arg[2]);
	free(arg);
	close(file);
	close(fd[1]);
	exit (0);
}

int	part_2(char **argv, int fd[2])
{
	char	**arg2;
	int		file;

	arg2 = NULL;
	file = open(argv[4],O_WRONLY | O_CREAT | O_TRUNC, 644);
	if (file == -1)
	{
		perror(argv[4]);
		return (-1);
	}
	close(fd[1]);
	dup2(fd[0], 0);
	dup2(file, 1);
	arg2 = alloc(arg2, argv[3]);
	execve("/bin/bash", arg2, NULL);
	perror("execve");
	free(arg2[0]);
	free(arg2[1]);
	free(arg2[2]);
	free(arg2);
	close(fd[0]);
	close(file);
	exit (0);
}

int	main(int argc, char *argv[])
{
	int		fd[2];
	int		pid[2];

	if (argc != 5)
		return (0);
	if (pipe(fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid[0] = fork();
	if (pid[0] == 0)
		part_1(argv, fd);
	else
	{
		pid[1] = fork();
		if (pid[1] == 0)
			part_2(argv, fd);
		close(fd[0]);
		close(fd[1]);
	}
	wait (NULL);
	close(fd[0]);
	close(fd[1]);
}
