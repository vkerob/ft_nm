#include"libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*buffer;

	if (!lst || !new)
		return ;
	if (!(*lst))
	{
		*lst = new;
		return ;
	}
	buffer = *lst;
	buffer = ft_lstlast(buffer);
	buffer->next = new;
}
