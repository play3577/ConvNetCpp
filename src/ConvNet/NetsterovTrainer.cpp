﻿#include "Training.h"

namespace ConvNet {
	
NetsterovTrainer::NetsterovTrainer(Net& net) : TrainerBase(net) {
	l1_decay = 0;
	l2_decay = 0;
	l2_decay_loss = 0;
	l1_decay_loss = 0;
	learning_rate = 0.01;
	momentum = 0.9;
}

void NetsterovTrainer::TrainImplem() {
	
	iter_count++;
	
	if ((iter_count % batch_size) == 0) {
		Vector<ParametersAndGradients>& parametersAndGradients = net->GetParametersAndGradients();
		
		// initialize lists for accumulators. Will only be done once on first iteration
		if (gsum.GetCount() == 0) {
			for(int i = 0; i < parametersAndGradients.GetCount(); i++) {
				ParametersAndGradients& t = parametersAndGradients[i];
				gsum.Add().SetCount(t.volume->GetLength(), 0.0);
			}
		}
		
		// perform an update for all sets of weights
		for (int i = 0; i < parametersAndGradients.GetCount(); i++) {
			ParametersAndGradients& parametersAndGradient = parametersAndGradients[i];
			
			// param, gradient, other options in future (custom learning rate etc)
			Volume& vol = *parametersAndGradient.volume;
			
			// learning rate for some parameters.
			double l2_decay_mul = IF_NULL_1(parametersAndGradient.l2_decay_mul);
			double l1_decay_mul = IF_NULL_1(parametersAndGradient.l1_decay_mul);
			double l2_decay = this->l2_decay * l2_decay_mul;
			double l1_decay = this->l1_decay * l1_decay_mul;
			
			int plen = vol.GetLength();
			for (int j = 0; j < plen; j++) {
				l2_decay_loss += l2_decay * vol.Get(j) * vol.Get(j) / 2; // accumulate weight decay loss
				l1_decay_loss += l1_decay * fabs(vol.Get(j));
				double l1_grad = l1_decay * (vol.Get(j) > 0 ? 1 : -1);
				double l2_grad = l2_decay * vol.Get(j);
				
				double gij = (l2_grad + l1_grad + vol.GetGradient(j)) / batch_size; // raw batch gradient
				
				Vector<double>& gsumi = gsum[i];
				
				double dx = gsumi[j];
				gsumi[j] = gsumi[j] * momentum + learning_rate * gij;
				dx = momentum * dx - (1.0 + momentum) * gsumi[j];
				vol.Set(j, vol.Get(j) + dx);
				
				vol.SetGradient(j, 0.0); // zero out gradient so that we can begin accumulating anew
			}
		}
	}
}

void NetsterovTrainer::Backward(int pos, double y) {
	TrainerBase::Backward(pos, y);
	
	l2_decay_loss = 0.0;
	l1_decay_loss = 0.0;
}

void NetsterovTrainer::Backward(const VolumeDataBase& y) {
	TrainerBase::Backward(y);
	
	l2_decay_loss = 0.0;
	l1_decay_loss = 0.0;
}

void NetsterovTrainer::Backward(int cols, const Vector<int>& pos, const Vector<double>& y) {
	TrainerBase::Backward(cols, pos, y);
	
	l2_decay_loss = 0.0;
	l1_decay_loss = 0.0;
}

void NetsterovTrainer::Reset() {
	TrainerBase::Reset();
	gsum.Clear();
}

String NetsterovTrainer::ToString() const {
	return Format("Netsterov: batch_size:%d, cost_loss:%2!,n, cost_reward:%2!,n, Beta1:%2!,n, Beta2:%2!,n,"
		"l1_decay:%2!,n, l2_decay:%2!,n, l1_decay_loss:%2!,n, l2_decay_loss:%2!,n, learning_rate:%2!,n, momentum:%2!,n, eps:%2!,n, ro:%2!,n",
		batch_size, cost_loss, cost_reward, Beta1, Beta2,
		l1_decay, l2_decay, l1_decay_loss, l2_decay_loss, learning_rate, momentum, eps, ro);
}

}
