#include "neuro.hpp"
#include "network.hpp"
using namespace std;

Neuro::Neuro (double potential) //std::vector<unsigned short int> const& synapses)
:MembranePotential(0), NumberOfSpikes(0),localTime(0),inputcurrent(0),state(false),focus(false),J(potential) {
    ring_buffer.resize(D/hequals+1);
}

Neuro::Neuro (const Neuro& neurone):MembranePotential(neurone.MembranePotential){}

double Neuro::getMembranePotential() const {
	return MembranePotential;
}


unsigned int Neuro::getNumberOfSpikes() const {
    return NumberOfSpikes;
}

std::vector<unsigned int> Neuro::getTimeOfSPike() const {
	return TimeOfSpike;
}

bool Neuro::isInRefractoryState() {
	if(TimeOfSpike.empty()) return false;
	return ((getLocalTime()*hequals-(TimeOfSpike[TimeOfSpike.size()-1])*hequals)<refrac); 
}

void Neuro::updatepotential(long time, double Iext, double S) {
	double potential(getMembranePotential());
    double inputCurrent = ring_buffer[time % (int(D/hequals)+1)];
    inputCurrent+=Network::Noise();
	
    setMembranePotential(exp(-h*hequals/tau_excitation)*potential+inputCurrent);
                           
}

bool Neuro::update(unsigned long h) {
    double S(0);
	bool spike(false);


		if((localTime-t_spike)<refrac) 
		{
			setMembranePotential(0.0);
			} 
			
		else if (MembranePotential > Vth){
			
			spike=true;
			t_spike=localTime;
			setMembranePotential(0.0);
				
				} else 
			{
				updatepotential(localTime, 0.0, 0);
			}

    ring_buffer[localTime%(int((D/hequals)+1))] = 0;
    localTime=localTime + 1;

	return spike;
}

void Neuro::setMembranePotential(double potential) {
	MembranePotential=potential;
}
long Neuro::getLocalTime() const {
	return localTime;
}

void Neuro::setLocalTime(long time) {
	localTime=time;
}
void::Neuro::setToRefactoryState() {
	setMembranePotential(Vr);
	state=true; //isrefractory
}
void Neuro::receive(unsigned long arrival, double J_E) {
    const size_t idx = arrival%(int((D/hequals)+1)); ///D over hequals give us effectively the number of steps delayed
    ring_buffer[idx] += J_E;
}

bool Neuro::focusedon() const
{
	
	return focus;
}

void Neuro::settofocusedon(bool focusedon)
{
	focus=focusedon;
	
}

Neuro::~Neuro() 
{
	
}
