NAME = ft_nm
CC = clang
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror

#-------------------------------------COLOR VARIABLE----------------------------#

red_li=$ \033[1;31m
red_da=$ \033[0;31m
grn_li=$ \033[1;32m
grn_da=$ \033[0;32m
yel=$ \033[1;33m
blu=$ \033[1;34m
mag=$ \033[1;35m
cyn=$ \033[1;36m
whi=$ \033[1;37m
end=$ \033[0m

#-----------------------------------------SRC/INC-------------------------------#

LIB_BIN 	= libft.a

#---------------------------------------PATH/FILES-------------------------------#

NM_PATH = ./nm/
OTOOL_PATH = ./otool/

SRC_PATH = /srcs/
INC_PATH = /includes/
OBJ_PATH = /obj/
LIB_PATH = ./libft/


SRC_FILES = $(LEM_IN_FILES:%=%.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

LIB_SRC = $(addprefix $(LIB_PATH), $(SRC_PATH))
OBJ_LIB = $(LIB_SRC:.c=.o)
LIB_INC = $(addprefix $(LIB_PATH), $(INC_PATH))

INC = $(addprefix $(INC_PATH), $(INC_FILES))
OBJ = $(patsubst %.c, $(OBJ_PATH)%.o, $(SRC_FILES))
LIB = $(addprefix $(LIB_PATH), $(LIB_BIN))

#------------------------------------------RULES--------------------------------#

.PHONY: clean, fclean, all, re, help, lib, lemin, norm

.DEFAULT_GOAL = help

help:
	@echo "$(whi)Usage:\n"
	@echo "\t$(red_li)make <command>$(end) : \n"
	@echo "- $(grn_li)lemin:\t$(whi)Compilation and creation of lem-in binary"
	@echo "- $(grn_li)lib:\t\t$(whi)Compilation of libft library"
	@echo "- $(grn_li)norm\t\t$(whi)Check"
	@echo "- $(grn_li)clean:\t$(whi)Delete only object files from library and lem-in"
	@echo "- $(grn_li)fclean:\t$(whi)Delete all object files and binay from lem-in and libft"
	@echo "- $(grn_li)re:\t\t$(whi)Recompile lem-in project"
	@echo "- $(grn_li)help:\t\t$(whi)Display this usage section"

lemin: $(NAME)

lib: $(LIB)

norm: $(SRC_PATH) $(INC_PATH)
	norminette $(LIB_INC)
	norminette $(LIB_SRC)
	norminette $(INC)
	norminette $(SRC_PATH)

$(OBJ_PATH):
	@mkdir $(OBJ_PATH)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@$(CC) $(CFLAGS) -c -I $(INC_PATH) -I $(LIB) $< -o $@
	@echo "Compilation of $(whi)$(notdir $<)$(grn_da) done.$(end)"

$(LIB): $(OBJ_LIB)
	@make -C $(LIB_PATH)

$(NAME): $(INC) $(LIB) $(OBJ_PATH) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME) -I $(INC_PATH)
	@echo "$(grn_li)$(notdir $(NAME))$(grn_da) created.$(end)"

clean:
	@$(RM) -R $(OBJ_PATH)
	@make clean -C $(LIB_PATH)
	@echo "$(red_li)Objects files of $(notdir $(NAME))$(red_da) removed.$(end)"

fclean: clean
	@$(RM) -R $(NAME)
	@make fclean -C $(LIB_PATH)
	@echo "$(red_li)$(notdir $(NAME))$(red_da) removed.$(end)"

re: fclean
	@make lemin
