#include"libft.h"

int	ft_lstsize(t_list *lst)
{
	int	ct;

	if (!lst)
		return (0);
	ct = 0;
	while (lst != NULL)
	{
		ct++;
		lst = lst->next;
	}
	return (ct);
}
