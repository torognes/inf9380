/// This is written by Nikolay Martyushenko

#include <cmath>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <time.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <algorithm>

using namespace std;

bool BRING_FORTH_THE_IMMORTAL_DOG = false;

template<class T>
using Array = vector<vector<T> >;

struct snp
{
    int pos;
    char alt;
    string chr;
    int file_pos;
};

void runThread(int start,int finish,snp * snp1,Array<snp> snp2,mutex &mut,vector<snp> &results);
int find_snp(vector<snp> &v,snp s);
int find_string(vector<string> &v,string s);
int gets_term (char * temp,char term,char * target);
void load_snps (string filename,Array<snp> &snp_arr,int cores,bool parallel);

int main (int argc, char **argv)
{
    if(BRING_FORTH_THE_IMMORTAL_DOG) {printf("\n\nThe IMMORTAL DOG uses \"return 1\"! It's supereffective!!!\n\n");return 1;}

    int cores = 1;
    bool parallel = true;
    string filenameSubject;
    string filenameControl;
    if (argc>1) {if((string)argv[1]=="u") parallel=false; else cores = atoi(argv[1]);}
    if (argc>3) 
    {
        filenameSubject = (string)argv[2];
        filenameControl = (string)argv[3];
    }
    else
    {
        filenameSubject = "MMR_1370_subject.raw.snps.vcf";
        filenameControl = "MMR_664_control.raw.snps.vcf";
    }
        

    if (parallel) printf("\nThread number used: %i\n",cores);
    else printf("\nUnthreaded!\n");

    Array<snp> snp1;
    Array<snp> snp2;
    load_snps(filenameSubject,snp1,cores,parallel); //MMR_1370_subject.raw.snps.vcf
    load_snps(filenameControl,snp2,cores,parallel);  //MMR_664_control.raw.snps.vcf

    int totNum = 0;
    vector<snp> newSnp;
    for(auto& i:snp1) {totNum+=i.size();newSnp.insert(newSnp.end(),i.begin(),i.end());}
    int totNum2 = 0;
    for(auto& i:snp2) {totNum2+=i.size();}
    printf("Numbers of snps in the loaded files are: %i and %i\n",totNum,totNum2);

    vector<snp> notFounds;

    if(!parallel)
    {
        for(uint i=0;i<snp1.size();i++)
        {
            for(uint j=0;j<snp1[i].size();j++)
            {
                int fo = find_snp(snp2[i],snp1[i][j]);
                if(fo<0 || snp1[i][j].alt!=snp2[i][fo].alt) notFounds.push_back(snp1[i][j]);
            }
        }
        printf("Found this number of unkonwn snps: %i\n",(int)notFounds.size());
    }
    else
    {
        thread threads [cores];
        mutex mut;
        int div = totNum/cores;
        for(int i=0;i<cores;i++)
        {
            int start = i*div;
            int finish = (i+1)*div-1;
            if(i==(cores-1)) finish = newSnp.size()-1;
            threads[i] = thread(runThread,start,finish,&newSnp[0],snp2,ref(mut),ref(notFounds));
        }

        for(auto& th : threads)
        {
            th.join();
        }

        printf("Found this number of unkonwn snps: %i\n",(int)notFounds.size());
    }

    FILE * output;
    output = fopen("differences.vcf","w");

    ifstream templ(filenameSubject.c_str(), ios::binary | ios::ate);
    streamsize Size = templ.tellg();
    templ.seekg(0, ios::beg);
    vector<char> buffer(Size);
    templ.read(buffer.data(), Size);
    templ.close();


    int position = 0;
    int iter = 0;
    int iter_snp = 0;
    while(position<(int)buffer.size())
    {
        iter++;
        char temp [10000];
        if(buffer[position]=='c')
        {
            if(position==notFounds[iter_snp].file_pos)
            {
                position += gets_term(temp,'\n',&buffer[position]);
                fprintf(output,"%s\n",temp);
                iter_snp++;
            }
            else position += gets_term(temp,'\n',&buffer[position]);   
        }
        else
        {
            position += gets_term(temp,'\n',&buffer[position]);
            fprintf(output,"%s\n",temp);
        }
    }

    fclose(output);
    
    return 0;
}

void runThread(int start,int finish,snp * snp1,Array<snp> snp2,mutex &mut,vector<snp> &results)
{
    vector<snp> notFounds;
    for(int i=start;i<=finish;i++)
    {
        int chr_n = 0;
        for(uint j=0;j<snp2.size();j++) if(snp2[j][0].chr==snp1[i].chr) {chr_n=j; break;}
        int fo = find_snp(snp2[chr_n],snp1[i]);
        if(fo<0 || snp1[i].alt!=snp2[chr_n][fo].alt) notFounds.push_back(snp1[i]);
    }
    mut.lock();
    results.insert(results.end(), notFounds.begin(), notFounds.end());
    mut.unlock();
}

int find_snp(vector<snp> &v,snp s)
{
    if(v.size()==0) return -1;
    bool stop = false;
    int b = v.size()-1;
    int e = 0;
    if(s.pos==v[b].pos) return b;
    else if (s.pos==v[e].pos) return e;
    while(!stop)
    {
        int n = 0.5*(b+e);
        if(n==e) stop=true;
        else if(s.pos<v[n].pos) b=n;
        else if(s.pos>v[n].pos) e=n;
        else return n;
    }
    return -1;
}

