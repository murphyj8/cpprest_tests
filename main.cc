#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <interrupt_handler.h>

#include <toml.hpp>

web::json::value responseNotImpl(const web::http::method& method_type, const std::string& method_name) {
    auto response = web::json::value::object();
    response["serviceName"] = web::json::value::string("NFT rest Server");
    response["http_method_type"] = web::json::value::string(method_type);
    response["http_method_name"] = web::json::value::string(method_name); 
    return response ;
}

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

void handle_get_status(web::http::http_request request){
    web::json::value response_json; 
    response_json[U("status")] = web::json::value::string(U("Server is ready")); 
    request.reply(web::http::status_codes::OK, response_json); 
    return; 
}

void handle_get_nft(web::http::http_request request){
    //request.reply(web::http::status_codes::NotImplemented, responseNotImpl(web::http::methods::GET, "GetNFT"));
    auto query = web::uri::split_query(web::uri::decode(request.request_uri().query()));
    auto iter = query.find(U("filename")); 
    if (iter != query.end()){
        const std::string& filename(utility::conversions::to_utf8string(iter->second));
        const std::string& nft_file_path("/Users/j.murphy/nchain/SDL/commitment-tokens/python/src/web3_py/build/"); 
        const std::string& nft(nft_file_path + filename); 
        web::json::value abi_json = load_abi(nft);
        if(!abi_json.is_null()){
            request.reply(web::http::status_codes::OK, abi_json); 
        }else{
            request.reply(web::http::status_codes::InternalError, U("Failed to load ABI for ") + filename); 
        }

    }else{
        request.reply(web::http::status_codes::BadRequest, U("Filename parameter missing"));
    }
}
void handle_get(web::http::http_request request){
    auto path = web::uri::split_path(web::uri::decode(request.relative_uri().path())); 
    std::cout << "PATH -> " << request.relative_uri().to_string() << std::endl; 
    if(path.empty()){
        request.reply(web::http::status_codes::BadRequest, U("Invalid Request")); 
    }
    std::string test_path = request.relative_uri().to_string(); 
    if(test_path.length() == 1 && test_path == "/"){
        request.reply(web::http::status_codes::BadRequest, U("Invalid Request ") + test_path); 
        return; 
    }

    std::cout << "In handle get" << std::endl; 
    std::string endpoint = utility::conversions::to_utf8string(path[0]); 
    if (endpoint == "status"){
        handle_get_status(request);
    }else if (endpoint == "GetNFT"){
        handle_get_nft(request); 
    }else{
        std::cout << "GET request not found" << std::endl; 
        request.reply(web::http::status_codes::NotFound, U("Invalid Request ") + endpoint); 
    }
    return;
}
void handle_post(web::http::http_request request){
    std::cout << "Handle Post Not Implemented yet" << std::endl;
    request.reply(web::http::status_codes::NotImplemented, responseNotImpl(web::http::methods::POST, "GenericPost"));
    return ; 
}

void handle_put(web::http::http_request request){
    request.reply(web::http::status_codes::NotImplemented, responseNotImpl(web::http::methods::PUT, "GenericPUT"));
}

void handle_delete(web::http::http_request request){
    request.reply(web::http::status_codes::NotImplemented, responseNotImpl(web::http::methods::DEL, "GenericDEL"));
}

int main(int argc, char * argv[]){
    std::cout << "Running" << std::endl; 
    InterruptHandler::Instance()->hookSIGINT(); 

    std::cout << "Load up the toml config" << std::endl;
    const auto data  = toml::parse("../data/http_server_config.toml");
    const auto& web_conf = toml::find(data, "web_interface");
    const std::string& server_end_point  = toml::find<std::string>(web_conf, "address");

    const std::string& server_url ("http://" + server_end_point); 
    web::uri_builder uri(server_url); 
    auto addr = uri.to_uri().to_string();
    web::http::experimental::listener::http_listener listener(addr);

    listener.support(web::http::methods::GET, handle_get);
    listener.support(web::http::methods::POST, handle_post);
    listener.support(web::http::methods::PUT, handle_put);
    listener.support(web::http::methods::DEL, handle_delete);


    try{
        listener.open()
                .then([&listener]() {std::cout << "Starting server at : " << listener.uri().to_string() << std::endl; })
                .wait(); 
        InterruptHandler::Instance()->waitForUserInterrupt();

    } catch (const std::exception& e){
        std::cout << "Error: " << e.what() << std::endl; 
    }
    return 0;
}