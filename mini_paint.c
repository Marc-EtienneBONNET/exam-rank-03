/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbonnet <mbonnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 11:42:11 by mbonnet           #+#    #+#             */
/*   Updated: 2021/12/08 12:24:01 by mbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct s_zone
{
	int		height;
	int		width;
	char	backgroud;
}	t_zone;

typedef struct s_shape
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	color;
}	t_shape;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char *get_zone(FILE *file, t_zone *zone)
{
	char	*tmp;
	int		i;

	if (fscanf(file,"%d %d %c", &zone->width, &zone->height, &zone->backgroud) != 3)
		return (NULL);
	if (zone->height > 300 || zone->height <= 0 || zone->width <= 0 || zone->width > 300)
		return (NULL);
	if (!(tmp = malloc(sizeof(char) * ((zone->height * zone->width) + 1))))
		return (NULL);
	i = 0;
	while (i < zone->height * zone->width)
		tmp[i++] = zone->backgroud;
	tmp[i] = '\0';
	return (tmp);
}

int	in_circle(int x, int y, t_shape *shape)
{
	float	dist;

	dist = sqrtf(powf(x - shape->x, 2.) + powf(y - shape->y, 2.));
	if (shape->radius >= dist)
	{
		if (shape->radius - dist < 1.00000000)
			return (2);
		return (1);
	}
	return (0);
}

void	draw_shape(t_zone *zone, t_shape *shape, char *drawing)
{
	int	x;
	int	y;
	int is_it;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			is_it = in_circle(x, y, shape);
			if ((shape->type == 'c' && is_it == 2) || (shape->type == 'C' && is_it))
				drawing[(y * zone->width) + x] = shape->color;
			x++;
		}
		y++;
	}
}

int	draw_shapes(FILE *file, t_zone *zone, char *drawing)
{
	t_shape	shape;
	int		ret;

	while (ret = (fscanf(file, "%c %f %f %f %c", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.color) == 5))
	{
		if (shape.radius <= 0.000000 || (shape.type != 'c' && shape.type != 'C'))
			return (-1);
		draw_shape(zone, &shape, drawing);
	}
	if (ret != 0)
		return (-1);
	return (0);
}

void	draw_drawing(char *drawing, t_zone *zone)
{
	int x;
	int y;

	y = 0;
	while (y < zone->height)
	{
		write(1, drawing + (y * zone->width), zone->width);
		write(1, "\n",1);
		y++;
	}
}

int	ft_erreur(char *str)
{
	if (str)
		write(1, str, ft_strlen(str));
	return (-1);
}

void	ft_clean(FILE *file, char *drawing, char *str)
{
	if (file)
		fclose(file);
	if (drawing)
		free(drawing);
	if (str)
		ft_erreur(str);
}

int	main(int ac, char **av)
{
	t_zone zone;
	char *drawing;
	FILE *file;

	zone.width = 0;
	zone.height = 0;
	zone.backgroud = 0;
	drawing = NULL;
	if (ac != 2)
		return (ft_erreur("ERREUR, les argument sont incorecte\n"));
	if (!(file = fopen(av[1], "r")))
		return (ft_erreur("ERREUR, les argument sont incorecte\n"));
	if (!(drawing = get_zone(file, &zone)))
	{
		ft_clean(file, drawing, "ERREUR, les argument sont incorecte\n");
		return (-1);
	}
	if (draw_shapes(file, &zone, drawing) == -1)
	{
		ft_clean(file, drawing, "ERREUR, le fichier est incorecte\n");
		return (-1);
	}
	draw_drawing(drawing, &zone);
	return (0);
}