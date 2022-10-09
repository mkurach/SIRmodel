
#include <iostream>
#include <vector>
#include <time.h>
#include <random>
#include <TGraph.h>
#include <TCanvas.h>
#include <TH2.h>
#include <string.h>
#include <TSystem.h>
using namespace std;


class Net { // S = 0, I = 1, R =2
    public:
        vector<vector<int>> vec;
        vector<vector<int>> vec_next;
        TH2D *histogram, *heatmap;

        int rows, col, n; 
        double alfa=0.3;
        double beta=0.05;
        int s, ii, rr;
        int inf;
        TGraph *gs, *gi, *gr;
        TMultiGraph *mg;
        TCanvas *c1, *c2;



    Net (int a=20, int b=20, int i = 3) {
        rows = a;
        col = b;
        vec =  vector<vector<int>> ( rows , vector<int> (col, 0)); //wektor 2d
        vec_next =  vector<vector<int>> ( rows , vector<int> (col, 0)); //wektor 2d

        n = a*b;
        inf=i;
        int r1, r2;
        for (int j = 0; j < inf; j++) { //losowanie zarażonych 
            r1 = rand()%rows;
            r2 = rand()%col;
            vec[r1][r2] = 1;
            vec_next[r1][r2] = 1;
        }
        gs = new TGraph();
        gi = new TGraph();
        gr = new TGraph();
        mg = new TMultiGraph();
    }


    void showNet() {
        for(int i = 0; i < rows; i++) {
            for( int j = 0; j < col; j++){
                 cout << vec[i][j] << " ";
            }
            cout<< endl;
        }
    }

    void simulation(int t) { //t - ile iteracji
        int i = 0;
        double random;
        int num = 0;
        do {
            addPoints(i);
            makeHeatmap(i);
            for(int r = 0; r < rows; r++) {
                for( int c = 0; c < col; c++){ //lecimy po każdym pikselu

                    if(vec[r][c]==0){ //jeśli S
                        //sprawdzamy ile ma sąsiadów, którzy są I
                        if(r!=0) { //patrzymy u góry
                            if(vec[r-1][c]==1)
                                num++;
                        }
                        if(r!=(rows-1)) {//patrzymy u dołu 
                            if(vec[r+1][c]==1)
                                num++;
                        }
                        if(c!=0) {//patrzymy po lewo
                            if(vec[r][c-1]==1)
                                num++;
                        }
                        if(c!=(col-1)) {//patrzymy po prawo
                            if(vec[r][c+1]==1)
                                num++;
                        }
                        //jeśli miał chociaż jednego zarażonego sąsiada
                        if(num>0) {
                            for(int k = 0; k < num; k++){
                                random = (double)(rand())/((double)(RAND_MAX));
                                if(random<alfa) //jeśli jest mniejsz niż prawd alfa zarażenie to się zaraził
                                    vec_next[r][c] = 1;
                            }
                        }
                        num=0; 
                    }
                    else if (vec[r][c]==1){ //jeśli jest I

                        random = (double)(rand())/((double)(RAND_MAX));
                        if(random<beta) //jeśli jest mniejsza niż prawd beta umarnięcia to dead
                            vec_next[r][c] = 2;
                    }
                  
                }
            }


            vec = vec_next;
            i++;
        } while (i<t);
        showGraph();
    }

