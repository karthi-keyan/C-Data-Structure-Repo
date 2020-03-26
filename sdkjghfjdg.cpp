#include<iostream>

using namespace std;

typedef unsigned short u16;
typedef unsigned int  u32;

struct Message{
    u32 msgID,time;
    u16 userID;
};

struct TrieNode{
    u16 child[26]={0},message[5]={0},counter=0;
};

static u16 userToUser[1000][1000];
static u16 userCounter[1000];

static u16 userMessages[1000][100];
static u16 userMessageCounter[1000];

static Message message[50001];
static u16 messageCounter;

static TrieNode trie[40000];//5000*8<==>maximum hashtag * max length
static u16 trieCounter;

int strToInt(char* str1){

    int value=0;

    while(*str1!='_'){
        if(*str1!=':'){
            value=(value*10)+(*str1-48);
        }
        ++str1;
    }
    return value;

}

bool comparator(int index1,int index2){

    if(message[index1].time>message[index2].time){
        return true;
    }
    else if( message[index1].time==message[index2].time
             && (message[index1].userID<message[index2].userID)){
        return true;
    }
    return false;
}

void organizeTrieQueue(int trieIndex,int index){

    int counter = trie[trieIndex].counter,temp;

    if(index!=0){
            
        if(counter<4){
            ++counter;
            ++trie[trieIndex].counter;
            trie[trieIndex].message[counter]=index;    
        }
        else if(comparator(index,trie[trieIndex].message[4])){
            trie[trieIndex].message[counter]=index;
        }
        else{
            return;
        }
        temp=counter;
        while(temp>0){
            if(comparator(trie[trieIndex].message[temp],trie[trieIndex].message[temp-1])){
                trie[trieIndex].message[temp]   = trie[trieIndex].message[temp]^trie[trieIndex].message[temp-1];
                trie[trieIndex].message[temp-1] = trie[trieIndex].message[temp]^trie[trieIndex].message[temp-1];
                trie[trieIndex].message[temp]   = trie[trieIndex].message[temp]^trie[trieIndex].message[temp-1];
            }
            else
                break;
            temp--;
        }

    }
}

void insertIntoTrie(char* hashTag,int index){
    
    int trieIndex=0,alphaIndex,temp;
    hashTag+=9;
    while(*hashTag){
        ++hashTag;
        //cout<<hashTag<<"\t";
        while(*hashTag!='_' && *hashTag ){
            alphaIndex = *hashTag - 97 ;
            if(trie[trieIndex].child[alphaIndex]==0){

                trie[trieIndex].child[alphaIndex]=trieCounter;
                trieIndex = trieCounter;
                ++trieCounter;
            }
            else{
                trieIndex = trie[trieIndex].child[alphaIndex];
            }

            ++hashTag;
        }
        if(*hashTag=='_')++hashTag;
        //cout<<trieIndex<<"\t"<<index<<endl;
        organizeTrieQueue(trieIndex,index);
        trieIndex=0;
    
    }
}

int searchFromTrie(char* hashTag){
    ++hashTag;
    //cout<<hashTag<<"\t";
    int trieIndex=0,alphaIndex;
    while(*hashTag){

        alphaIndex = *hashTag - 97 ;
        if(trie[trieIndex].child[alphaIndex]==0){
            return 0;
        }
        else{
            trieIndex = trie[trieIndex].child[alphaIndex];
        }
        ++hashTag;
    }
    //cout<<trieIndex<<endl;
    return trieIndex;
}


void init(){

    messageCounter=trieCounter=1;
    for(int i=0;i<1000;i++)
        userCounter[i]= userMessageCounter[i] = 0;
    for(int i=0;i<=50000;i++){
        message[i]={};
    }
    for(int i=0;i<40000;i++){
        trie[i]={};
    }
}

void createMessage(int msgID,int userID,char msgData[]){

    message[messageCounter].msgID  = msgID ; 
    message[messageCounter].userID = userID;
    message[messageCounter].time   = strToInt(msgData);

    insertIntoTrie(msgData,messageCounter);
    
    userMessages[userID][userMessageCounter[userID]]=messageCounter;
    
    ++userMessageCounter[userID];
    ++messageCounter;

}

