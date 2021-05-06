#ifndef FT_NM_H
# define FT_NM_H

# include "../libft/includes/libft.h"
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h> 
# include <elf.h>

int     parse_64bits_files(char *name, char *ptr, int ac);
int     parse_32bits_files(char *name, char *ptr);

#endif
