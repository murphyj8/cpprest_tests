#include <iostream>
#include <string>

#include <cpprest/http_client.h>
#include <cpprest/json.h>

void perform_post(web::http::client::http_client& client, const web::json::value& data){
    client.request(web::http::methods::POST, U("/"), data)
    .then([](web::http::http_response response){
        if(response.status_code() == web::http::status_codes::OK){
            return response.extract_json(); 
        }
        return pplx::task_from_result(web::json::value());
    })
    .then([](web::json::value response_json){
        std::cout << "Post Response : " << response_json.serialize() << std::endl; 
    })
    .wait(); 

    return; 
}

void perform_get(web::http::client::http_client& client, int id = -1){
    web::uri_builder builder(U("/"));
    if (id != -1){
        builder.append_path(std::to_string(id)); 
    }
    client.request(web::http::methods::GET, builder.to_string())
            .then([](web::http::http_response response){
                if(response.status_code () == web::http::status_codes::OK){
                    return response.extract_json();
                }
                return pplx::task_from_result(web::json::value()); 
            })
            .then([](web::json::value response_json){
                std::cout << "Get Response : " << response_json.serialize() << std::endl; 
            })
            .wait(); 
    return; 
}

int main(int argc, char * argv[]){
    web::uri_builder uri(U("http://localhost:8080")); 
    auto addr = uri.to_uri().to_string(); 
    web::http::client::http_client client(addr); 

    web::json::value data; 
    data[U("name")] = web::json::value::string(U("item1")); 
    data[U("value")] = web::json::value::number(10); 

    perform_post(client, data); //create

    perform_get(client); // get

}