#include "ft_nm.h"

//find if 32 or 64bits files for parse it. If don't find -> error
int	parse_elf(char *ptr, char *name, int ac, char *size_file)
{
	if ((unsigned char)ptr[EI_CLASS] == ELFCLASS64)
	{
		if (parse_64bits_files(name, ptr, ac, big_or_little_endian(ptr), size_file) == -1)
			return (-1);
	}
	else if ((unsigned char)ptr[EI_CLASS] == ELFCLASS32)
	{
		if (parse_32bits_files(name, ptr, ac, big_or_little_endian(ptr), size_file) == -1)
			return (-1);
	}
	else
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": file format not recognized\n", 2);
		return (-1);
	}
	return (0);
}

// Check the magic number of elf file or the archive id "!<arch>"
int	parse_file(char *ptr, char *name, int ac, char *size_file)
{
	if (ptr > size_file)
	{
		ft_putstr_fd("erreor size\n", 2);
		return (0);
	}
	if (
		(unsigned char)ptr[EI_MAG0] == 0x7f &&
		(unsigned char)ptr[EI_MAG1] == 'E' &&
		(unsigned char)ptr[EI_MAG2] == 'L' &&
		(unsigned char)ptr[EI_MAG3] == 'F'	
	)
	{
		parse_elf(ptr, name, ac, size_file);
	}
	else if (ft_strncmp(ptr, ARMAG, SARMAG) == 0)
		parse_lib(ptr, ac, size_file);
	else
		return (-1);
	return (0);
}

// ft to close properly file
int	close_file(int fd, char *ptr, struct stat buf)
{
	if ((munmap(ptr, buf.st_size)) < 0)
	{
		ft_putstr_fd("munap erreur\n", 2);
		return (-1);
	}
	if (close(fd) == -1)
	{	
		ft_putstr_fd("close erreur\n", 2);
		return (-1);
	}
	return (0);
}

// looking for a.out file fo open and read it, if ok go parse
int				no_para(void)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if ((fd = open("./a.out", O_RDONLY)) < 0)
	{
		ft_putstr_fd("ft_nm: 'a.out': No such file\n", 2);
		return (-1);
	}
	if (fstat(fd, &buf) < 0)
	{
		ft_putstr_fd("fstat erreur\n", 2);
		return (-1);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_putstr_fd("mmap erreur\n", 2);
		return (-1);
	}
	if (parse_file(ptr, NULL, 1, ptr + buf.st_size) == -1)
	{
		ft_putstr_fd("ft_nm: a.out: file format not recognized\n", 2);
		return (-1);
	}
	if (close_file(fd, ptr, buf))
		return (-1);
	return (0);
}

//Open and read each file of para and go to parse
int	para(char **av, int i, char *name, int ac)
{
	int		fd;
	char		*ptr;
	struct stat	buf;

	if ((fd = open(av[i], O_RDONLY)) < 0)
	{
		ft_putstr_fd("open erreur\n", 2);
		return (-1);
	}
	if (fstat(fd, &buf) < 0)
	{
		ft_putstr_fd("fstat erreur\n", 2);
		return (-1);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_putstr_fd("mmap erreur\n", 2);
		return (-1);
	}
	if (parse_file(ptr, name, ac, ptr + buf.st_size) == -1)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": file format not recognized\n", 2);
		return (-1);
	}
	if (close_file(fd, ptr, buf))
		return (-1);
	return (0);
}

// Check if para, if not go check if a.out, if yes go parse it
int	main(int ac, char **av)
{
	int	i = 1;

	if (ac <= 1) {
		if (!no_para())
			return (-1);
	} else {
		while (i < ac)
		{
			para(av, i, av[i], ac);
			i++;
		}
	}
	return (0);
}
