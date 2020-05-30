#include<iostream>

class LFUCache {
public:
    struct record{
        int key,frequency,timeStamp;
        record(){}
        record(int data){
            key=data;
            frequency=1;
        }
    };
    int heapCapacity=0;
    int timeStamp=1;
    int heapCounter=1;
    int valueHashMap[10000]={-1};
    int recordHashMap[10000]={-1};
    record *minHeap[10000]={};


    LFUCache(int capacity) {
        heapCapacity=capacity;
        for(int i=0;i<10000;++i)
            valueHashMap[i]=-1;
    }
    
    bool comparator(int a,int b){
        if(minHeap[a]->frequency==minHeap[b]->frequency){
            return minHeap[a]->timeStamp<minHeap[b]->timeStamp;
        }
        return minHeap[a]->frequency<minHeap[b]->frequency;
    }

    void addToleaf(int key){
        minHeap[heapCounter] = new record(key);
        minHeap[heapCounter]->timeStamp=timeStamp;
        ++timeStamp;
        int child=heapCounter;
        int parent=heapCounter/2;
        recordHashMap[key]=heapCounter;
        while(child>1){
            if(comparator(child,parent)){
                recordHashMap[minHeap[parent]->key]=child;
                recordHashMap[minHeap[child]->key]=parent;
                record *temp;
                temp=minHeap[child];
                minHeap[child]=minHeap[parent];
                minHeap[parent]=temp; 
            }
            else{
                break;
            }
            child=parent;
            parent=parent/2;
        }
    }


    void addToRoot(int index){
        int left = index*2;
        int right= index*2+1;
        int smallest=index;
        while(index<heapCounter){            
            if(left<heapCounter && comparator(left,smallest))
                smallest=left;
            if(right<heapCounter && comparator(right,smallest))
                smallest=right;
            if(smallest!=index){
                recordHashMap[minHeap[smallest]->key]=index;
                recordHashMap[minHeap[index]->key]=smallest;
                record *temp;
                temp=minHeap[index];
                minHeap[index]=minHeap[smallest];
                minHeap[smallest]=temp;
            }
            else{
                break;
            }
            index=smallest;
            left = index*2;
            right= index*2+1; 
        }
    }
    
    int get(int key) {
        if(valueHashMap[key]==-1){
            return -1;
        }
        int index = recordHashMap[key];
        ++minHeap[index]->frequency;
        minHeap[index]->timeStamp=timeStamp;
        ++timeStamp;
        addToRoot(index);
        return valueHashMap[key];
    }

    void put(int key, int value) {
        if(heapCapacity==0)return;
        if(valueHashMap[key]!=-1){
            int index=recordHashMap[key];
            ++minHeap[index]->frequency;
            minHeap[index]->timeStamp=timeStamp;
            ++timeStamp;
            addToRoot(index);
            valueHashMap[key]=value;
            return;
        }
        if(heapCapacity<heapCounter){
            valueHashMap[minHeap[1]->key]=-1;
            recordHashMap[key]=1;
            minHeap[1]->key=key;
            minHeap[1]->frequency=1;
            minHeap[1]->timeStamp=timeStamp;
            ++timeStamp;
            addToRoot(1);
        }
        else{
            addToleaf(key);
            ++heapCounter;
        }
        valueHashMap[key]=value;
    }

};
