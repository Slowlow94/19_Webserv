NAME	= webserv

CC	= c++
FLAGS	= -std=c++98 -Wall -Wextra -Werror
RM	= rm -rf

OBJDIR	= .objs
FILES	=	src/main \
			src/CGI/Cgi \
			src/ServerHandler \
			src/ServerBase \
			src/HttpRequestHandler \
			src/HttpResponseHandler \
			src/HttpResponseMethod \
			src/HttpResponseUtils \
			src/HttpRequestUtils \
			src/HttpRequestParsing \
			src/HttpRequestDir \
			src/HttpRequestLocUtils \
			src/DirectoryHandler \
			src/RequestResponseState \
			src/Logger \
			src/ErrorHandler \
			src/configuration_file_parsing/config_parser/ConfigParser \
			src/configuration_file_parsing/config_parser/ConfigParserUtils \
			src/configuration_file_parsing/config_parser/DirectiveParser \
			src/configuration_file_parsing/server_config/ADirective \
			src/configuration_file_parsing/server_config/LocationBlock \
			src/configuration_file_parsing/server_config/ServerBlock \
			src/configuration_file_parsing/server_config/ServerConfig \
			src/configuration_file_parsing/utils/PathValidator \
			src/configuration_file_parsing/utils/TokenCounter 

SRC	= $(FILES:=.cpp)
OBJ	= $(addprefix $(OBJDIR)/, $(FILES:=.o))
DEPS	= $(OBJ:.o=.d)
HEADER	=	src/CGI/Cgi.hpp \
			src/ServerHandler.hpp \
			src/ServerHandler.hpp \
			src/HttpRequestHandler.hpp \
			src/HttpResponseHandler.hpp \
			src/RequestResponseState.hpp \
			src/Logger.hpp \
			src/ErrorHandler.hpp \
			src/configuration_file_parsing/config_parser/ConfigParser.hpp \
			src/configuration_file_parsing/server_config/ADirective.hpp \
			src/configuration_file_parsing/server_config/LocationBlock.hpp \
			src/configuration_file_parsing/server_config/ServerBlock.hpp \
			src/configuration_file_parsing/server_config/ServerConfig.hpp \
			src/configuration_file_parsing/utils/PathValidator.hpp \
			src/configuration_file_parsing/utils/TokenCounter.hpp 

GREEN	= \e[92m
YELLOW	= \e[93m
BLUE	= \e[94m
RESET	= \e[0m

all:	$(NAME)

$(NAME):	$(OBJ) $(HEADER)
	@$(CC) $(OBJ) -o $(NAME)
	@printf "$(GREEN)- $(NAME) is ready\n$(RESET)"

run:	all
	./$(NAME)

$(OBJDIR)/%.o: %.cpp $(HEADER)
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

clean:
	@$(RM) $(OBJDIR) $(OBJ)
	@printf "$(BLUE)- .o files removed\n$(RESET)"

fclean:	clean
	@$(RM) $(NAME)
	@printf "$(YELLOW)- program deleted\n$(RESET)"

re: fclean all