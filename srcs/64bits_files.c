#include "ft_nm.h"

Elf64_Sym	**ft_set_symb_tab(char *ptr, Elf64_Sym **tab, char *symb_str, int idx_symb, int swap)
{
	int		i;
	int		j;
	int		size;
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;
	Elf64_Sym	*table_symb;

	i = 0;
	j = 0;
	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap));


	size = swap64(shdr[idx_symb].sh_size, sizeof(shdr[idx_symb].sh_size), swap) / swap64(shdr[idx_symb].sh_entsize, sizeof(shdr[idx_symb].sh_entsize), swap);
	table_symb = (Elf64_Sym *)(ptr + swap64(shdr[idx_symb].sh_offset, sizeof(shdr[idx_symb].sh_offset), swap));
	while (i < size)
	{
		if (ft_strlen(symb_str + swap64(table_symb[i].st_name, sizeof(table_symb[i].st_name), swap))
				&& ELF64_ST_TYPE(swap64(table_symb[i].st_info, sizeof(table_symb[i].st_info), swap) != STT_FILE))
			tab[j++] = &table_symb[i];
		i++;
	}
	return (tab);
}

int	ft_find_nb_symb(char *ptr, char *symb_str, int idx_symb, int swap)
{
	int		nb_symb;
	unsigned long	i;
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;
	Elf64_Sym	*table_symb;

	i = 0;
	nb_symb = 0;
	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap));
	if (idx_symb == -1 || symb_str == NULL)
		return (-1);
	table_symb = (Elf64_Sym *)(ptr + swap64(shdr[idx_symb].sh_offset, sizeof(shdr[idx_symb].sh_offset), swap));
	while (i < (swap64(shdr[idx_symb].sh_size, sizeof(shdr[idx_symb].sh_size), swap) / swap64(shdr[idx_symb].sh_entsize, sizeof(shdr[idx_symb].sh_entsize), swap)))
	{
		if (ft_strlen(symb_str + swap64(table_symb[i].st_name, sizeof(table_symb[i].st_name), swap))
				&& ELF64_ST_TYPE(swap64(table_symb[i].st_info, sizeof(table_symb[i].st_info), swap) != STT_FILE))
			nb_symb++;
		i++;
	}
	return (nb_symb);
}

char	*ft_find_symb_str(char *ptr, int idx_symb, int swap)
{
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;
	uint64_t	tmp_sh_link;

	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap));
	if (idx_symb == -1)
		return (NULL);
	tmp_sh_link =  swap64(shdr[idx_symb].sh_link, sizeof(shdr[idx_symb].sh_link), swap);
	return ((char *)(ptr + swap64(shdr[tmp_sh_link].sh_offset, sizeof(shdr[tmp_sh_link].sh_offset), swap)));
}

int	ft_find_index_symb(char *ptr, int swap)
{
	unsigned int	i;
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;

	i = 0;
	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap));
	while (i < swap64(elf->e_shnum, sizeof(elf->e_shnum), swap))
	{
		if (swap64(shdr[i].sh_type, sizeof(shdr[i].sh_type), swap) == SHT_SYMTAB)
			return (i);
		i++;
	}
	return (-1);
}

int	check_erreur_64(char *ptr, char *size_file, int swap, char *name)
{
	Elf64_Ehdr	*elf;

	elf = (Elf64_Ehdr *)ptr;
	if (elf->e_shnum <= 0 || (ptr[EI_DATA] != 1 && ptr[EI_DATA] != 2) || ptr[EI_VERSION] != EV_CURRENT)
	{
                ft_putstr_fd("ft_nm: ", 2);
                ft_putstr_fd(name, 2);
                ft_putstr_fd(": Format de fichier non reconnu\n", 2);
                return (-1);
	}
	else if ((ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap)) > size_file)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": Fichier tronqué\n", 2);
		return (-1);
	}
	return (0);
}

int	parse_64bits_files(char *name, char *ptr, int ac, int swap, char *size_file)
{
	int		nb_symb;
	int		idx_symb;
	char		*symb_str;
	Elf64_Sym	**tab;

	tab = NULL;
	if (check_erreur_64(ptr, size_file, swap, name) == -1)
		return (-1);
	idx_symb = ft_find_index_symb(ptr, swap);
	symb_str = ft_find_symb_str(ptr, idx_symb, swap);
	nb_symb = ft_find_nb_symb(ptr, symb_str, idx_symb, swap);
	if (nb_symb == 0 || nb_symb == -1)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": aucun symbole\n", 2);
		return (0);
	}
	else if (idx_symb == -1 || symb_str == NULL || nb_symb == -1)
		return (-1);
	if (ac > 2)
		ft_printf("%s:\n", name);
	if (!(tab = (Elf64_Sym **)malloc(sizeof(Elf64_Sym *) * (nb_symb + 1))))
		return (-3);
	tab = ft_set_symb_tab(ptr, tab, symb_str, idx_symb, swap);
	tab[nb_symb] = NULL;
	sort_and_print_64(tab, ptr, nb_symb, symb_str, swap);
	free(tab);
	return (0);
}
