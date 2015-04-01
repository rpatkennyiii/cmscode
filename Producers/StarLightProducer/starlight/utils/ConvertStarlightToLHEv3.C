/* Routine for the conversion of starlight data format
 * into something readable by CMSSW_4_4_3
 *
 * Modification by X. Rouby on a routine kindly offered by J. Hollar
 * Sept. 28, 2007
 *
 * Modification by P. Kenny on a routine kindly offered by J. Hollar
 * March 6, 2012
 */ 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

void ConvertStarlightToLHEv3(int nPerFile=1000, string filename = "slight.out", string outdir="")
{
  bool gamma=false;
  ifstream infile(filename.c_str());
  if(filename.find("Gamma") != filename.npos) gamma=true;
  char outfilename[100];
  if (! infile.is_open()) { cout << "\t ERROR: I can not open \"" << filename << "\"" << endl; return; }

  string temp_string, temp;
  istringstream curstring;
  double MU = 0.105658369; // muon mass [GeV]
  int evt_n=0; // event_number, read from the input-file
  int nn=0; // event_counter, in the output file

  ofstream output;

  while (getline(infile,temp_string)) {
    curstring.clear(); // needed when using several tims istringstream::str(string)
    curstring.str(temp_string);

    if(strstr(temp_string.c_str(),"EVENT")){
      curstring >> temp >> evt_n;

        if(!(nn%nPerFile)){
          sprintf(outfilename,"%sslight%i.lhe",outdir.c_str(),nn/nPerFile);
          output.open(outfilename);
          output << "<LesHouchesEvents version=\"1.0\">"  << endl;
          output << "<header>" << endl;
          output << "This file was created from the output of the STARLIGHT generator" << endl;
          output << "</header>" << endl;

          output << "<init>" << endl;
          output << "2212  2212  0.50000000000E+04  0.50000000000E+04 0 0 10042 10042 2 1" << endl;
          output << "0.10508723460E+01  0.96530000000E-02  0.26731120000E-03   0" << endl;
          output << "</init>" << endl;
        }

        output << "<event>" << endl;
	if (gamma) output << "4   0  0.2983460E-04  0.9118800E+02  0.7546772E-02  0.1300000E+00" << endl;
	else output << "5   0   1  0.7546772E-02  0.1300000E+00" << endl;
    } else if(strstr(temp_string.c_str(),"VERTEX")){
      float x,y,z,t;
      curstring >> temp >> x >> y >> z >> t; 
    } else if(strstr(temp_string.c_str(),"TRACK")){
      int useless, part_n, pdg_id[2];
      float px[2], py[2], pz[2], E[2], mass[2], charge[2];
    
      for(int i=0;i<2;i++){
        curstring >> temp >> useless >> px[i] >> py[i] >> pz[i] >> part_n >> useless >> useless >> pdg_id[i];
        charge[i] = pdg_id[i]>0?1:-1;
        mass[i] = abs(pdg_id[i])==13?MU:0.000511;
        E[i]=sqrt(MU*MU + px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i]);
        if(i==0) {
          getline(infile,temp_string);
          curstring.clear(); 
          curstring.str(temp_string);
        } 
      }
  
      float pxMom=px[0]+px[1], pyMom=py[0]+py[1], pzMom=pz[0]+pz[1], Emom=E[0]+E[1];
      float massMom=sqrt(pow(Emom,2)-(pow(pxMom,2)+pow(pyMom,2)+pow(pzMom,2)));

      output << "22   -1    0    0    0    0  0.00000000000E+00  0.00000000000E+00 0.00000000000E+02  0.10000000000E+02  0.00000000000E+00 0. 1." << endl;  
      if(gamma) output << "22   -1    0    0    0    0  0.00000000000E+00  0.00000000000E+00 0.00000000000E+02  0.10000000000E+02  0.00000000000E+00 0. -1." << endl; 
      else {
	 output << "990  -1    0    0    0    0  0.00000000000E+00  0.00000000000E+00 0.00000000000E+00  0.10000000000E+02  0.00000000000E+00 0. 0." << endl;
	 output << "443 2 1 0 0 0 " << pxMom << " " << pyMom << " " << pzMom << " " << Emom << " " << massMom << " 2.1585057E-9 1" << endl;
      }
      if (gamma){
	 output << pdg_id[0] << " 1 1 2 0 0 " << px[0] << " " << py[0] << " " << pz[0] << " " << E[0] << " " << mass[0] << " 0. " << charge[0] << endl;
	 output << pdg_id[1] << " 1 1 2 0 0 " << px[1] << " " << py[1] << " " << pz[1] << " " << E[1] << " " << mass[1] << " 0. " << charge[1] << endl;
      } else {
	 output << pdg_id[0] << " 1 2 0 0 0 " << px[0] << " " << py[0] << " " << pz[0] << " " << E[0] << " " << mass[0] << " 0. " << charge[0] << endl;
	 output << pdg_id[1] << " 1 2 0 0 0 " << px[1] << " " << py[1] << " " << pz[1] << " " << E[1] << " " << mass[1] << " 0. " << charge[1] << endl;
      }

      output << "</event>" << endl; // Added by R. P. Kenny III to close out the last event. 
      
      char tmp;
      infile.get(tmp); 
      if((nPerFile-(nn%nPerFile))==1||infile.eof()){
        output << "</LesHouchesEvents>" << endl;
        output.close();
        cout << (nn%nPerFile+1) << " events written in " << outfilename << endl;
      }
      infile.putback(tmp);
      nn++;
    }
  } // reading loop of the input file
  infile.close();
  return;
}
