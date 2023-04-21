#include<stdio.h>



struct vektor
{
	int x;
	struct vektor *next;
};

void push_back(struct vektor *dane, int a, int *size)
{

	if (*size == 0)
	{
		dane->x = a;
	}
	else
	{
		struct vektor *danenext;
		danenext = dane;
		for (int i = 1; i < *size; i++)
		{
			danenext = danenext->next;
		}
		struct vektor *tmp;
		tmp = (struct vektor*)malloc(sizeof(struct vektor));
		tmp->x = a;
		tmp->next = 0;
		danenext->next = tmp;


	}
	*size = *size + 1;
}
int szukaj(int i, struct vektor dane)
{
	struct vektor *tmp = 0;

	if (i == 0)
	{
		return dane.x;
	}
	tmp = dane.next;

	for (int j = 1; j <= i; j++)
	{
		if (j == i)
			return tmp->x;
		else
			tmp = tmp->next;
	}

}
int* przypisz(int i, struct vektor *dane)
{
	struct vektor *tmp;
	if (i == 0)
	{
		return &(dane->x);
	}
	tmp = dane->next;

	for (int j = 1; j <= i; j++)
	{
		if (j == i)
			return &(tmp->x);
		else
			tmp = tmp->next;
	}

}
void kasuj(struct vektor *dane, int *size, int i)
{

	if (i == 0)
	{
		dane = dane->next;
	}
	else
	{
		struct vektor *danenext;
		danenext = dane;
		for (int j = 0; j < i - 1; j++)
		{
			danenext = danenext->next;
		}
		if (danenext->next->next == 0)
		{
			free(danenext->next);
			danenext->next == NULL;
		}
		else
		{
			struct vektor *next;
			next = danenext->next;
			danenext->next = danenext->next->next;
			free(next);
		}
	}
	*size = *size - 1;
}
void destroy_vecktor(struct vektor *dane,int *size)
{
	int i;
	int j;
	
	struct vektor *danenext;

		while (*size > 0)
		{
			danenext = dane;
			for (int i = 1; i < *size - 1; i++)
			{
				danenext = danenext->next;
			}
			free(danenext->next);
			danenext->next = NULL;
			*size = *size - 1;
		}

}
