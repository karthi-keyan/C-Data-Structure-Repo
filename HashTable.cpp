#include<iostream>
#include<bits/stdc++.h>

using namespace std;

enum FIELD{NAME,ID,BIRTH_DATE,EMAIL,MEMO};

struct record{

 char  *data[5];
 int next[5];

};

struct Result
{
    int count;
    record data;
};

static int record_counter=1;
static record Record[50000];
static int hash_table[5][200000];


int getHash(char *data){

     int hash=5381;
     while(*data!='\0'){
         hash=((hash<<5)+hash)+*data;
         data++;
     }
     return hash%200000;
}


void Add(char *name,char *id,char *birthdate,char *email,char *memo){
    int hash;
    
    record rcd          ={};
    rcd.data[ID]        =id;
    rcd.data[NAME]      =name;
    rcd.data[BIRTH_DATE]=birthdate;
    rcd.data[EMAIL]     =email;
    rcd.data[MEMO]      =memo;

    Record[record_counter]=rcd;
    
    hash = getHash(name);
    Record[record_counter].next[0]=hash_table[0][hash];
    hash_table[0][hash]=record_counter;
    
    hash = getHash(id);
    Record[record_counter].next[1]=hash_table[1][hash];
    hash_table[1][hash]=record_counter;
    
    hash = getHash(birthdate);
    Record[record_counter].next[2]=hash_table[2][hash];
    hash_table[2][hash]=record_counter;
    
    hash = getHash(email);
    Record[record_counter].next[3]=hash_table[3][hash];
    hash_table[3][hash]=record_counter;
    
    hash = getHash(memo);
    Record[record_counter].next[4]=hash_table[4][hash];
    hash_table[4][hash]=record_counter;
    
    record_counter++;
    
}

Result Search(FIELD field,char *data){

    int index = hash_table[field][getHash(data)];
    int count=0;
    Result res;
    record rec={};
    while(index!=0){
        index=Record[index].next[field];
        rec = Record[index];
        count++;
    }
    res.count=count;
    if(count==1){
        res.data=rec;
    }
    return res;
}

void Change(FIELD field,char *str,FIELD rfield,char *rstr){

    int index = hash_table[field][getHash(str)];
    int rindex= getHash(rstr);
    char *prev_data;
    int p
    while(index!=0){

        prev_data=Record[index].data[rfield];
        Record[index].data[rfield]=rstr;
        



        Record[index].next[]=hash_table[4][hash];
        hash_table[4][hash]=record_counter;

        index=Record[index].next[field];
    }

}

void Delete(FIELD field,char *data){

                 

}

void InitDB(){
    
    for(int i=0;i<200000;i++){
       hash_table[0][i]=hash_table[1][i]=hash_table[2][i]=hash_table[3][i]=0;
    }
    cout<<"InitDB ends..."<<endl;

}

void print_util(){

    int index = hash_table[1][getHash("1234")];
     
    while(index!=0){
        
        cout<<Record[index].data[NAME]<<endl;
        index=Record[index].next[0];
    }


}

int main(){
 
    time_t start,end;
  
    time(&start); 
  
    ios_base::sync_with_stdio(false); 
  
    InitDB();

    Add("karthi","1234","220499","a@gmail.com","Good");
    Add("karthi","2345","220499","b@gmail.com","Good");
    Add("karthi","3456","220499","c@gmail.com","Good");
    
    cout<<Search(ID,"1234").count<<endl;
    cout<<Search(NAME,"karthi").count<<endl;
    cout<<Search(MEMO,"Good").count<<endl;
    cout<<Search(BIRTH_DATE,"0000").count<<endl;

    time(&end); 
  
    double time_taken = double(end - start); 
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(10); 
    cout << " sec " << endl; 
    
    return 1;
}

