#include <fstream>
#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

ifstream fin("intrare.in");

#define nrMax 10000
#define nrit 50
bool esteSol(int sol[]);
void cp(int p1[], int p2[]);
void reset();//reinitializeaza vectorul de culori disponibile
int eval(int sol[]);

bool ad[nrMax][nrMax];
int disp[nrMax + 1];
int newvec[nrMax + 1];
int sol[nrMax];
int csol[nrMax];
int globalsol[nrMax];
int bestn[nrMax];
int n;

int main() {
	using namespace std::literals::chrono_literals;
	auto startT = std::chrono::high_resolution_clock::now();
	int i, j, it, val, pos, cr, semn, cop, x, avg = 0, m;
	fin >> n; fin >> m;
	bool local;
	int best = n + 1;
	int worst = -1;
	double infL = 0, supL = n;
	for (x = 1; x <= m; x++)
	{
		fin >> i >> j;
		ad[i][j] = 1;
	}
	reset();
	srand(time(nullptr));

	//incepe hill Climb
	for (it = 1; it <= nrit; it++)
	{
		//generez solutia random

		for (i = 1; i <= n; i++)//la fiecare nod
		{//parcurg vecinii si le elimin culorile in disp

			for (j = 1; j <= n; j++)
				if ((ad[i][j]) && (sol[j]))
					disp[sol[j]] = 0;

			//nodul i primeste valoarea random din cele dinsponibile
			pos = 1;
			for (j = 1; j <= n; j++)
				if (disp[j]) {
					newvec[pos] = disp[j];
					pos++;
				}
			pos--;
			val = rand() % pos + 1;
			sol[i] = newvec[val];
			reset();
		}
		//--------pana aici e corect sigur si verificat
	//fac HC
		local = 0;
		do {
			cr = eval(sol);
			//cout<<cr<<' ';
			cp(csol, sol);
			for (j = 1; j <= n; j++)
				for (semn = 0; semn < 2; semn++) {
					cop = csol[j];
					csol[j] = csol[j] + semn - (1 - semn);
					if ((cr > eval(csol)) && (esteSol(csol))) {
						cr = eval(csol);
						cp(bestn, csol);
					}
					csol[j] = cop;
				}
			if (eval(sol) > eval(bestn))
			{
				cp(sol, bestn);
			}
			else local = true;
		} while (local == 0);
		//--aici ies din parcurgerea vecinilor a unei solutii

		//afisare de proba
		/*for(x=1;x<=n;x++)
			cout<<sol[x]<<' ';
		cout<<'\n';*/

		//aici fac selectia minimului si a maximului global , dupa ce solutia mea este optima locala
		//se poate folo
		val = eval(sol);
		if ((val) && (val < best))
		{
			best = val;
			cp(globalsol, sol);
		}
		if (val > worst)
			worst = val;
		avg = avg + val;
	}
	avg = avg / nrit;
	//afisarile finale, singurele care vor ramane in prezentare
	cout << best << ' ' << avg << ' ' << worst << ' ';
	auto endT = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = endT - startT;
	cout << duration.count();
	//cout<<'\n';
	/*for(x=1;x<=n;x++)
		cout<<globalsol[x]<<' ';
	cout<<'\n';*/
	return 0;
}
void reset()
{
	int i;
	for (i = 1; i <= n; i++)
	{
		disp[i] = i;
		newvec[i] = 0;
	}
}
void cp(int p1[], int p2[])
{
	int k;
	for (k = 1; k <= n; k++)
	{
		p1[k] = p2[k];
	}
}
int eval(int sol[])//verifica cate culori au fost folosite
{
	int i;
	bool used[nrMax];
	int nrused = 0;
	for (i = 1; i <= n; i++)
	{
		if (used[sol[i]] == 0)
		{
			nrused++;
			used[sol[i]] = 1;
		}
	}
	for (i = 1; i <= n; i++) used[i] = 0;
	return nrused;
}
bool esteSol(int prm[])
{
	int i, j;
	for (i = 1; i <= n; i++)
	{
		if ((prm[i] == 0) || (prm[i] == n + 1))
			return false;
		for (j = 1; j <= n; j++)
			if ((i != j) && (ad[i][j]) && (prm[i] == prm[j]))
				return 0;
	}
	return 1;
}