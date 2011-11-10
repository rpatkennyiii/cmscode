
{
TFile f("zdctrees.root"); //Read in file zdctrees and call it f
TTree *ZDCLaser =f.Get("ZDCDigiTree"); //Grab the ZDC Digi Tree and Call it ZDCLaser

//Start the loop
TCanvas c;
TH1F *PZDC[9];
TH1F *NZDC[9];
int i;
int j;
i=1;
j=0;        //We have made two variables and set them equal to 1 and 0

char* draw="posEM1fC*(Iteration$<10)";
char* selection="posEM1fC*(Iteration$<10)>0";
char* title="ZDC Laser Run:181007 PZDC-EM1";
char* draw1="negEM1fC*(Iteration$<10)";
char* selection1="negEM1fC*(Iteration$<10)>0";
char* title1="ZDC Laser Run:181007 NZDC-EM1";
//_______________________________________________________Had Section
char* title2="ZDC Laser Run:181007 PZDC-HD1";
char* title3="ZDC Laser Run:181007 NZDC-HD1";
char* draw2="posHD1fC*(Iteration$<10)";
char* draw3="negHD1fC*(Iteration$<10)";
char* selection2="posHD1fC*(Iteration$<10)>0";
char* selection3="negHD1fC*(Iteration$<10)>0";

 for (int j = 0; j < 2;j++) {
  if(j==0){
    for ( int i = 1 ; i < 6;i++) 
   {
     if (i<=4){
       PZDC[i+4]=new TH1F(title2,title2,200,0,600);
       ZDCLaser->Project(title2,draw2,selection2) ;
       draw2[5]++;
       selection2[5]++;
       PZDC[i+4]->SetTitle(title2);
       PZDC[i+4]->SetXTitle("Charge (fc)");
       PZDC[i+4]->SetYTitle("Counts");
       PZDC[i+4]->Draw();
       c->Print((string(title2)+".png").c_str());
       title2[28]++;
     }

     PZDC[i-1]=new TH1F(title,title,100,0,500);
     ZDCLaser->Project(title,draw,selection) ;
     draw[5]++;
     selection[5]++;
     PZDC[i-1]->SetTitle(title);
     PZDC[i-1]->SetXTitle("Charge (fc)");
     PZDC[i-1]->SetYTitle("Counts");
     PZDC[i-1]->Draw();
     c->Print((string(title)+".png").c_str());
     title[28]++;
     cout << "end loop i1\n";
   }
   else {
    for (int i = 1 ; i < 6; i++) 
     {
    if (i<=4){
       NZDC[i+4]=new TH1F(title3,title3,200,0,600);
       ZDCLaser->Project(title3,draw3,selection3) ;
       draw3[5]++;
       selection3[5]++;
       PZDC[i+4]->SetTitle(title3);
       PZDC[i+4]->SetXTitle("Charge (fc)");
       PZDC[i+4]->SetYTitle("Counts");
       PZDC[i+4]->Draw();
       c->Print((string(title3)+".png").c_str());
       title3[28]++;
     }
      NZDC[i-1]=new TH1F(title1,title1,100,0,500);
      ZDCLaser->Project(title1,draw1,selection1) ;
      draw1[5]++;
      selection1[5]++;
      NZDC[i-1]->SetTitle(title1);
      NZDC[i-1]->SetXTitle("Charge (fc)");
      NZDC[i-1]->SetYTitle("Counts");
      NZDC[i-1]->Draw();
      c->Print((string(title1)+".png").c_str());
      title1[28]++;
      cout << "end loop i2 \n";
     }
    }
  }
 }
}
