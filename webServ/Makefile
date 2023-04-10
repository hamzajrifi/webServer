name = webserver

C_FLAGS = -std=c++98 -Wall -Wextra -Werror # -fsanitize=address

all : ${name}

C_FILES = 	pars_reqest.cpp \
			main.cpp \
			socket_info.cpp \
			pars_config.cpp\
			send_data.cpp\
			../response/send_response.cpp\
			../response/check_response.cpp


${name}: $(C_FILES)
	g++ ${C_FLAGS} ${C_FILES} -o ${name}

clean :
	rm -rf ${name}

re : clean all
