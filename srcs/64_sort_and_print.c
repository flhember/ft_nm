#include "ft_nm.h"

char    ft_find_type_bis_64(uint64_t st_shndx, Elf64_Shdr *shdr, char c, int swap)
{
	uint64_t	sh_type, sh_flags;

	sh_type = swap64(shdr[st_shndx].sh_type, sizeof(shdr[st_shndx].sh_type), swap);
	sh_flags = swap64(shdr[st_shndx].sh_flags, sizeof(shdr[st_shndx].sh_flags), swap);
	if (sh_type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.bss
		c = 'B';
	else if (sh_type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS)) //.tbss
		c = 'B';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_MERGE)) //.rodata*
		c = 'R';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC)) //.rodata*
		c = 'R';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_MERGE | SHF_STRINGS)) //.rodata str
		c = 'R';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.data*
		c = 'D';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_WRITE | SHF_ALLOC | SHF_TLS)) //.tdata*
		c = 'D';
	else if (sh_type == SHT_INIT_ARRAY && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.init_array
		c = 'D';
	else if (sh_type == SHT_FINI_ARRAY && sh_flags == (SHF_ALLOC | SHF_WRITE)) //.fini_array
		c = 'D';
	else if (sh_type == SHT_PREINIT_ARRAY && sh_flags == (SHF_WRITE | SHF_ALLOC)) //.preinit_array
		c = 'D';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) //.text
		c = 'T';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR | SHF_GROUP)) //.text with group
		c = 'T';
	else if (sh_type == SHT_GROUP && sh_flags == 0) //.group
		c = 'N';
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
		if (ELF64_ST_TYPE(st_info) == STT_OBJECT) {
			c = 'V';
			if (st_shndx == SHN_UNDEF)
				c = 'v';
		}
	}
	else if (st_shndx == SHN_UNDEF)
		c = 'U';
	else if (st_shndx == SHN_ABS)
		c = 'A';
	else if (st_shndx == SHN_COMMON)
		c = 'C';
	else if (ELF64_ST_TYPE(st_info) == STT_GNU_IFUNC)
		c = 'i';
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
	uint64_t	st_shndx;

	//uint64_t	st_info;
	//uint64_t	sh_flags;

	elf = (Elf64_Ehdr *)ptr;
	shdr = (Elf64_Shdr *)(ptr + swap64(elf->e_shoff, sizeof(elf->e_shoff), swap));
	i = 0;
	while (i < size)
	{
		c = ft_find_type_64(*tab[i], shdr, swap);
		st_shndx = swap64(tab[i]->st_shndx, sizeof(tab[i]->st_shndx), swap);
		if (st_shndx == SHN_UNDEF)
			printf("                ");
		else
			printf("%016lx", (unsigned long)swap64(tab[i]->st_value, sizeof(tab[i]->st_value), swap));
		printf(" %c ", c);
		printf("%s\n", symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap));
		/*if (ft_strcmp(symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap), "__progname@GLIBC_2.2.5") == 0
			|| ft_strcmp(symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap), "__init_array_end") == 0
			|| ft_strcmp(symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap), "__init_array_start") == 0
			|| ft_strcmp(symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap), "__do_global_dtors_aux_fini_array_entry") == 0)
		{
			st_info = swap64(tab[i]->st_info, sizeof(tab[i]->st_info), swap);
			sh_flags = swap64(shdr[st_shndx].sh_flags, sizeof(shdr[st_shndx].sh_flags), swap);


			printf("\n");
			printf("%s\n", symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap));
			printf("shndx = %lu\n", (unsigned long)swap64(tab[i]->st_shndx, sizeof(tab[i]->st_shndx), swap));
			printf("st bind = %d\n", ELF64_ST_BIND(st_info));
			printf("st type = %lu\n", ELF64_ST_TYPE(st_info));
			printf("st flags = %lu\n", sh_flags);
			printf("%u\n", SHT_SUNW_move);
			printf("\n");
		}*/
		i++;
	}
}

// for sort we pass all str in "neutral" like write@GLIBC_2.2.5 -> WRITEGLIBC225
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
	int		res;
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
			res = ft_strcmp(str1, str2);
			if (res > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			else if (res == 0)
			{
				res = ft_strcmp((symb_str + swap64(tab[i]->st_name, sizeof(tab[i]->st_name), swap)), (symb_str + swap64(tab[j]->st_name, sizeof(tab[j]->st_name), swap)));
				//if (tab[i]->st_value > tab[j]->st_value)
				if (res > 0)
				{
					tmp = tab[i];
					tab[i] = tab[j];
					tab[j] = tmp;
				}
				else if (res == 0)
				{
					if (swap64(tab[i]->st_value, sizeof(tab[i]->st_value), swap) > swap64(tab[j]->st_value, sizeof(tab[j]->st_value), swap))
					{
						tmp = tab[i];
						tab[i] = tab[j];
						tab[j] = tmp;
					}
					/*else if (swap64(tab[i]->st_value, sizeof(tab[i]->st_value), swap) == swap64(tab[j]->st_value, sizeof(tab[j]->st_value), swap))
					{
						printf("value = value\n");
						tmp = tab[j];
						tab[j] = tab[i];
						tab[i] = tmp;
					}*/
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
