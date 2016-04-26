#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;
typedef map<string,string> mss;

void vcfmap(mss&vcf, string st){
  stringstream ss(st);
  string chr, pos, id, tt;
  char   a, b;

  ss>>chr>>pos>>id>>a>>b;

  tt = chr+'_'+pos+id;
  if(a<b){ tt+=a; tt+=b;}
  else   { tt+=b; tt+=a;};
  vcf[tt] = st;
}


int main(int argc, char *argv[])
{
  mss vcfa, vcfb;
  ifstream fnew(argv[1]), fold(argv[2]);
  string line;
  //read snps
  while(getline(fnew,line)) vcfmap(vcfa,line);
  while(getline(fold,line)) vcfmap(vcfb,line);
  for(auto&p:vcfa) if(vcfb.find(p.first)==vcfb.end()) cout<<p.second<<'\n';
  return 0;
}
