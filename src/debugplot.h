/*
Copyright (c) 2013, Regents of the University of Alaska

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

This code was developed by Dan Stahlke for the Geographic Information Network of Alaska.
*/



#ifndef DANGDAL_DEBUGPLOT_H
#define DANGDAL_DEBUGPLOT_H

#include <string>
#include <vector>

#include "common.h"
#include "polygon.h"

namespace dangdal {

enum DebugPlotMode {
	PLOT_NORMAL,
	PLOT_RECT4,
	PLOT_CONTOURS,
	PLOT_PINCH
};

class DebugPlot {
public:
	DebugPlot(double w, double h, DebugPlotMode _mode);

	void writePlot(const std::string &fn);
	void plotPointBig(double x, double y, uint8_t r, uint8_t g, uint8_t b);
	void plotPoint(double x, double y, uint8_t r, uint8_t g, uint8_t b);
	void plotLine(Vertex p0, Vertex p1, uint8_t r, uint8_t g, uint8_t b);
	void debugPlotRing(const Ring &ring, uint8_t r, uint8_t g, uint8_t b);
	void debugPlotMpoly(const Mpoly &mpoly);

private:
	std::vector<uint8_t> img;
	double canvas_w, canvas_h;
	size_t img_w, img_h;
public:
	int stride_x, stride_y;
	DebugPlotMode mode;
};

} // namespace dangdal

#endif // ifndef DANGDAL_DEBUGPLOT_H
