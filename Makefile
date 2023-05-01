name = webserver

C_FLAGS = -std=c++98 -Wall -Wextra -Werror

all : ${name}

C_FILES = 	webServ/pars_reqest.cpp \
			webServ/main.cpp \
			webServ/socket_info.cpp \
			webServ/pars_config.cpp\
			webServ/send_data.cpp\
			response/send_response.cpp\
			response/check_response.cpp\
			response/utils.cpp\
			response/status_codes.cpp\
			response/methods.cpp\
			response/path_checker.cpp\
			response/matching_location.cpp\
			response/cgi-php/php_cgi.cpp

HEADERS =	webServ/header.h\
			response/response.hpp

${name}: ${C_FILES} ${HEADERS}
	g++ ${C_FLAGS} ${C_FILES} -o ${name}

clean :
	rm -rf ${name}

re : clean all
