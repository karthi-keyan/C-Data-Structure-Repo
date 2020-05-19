class Twitter {
public:
    
struct tweet{
        int id,time;
        tweet* next=NULL;
        tweet(int ident,int tm){
            id=ident;
            time=tm;
        }
    };

    struct frnd{
        int id;
        frnd* next = NULL;
        frnd(int ident){
            id=ident;
        }
    };

    frnd *friendList[750]={NULL};
    tweet *personToTweet[750]={NULL};
    int global_timer=0;
    int heap_counter=0;

    Twitter() {
        for(int i=0;i<750;++i){
            friendList[i]=NULL;
            personToTweet[i]=NULL;
        }
    }
    
    void postTweet(int userId, int tweetId) {
        tweet *twt = new tweet(tweetId,++global_timer);
        twt->next=personToTweet[userId];
        personToTweet[userId]=twt;
    }

    bool comparator(tweet *t1,tweet *t2){
        if(t2->time>t1->time)
            return true;
        return false;
    }

    void heapInsert(vector<tweet*>& arr,int id){
        int count=0;
        tweet *twt=personToTweet[id];
        while(twt!=NULL && count!=10){
            arr.push_back(twt);
            ++heap_counter;
            int child=heap_counter;
            int parnt=child/2;
            while(child!=1){
                if(comparator(arr[parnt-1],arr[child-1]))
                {
                    tweet* tmp=arr[parnt-1];
                    arr[parnt-1]=arr[child-1];
                    arr[child-1]=tmp;
                }
                else{
                    break;
                }
                child=parnt;
                parnt=parnt/2;
            }
            twt=twt->next;
            ++count;
        }
    }
    
    void heapify(vector<tweet*>& arr, int n, int i) 
    { 
        int largest = i;  
        int l = 2*i + 1;  
        int r = 2*i + 2;  
        if (l < n && arr[l]->time > arr[largest]->time) 
            largest = l; 
        if (r < n && arr[r]->time > arr[largest]->time) 
            largest = r; 
        if (largest != i) 
        { 
            tweet* tmp=arr[i];
            arr[i]=arr[largest];
            arr[largest]=tmp;
            heapify(arr, n, largest); 
        } 
    } 

    vector<int> getNewsFeed(int userId) {
        vector<tweet*> res_;
        vector<int> result;
        frnd *f=friendList[userId];
        heap_counter=0;
        
        heapInsert(res_,userId);
        
        while(f!=NULL){
            if(f->id!=userId)
                heapInsert(res_,f->id);
            f=f->next; 
        }
        int cnt=10;
        for (int i=res_.size()-1; i>0 && cnt!=0; --i,--cnt) 
        { 
             tweet* tmp=res_[0];
             res_[0]=res_[i];
             res_[i]=tmp;
             heapify(res_, i, 0); 
        } 
        int count=10;
        for(int i=res_.size()-1;i>=0 && count!=0;--i,--count){
            result.push_back(res_[i]->id);
        }      
        return result;
    }
    
    void follow(int followerId, int followeeId) {
        frnd *f = friendList[followerId];
        while(f!=NULL){
            if(f->id==followeeId)
                return;
            f=f->next;
        }
        f=new frnd(followeeId);
        f->next=friendList[followerId];
        friendList[followerId]=f;
    }
    
    void unfollow(int followerId, int followeeId) {
        frnd *f=friendList[followerId];
        frnd *prev=friendList[followerId];
        if(followerId==followeeId || f==NULL)
            return;
        if(f->id==followeeId)
            friendList[followerId]=f->next;
        else{
            f=f->next;
            while(f!=NULL){
                if(f->id==followeeId){
                    prev->next=f->next;
                    return;
                }
                else{
                    prev=f;
                    f=f->next;
                }
            }
        }
    }
};

