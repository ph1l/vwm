/*
 *                                  \/\/\
 *
 *  Copyright (C) 2012-2016  Vito Caputo - <vcaputo@gnugeneration.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 3 as published
 *  by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <X11/Xlib.h>

#include "screen.h"
#include "vwm.h"

	/* startup logo */

/* animated \/\/\ logo done with simple XOR'd lines, a display of the WM being started and ready */
#define VWM_LOGO_POINTS 6
void vwm_draw_logo(vwm_t *vwm)
{
	int			i;
	unsigned int		width, height, yoff, xoff;
	XPoint			points[VWM_LOGO_POINTS];
	const vwm_screen_t	*scr = vwm_screen_find(vwm, VWM_SCREEN_REL_POINTER);

	XGrabServer(vwm->display);

	/* use the dimensions of the pointer-containing screen */
	width = scr->width;
	height = scr->height;
	xoff = scr->x_org;
	yoff = scr->y_org + ((float)height * .333);
	height /= 3;

	/* the logo gets shrunken vertically until it's essentially a flat line */
	while (height -= 2) {
		/* scale and center the points to the screen size */
		for (i = 0; i < VWM_LOGO_POINTS; i++) {
			points[i].x = xoff + (i * .2 * (float)width);
			points[i].y = (i % 2 * (float)height) + yoff;
		}

		XDrawLines(vwm->display, VWM_XROOT(vwm), vwm->gc, points, sizeof(points) / sizeof(XPoint), CoordModeOrigin);
		XFlush(vwm->display);
		usleep(3333);
		XDrawLines(vwm->display, VWM_XROOT(vwm), vwm->gc, points, sizeof(points) / sizeof(XPoint), CoordModeOrigin);
		XFlush(vwm->display);

		/* the width is shrunken as well, but only by as much as it is tall */
		yoff++;
		width -= 4;
		xoff += 2;
	}

	XUngrabServer(vwm->display);
}
