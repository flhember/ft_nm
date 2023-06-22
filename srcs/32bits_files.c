#include "ft_nm.h"

//save all valid symb in tab for sort and print after
Elf32_Sym	**ft_set_symb_tab_32(char *ptr, Elf32_Sym **tab, char *symb_str, int idx_symb, int swap)
{
	int		i;
	int		j;
	int		size;
	Elf32_Ehdr	*elf;
	Elf32_Shdr	*shdr;
	Elf32_Sym	*table_symb;

	i = 0;
	j = 0;
	elf = (Elf32_Ehdr *)ptr;
	shdr = (Elf32_Shdr *)(ptr + swap32(elf->e_shoff, sizeof(elf->e_shoff), swap));
	size = swap32(shdr[idx_symb].sh_size, sizeof(shdr[idx_symb].sh_size), swap) / swap32(shdr[idx_symb].sh_entsize, sizeof(shdr[idx_symb].sh_entsize), swap);
	table_symb = (Elf32_Sym *)(ptr + swap32(shdr[idx_symb].sh_offset, sizeof(shdr[idx_symb].sh_offset), swap));
	while (i < size)
	{
		if (ft_strlen(symb_str + swap32(table_symb[i].st_name, sizeof(table_symb[i].st_name), swap))
				&& ELF32_ST_TYPE(swap32(table_symb[i].st_info, sizeof(table_symb[i].st_info), swap) != STT_FILE))
			tab[j++] = &table_symb[i];
		i++;
	}
	return (tab);
}

//sh_size / sh_entsize (total size of all symbtab / size fix of each symbtab), loop on all symb, take only symb with name and not file.
int	ft_find_nb_symb_32(char *ptr, char *symb_str, int idx_symb, int swap)
{
	int		nb_symb;
	unsigned long	i;
	Elf32_Ehdr	*elf;
	Elf32_Shdr	*shdr;
	Elf32_Sym	*table_symb;

	i = 0;
	nb_symb = 0;
	elf = (Elf32_Ehdr *)ptr;
	shdr = (Elf32_Shdr *)(ptr + swap32(elf->e_shoff, sizeof(elf->e_shoff), swap));
	if (idx_symb == -1 || symb_str == NULL)
		return (-1);
	table_symb = (Elf32_Sym *)(ptr + swap32(shdr[idx_symb].sh_offset, sizeof(shdr[idx_symb].sh_offset), swap));
	while (i < (swap32(shdr[idx_symb].sh_size, sizeof(shdr[idx_symb].sh_size), swap) / swap32(shdr[idx_symb].sh_entsize, sizeof(shdr[idx_symb].sh_entsize), swap)))
	{
		if (ft_strlen(symb_str + swap32(table_symb[i].st_name, sizeof(table_symb[i].st_name), swap))
				&& ELF32_ST_TYPE(swap32(table_symb[i].st_info, sizeof(table_symb[i].st_info), swap) != STT_FILE))
			nb_symb++;
		i++;
	}
	return (nb_symb);
}

//for symtab we need sh_link of symtab to find offset of symtab section, return ptr on symtab section
char	*ft_find_symb_str_32(char *ptr, int idx_symb, int swap)
{
	Elf32_Ehdr	*elf;
	Elf32_Shdr	*shdr;
	uint32_t	tmp_sh_link;

	elf = (Elf32_Ehdr *)ptr;
	shdr = (Elf32_Shdr *)(ptr + swap32(elf->e_shoff, sizeof(elf->e_shoff), swap));
	if (idx_symb == -1)
		return (NULL);
	tmp_sh_link =  swap32(shdr[idx_symb].sh_link, sizeof(shdr[idx_symb].sh_link), swap);
	return ((char *)(ptr + swap32(shdr[tmp_sh_link].sh_offset, sizeof(shdr[tmp_sh_link].sh_offset), swap)));
}

//function loop on every section of elf file to find the index of symbole table section
int	ft_find_index_symb_32(char *ptr, int swap)
{
	uint32_t	i;
	Elf32_Ehdr	*elf;
	Elf32_Shdr	*shdr;

	i = 0;
	elf = (Elf32_Ehdr *)ptr;
	shdr = (Elf32_Shdr *)(ptr + swap32(elf->e_shoff, sizeof(elf->e_shoff), swap));
	while (i < swap32(elf->e_shnum, sizeof(elf->e_shnum), swap))
	{
		if (swap32(shdr[i].sh_type, sizeof(shdr[i].sh_type), swap) == SHT_SYMTAB)
			return (i);
		i++;
	}
	return (-1);
}

// Check error like if number of each section is > 0, if endianness is good, if have the good verions of elf (1) and if the file is not cut
int	check_erreur_32(char *ptr, char *size_file, int swap, char *name)
{
	Elf32_Ehdr	*elf;

	elf = (Elf32_Ehdr *)ptr;
	if (elf->e_shnum <= 0 || (ptr[EI_DATA] != 1 && ptr[EI_DATA] != 2) || ptr[EI_VERSION] != EV_CURRENT)
	{
                ft_putstr_fd("ft_nm: ", 2);
                ft_putstr_fd(name, 2);
                ft_putstr_fd(": file format not recognized\n", 2);
                return (-1);
	}
	else if ((ptr + swap32(elf->e_shoff, sizeof(elf->e_shoff), swap)) > size_file)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": file too short\n", 2);
		return (-1);
	}
	return (0);
}

// check error, find index of symb table, find how many valid symb, and creat and fill tab with symb
int     parse_32bits_files(char *name, char *ptr, int ac, int swap, char *size_file)
{
	int		nb_symb;
	int		idx_symb;
	char		*symb_str;
	Elf32_Sym	**tab;

	tab = NULL;
	if (check_erreur_32(ptr, size_file, swap, name) == -1)
		return (-1);
	idx_symb = ft_find_index_symb_32(ptr, swap);
	symb_str = ft_find_symb_str_32(ptr, idx_symb, swap);
	nb_symb = ft_find_nb_symb_32(ptr, symb_str, idx_symb, swap);
	if (nb_symb == 0 || nb_symb == -1)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": no symbols\n", 2);
		return (0);
	}
	else if (idx_symb == -1 || symb_str == NULL || nb_symb == -1)
		return (-1);
	if (ac > 2)
		ft_printf("\n%s:\n", name);
	if (!(tab = (Elf32_Sym **)malloc(sizeof(Elf32_Sym *) * (nb_symb + 1))))
		return (-3);
	tab = ft_set_symb_tab_32(ptr, tab, symb_str, idx_symb, swap);
	tab[nb_symb] = NULL;
	sort_and_print_32(tab, ptr, nb_symb, symb_str, swap);
	free(tab);
	return (0);
}
