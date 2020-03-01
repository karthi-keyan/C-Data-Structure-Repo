#include<iostream>
#include<bits/stdc++.h>

using namespace std;

enum FIELD{NAME,ID,BIRTH_DATE,EMAIL,MEMO};

struct record{

 char data[5][19];
 bool isAlive;
 int next[5];

};

struct Result
{
    int count;
    char data[19];
};

static int record_counter=1;
static record Record[50000];
static int hash_table[5][200000];


int getHash(char *data){
     
     unsigned int hash=5381;
     while(*data!='\0'){
         hash=((hash<<5)+hash)+*data;
         data++;
     }
     return hash%200000;
}


void Add(char *name,char *id,char *birthdate,char *email,char *memo){
    int hash;
    
    strcpy(Record[record_counter].data[ID],id);
    strcpy(Record[record_counter].data[NAME],name);
    strcpy(Record[record_counter].data[BIRTH_DATE],birthdate);
    strcpy(Record[record_counter].data[EMAIL],email);
    strcpy(Record[record_counter].data[MEMO],memo);
    Record[record_counter].isAlive=true;
    
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

Result Search(FIELD field,char *data,FIELD rfield){

    int index = hash_table[field][getHash(data)];
    int count=0;
    Result res;
    while(index!=0){
        if(strcmp(Record[index].data[field],data)==0 && Record[index].isAlive){
            if(count==0)
                strcpy(res.data,Record[index].data[rfield]);
            count++;
        }
        index=Record[index].next[field];
    }
    res.count=count;
    return res;
}

void Change(FIELD field,char *data,FIELD rfield,char *rdata){

    int index = hash_table[field][getHash(data)];
    int rIndex,rhash,rpIndex,t_index;
    char prev_field_val[19];
    while(index!=0){
        t_index=Record[index].next[field];
        if(strcmp(Record[index].data[field],data)==0 && Record[index].isAlive){

            strcpy(prev_field_val, Record[index].data[rfield]);
            strcpy(Record[index].data[rfield],rdata);

            rpIndex=hash_table[rfield][getHash(prev_field_val)];
            if(strcmp(Record[rpIndex].data[field],data)==0){
                
                hash_table[rfield][getHash(prev_field_val)]=Record[rpIndex].next[rfield];
                goto INIT_SKIP;

            }
            while(Record[rpIndex].next[rfield]!=0){

               if(strcmp(Record[Record[rpIndex].next[rfield]].data[field],data)==0){
                
                Record[rpIndex].next[rfield]=Record[Record[rpIndex].next[rfield]].next[rfield];
                break;
               
               }
               rpIndex=Record[rpIndex].next[rfield];

            }
            INIT_SKIP:
            rhash=getHash(rdata);
            rIndex = hash_table[rfield][rhash];
            Record[index].next[rfield]=rIndex;
            hash_table[rfield][rhash]=index;

        }
        index=t_index;
       
    }

}

int Delete(FIELD field,char *data){

    int index = hash_table[field][getHash(data)];
    int count=0;
    while(index!=0){
        if(strcmp(Record[index].data[field],data)==0 && Record[index].isAlive){
            Record[index].isAlive=false;
            count++;
        }
        index=Record[index].next[field];
    }

    return count;
}

void InitDB(){
    
    for(int i=0;i<200000;i++){
       hash_table[0][i]=hash_table[1][i]=hash_table[2][i]=hash_table[3][i]=hash_table[4][i]=0;
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
    Add("naveen","9600","230295","b@gmail.com","Okay");
    Add("vijay","3456","220196","c@gmail.com","Bad");
    Add("rishi","3454","220499","a@gmail.com","Good");
    Add("mukesh","9879","100911","y@gmail.com","Bad");
    Add("karthi","1343","050778","f@gmail.com","Goo");
    
    cout<<"Add Ends....."<<endl;

    cout<<Search(ID,"3454",NAME).count<<endl;
    cout<<Search(NAME,"karthi",MEMO).count<<endl;
    cout<<Search(MEMO,"Good",EMAIL).data<<endl;
    cout<<Search(EMAIL,"a@gmail.com",MEMO).count<<endl;
    cout<<Search(NAME,"vijay",MEMO).count<<endl;

    cout<<"Search Ends....."<<endl;

    cout<<Search(NAME,"karthi",MEMO).count<<endl;
    cout<<Search(BIRTH_DATE,"220499",MEMO).count<<endl;
    time(&end); 

    Change(NAME,"vijay",EMAIL,"a@gmail.com");
    Change(EMAIL,"f@gmail.com",EMAIL,"fu@gmail.com");
    

    cout<<Search(EMAIL,"a@gmail.com",NAME).count<<endl;
    cout<<Search(EMAIL,"f@gmail.com",NAME).count<<endl;
    cout<<Search(EMAIL,"fu@gmail.com",NAME).count<<endl;
    cout<<Delete(NAME,"karthi")<<endl;
    
  
    double time_taken = double(end - start); 
    cout << "Time taken by program is : " << fixed 
         << time_taken << setprecision(10); 
    cout << " sec " << endl; 
    return 1;
}

