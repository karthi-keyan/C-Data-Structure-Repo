#include<iostream>
struct node{
   int data;
   node* next;
};

class HashTable{

private:
    int SIZE;
    node** head_cursor;
    int get_hash(char str[]){
         int size = sizeof(str)/sizeof(str[0]),sum=0;
         for(int i=0;i<size;i++){
            sum+=(int)str[i];
         }
         //std::cout<<"Hash Value for "<<str<<"  "<<sum%SIZE<<'\n';
         return sum%SIZE;
    }
public:
    HashTable(int size){
       SIZE=size;
       head_cursor =new node*[size];
       init();
    }
    void init(){
        node** cursor = head_cursor;
        for(int i=0;i<SIZE;i++){
            *cursor=NULL;
            cursor++;
         }
    }
    void add(char data[],int ID){
        int index = get_hash(data);
        node** cursor = head_cursor;
        node* temp = new node();
        temp->data = ID;
        temp->next =NULL;
        cursor+=index;
        if(*cursor==NULL){
            *cursor=temp;
        }
        else{
            temp->next=*cursor;
            *cursor= temp;
        }
    }

    void get_val(char data[]){
        int index = get_hash(data);
        node **cursor = head_cursor;
        cursor+=index;
        if(*cursor==NULL){
            std::cout<<"NO DATA AVAILABLE\n";
        }
        else{
            node* temp= *cursor;
            while(temp!=NULL){
            std::cout<<temp->data<<'\n';
            temp=temp->next;
            }
        }
    }
};


int main(){

    HashTable* table = new HashTable(100);

    table->add("karthi",567);
    table->add("karthi",789);
    table->get_val("karthi");
    table->add("naveen",33);
    table->add("naveen",32);
    table->get_val("naveen");
    table->add("Karthi",1);
    table->add("Karthi",2);
    table->add("Karthi",3);
    table->add("Karthi",4);
    table->get_val("Karthi");
    table->get_val("Vijay");
    table->get_val("Mukesh");
   
    return 0;
}
