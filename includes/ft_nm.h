#ifndef FT_NM_H
# define FT_NM_H

# include "../libft/includes/libft.h"
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h> 
# include <elf.h>
# include <ar.h>

int		parse_elf(char *ptr, char *name, int ac, char *size_file);
int		parse_lib(char *ptr, int ac, char *size_file);
int		parse_64bits_files(char *name, char *ptr, int ac, int swap, char *size_file);
int		parse_32bits_files(char *name, char *ptr, int ac, int swap, char *size_file);
int		sort_and_print_64(Elf64_Sym **tab, char *ptr, int nb_symb, char *symb_str, int swap);
int		sort_and_print_32(Elf32_Sym **tab, char *ptr, int nb_symb, char *symb_str, int swap);
char		*ft_neutral_str(char *str);
int 		big_or_little_endian(char *ptr);
uint32_t        swap32(uint32_t val, size_t size, int swap);
uint64_t        swap64(uint64_t val, size_t size, int swap);

#endif
