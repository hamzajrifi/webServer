#include "response.hpp"
# include <sys/time.h>

size_t	responseClient::get_current_time(char tmp)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (tmp == 'm')
		return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	else if (tmp == 's')
		return (tv.tv_sec);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
