
name = webserver

C_FLAGS = -std=c++98 -Wall -Wextra -Werror # -fsanitize=address

all : ${name}

C_FILES = 	pars_reqest.cpp \
			serv.cpp \
			socket_info.cpp \
			pars_config.cpp\
			../response/check_response.cpp


${name}: $(C_FILES)
	g++ ${C_FLAGS} ${C_FILES} -o ${name}

clean :
	rm -rf ${name}

re : clean all