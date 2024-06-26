#include <iostream>
#include <fstream>
#include <sstream>
#include <cpprest/json.h>


web::json::value load_abi(const std::string& abi_file_path){
    std::ifstream abi_file(abi_file_path);
    if(!abi_file.is_open()){
        std::cout << "Failed to open nft " << abi_file_path << std::endl; 
        return web::json::value::null(); 
    }
    std::stringstream buffer; 
    buffer << abi_file.rdbuf(); 
    abi_file.close(); 
    std::cout << "Parsing file " << std::endl; 
    return web::json::value::parse(buffer.str()); 
}

int main(int argc, char * argv[]){
    std::cout << "Running" << std::endl;

    const std::string& nft_path("/Users/j.murphy/nchain/SDL/commitment-tokens/python/src/web3_py/build/SmartUTXO.abi");

    web::json::value nft_abi = load_abi(nft_path); 
    std::cout << nft_abi.serialize() << std::endl; 
    std::cout << "Ending" << std::endl; 
}