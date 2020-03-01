#include<iostream>

using namespace std;

typedef unsigned int u32;
typedef unsigned short u16;

//File System basic structure
struct fNode{

    u16 parent=0,child=0,sibling=0,hnext=0;
    u32 orgSize,infSize,id;
    bool isDir=false,isInf=false,isAlive=true;

};


static int record_counter=1;
static fNode fNodes[10000];
static int HashTable[40000];
static int total_size=0,total_files=0;

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
    HashTable[getHash(10000)]=0;
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

    if(size==0){
        fNodes[record_counter].isDir=true;
    }
    
    //update parent
    temp=fNodes[ptemp].child;
    fNodes[ptemp].child=record_counter;
    fNodes[record_counter].sibling=temp;
    
    temp=ptemp;
    //update size of parent directories
    while(true){
        
        if(temp==0){
            fNodes[temp].orgSize+=size;
            break;
        }
        else if(fNodes[temp].isDir)
            fNodes[temp].orgSize+=size;
        
        temp=fNodes[temp].parent;
        
    }
    
    ++total_files;
    total_size+=size;
    ++record_counter;

    return fNodes[ptemp].orgSize;

}

//function to delete the file or directory
int remove(int id){
    int temp;
    int index = HashTable[getHash(id)];
    int pindex= fNodes[index].parent;
    int size  = fNodes[index].orgSize;
    if(!fNodes[index].isDir)
        --total_files;
    total_size-=size;
    
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
        
        if(temp==0){
            fNodes[temp].orgSize-=size;
            break;
        }
        else if(fNodes[temp].isDir)
            fNodes[temp].orgSize-=size;
        
        temp=fNodes[temp].parent;
        
    }

    return fNodes[pindex].orgSize;

}

int move(int id,int pid){

    remove(id);
    int temp;
    int index = HashTable[getHash[id]];
    int ptemp = HashTable[getHash(pid)];
    int size  = fNodes[index].orgSize;

    temp=fNodes[ptemp].child;
    fNodes[ptemp].child=index;
    fNodes[index].sibling=temp;

    temp=ptemp;
    //update size of parent directories
    while(true){
        
        if(temp==0){
            fNodes[temp].orgSize+=size;
            break;
        }
        else if(fNodes[temp].isDir)
            fNodes[temp].orgSize+=size;
        
        temp=fNodes[temp].parent;
        
    }
    

    

}

void printUtil(int id){

    int hash= HashTable[getHash(id)];
    cout<<"ID       = "<<fNodes[hash].id<<endl;
    cout<<"Child    = "<<fNodes[hash].child<<endl;
    cout<<"Parent   = "<<fNodes[hash].parent<<endl;
    cout<<"Sibling  = "<<fNodes[hash].sibling<<endl;
    cout<<"Size     = "<<fNodes[hash].orgSize<<endl;
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
    
    return 0;

}




