#include "ft_nm.h"

char	**ft_sort_symb_tab(char **tab, int size)
{
	int	i;
	int	j;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1)
		{
			if (ft_strcmp(tab[j], tab[j + 1]) > 0)
			{
				tmp = ft_strdup(tab[j]);
				ft_strdel(&tab[j]);
				tab[j] = ft_strdup(tab[j + 1]);
				ft_strdel(&tab[j + 1]);
				tab[j + 1] = ft_strdup(tmp);
				free(tmp);
			}
			j++;
		}
		i++;
	}
	return (tab);
}

char    find_type(Elf64_Sym sym, Elf64_Shdr *shdr)
{
	char c;

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
		c = 'B';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_MERGE)) //.rodata*
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC)) //.rodata*
		c = 'R';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) //.data*
		c = 'D';
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) //.text
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) //.init_array
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) //.fini_array
		c = 'T';
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC) //.dynamic
		c = 'D';
	else
		c = '?';
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return (c);
		
}

char	*ft_return_string_symb(char *letter, char *name, char *addr)
{
	char	*str_tmp1;
	char	*str_tmp2;

	str_tmp1 = ft_strjoin(name, " ");
	str_tmp2 = ft_strjoin(addr, letter);
	return (ft_strjoinfree(str_tmp1, str_tmp2, 3));
}

char	**ft_set_symb_tab(char *ptr, char **tab)
{
	int		i;
	int		j;
	int		size;
	char		*c;
	char		*symb_str;
	char		*str_addr;
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;
	Elf64_Sym	*table_symb;
	
	i = 0;
	j = 0;
	c = ft_strdup("  ");
	size = -1;
	symb_str = NULL;
	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + elf->e_shoff);
	while (i < elf->e_shnum)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			table_symb = (Elf64_Sym *)(ptr + shdr[i].sh_offset);
			size = shdr[i].sh_size / shdr[i].sh_entsize;
			symb_str = (char *)(ptr + shdr[shdr[i].sh_link].sh_offset);
		}
		i++;
	}
	i = 0;
	while (i < size)
	{
		if (ft_strlen(symb_str + table_symb[i].st_name) && ELF64_ST_TYPE(table_symb[i].st_info) != STT_FILE)
		{
			c[0] = find_type(table_symb[i], shdr);
			if (table_symb[i].st_value == 0 && c[0] != 'T')
				str_addr = ft_strdup("                 ");
			else
				str_addr = ft_strdup("0000000000000000 ");
			//ft_printf("%016lx", (unsigned long)table_symb[i].st_value);
			tab[j++] = ft_return_string_symb(c, symb_str + table_symb[i].st_name, str_addr);
			free(str_addr);
		} 
		i++;
	}
	free(c);
	return (tab);
}

int	ft_find_nb_symb(char *ptr)
{
	int		i;
	int		size;
	int		nb_symb;
	char		*symb_str;
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;
	Elf64_Sym	*table_symb;
	
	i = 0;
	size = -1;
	nb_symb = 0;
	symb_str = NULL;
	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + elf->e_shoff);
	while (i < elf->e_shnum)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
		{
			table_symb = (Elf64_Sym *)(ptr + shdr[i].sh_offset);
			size = shdr[i].sh_size / shdr[i].sh_entsize;
			symb_str = (char *)(ptr + shdr[shdr[i].sh_link].sh_offset);
		}
		i++;
	}
	if (size == -1)
		return (-1);
	i = 0;
	while (i < size)
	{
		if (ft_strlen(symb_str + table_symb[i].st_name) && ELF64_ST_TYPE(table_symb[i].st_info) != STT_FILE)
			nb_symb++;	
		i++;
	}
	return (nb_symb);
}

int	parse_64bits_files(char *name, char *ptr, int ac)
{
	char 	**tab;
	int	nb_symb;

	tab = NULL;
	nb_symb = ft_find_nb_symb(ptr);
	if (nb_symb == -1)
		return (-1);
	else if (nb_symb == 0)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd(name, 2);
		ft_putstr_fd(": aucun symbole\n", 2);
		return (0);
	}
	if (ac > 2)
        	ft_printf("%s:\n", name);
	if (!(tab = (char **)malloc(sizeof(char *) * (nb_symb + 1))))
		return (-3);
	tab = ft_set_symb_tab(ptr, tab);
	tab[nb_symb] = NULL;
	tab = ft_sort_symb_tab(tab, nb_symb);
	//ft_printf("%d\n", nb_symb);
        //print_res(ptr);
	

	int	i = 0;
	while (i < nb_symb)
		ft_printf("%s\n", tab[i++]);

	return (0);
}

