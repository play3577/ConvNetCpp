#ifndef _ConvNetCtrl_HeatmapTimeView_h_
#define _ConvNetCtrl_HeatmapTimeView_h_

#include <ConvNet/ConvNet.h>
#include <CtrlLib/CtrlLib.h>

namespace ConvNet {
using namespace Upp;
using namespace ConvNet;

class HeatmapTimeView : public Ctrl {
	Session* ses;
	Graph* graph;
	RecurrentSession* rses;
	Array<Image> lines;
	Vector<double> tmp;
	int mode;
	
	enum {MODE_SESSION, MODE_GRAPH, MODE_RECURRENTSESSION};
	
public:
	typedef HeatmapTimeView CLASSNAME;
	HeatmapTimeView();
	
	virtual void Paint(Draw& d);
	void PaintSession(Draw& d);
	void PaintGraph(Draw& d);
	void PaintRecurrentSession(Draw& d);
	
	void SetSession(Session& ses) {this->ses = &ses; mode = MODE_SESSION;}
	void SetGraph(Graph& g) {this->graph = &g; mode = MODE_GRAPH;}
	void SetRecurrentSession(RecurrentSession& rses) {this->rses = &rses; mode = MODE_RECURRENTSESSION;}
	
};

}

#endif
