#include<iostream>

using namespace std;

typedef unsigned short u16;
typedef unsigned int  u32;

struct Message{
    u32 msgID,time;
    u16 userID,trieNext=0;
};

struct TrieNode{
    u16 child[26]={0},message=0;
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

void insertIntoTrie(char* hashTag,int index){
    
    int trieIndex=0,alphaIndex,temp;
    hashTag+=9;
    while(*hashTag){
        ++hashTag;
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
        temp = trie[trieIndex].message;
        trie[trieIndex].message = index;
        message[index].trieNext = temp;
        trieIndex=0;
    
    }
}

int searchFromTrie(char* hashTag){

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
    return trie[trieIndex].message;
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

        while(index!=0){
            
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
            index=message[index].trieNext;
        }
    }
    for(int i=0;i<5 && result[i]!=0;i++){
        retIDs[i]=message[result[i]].msgID;
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

int main(){
    
    init();
    
    createMessage(1111,1,"00:32:45_#parrot_#papa");
    createMessage(2222,2,"23:32:45_#party");
    createMessage(3333,3,"23:32:45_#papa");
    createMessage(4444,4,"23:32:45_#pop");
    createMessage(5555,5,"23:32:45_#p");

    printUtilMess();
    //printUtilTrie();

    followUser(1,2);
    followUser(1,3);
    followUser(1,5);
    followUser(2,3);

    printUser();

    int result[]={0,0,0,0,0};

    //searchByHashtag("papadsfhjk",result);

    getMessages(6,result);

    for(int i=0;i<=4;i++){

        cout<<result[i]<<" "<<endl;

    }

}

