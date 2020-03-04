#include<iostream>

using namespace std;

typedef unsigned int u32;
typedef unsigned short u16;

//File System basic structure
struct fNode{

    u16 parent=0,child=0,sibling=0,hnext=0;
    u32 orgSize,infSize,id,tFiles;
    bool isDir=true,isInf=false,isAlive=true;

};


static int record_counter=1;
static fNode fNodes[10000];
static int HashTable[40000];
static int infcount=0;

//Hash Function (Modified DJB)
int getHash(int index){

    u32 hash = 5381,temp;
    while(index!=0){

        temp=index%10;
        hash=((hash<<5)+hash)+temp;
        index/=10;

    }
    return hash%40000;
}

//Function to initialize all the variables
void init(){

    record_counter=1;
    fNodes[0].id=10000;
    fNodes[0].parent=fNodes[0].child=fNodes[0].sibling=0;
    for(int i=0;i<40000;i++){
        HashTable[i]=0;
    }
    cout<<"Init DB..\n";
}

//Function to add the file or directory
int add(int id,int pid,int size){

    int hash,temp,ptemp;
    hash            = getHash(id);
    ptemp           = HashTable[getHash(pid)];
    HashTable[hash] =record_counter;

    fNodes[record_counter].id=id;
    fNodes[record_counter].parent=ptemp;
    fNodes[record_counter].orgSize=size;
    fNodes[record_counter].infSize=size;
    

    if(size!=0){
        fNodes[record_counter].isDir=false;
    }
    
    //update parent
    temp=fNodes[ptemp].child;
    fNodes[ptemp].child=record_counter;
    fNodes[record_counter].sibling=temp;
    
    temp=ptemp;
    //update size of parent directories
    if(size>0){
        while(true){
            
            fNodes[temp].orgSize+=size;
            fNodes[temp].infSize+=size;
            ++fNodes[temp].tFiles;
            
            if(temp==0)
                break;

            temp=fNodes[temp].parent;
            
        }
    }
    ++record_counter;

    return fNodes[ptemp].infSize;

}

//function to delete the file or directory
int remove(int id){

    int temp;
    int index = HashTable[getHash(id)];
    int pindex= fNodes[index].parent;
    int isize  = fNodes[index].infSize;
    int osize  = fNodes[index].orgSize;
    int tCount = fNodes[index].isDir?fNodes[index].tFiles:1;

    if(fNodes[pindex].child==index){
        fNodes[pindex].child=fNodes[index].sibling;
    }
    else{
        int temp = fNodes[pindex].child;
        while(true){

            if(fNodes[temp].sibling==index){
                fNodes[temp].sibling=fNodes[index].sibling;
                break;
            }
            temp=fNodes[temp].sibling;

        }
    }
    
    temp=pindex;
    //update parent directory size
    while(true){
        fNodes[temp].orgSize-=osize;
        fNodes[temp].infSize-=isize;
        fNodes[temp].tFiles-=tCount;
        if(temp==0){
            break;
        }        
        temp=fNodes[temp].parent;
    }

    return fNodes[pindex].infSize;

}

//function to move the file or directory
int move(int id,int pid){

    remove(id);
    int temp;
    int index = HashTable[getHash(id)];
    int ptemp = HashTable[getHash(pid)];
    int size  = fNodes[index].orgSize;
    int isize  = fNodes[index].infSize;
    int tCount = fNodes[index].isDir?fNodes[index].tFiles:1;
    

    temp=fNodes[ptemp].child;
    fNodes[ptemp].child=index;
    fNodes[index].sibling=temp;
    fNodes[index].parent=ptemp;

    temp=ptemp;
    //update size of parent directories
    while(true){
         
        fNodes[temp].orgSize+=size;
        fNodes[temp].infSize+=isize;
        fNodes[temp].tFiles+=tCount;
        if(temp==0)
            break;
        temp=fNodes[temp].parent;
        
    }

    return fNodes[temp].infSize;


}

