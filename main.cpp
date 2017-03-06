#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class PompaPaliwa;

class StanPompyPaliwa {
protected:
	PompaPaliwa* pompa;
public:
	StanPompyPaliwa() {}
	StanPompyPaliwa(PompaPaliwa* p) : pompa(p) {}
	virtual void rob_swoje() = 0;
	virtual ~StanPompyPaliwa() {}
};

class PompaPaliwaWylaczona: public StanPompyPaliwa {
public:
	PompaPaliwaWylaczona() {
		rob_swoje();
	}
	
	void rob_swoje() {
		cout << "Pompa paliwa wylaczona" << endl;
	}
};

class PompaPaliwa {
private:
	friend class PompaPaliwaCisnienie;
	friend class PompaPaliwaZasilona;
	vector<StanPompyPaliwa*> stare_stany;
	StanPompyPaliwa* stan_pompy_paliwa;
	short cisnienie;
	
	void skasuj_stare_stany() {
		vector<StanPompyPaliwa*>::iterator it;
		for(it = stare_stany.begin(); it < stare_stany.end(); it++) {
			delete *it;
		}
	}
	
public:
	PompaPaliwa() : stan_pompy_paliwa(new PompaPaliwaWylaczona),
                    cisnienie(0) {}

	void zasil();
	
	void wlacz();
	
	void odetnij();
	
	~PompaPaliwa() {
		skasuj_stare_stany();
		delete stan_pompy_paliwa;
	}
};

class PompaPaliwaCisnienie: public StanPompyPaliwa {
public:
	PompaPaliwaCisnienie(PompaPaliwa* p) : StanPompyPaliwa(p) { }

	void rob_swoje() {
		if(!pompa->cisnienie) {
			pompa->cisnienie = 50;
			cout << "Pompa paliwa buduje cisnienie" << endl;
		}
		else {
			cout << "Pompa paliwa zeruje cisnienie" << endl;
			pompa->cisnienie = 0;
			pompa->stare_stany.push_back(this);
			pompa->stan_pompy_paliwa = new PompaPaliwaWylaczona;
		}
	}
};

class PompaPaliwaZasilona: public StanPompyPaliwa {
public:
	PompaPaliwaZasilona(PompaPaliwa *p) : StanPompyPaliwa(p) { }
	
	void rob_swoje() {
		cout << "Pompa paliwa zasilona" << endl;
		pompa->stare_stany.push_back(this);
		pompa->stan_pompy_paliwa = new PompaPaliwaCisnienie(pompa);
		pompa->stan_pompy_paliwa->rob_swoje();
	}
};

class PompaPaliwaPompuje: public StanPompyPaliwa {
private:
	void rob_swoje() {
		cout << "Pompa paliwa pompuje" << endl;
	}
public:
	PompaPaliwaPompuje() {
		rob_swoje();
	}
};

// stany nie są ustawiane z zewnątrz bezpośrednio
// klasa może je zmieniać wewnętrznie pod wpływem zewnętrznych akcji
void PompaPaliwa::zasil() {
	stare_stany.push_back(stan_pompy_paliwa);
	stan_pompy_paliwa = new PompaPaliwaZasilona(this);
	stan_pompy_paliwa->rob_swoje();
}

void PompaPaliwa::wlacz() {
	stare_stany.push_back(stan_pompy_paliwa);
	stan_pompy_paliwa = new PompaPaliwaPompuje;
}

void PompaPaliwa::odetnij() {
	stare_stany.push_back(stan_pompy_paliwa);
	stan_pompy_paliwa = new PompaPaliwaCisnienie(this);
	stan_pompy_paliwa->rob_swoje();
}

int main()
{
	PompaPaliwa pompa;
	pompa.zasil();
	pompa.wlacz();
	pompa.odetnij();
}