int find_string(vector<string> &v,string s)
{
    if(v.size()==0) return -1;
    bool stop = false;
    int b = v.size()-1;
    int e = 0;
    if(s==v[b]) return b;
    else if (s==v[e]) return e;
    while(!stop)
    {
        int n = 0.5*(b+e);
        if(n==e) stop=true;
        else if(s>v[n]) b=n;
        else if(s<v[n]) e=n;
        else return n;
    }
    return -1;
}

int gets_term (char * temp,char term,char * target)
{
    int iter = 0;
    char c = -1;
    do
    {
        c = target[iter];
        if (c!=term && c!='\0')
        {
            temp[iter]=c;
            iter++;
        }
    } while(c!=term && c!='\0');
    temp[iter]='\0';
    return (iter+1);
}

void threadFile(int start,int finish,char * buffer,mutex &mut,Array<snp> &snp_arr,int &tnum,int locnum)
{
    vector<string> chroms;
    Array<snp> snp_arr_temp;
    int position = start;
    int iter = 0;
    while(position<=finish)
    {
        iter++;
        char temp [10000];
        if(buffer[position]=='c')
        {
            snp SNP;
            SNP.file_pos = position;
            position += gets_term(temp,'\t',&buffer[position]);
            SNP.chr=(string)temp;
            int chr_pos = find_string(chroms,SNP.chr);
            if(chr_pos<0)
            {
                chroms.push_back(SNP.chr);
                snp_arr_temp.resize(snp_arr_temp.size()+1);
                chr_pos=snp_arr_temp.size()-1;
            }
            position += gets_term(temp,'\t',&buffer[position]);
            SNP.pos = atoi(temp);
            position += gets_term(temp,'\t',&buffer[position]);
            position += gets_term(temp,'\t',&buffer[position]);
            position += gets_term(temp,'\t',&buffer[position]);
            SNP.alt=temp[0];
            position += gets_term(temp,'\n',&buffer[position]);
            snp_arr_temp[chr_pos].push_back(SNP);
        }
        else
        {
            position += gets_term(temp,'\n',&buffer[position]);
        }
    }
    bool terminate = false;
    while(!terminate)
    {
        mut.lock();
        if((tnum+1)==locnum)
        {
            for(uint i=0;i<snp_arr_temp.size();i++)
            {
                int found = -1;
                for(uint j=0;j<snp_arr.size();j++)
                {
                    if(snp_arr_temp[i][0].chr==snp_arr[j][0].chr) {found = j; break;}
                }
                if(found<0)
                {
                    snp_arr.push_back(snp_arr_temp[i]);
                }
                else
                {
                    snp_arr[found].insert(snp_arr[found].end(),snp_arr_temp[i].begin(),snp_arr_temp[i].end());
                }
            }
            tnum++;
            mut.unlock();
            terminate = true;
        }
        else mut.unlock();
    }
}

void load_snps (string filename,Array<snp> &snp_arr,int cores,bool parallel)
{
    vector<string> chroms;

    ifstream file(filename, ios::binary | ios::ate);
    streamsize Size = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(Size);
    file.read(buffer.data(), Size);

    file.close();
    if(parallel)
    {
        thread threads [cores];

        mutex mut;
        int div = Size/cores;
        vector<int> st_fin (cores+1,0);
        st_fin[0]=0;
        st_fin[cores]=Size-1;
        char te [10000];
        for(int i=1;i<cores;i++) st_fin[i]=i*div+gets_term(te,'\n',&buffer[i*div]);
        int tnum = 0;
        for(int i=0;i<cores;i++)
        {
            int start = st_fin[i];
            int finish = st_fin[i+1];
            if(i!=0) start++;
            threads[i] = thread(threadFile,start,finish,&buffer[0],ref(mut),ref(snp_arr),ref(tnum),i+1);
        }

        for(auto& th : threads)
        {
            th.join();
        }
    }

    else
    {
        int position = 0;
        int iter = 0;
        while(position<(int)buffer.size())
        {
            iter++;
            char temp [10000];
            if(buffer[position]=='c')
            {
                snp SNP;
                SNP.file_pos = position;
                position += gets_term(temp,'\t',&buffer[position]);
                SNP.chr=(string)temp;
                int chr_pos = find_string(chroms,SNP.chr);
                if(chr_pos<0)
                {
                    chroms.push_back(SNP.chr);
                    snp_arr.resize(snp_arr.size()+1);
                    chr_pos=snp_arr.size()-1;
                }
                position += gets_term(temp,'\t',&buffer[position]);
                SNP.pos = atoi(temp);
                position += gets_term(temp,'\t',&buffer[position]);
                position += gets_term(temp,'\t',&buffer[position]);
                position += gets_term(temp,'\t',&buffer[position]);
                SNP.alt=temp[0];
                position += gets_term(temp,'\n',&buffer[position]);
                snp_arr[chr_pos].push_back(SNP);
            }
            else
            {
                position += gets_term(temp,'\n',&buffer[position]);
            }
        }
    }
}
