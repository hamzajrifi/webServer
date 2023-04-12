name = webserver

all : ${name}

${name}:
	g++ -std=c++98 -Wall -Wextra -Werror pars_reqest.cpp main.cpp socket_info.cpp pars_config.cpp send_data.cpp -o ${name}

clean :
	rm ${name}

re : clean all