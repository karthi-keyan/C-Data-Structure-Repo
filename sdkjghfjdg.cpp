#include<iostream>

using namespace std;

typedef unsigned int u32;
typedef unsigned short u16;

struct package{

    u32 pid;
    u16 area,price,next=0;
    bool isAlive=true;

};

static u16 HashTable[150000];

static u16 userToFriendList[1001][21];
static u16 friendCounter[1001];

static u16 userArea[1001][11];
static u16 userFriendArea[1001][12];

static u16 packageHeap[11][40001];
static u16 heapCounter[11];

static package packageList[40001];
static u16 packageCounter=1;

void init(u16 N,u16 M){
    
    packageCounter=1;

    for(int i=0;i<150000;i++){

        HashTable[i]=0;
        if(i<=40001)
            packageList[i]={};

        if(i<=1000){
            for(int j=0;j<=20;j++)
                userToFriendList[i][j]=0;
            friendCounter[i]=1;
        }
        if(i<=1000){
            for(int j=0;j<11;j++){
                userFriendArea[i][j]=0;
                userArea[i][j]=0;
            }
            userFriendArea[i][11]=0;
        }
    }
    
    for(int j=1;j<=10;j++){
        for(int i=1;i<=40000;i++){
                packageHeap[j][i]=0;
        }
        heapCounter[j]=1;
    }
}
int getHash(int pid){

    u32 hash = 5381,temp;
    while(pid!=0){

        temp=pid%10;
        hash=((hash<<5)+hash)+temp;
        pid/=10;

    }
    return hash%150000;
}

int hashTable(int pid){

    int index = HashTable[getHash(pid)];
    while(index!=0){

        if(packageList[index].pid==pid)
            return index;
        
        index=packageList[index].next;

    }
    return 0;
}

int parent(int index){
    return index/2;
}

int left(int index){
    return index*2;
}

int right(int index){
    return index*2+1;
}

bool comparator(int index1,int index2){

    if(packageList[index1].price<packageList[index2].price ||
           (packageList[index1].price==packageList[index2].price && 
            packageList[index1].pid<packageList[index2].pid))
            return true;
    return false;

}


void addToHeap(int index_,int area){
    
    int prnt;
    u16 index = heapCounter[area];
    packageHeap[area][index] = index_;
    prnt = parent(index);
    while(parent>0){

        if(comparator(packageHeap[area][index],packageHeap[area][prnt])){
            packageHeap[area][prnt] = packageHeap[area][prnt] ^ packageHeap[area][index];  
            packageHeap[area][index] = packageHeap[area][prnt] ^ packageHeap[area][index];
            packageHeap[area][prnt] = packageHeap[area][prnt] ^ packageHeap[area][index]; 
  
        }
        else{
            break;
        }
        
        index = prnt;
        prnt  = parent(index);

    }

    ++heapCounter[area];

}

void deleteFromHeap(int area){

    int index,l,r,p,small=1;
    --heapCounter[area];
    index=heapCounter[area];
    packageHeap[area][1]=packageHeap[area][index];
    packageHeap[area][index]=0;
    while(true){
        
        p = small;
        r = right(p);
        l = left(p);

        if(l<index && comparator(packageHeap[area][l],packageHeap[area][p]))
            small=l;
        if(r<index && comparator(packageHeap[area][r],packageHeap[area][small]))
            small=r;
        if(small!=p){

            packageHeap[area][p]     = packageHeap[area][p] ^ packageHeap[area][small];  
            packageHeap[area][small] = packageHeap[area][p] ^ packageHeap[area][small];
            packageHeap[area][p]     = packageHeap[area][p] ^ packageHeap[area][small]; 

        }
        else{
            break;
        }
    }

}

