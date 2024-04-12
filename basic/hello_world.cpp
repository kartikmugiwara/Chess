#include <iostream>
#include <string>
#include <vector>


typedef struct loda kartik;
// struct loda {
//     uint8_t a;
//     uint16_t b;
// } mera_type,tera_struct;
uint16_t& manipulate_vec(std::vector<uint16_t>& vec_r, uint16_t& num){
    
    // vec_r[0] = 5;
    uint16_t num1 = 18; 
    // uint16_t& num2 = num1; 
    std::cout << "Address of provided arg : " << &vec_r << std::endl; //this failed because you tried to access memory for empty vector.
    return num1;
}

uint8_t& arr_ref_check(uint8_t arr[]){
    return arr[0];
}


void frame_string()
{
    const uint8_t FRAME_X = 1;
    const uint8_t FRAME_Y = 0;
 
    std::cout << "Enter the string you want to frame" << std::endl;
    std::string reply;
    std::cin >> reply;
    std::cout << "Your reply was: " << reply << std::endl; 
    uint16_t reply_length = reply.length();
    for(uint8_t i=0; i <= (2*(FRAME_Y + 1)); i++){
        for(uint8_t j=0; j< (reply_length+(2*(FRAME_X + 1))); j++){
            if(i==0 || i ==(2*(FRAME_Y + 1)) || j==0 || j==reply_length+(2*(FRAME_X) + 1))
                std::cout << "*";
            else{
                if(i == (FRAME_Y+1) && (j>=(FRAME_X+1) && j<(FRAME_X+1+reply_length)) ){
                    std::cout << reply[j-(FRAME_X+1)];
                }
                else
                    std::cout << " ";
            }           
        }
        std::cout << std::endl;
    }
}
 

int main(){

    // mera_type.a = 4;
    // kartik another_one = {4,20};

    std::vector<uint16_t> lookup_t;
    std::cout << "Address of passed arg : " << &lookup_t << std::endl;
    std::vector<uint16_t>& local_ref = lookup_t;
    // lookup_t.push_back(5);
    std::cout << "Address of local ref : " << &local_ref << std::endl;
    std::cout << "is nullpter? " << (&local_ref == nullptr) << std::endl;
    
    uint16_t num = -2;

    // num = manipulate_vec(lookup_t, num);
    std::cout << std::hex << num ;

    uint8_t arr[]={1, 2, 3};
    // Cpp array always passes arrays as call by reference
    // can use int (&arr)[ssize] if you want tospecifically pass size info as well
    // needed to cast uint8_t& to int as cout was expecting string or something
    std::cout << "returned arr val : " << static_cast<int>(arr_ref_check(arr)) << std::endl;
    return 0;
}


//TODO: 
/*
1. max is a complicated generic function, Must have same typed arguments. Learn in chapter 8

*/