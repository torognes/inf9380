/**
 * parallel version to print new SNP against a dictionary.
 * an easy parallel way is to split the new vcf file into blocks. 
 *  then look up each block in the dictionary
 * this parallel version is to distribute a block into multiple threads. 
 *                                                  by Xijiang Yu
 *                                                  Apr. 26, 2016
 */
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <cstdlib>

using namespace std;

typedef map<string,string> MSS;
typedef vector<string>     VCF;

string  chrstr(const string&str){
  stringstream ss(str);
  string chr, pos, id, tt;
  char   a, b;

  ss>>chr>>pos>>id>>a>>b;

  tt = chr+'_'+pos+id;
  if(a<b){ tt+=a; tt+=b;}
  else   { tt+=b; tt+=a;};
  return tt;
}

void split_jobs(size_t*beg, size_t*end, size_t nth, size_t len){
  size_t step;
  step = len/nth;
  if(len%nth) ++step;
  beg[0] = 0;
  end[0] = step;
  for(int i=1; i<nth; ++i){
    beg[i]=i*step;
    end[i]=beg[i]+step;
  }
  end[nth-1] = len;
}

void look_up(VCF&vcf, MSS&dict, size_t beg, size_t end, VCF&hit){
  string cstr;
  for(auto i=beg; i<end; ++i){
    cstr = chrstr(vcf[i]);
    if(dict.find(cstr)==dict.end()) hit.push_back(vcf[i]);
  }
}

int main(int argc, char *argv[])
{
  MSS dict;
  VCF vnew;
  ifstream fnew(argv[1]), fdct(argv[2]);
  string line;
  size_t nth{1};

  if(argc ==2) nth=atoi(argv[1]);
  if(nth<1 || nth>thread::hardware_concurrency()) nth = 1;
  
  //read snps
  while(getline(fnew,line)) vnew.push_back(line);
  while(getline(fdct,line)) dict[chrstr(line)] = line;
  
  size_t beg[nth], end[nth];
  VCF    hits[nth];
  thread th[nth];
  split_jobs(beg, end, nth, vnew.size());

  for(auto i=0; i<nth; ++i)
    th[i] = thread(look_up, ref(vnew), ref(dict), beg[i], end[i], ref(hits[i]));
  for(auto i=0; i<nth; ++i) th[i].join();

  for(auto i=0; i<nth; ++i)
    for(auto&p:hits[i]) cout<<p<<'\n';
  
  return 0;
}
