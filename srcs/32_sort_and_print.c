#include "ft_nm.h"

char    ft_find_type_bis_32(uint32_t st_shndx, Elf32_Shdr *shdr, char c, int swap)
{
	uint32_t	sh_type, sh_flags;

	sh_type = swap32(shdr[st_shndx].sh_type, sizeof(shdr[st_shndx].sh_type), swap);
	sh_flags = swap32(shdr[st_shndx].sh_flags, sizeof(shdr[st_shndx].sh_flags), swap);
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
	else if (sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR)) //.plt
		c = 'T';
	else if (sh_type == SHT_DYNAMIC) //.dynamic
		c = 'D';
	else
		c = '?';
	return (c);
}

char    ft_find_type_32(Elf32_Sym sym, Elf32_Shdr *shdr, int swap)
{
	char		c;
	uint32_t	st_info, st_shndx;

	c = '\0';
	st_info = swap32(sym.st_info, sizeof(sym.st_info), swap);
	st_shndx = swap32(sym.st_shndx, sizeof(sym.st_shndx), swap);
	if (ELF32_ST_BIND(st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF32_ST_BIND(st_info) == STB_WEAK)
	{
		c = 'W';
		if (st_shndx == SHN_UNDEF)
			c = 'w';
		if (ELF32_ST_TYPE(st_info) == STT_OBJECT)
			c = 'V';
	}
	else if (st_shndx == SHN_UNDEF)
		c = 'U';
	else if (st_shndx == SHN_ABS)
		c = 'A';
	else if (st_shndx == SHN_COMMON)
		c = 'C';
	else
		c = ft_find_type_bis_32(st_shndx, shdr, c, swap);
	if (ELF32_ST_BIND(st_info) == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}

void	ft_print_nm_32(Elf32_Sym **tab, char *ptr, int size, char *symb_str, int swap)
{
	int		i;
	char		c;
	Elf32_Ehdr	*elf;
	Elf32_Shdr	*shdr;
	uint32_t	st_shndx;

	i = 0;
	elf = (Elf32_Ehdr *)ptr;
	shdr = (Elf32_Shdr *)(ptr + swap32(elf->e_shoff, sizeof(elf->e_shoff), swap));
	while (i < size)
	{
		c = ft_find_type_32(*tab[i], shdr, swap);
		st_shndx = swap32(tab[i]->st_shndx, sizeof(tab[i]->st_shndx), swap);
		if (st_shndx == SHN_UNDEF)
			printf("        ");
		else
			printf("%08lx", (unsigned long)swap32(tab[i]->st_value, sizeof(tab[i]->st_value), swap));
		printf(" %c ", c);
		printf("%s\n", symb_str + swap32(tab[i]->st_name, sizeof(tab[i]->st_name), swap));
		i++;
	}
}

Elf32_Sym	**ft_sort_symb_tab_32(Elf32_Sym **tab, int size, char *symb_str, int swap)
{
	int		i;
	int		j;
	int		res;
	Elf32_Sym	*tmp;
	char		*str1, *str2;

	i = 0;
	j = 1;
	tmp = NULL;
	while (i < size)
	{
		j = i + 1;
		while (j < size)
		{
			str1 = ft_neutral_str(symb_str + swap32(tab[i]->st_name, sizeof(tab[i]->st_name), swap));
			str2 = ft_neutral_str(symb_str + swap32(tab[j]->st_name, sizeof(tab[j]->st_name), swap));
			res = ft_strcmp(str1, str2);
			if (res > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			else if (res == 0)
			{
				if (swap32(tab[i]->st_value, sizeof(tab[i]->st_value), swap) > swap32(tab[j]->st_value, sizeof(tab[j]->st_value), swap))
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

int	sort_and_print_32(Elf32_Sym **tab, char *ptr, int nb_symb, char *symb_str, int swap)
{
	tab = ft_sort_symb_tab_32(tab, nb_symb, symb_str, swap);
	ft_print_nm_32(tab, ptr, nb_symb, symb_str, swap);
	return (0);
}