void followUser(int userID1,int userID2){

    userToUser[userID1][userCounter[userID1]]=userID2;
    ++userCounter[userID1];

}

int searchByHashtag(char tagName[],int retIDs[]){

    int counter=0,temp;
    int result[]={0,0,0,0,0};
    int index = searchFromTrie(tagName);
    if(index==0){
        return 0;
    }
    else{
        for(int i=0;i<5 && trie[index].message[i]!=0;i++){
            retIDs[i]=message[trie[index].message[i]].msgID;
            ++counter;
        }    
    }
    
    return counter;
}

int getMessages(int userID,int retIDs[]){

    int counter=0,temp,index,user_temp=userID;
    int result[]={0,0,0,0,0};
    int following = userCounter[userID];

    while(true){
        if(userMessageCounter[userID]>0){
            
            for(int i=0;i<userMessageCounter[userID];i++){
                
                index=userMessages[userID][i];
                if(counter<4){
                    ++counter;
                    result[counter]=index;    
                }
                else if(comparator(index,result[4])){
                    result[counter]=index;
                }
                else{
                    continue;
                }
                temp=counter;
                while(temp>0){
                    if(comparator(result[temp],result[temp-1])){
                        result[temp]   = result[temp]^result[temp-1];
                        result[temp-1] = result[temp]^result[temp-1];
                        result[temp]   = result[temp]^result[temp-1];
                    }
                    else
                        break;
                    temp--;
                }
            }
        }
    
        if(following<=0){
            break;
        }
        else{
           userID=userToUser[user_temp][following-1];
           --following;
        }
    }

    for(int i=0;i<5 && result[i]!=0;i++){
        retIDs[i]=message[result[i]].msgID;
    }
    return counter;
}

void printUtilTrie(){

    cout<<endl;
    for(int i=0;i<15;i++){

        for(int j=0;j<26;j++){
    
            cout<<trie[i].child[j]<<' ';

        }
        cout<<endl<<trie[i].message<<endl;

    }

}

void printUtilMess(){

    cout<<endl;
    for(int i=0;i<15;i++){

        cout<<message[i].userID<<"  "<<message[i].msgID<<"  "<<message[i].time;
        cout<<endl;

    }

}

void printUser(){

    for(int i=1;i<5;i++){

        for(int j=0;j<userCounter[i];j++){
            
            cout<<userToUser[i][j]<<" ";

        }
        cout<<endl;
        break;
    }
}

void printResult(int result[]){
    for(int i=0;i<=4;i++){
        cout<<result[i]<<" ";
    }
    cout<<endl;
}

int main(){
    
    init();
    int result[]={0,0,0,0,0};

    createMessage(1,111,"01:12:34_#watch_#work");
    createMessage(5,222,"01:12:34_#game");
    createMessage(3,333,"01:12:34_#dinner_#watch");
    createMessage(9,333,"01:12:37_#dog_#eat");
    followUser(222,333);
    searchByHashtag("#event",result);
    printResult(result);
    getMessages(6,result);
    printResult(result);
    createMessage(2,333,"01:12:38_#dog_#eat");
    getMessages(222,result);
    printResult(result);
    followUser(111,333);
    createMessage(7,333,"01:13:00_#watch_#game");
    searchByHashtag("#game",result);
    printResult(result);
    createMessage(8,333,"01:14:00_#watch_#eat");
    followUser(333,111);
    getMessages(333,result);
    printResult(result);
    createMessage(6,333,"01:15:07_#table_#watch");
    getMessages(111,result);
    printResult(result);
    createMessage(4,111,"01:15:10_#dinner_#watch");
    searchByHashtag("#watch",result);//4 6 8 7 1
    printResult(result);
    searchByHashtag("#eat",result);//8 2 9 
    printResult(result);
    

    
    
    
    
    
    
    
    


}

