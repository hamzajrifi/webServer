#include "response.hpp"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*ptr;
	int		len;
	int		len1;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	len = strlen((char *)s1);
	len1 = strlen((char *)s2);
	ptr = (char *)malloc(sizeof(char) * (len + len1 + 1));
	if (!ptr)
		return (NULL);
	*ptr = '\0';
	strlcat(ptr, (char *)s1, len + 1);
	strlcat(ptr, (char *)s2, len + len1 + 1);
	return (ptr);
}
 