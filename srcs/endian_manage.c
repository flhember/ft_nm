#include "ft_nm.h"

int					big_or_little_endian(char *ptr)
{
	int				x;
	char			*y;

	x = 1;
	y = (char *)&x;
	if ((unsigned char)ptr[EI_DATA] == (unsigned char)(*y))
		return (0);
	return (1);
}

uint32_t			swap32(uint32_t val, size_t size, int swap)
{
	int				i;
	uint32_t		val_swap;
	unsigned char	*ptr_val;
	unsigned char	*ptr_final;

	i = 0;
	val_swap = 0;
	if (swap == 0)
		return (val);
	ptr_val = (unsigned char *)&val;
	ptr_final = (unsigned char *)&val_swap;
	while (--size)
		ptr_final[i++] = ptr_val[size];
	ptr_final[i++] = ptr_val[size];
	return (val_swap);
}

uint64_t			swap64(uint64_t val, size_t size, int swap)
{
	int				i;
	uint64_t		val_swap;
	unsigned char	*ptr_val;
	unsigned char	*ptr_final;

	i = 0;
	val_swap = 0;
	if (swap == 0)
		return (val);
	ptr_val = (unsigned char *)&val;
	ptr_final = (unsigned char *)&val_swap;
	while (--size)
		ptr_final[i++] = ptr_val[size];
	ptr_final[i++] = ptr_val[size];
	return (val_swap);
}
