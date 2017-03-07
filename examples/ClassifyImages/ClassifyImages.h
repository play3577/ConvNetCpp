#ifndef _ClassifyImages_ClassifyImages_h
#define _ClassifyImages_ClassifyImages_h

#include <CtrlLib/CtrlLib.h>
#include <Docking/Docking.h>
#include <ConvNetCtrl/ConvNetCtrl.h>
using namespace Upp;
using namespace ConvNet;

#include "LoaderMNIST.h"
#include "LoaderCIFAR10.h"

#define IMAGECLASS ClassifyImagesImg
#define IMAGEFILE <ClassifyImages/ClassifyImages.iml>
#include <Draw/iml_header.h>

enum {LOADER_MNIST, LOADER_CIFAR10};
enum {TYPE_LEARNER, TYPE_AUTOENCODER};

class ClassifyImages : public DockWindow {
	ParentCtrl settings;
	Label lrate, lmom, lbatch, ldecay;
	EditDouble rate, mom, decay;
	EditInt batch;
	Button apply, save_net, load_net;
	TrainingGraph graph;
	Label status;
	ParentCtrl net_ctrl;
	DocEdit net_edit;
	Button reload_btn;
	SessionConvLayers layer_view;
	ImagePrediction pred_view;
	LayerCtrl aenc_view;
	
	Splitter v_split;
	
	Session ses;
	String t;
	SpinLock ticking_lock;
	Size img_sz;
	int average_size;
	int max_diff_imgs;
	int augmentation;
	int loader, type;
	bool is_training;
	bool do_flip;
	bool has_colors;
	bool running, stopped;
	
public:
	typedef ClassifyImages CLASSNAME;
	ClassifyImages(int loader, int type);
	~ClassifyImages();
	
	virtual void DockInit();
	
	Session& GetSession() {return ses;}
	
	void Start();
	void Refresher();
	void ApplySettings();
	void OpenFile();
	void SaveFile();
	void Reload();
	void RefreshStatus();
	void StopRefresher() {running = false; while (!stopped) Sleep(100);}
	void RefreshPredictions() {pred_view.Refresh();}
	
	void UpdateNetParamDisplay();
	void ResetAll();
	void PostReload() {PostCallback(THISBACK(Reload));}
	void StepInterval(int steps);
	
};

#endif