    void simulation2(int t, double krok) { 
        heatmap = new TH2D("heatmapa",("Heatmapa liczby removed osob z "+to_string(n)+" wszystkich osob w zaleznosci od #alpha i #beta;#alpha    S->I;#beta    I -> R").c_str(),1/krok,0,1,1/krok,0,1);
        heatmap->SetStats(0);
        //string title = ;
        //heatmap->SetTitle(title);

        int i = 0;
        double random;
        int num = 0;
        int removed = 0;
        int r1, r2;
        int binalfa = 0;
        int binbeta = 0;
        for (double a = 0; a < 1+krok; a+=krok) { //dla różnych alfa
            alfa = a;
                for (double b = 0; b < 1+krok; b+=krok) { // dla różnych beta
                    beta = b;
                    cout<<"alfa: "<<alfa<<"; beta: "<<beta<<endl;

                    //symulacja, przekopiownay kod bo nie działa inaczej idk

                    //tworzymy siątkę od nowa
                    
                    vec =  vector<vector<int>> ( rows , vector<int> (col, 0)); //wektor 2d
                    vec_next =  vector<vector<int>> ( rows , vector<int> (col, 0)); //wektor 2d
                    
                    for (int j = 0; j < inf; j++) { //losowanie zarażonych 
                        r1 = rand()%rows;
                        r2 = rand()%col;
                        vec[r1][r2] = 1;
                        vec_next[r1][r2] = 1;
                    }

                    
                    //symulacja
                    i = 0;
                    num = 0;
                    do {
                        for(int r = 0; r < rows; r++) {
                            for( int c = 0; c < col; c++){ //lecimy po każdym pikselu

                                if(vec[r][c]==0){ //jeśli S
                                    //sprawdzamy ile ma sąsiadów, którzy są I
                                    if(r!=0) { //patrzymy u góry
                                        if(vec[r-1][c]==1)
                                            num++;
                                    }
                                    if(r!=(rows-1)) {//patrzymy u dołu 
                                        if(vec[r+1][c]==1)
                                            num++;
                                    }
                                    if(c!=0) {//patrzymy po lewo
                                        if(vec[r][c-1]==1)
                                            num++;
                                    }
                                    if(c!=(col-1)) {//patrzymy po prawo
                                        if(vec[r][c+1]==1)
                                            num++;
                                    }
                                    //jeśli miał chociaż jednego zarażonego sąsiada
                                    if(num>0) {
                                        for(int k = 0; k < num; k++){
                                            random = (double)(rand())/((double)(RAND_MAX));
                                                if(random<alfa) //jeśli jest mniejsz niż prawd alfa zarażenie to się zaraził
                                                vec_next[r][c] = 1;
                                        }
                                    }
                                    num=0; 
                                }
                                else if (vec[r][c]==1){ //jeśli jest I

                                    random = (double)(rand())/((double)(RAND_MAX));
                                    if(random<beta) //jeśli jest mniejsza niż prawd beta umarnięcia to dead
                                        vec_next[r][c] = 2;
                                }
                                else if (vec[r][c]==2  && i == (t-1)) {
                                    removed++;
                                }
                  
                            }
                        }
                        vec = vec_next;
                        i++;
                    } while (i<t);

                    heatmap->SetBinContent(binalfa, binbeta, removed);
                    binbeta++;
                    cout<<"removed: "<<removed<<endl;
                    removed=0;
                }
                binbeta = 0;
                binalfa++;
        }
        c2 = new TCanvas("c2","Heatmapa",1000,1000);
        heatmap->Draw("colz");
        c2->Print("wyniki.pdf");




    }

    void addPoints(int i ) {
        s = 0;
        ii = 0;
        rr = 0;
        for(int r = 0; r < rows; r++) {
            for( int c = 0; c < col; c++){ //lecimy po każdym pikselu
                switch (vec[r][c]){
                    case 0:
                        s++;
                        break;
                    case 1:
                        ii++;
                        break;
                    case 2:
                        rr++; 
                        break;  
                }

            }
        }
        gs->SetPoint(i,i,s);
        gi->SetPoint(i,i,ii);
        gr->SetPoint(i,i,rr);

    }

    void showGraph(){
        
        gs->SetTitle("Susceptible S(t)");
        gs->SetLineColor(2);
        gs->SetLineWidth(4);
        gs->SetFillStyle(0);

        gi->SetTitle("Infectious I(t)");
        gi->SetLineColor(3);
        gi->SetLineWidth(4);
        gi->SetFillStyle(0);

        gr->SetTitle("Removed R(t)");
        gr->SetLineColor(4);
        gr->SetLineWidth(4);
        gr->SetFillStyle(0);

        mg->Add(gs);
        mg->Add(gi);
        mg->Add(gr);
        c1 = new TCanvas("c1","Krzywe epidemiologiczne",1000,1000);
        
        mg->SetTitle("Krzywe epidemiologiczne;t;N");
        gPad->SetLeftMargin(0.2);
        mg->Draw("AL");
        c1->BuildLegend();
        
    }

    void makeHeatmap (int i) {

        histogram = new TH2D(("h"+to_string(i)).c_str(),("Siatka zarazen t="+to_string(i)).c_str(),col,0,col,rows,0,rows);
        histogram->SetStats(0);
        for(int i = 0; i < rows; i++) {
            for( int j = 0; j < col; j++){ //pętla po kazdym pikselu siatki
                 for(int k = 0; k<= vec[i][j]; k++)//uzupełniamy dany piksel histogramu tyle razy ile jest wartość danego piksela na siatce
                    histogram->Fill(i,j);
            }
            
        }
        if (i==0) {
            c2 = new TCanvas("c2","Heatmapa zarazen",1000,1000);
            
            histogram->Draw("colz");
        }
        else {
            gPad->Modified(); 
            gPad->Update(); 
            gSystem->ProcessEvents();
            histogram->Draw("colz");
            gSystem->Sleep(1); //ms
        }
       


    }


};

int main () {
    srand (time(NULL));
    Net net(20,20,3); //n x n, ile zarażonych losowo na początek
    //net.simulation(200);
    net.simulation2(200,0.005);
    return 0;
}

void sir2() {
	main();
}
