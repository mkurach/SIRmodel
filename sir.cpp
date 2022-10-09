
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
        TH2D *histogram;
        int rows, col, n; 
        double alfa = 0.3;
        double beta = 0.5;
        int s, ii, rr;
        //vector<vector<int>> tvec,svec,ivec,rvec;
        TGraph *gs, *gi, *gr;
        TMultiGraph *mg;
        TCanvas *c1, *c2;



    Net (int a=20, int b=20, int i = 3) {
        vec =  vector<vector<int>> ( a , vector<int> (b, 0)); //wektor 2d
        vec_next =  vector<vector<int>> ( a , vector<int> (b, 0)); //wektor 2d
        rows = a;
        col = b;
        n = a*b;
        int r1, r2;
        for (int j = 0; j < i; j++) { //losowanie zarażonych 
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
        //makeHeatmap(i);
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
                               // cout<<random<<endl;
                                if(random<alfa) //jeśli jest mniejsz niż prawd alfa zarażenie to się zaraził
                                    vec_next[r][c] = 1;
                            }
                        }
                        num=0; 
                    }
                    else if (vec[r][c]==1){ //jeśli jest I

                        random = (double)(rand())/((double)(RAND_MAX));
                        //cout<<random<<endl;
                        if(random<beta) //jeśli jest mniejsza niż prawd beta umarnięcia to dead
                            vec_next[r][c] = 2;
                        //else // jak nie to dalej zaraża
                            //vec_next[r][c] = 1;
                    }
                    //else { //jak jest R
                        //vec_next[r][c] = 2;

                    //}
                }
            }


            vec = vec_next;
            //cout<<"Iteracja: "<<i<<endl;
            //showNet();

            i++;
        } while (i<t);
        //cout<<"removed ostatnie "<<rr<<endl;
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
        //string title = ("Krzywe epidemiologiczne dla #alpha = "+to_string(alfa)+" i #beta = "+beta+";t;N").c_str();
        
       // mg->SetTitle("Krzywe epidemiologiczne dla #alpha = "+to_string(alfa)+" i #beta = "+beta+";t;N");
     //  String.Format("{0:0.00}", 123.4567);
       // mg->SetTitle("Krzywe epidemiologiczne dla #alpha = "+alfa.str()+" i #beta = "+beta.str()+";t;N");
       char title[100];
       sprintf(title,"Krzywe epidemiologiczne dla #alpha = %.2f i #beta = %.2f;t;N",alfa,beta);
       mg->SetTitle(title);
        gPad->SetLeftMargin(0.2);
        mg->Draw("AL");
        c1->BuildLegend();
        c1->Print("wyniki5.png");
        
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

        if (i==50){
            c2->Print("wyniki.pdf(");
        }
       else if (i==100 ||i==200) {
            c2->Print("wyniki.pdf"); 
       }


    }


};

int main () {
    srand (time(NULL));
    Net net(300,300,10); //n x n, ile zarażonych losowo na początek
    //net.showNet();
    net.simulation(300);
   // net.addPoints();
   net.showGraph();
    return 0;
}

void sir() {
	main();
}
