name = webserver

all : ${name}

${name}:
	g++ -Wall -Wextra -Werror pars_reqest.cpp serv.cpp socket_info.cpp pars_config.cpp -o ${name}

clean :
	rm ${name}

re : clean all