#include "ft_nm.h"

char    ft_find_type_bis_64(uint64_t st_shndx, Elf64_Shdr *shdr, char c, int swap)
{
	uint64_t	sh_type, sh_flags;

	sh_type = swap64(shdr[st_shndx].sh_type, sizeof(shdr[st_shndx].sh_type), swap);
	sh_flags = swap64(shdr[st_shndx].sh_flags, sizeof(shdr[st_shndx].sh_flags), swap);
	if (sh_type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
		c = 'B';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_MERGE)) //.rodata*
		c = 'R';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC)) //.rodata*
		c = 'R';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.data*
		c = 'D';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) //.text
		c = 'T';
	else if (sh_type == SHT_INIT_ARRAY && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.init_array
		c = 'T';
	else if (sh_type == SHT_FINI_ARRAY && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.fini_array
		c = 'T';
	else if (sh_type == SHT_DYNAMIC) //.dynamic
		c = 'D';
	else
		c = '?';
	return (c);
}

char    ft_find_type_64(Elf64_Sym sym, Elf64_Shdr *shdr, int swap)
{
	char		c;
	uint64_t	st_info, st_shndx;

	c = '\0';
	st_info = swap64(sym.st_info, sizeof(sym.st_info), swap);
	st_shndx = swap64(sym.st_shndx, sizeof(sym.st_shndx), swap);
	if (ELF64_ST_BIND(st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(st_info) == STB_WEAK)
	{
		c = 'W';
		if (st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(st_info) == STB_WEAK && ELF64_ST_TYPE(st_info) == STT_OBJECT)
	{
		c = 'V';
		if (st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (st_shndx == SHN_UNDEF)
		c = 'U';
	else if (st_shndx == SHN_ABS)
		c = 'A';
	else if (st_shndx == SHN_COMMON)
		c = 'C';
	else
		c = ft_find_type_bis_64(st_shndx, shdr, c, swap);
	if (ELF64_ST_BIND(st_info) == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

void	ft_print_nm_64(Elf64_Sym **tab, char *ptr, int size, char *symb_str, int swap)
{
	int		i;
	char		c;
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*shdr;

	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap));
	i = 0;
	while (i < size)
	{
		c = ft_find_type_64(*tab[i], shdr, swap);
		if (swap64(tab[i]->st_value, sizeof(tab[i]->st_value), swap) == 0 && c != 'T' && c != 'b') // t ou B ?
			ft_printf("                ");
		else
			ft_printf("%016lx", (unsigned long)swap64(tab[i]->st_value, sizeof(tab[i]->st_value), swap));
		ft_printf(" %c ", c);
		ft_printf("%s\n", symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap));
		i++;
	}
}

char		*ft_neutral_str(char *str)
{
	size_t	i;
	size_t	j;
	char	*str_final;

	i = 0;
	j = 0;
	str_final = ft_strdup(str);
	while (str[i] == '_')
		i++;
	while (str[i] != '\0')
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str_final[j] = str[i] - 32;
		else if (!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z') && !(str[i] >= '0' && str[i] <= '9'))
			j--;
		else
			str_final[j] = str[i];
		j++;
		i++;
	}
	while (j < ft_strlen(str))
		str_final[j++] = '\0';
	return (str_final);
}

Elf64_Sym	**ft_sort_symb_tab_64(Elf64_Sym **tab, int size, char *symb_str, int swap)
{
	int		i;
	int		j;
	char		*str1;
	char		*str2;
	Elf64_Sym	*tmp;

	i = 0;
	j = 1;
	tmp = NULL;
	while (i < size)
	{
		j = i + 1;
		while (j < size)
		{	
			str1 = ft_neutral_str(symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap));
			str2 = ft_neutral_str(symb_str + swap64(tab[j]->st_name, sizeof(tab[j]->st_name), swap));
			if (ft_strcmp(str1, str2) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			else if (ft_strcmp(str1, str2) == 0)
			{
				if (tab[i]->st_value > tab[j]->st_value)
				{
					tmp = tab[i];
					tab[i] = tab[j];
					tab[j] = tmp;
				}
			}
			free(str1);
			free(str2);
			j++;
		}
		i++;
	}
	return (tab);
}

int	sort_and_print_64(Elf64_Sym **tab, char *ptr, int nb_symb, char *symb_str, int swap)
{
	tab = ft_sort_symb_tab_64(tab, nb_symb, symb_str, swap);
	ft_print_nm_64(tab, ptr,nb_symb, symb_str, swap);
	return (0);
}