void infUtil(int index,int infSize){

    index=fNodes[index].child;
    while(index!=0){
        cout<<"Index: "<<index<<endl;
        if(!fNodes[index].isDir){
            fNodes[index].isInf=true;
            fNodes[index].infSize+=infSize;
            ++infcount;
        }
        else{
            
            int lcount=infcount;
            infUtil(index,infSize);
            fNodes[index].isInf=true;
            fNodes[index].infSize+=((infcount-lcount)*infSize);
            
        }
        index=fNodes[index].sibling;
    }

}

//function to infect the file or directory
int infect(int id){

    infcount=0;
    int index = HashTable[getHash(id)];
    int ptemp = fNodes[index].parent;

    int infSize = fNodes[0].infSize/fNodes[0].tFiles;
    cout<<"Index: "<<index<<"INF :"<<infSize<<endl;
    if(!fNodes[index].isDir){
        fNodes[index].isInf=true;
        fNodes[index].infSize+=infSize;
    }
    else{
        infUtil(index,infSize);
        fNodes[index].isInf=true;
        fNodes[index].infSize+=(infcount*infSize);
        infSize=infcount*infSize;
        cout<<"Index: "<<index<<"INF :"<<infSize<<endl;
    }

    //update parent
    while(index!=0){
            fNodes[ptemp].infSize+=infSize;
            if(ptemp==0)
                break;
            ptemp=fNodes[ptemp].parent;
    }    

    return fNodes[index].infSize;

}

void rcvUtil(int index){

    index=fNodes[index].child;
    while(index!=0){

        if(!fNodes[index].isDir && fNodes[index].isInf){
            fNodes[index].isInf=false;
            fNodes[index].infSize=fNodes[index].orgSize;
        }
        else{
            
            rcvUtil(index);
            fNodes[index].isInf=false;
            fNodes[index].infSize=fNodes[index].orgSize;

        }
        index=fNodes[index].sibling;
    }

}

//function to recover the function and directories
int recover(int id){

    int index = HashTable[getHash(id)];
    int ptemp = fNodes[index].parent;

    int infDec = fNodes[index].infSize-fNodes[index].orgSize;

    if(!fNodes[index].isDir && fNodes[index].isInf){
        fNodes[index].isInf=false;
        fNodes[index].infSize=fNodes[index].orgSize;
    }
    else{
        
        rcvUtil(index);
        fNodes[index].isInf=false;
        fNodes[index].infSize=fNodes[index].orgSize;

    }

    //update parent
    while(index!=0){
            fNodes[ptemp].infSize-=infDec;
            if(ptemp==0)
                break;
            ptemp=fNodes[ptemp].parent;
    }    

    return fNodes[index].infSize;
}

void printUtil(int id){

    int hash= HashTable[getHash(id)];
    cout<<"ID               = "<<fNodes[hash].id<<endl;
    cout<<"Child            = "<<fNodes[hash].child<<endl;
    cout<<"Parent           = "<<fNodes[hash].parent<<endl;
    cout<<"Sibling          = "<<fNodes[hash].sibling<<endl;
    cout<<"Orginal Size     = "<<fNodes[hash].orgSize<<endl;
    cout<<"Infected Size    = "<<fNodes[hash].infSize<<endl;
    cout<<"Total Files      = "<<fNodes[hash].tFiles<<endl;
    cout<<"\n\n";

}

int main(){

    init();
    cout<<add(11,10000,10)<<endl;
    cout<<add(22,10000,0)<<endl;
    cout<<add(33,10000,70)<<endl;
    cout<<add(44,22,20)<<endl;
    cout<<add(55,22,40)<<endl;
    cout<<add(66,22,50)<<endl;

    printUtil(10000);
    printUtil(22);
    printUtil(55);
    
    cout<<remove(55)<<endl;

    printUtil(44);
    printUtil(22);
    printUtil(10000);
    
    cout<<infect(10000)<<endl;

    printUtil(44);
    printUtil(22);
    printUtil(10000);
    
    cout<<infect(22)<<endl;

    printUtil(44);
    printUtil(22);
    printUtil(10000);
    

    cout<<recover(22)<<endl;

    printUtil(44);
    printUtil(22);
    printUtil(10000);
    

    return 0;

}