void befriend(int uid1,int uid2){

    userToFriendList[uid1][friendCounter[uid1]]=uid2;
    ++friendCounter[uid1];
    userToFriendList[uid2][friendCounter[uid2]]=uid1;
    ++friendCounter[uid2];

    for(int i=1;i<12;i++){

        userFriendArea[uid1][i]+=userArea[uid2][i];
        if(userFriendArea[uid1][i]>userFriendArea[uid1][11])
            userFriendArea[uid1][11]=userFriendArea[uid1][i];
        userFriendArea[uid2][i]+=userArea[uid1][i];
        if(userFriendArea[uid2][i]>userFriendArea[uid2][11])
            userFriendArea[uid2][11]=userFriendArea[uid2][i];

    }
}

void add(int pid,int area,int price){

    int temp,hash;
    packageList[packageCounter].pid  =pid;
    packageList[packageCounter].area =area;
    packageList[packageCounter].price=price;

    hash = getHash(pid);
    temp = HashTable[hash];
    HashTable[hash]=packageCounter;
    packageList[packageCounter].next = temp;

    addToHeap(packageCounter,area);

    ++packageCounter;

}

void reserve(int uid,int pid){

    int index = hashTable(pid);
    int area  = packageList[index].area;
    packageList[index].isAlive=false;
    int frnd;

    ++userArea[uid][area];

    for(int i=1;i<friendCounter[uid];i++){
       
        frnd = userToFriendList[uid][i];
        ++userFriendArea[frnd][area];
        if(userFriendArea[frnd][area]>userFriendArea[frnd][11])
            userFriendArea[frnd][11]=userFriendArea[frnd][area];
        
    }
    ++userFriendArea[uid][area];
    if(userFriendArea[uid][area]>userFriendArea[uid][11])
        userFriendArea[uid][11]=userFriendArea[uid][area];

}

int recommend(int uid){

    int maxArea = userFriendArea[uid][11];
    bool visited =false;
    u16 min_price=10000;
    u32 min_id=2000000000;
    
    while(true){

        for(int i=1;i<11;i++){

            if(userFriendArea[uid][i]==maxArea){

                while( packageHeap[i][1]!=0){
                    
                    int index=packageHeap[i][1];
                    
                    if(!packageList[index].isAlive)
                        deleteFromHeap(i);

                    else if( packageList[index].price<min_price ||
                    (packageList[index].price==min_price&&
                        packageList[index].pid<min_price)){

                            min_price=packageList[index].price;
                            min_id   =packageList[index].pid;
                            visited=true;
                            break;
                        }
                    else
                        break;
                }

            }

        }
        if(visited)
            break;
        else{
            --maxArea;
        }

    }

    return min_id;

}

void printUtil(int area){

    for(int i=0;i<11;i++){

        cout<<packageList[packageHeap[area][i]].pid<<"\t";

    }
    cout<<endl;

}

void printFriend(int frnd){

    for(int i=0;i<=11;i++){

        cout<<userFriendArea[frnd][i]<<"\t";

    }
    cout<<endl;
    
}


int main(){
    init(5,4);
    befriend(1,2);
    befriend(1,3);
    befriend(1,4);
    befriend(5,3);
    befriend(2,3);

    add(11,1,55);
    add(12,1,34);
    add(13,1,544);
    add(15,1,54);
    add(14,1,54);
    add(16,2,45);
    add(2,3,564);
    add(67,2,5);
    add(9,2,67);

    printUtil(1);
    printUtil(2);
    printUtil(3);


    cout<<recommend(1)<<endl;
    cout<<recommend(2)<<endl;
    cout<<recommend(3)<<endl;
    cout<<recommend(4)<<endl;
    cout<<recommend(5)<<endl;

    reserve(1,12);
    reserve(1,14);
    reserve(1,13);
    reserve(1,11);
    reserve(1,15);
    reserve(1,67);
    reserve(1,16);
    reserve(1,9);

    printUtil(1);
    printUtil(2);
    printUtil(3);

    cout<<"\n\n";

    printFriend(2);
    printFriend(3);
    printFriend(4);
    printFriend(5);

    cout<<recommend(2)<<endl;
    cout<<recommend(3)<<endl;
    cout<<recommend(4)<<endl;
    
    printUtil(1);
    printUtil(2);
    printUtil(3);

    cout<<"\n\n";
    befriend(1,5);
    printFriend(5);



}
