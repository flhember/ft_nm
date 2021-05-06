#include "ft_nm.h"

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


void	print_res(char *ptr)
{
	Elf64_Ehdr	*elf;
	Elf64_Shdr	*section_header;
	Elf64_Shdr	*section_header_strtab;
	Elf64_Sym	*table_symb;
	unsigned long	i;
	unsigned long	size;
	char		*sh_str_table;
	char 		*symb_str;

	i = 0;
	elf = (Elf64_Ehdr *)ptr;
	section_header = (Elf64_Shdr *)(ptr + elf->e_shoff);
	section_header_strtab = &section_header[elf->e_shstrndx];
	sh_str_table = (char *)ptr + section_header_strtab->sh_offset;

	while (i < elf->e_shnum)
	{
		if (section_header[i].sh_type == SHT_SYMTAB)
		{
			table_symb = (Elf64_Sym *)(ptr + section_header[i].sh_offset);
			size = section_header[i].sh_size / section_header[i].sh_entsize;
			symb_str = (char *)(ptr + section_header[section_header[i].sh_link].sh_offset);
		}
		i++;
	}
	
	i = 0;
	while (i < size)
	{
		if (ft_strlen(symb_str + table_symb[i].st_name) && ELF64_ST_TYPE(table_symb[i].st_info) != STT_FILE)
		{
			if (table_symb[i].st_value == 0)
				ft_printf("                ");
			else
				ft_printf("%016lx", (unsigned long)table_symb[i].st_value);
			ft_printf(" %c ", find_type(table_symb[i], section_header));
			ft_printf(" %s ", sh_str_table + section_header[table_symb[i].st_shndx].sh_name);
			ft_printf("%s\n", symb_str + table_symb[i].st_name);
		}
		i++;
	}

}

int	parse_64bits_files(char *name, char *ptr)
{
	if (name != NULL)
        	ft_printf("%s:\n", name);
        print_res(ptr);
	return (0);
}

