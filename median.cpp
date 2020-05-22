class MedianFinder {
public:
    
    int minHeap[10000]={0};
    int maxHeap[10000]={0};
    int minHeapCounter=1,maxHeapCounter=1;

    MedianFinder() {
        
    }
    
    bool comparator(int a,int b,bool selector){
        if(selector)
            return a>b;
        return a<b;
    }

    void heapyfy(int heap[],bool selector){
        int child = selector?maxHeapCounter:minHeapCounter;
        int parent= child/2;
        while(child>1){
            if(comparator(heap[child],heap[parent],selector)){
                heap[child]=heap[child]^heap[parent];
                heap[parent]=heap[child]^heap[parent];
                heap[child]=heap[child]^heap[parent];
            }
            else{
                break;
            }
            child=parent;
            parent=child/2;
        }
    }

    int removeFromHeap(int heap[],int selector){
        int val=heap[1];
        int n=selector?maxHeapCounter:minHeapCounter;
        heap[1]=heap[n];
        int l,i,r,temp=1;
        while(temp<n){
            l=temp*2;
            r=temp*2+1;
            i=temp;
            if(l<n && comparator(heap[l],heap[i],selector))
                i=l;
            if(r<n && comparator(heap[r],heap[i],selector))
                i=r;
            if(i!=temp){
                heap[temp]=heap[i]^heap[temp];
                heap[i]=heap[i]^heap[temp];
                heap[temp]=heap[i]^heap[temp];
            }
            else{
                break;
            }
            temp=i;
        }

        return val;
    }

    void addNum(int num) {
        
        if(minHeap[1]<=num){
            minHeap[minHeapCounter]=num;
            heapyfy(minHeap,false);
            ++minHeapCounter;
        }
        else{
            maxHeap[maxHeapCounter]=num;
            heapyfy(maxHeap,true);
            ++maxHeapCounter;
        }

        if((maxHeapCounter-minHeapCounter)>1){
            --maxHeapCounter;
            int val = removeFromHeap(maxHeap,true);
            minHeap[minHeapCounter]=val;
            heapyfy(minHeap,false);
            ++minHeapCounter;
        }
        else if((minHeapCounter-maxHeapCounter)>1){
            --minHeapCounter;
            int val = removeFromHeap(minHeap,false);
            maxHeap[maxHeapCounter]=val;
            heapyfy(maxHeap,true);
            ++maxHeapCounter;
        }
         
    }
    
    double findMedian() {
        if(minHeapCounter==maxHeapCounter)
            return (double)(minHeap[1]+maxHeap[1])/2;
        else if(minHeapCounter>maxHeapCounter)
            return minHeap[1];
        return maxHeap[1];
    }
    
};
/**
 * Your MedianFinder object will be instantiated and called as such:
 * MedianFinder* obj = new MedianFinder();
 * obj->addNum(num);
 * double param_2 = obj->findMedian();
 */
