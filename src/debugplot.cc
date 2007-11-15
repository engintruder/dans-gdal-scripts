/*
Copyright (c) 2007, Regents of the University of Alaska

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of the Geographic Information Network of Alaska nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include "debugplot.h"

extern int VERBOSE;

report_image_t *create_plot(double w, double h) {
	report_image_t *dbuf = (report_image_t *)malloc_or_die(sizeof(report_image_t));
	dbuf->canvas_w = w+2;
	dbuf->canvas_h = h+2;

	dbuf->img_w = (int)w+1;
	dbuf->img_h = (int)h+1;
	if(dbuf->img_w > 800) {
		dbuf->img_w = 800;
		dbuf->img_h = (int)(800.0 * (h+1) / (w+1));
	}
	if(dbuf->img_h > 800) {
		dbuf->img_w = (int)(800.0 * (w+1) / (h+1));
		dbuf->img_h = 800;
	}

	dbuf->stride_x = (int)floor(w / (double)dbuf->img_w);
	dbuf->stride_y = (int)floor(h / (double)dbuf->img_h);
	if(dbuf->stride_x < 1) dbuf->stride_x = 1;
	if(dbuf->stride_y < 1) dbuf->stride_y = 1;

	dbuf->img = (unsigned char *)malloc_or_die(dbuf->img_w*dbuf->img_h*3);
	int i;
	for(i=0; i<dbuf->img_w*dbuf->img_h*3; i++) dbuf->img[i] = 0;
	
	return dbuf;
}

void write_plot(report_image_t *dbuf, char *fn) {
	FILE *fout = fopen(fn, "wb");
	fprintf(fout, "P6\n%d %d\n255\n", dbuf->img_w, dbuf->img_h);
	fwrite(dbuf->img, dbuf->img_w*dbuf->img_h, 3, fout);
	fclose(fout);
}

void plot_point_big(report_image_t *dbuf, double x, double y, unsigned char r, unsigned char g, unsigned char b) {
	int center_x = (int)(x / dbuf->canvas_w * (double)(dbuf->img_w-1) + .5);
	int center_y = (int)(y / dbuf->canvas_h * (double)(dbuf->img_h-1) + .5);
	int dx, dy;
	for(dx=-1; dx<=1; dx++) for(dy=-1; dy<=1; dy++) {
		int plot_x = center_x + dx;
		int plot_y = center_y + dy;
		if(plot_x>=0 && plot_y>=0 && plot_x<dbuf->img_w && plot_y<dbuf->img_h) {
			unsigned char *p = dbuf->img + (plot_x + dbuf->img_w*plot_y)*3;
			*(p++) = r; *(p++) = g; *(p++) = b;
		}
	}
}

void plot_point(report_image_t *dbuf, double x, double y, unsigned char r, unsigned char g, unsigned char b) {
	int plot_x = (int)(x / dbuf->canvas_w * (double)(dbuf->img_w-1) + .5);
	int plot_y = (int)(y / dbuf->canvas_h * (double)(dbuf->img_h-1) + .5);
	if(plot_x>=0 && plot_y>=0 && plot_x<dbuf->img_w && plot_y<dbuf->img_h) {
		unsigned char *p = dbuf->img + (plot_x + dbuf->img_w*plot_y)*3;
		*(p++) = r; *(p++) = g; *(p++) = b;
	}
}

void plot_line(report_image_t *dbuf, double x0, double y0, double x1, double y1, 
unsigned char r, unsigned char g, unsigned char b) {
	double dx = (x1-x0) / dbuf->canvas_w * (double)dbuf->img_w;
	double dy = (y1-y0) / dbuf->canvas_h * (double)dbuf->img_h;
	double len = sqrt(dx*dx + dy*dy) + 2.0;
	double alpha;
	for(alpha=0; alpha<=1; alpha+=1.0/len) {
		double x = x0+(x1-x0)*alpha;
		double y = y0+(y1-y0)*alpha;
		plot_point(dbuf, x, y, r, g, b);
	}
}

void debug_plot_contours(mpoly_t *mpoly, report_image_t *dbuf) {
	if(VERBOSE) fprintf(stderr, "plotting...\n");

	int i, j;
	for(i=0; i<mpoly->num_contours; i++) {
		int v = (i%62)+1;
		int r = ((v&1) ? 150 : 0) + ((v&8) ? 100 : 0);
		int g = ((v&2) ? 150 : 0) + ((v&16) ? 100 : 0);
		int b = ((v&4) ? 150 : 0) + ((v&32) ? 100 : 0);
		contour_t c = mpoly->contours[i];
		if(c.is_hole) { r=255; g=0; b=0; }
		else { r=255; g=255; b=0; }
		if(VERBOSE) fprintf(stderr, "contour %d: %d pts color=%02x%02x%02x\n", i, c.npts, r, g, b);
		for(j=0; j<c.npts; j++) {
			double x0 = c.pts[j].x;
			double y0 = c.pts[j].y;
			double x1 = c.pts[(j+1)%c.npts].x;
			double y1 = c.pts[(j+1)%c.npts].y;
			plot_line(dbuf, x0, y0, x1, y1, r, g, b);
			plot_point(dbuf, x0, y0, 255, 255, 255);
			plot_point(dbuf, x1, y1, 255, 255, 255);
		}
	}
}

