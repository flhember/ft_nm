#include "ft_nm.h"

char	*parse_long_file(char *long_file)
{
	int	i;

	i = 0;
	while (*long_file != '\n')
		long_file++;
	long_file++;
	return (long_file);
}

char	*return_name_file(char *long_file, int size)
{
	int	i;
	char	*tmp;
	int	size_malloc;
	
	i = 0;
	tmp = NULL;
	size_malloc = 0;
	if (size != -1)
		long_file += size;
	while (long_file[size_malloc] != '/')
		size_malloc++;
	if (!(tmp = (char *)malloc(sizeof(char) * (size_malloc + 1))))
		return (NULL);
	while (i < size_malloc)
	{
		tmp[i] = long_file[i];
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

void	print_ar_hdr(struct ar_hdr *lib, char *long_file)
{
	int	test;
	char	*tmp;

	test = 0;
	if (lib->ar_name[0] == '/')
	{
		test = ft_atoi(lib->ar_name + 1);
		tmp = return_name_file(long_file, test);
		printf("\n%s:\n", tmp);
		free(tmp);
	}
	else
	{
		tmp = return_name_file(lib->ar_name, -1);
		printf("\n%s:\n", tmp);
		free(tmp);
	}
}

int	parse_lib(char *ptr, int ac, char *size_file)
{
	struct ar_hdr	*lib;
	char		*long_file;

	lib = NULL;
	long_file = NULL;
	ptr += SARMAG;
	lib = (struct ar_hdr *)ptr;
	ptr += sizeof(*lib);
	ptr += ft_atoi(lib->ar_size);
	lib = (struct ar_hdr *)ptr;
	if (ptr[0] == '/' && ptr[1] == '/')
	{
		long_file = ptr;
		long_file = parse_long_file(long_file);
		ptr += sizeof(*lib);
		ptr += ft_atoi(lib->ar_size);
		lib = (struct ar_hdr *)ptr;
	}
	while (ptr < size_file)
	{
		print_ar_hdr(lib, long_file);
		ptr += sizeof(*lib);
		parse_elf(ptr, NULL, ac, size_file);
		ptr += ft_atoi(lib->ar_size);
		lib = (struct ar_hdr *)ptr;
	}
	return (0);
}
