#include "cub3d.h"

/*void	disp_line(double a_x, double a_y, double b_x, double b_y, int color, t_p ptr)
{
	printf("disp_line\n");
	int x; 
	int y; 
	int m;//pente 
	int h;//origin ordonnee
	int y_max;
	int x_max;
	
	m = (b_y - a_y) / (b_x - a_x);
	h = a_y - (m * a_x);
	y = a_y;
	y_max = b_y;
	if(a_y > b_y)
	{
		y = b_y;	
		y_max = a_y;
	}
	while (y <= y_max)
	{
	//	printf("y = %f, x = %f\n", y, x);
	//	printf("m = %f, h = %f\n",m,h);

		x = a_x;
		x_max = b_x;
		if(a_y > b_y)
		{
			x = b_x;	
			x_max = a_x;
		}
		while (x <= x_max)
		{
		//printf("y = %f, x = %f\n", y, x);
		//		printf("m = %f, h = %f\n",m,h);
		//		printf("true = %d\n",y == (m * x) + h);
					if (y == (m * x) + h)
						mlx_pixel_put(ptr.mlx, ptr.window, x, y, color);
			x++;
		}
		y++;
	}
}*/

/*void	disp_line(int width, int heigth, int color, t_p ptr)
{
	int x;
	int y;
 
	y = 0; 
	while (y < heigth)
	{
		x = 0;
		while (x < width)
		{	
			if((3 * x) == y)
				mlx_pixel_put(ptr.mlx, ptr.window, x, y, color);
			x++;
		}
		y++;
	}
}*/

void	my_mlx_pixel_put(t_d data, int x, int y, int color)
{
    char *dst;
	dst = data.ptr.imgs + (y * data.res.size_line + x * (data.color.bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	disp_square(int x, int y, int color, t_d data, int width)
{
	int i;
	int j;

	j = y + width; 
	while (j >= y)
	{
		i = x + width;
		while (i >= x)
		{	
				my_mlx_pixel_put(data, i, j, color);
			i--;
		}
		j--;
	}
}

void    glbDrawLine(int firstx,int firsty,int lastx,int lasty, int color, t_d data)
{
    int         x1;        /* debut de la ligne => milieu de la ligne */
    int         y1;        /* debut de la ligne => milieu de la ligne */
    int         x2;        /* fin de la ligne => milieu de la ligne */
    int         y2;        /* fin de la ligne => milieu de la ligne */
    int         dx;        /* longueur X de la droite */ 
    int         dy;        /* longueur Y de la droite */ 
    int         sub;        /* utilise pour des soustractions */
    int         remain;        /* nombre de pixel restant */
    int         error;        /* erreur du trace, doit etre corrige si positif */
    int         inc1;        /* pour des incrementations/decrementations */
    int         inc2;        /* pour des incrementations/decrementations */

    if (firstx>lastx)
    {
        x1=lastx;
        x2=firstx;
        y1=lasty;
        y2=firsty;
    }
    else
    {
        x1=firstx;
        x2=lastx;
        y1=firsty;
        y2=lasty;
    }
    dx=x2-x1;    /* longueur X de la droite */ 
    dy=y2-y1;    /* longueur Y de la droite */ 
    if ((!dx)&&(!dy)) return; /* quitte si aucun trace a effectuer */
    if (dy<0) 
    {
        dy=-dy;
        inc1=-1;
        inc2=1;
    }
    else 
    {
        inc1=1;
        inc2=1;
    }

    if (dx>dy)
    {   
        sub=dx-dy;          
        error=dy-(dx>>1);       
        remain=(dx+1)>>1;    
        do
        {
			my_mlx_pixel_put(data, x1, y1, color);
			my_mlx_pixel_put(data, x2, y2, color);
            x1+=inc2;    /* la position x1 augmente */
            x2-=inc2;   /* et la position x2 diminue (on va vers le centre) */
            if (error>=0)    /* si l'erreur est assez grande pour etre corrigee */
            {
                y1+=inc1;    /* la position y1 augmente */
                y2-=inc1;    /* et la position y2 diminue (on va vers le centre) */
                error-=sub;    /* diminue l'erreur */
            }
            else error+=dy;    /* sinon augmente l'erreur */
        } while (--remain>0);    /* diminue remain et recommence s'il reste des points a tracer */
        if (!(dx&1)) my_mlx_pixel_put(data, x1, y1, color); 
        return;        /* fin de la fonction */
    }
    else
    {   
        sub=dy-dx;
        error=dx-(dy>>1);
        remain=(dy+1)>>1;
        do
        {
			my_mlx_pixel_put(data, x1, y1, color);
            my_mlx_pixel_put(data, x2, y2, color);
            y1+=inc2;    /* la position y1 augmente */
            y2-=inc2;   /* et la position y2 diminue (on va vers le centre) */
            if (error>=0)    /* si l'erreur est assez grande pour etre corrigee */
            {
                x1+=inc2;    /* la position x1 augmente */
                x2-=inc2;    /* et la position x2 diminue (on va vers le centre) */
                error-=sub;    /* diminue l'erreur */
            }
            else error+=dx;    /* sinon augmente l'erreur */
        } while (--remain>0);    /* diminue remain et recommence s'il reste des points a tracer */
        if (!(dy&1)) my_mlx_pixel_put(data, x1, y1, color);
        return;        /* fin de la fonction */
    }
}
